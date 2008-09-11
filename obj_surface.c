static int
image_surface_create (lua_State *L) {
    cairo_format_t fmt;
    int width, height;
    cairo_surface_t **obj;

    fmt = format_option_values[luaL_checkoption(L, 1, 0, format_option_names)];
    width = luaL_checkint(L, 2);
    luaL_argcheck(L, width >= 0, 2, "image width cannot be negative");
    height = luaL_checkint(L, 3);
    luaL_argcheck(L, height >= 0, 3, "image height cannot be negative");

    obj = lua_newuserdata(L, sizeof(cairo_surface_t *));
    *obj = 0;
    luaL_getmetatable(L, MT_NAME_SURFACE);
    lua_setmetatable(L, -2);
    *obj = cairo_image_surface_create(fmt, width, height);
    return 1;
}

static int
image_surface_create_from_png (lua_State *L) {
    const char *filename = luaL_checkstring(L, 1);
    cairo_surface_t **obj;
    obj = lua_newuserdata(L, sizeof(cairo_surface_t *));
    *obj = 0;
    luaL_getmetatable(L, MT_NAME_SURFACE);
    lua_setmetatable(L, -2);
    *obj = cairo_image_surface_create_from_png(filename);
    switch (cairo_surface_status(*obj)) {
        case CAIRO_STATUS_FILE_NOT_FOUND:
            return luaL_error(L, "PNG file '%s' not found", filename);
        case CAIRO_STATUS_READ_ERROR:
            return luaL_error(L, "error reading PNG file '%s'", filename);
        default:;
    }
    return 1;
}

struct ReadInfoLuaStream {
    lua_State *L;
    int fhpos;
    const char *errmsg;
};

static cairo_status_t
read_chunk_from_fh (void *closure, unsigned char *buf, unsigned int lentoread)
{
    struct ReadInfoLuaStream *readinfo = closure;
    const char *s;
    size_t len;

    lua_State *L = readinfo->L;
    lua_getfield(L, readinfo->fhpos, "read");
    lua_pushvalue(L, readinfo->fhpos);
    lua_pushnumber(L, lentoread);
    if (lua_pcall(L, 2, 1, 0)) {
        if (lua_isstring(L, -1))
            readinfo->errmsg = lua_tostring(L, -1);
        return CAIRO_STATUS_READ_ERROR;
    }

    s = lua_tolstring(L, -1, &len);
    if (!s) {
        readinfo->errmsg = "'read' method on file handle didn't return string";
        return CAIRO_STATUS_READ_ERROR;
    }
    if (len != lentoread) {
        readinfo->errmsg = "'read' method on file handle returned wrong amount"
                           " of data";
        return CAIRO_STATUS_READ_ERROR;
    }

    memcpy(buf, s, len);
    lua_pop(L, 1);
    return CAIRO_STATUS_SUCCESS;
}

static int
image_surface_create_from_png_stream (lua_State *L) {
    struct ReadInfoLuaStream readinfo;
    cairo_surface_t **obj;
    obj = lua_newuserdata(L, sizeof(cairo_surface_t *));
    *obj = 0;
    luaL_getmetatable(L, MT_NAME_SURFACE);
    lua_setmetatable(L, -2);

    readinfo.L = L;
    readinfo.fhpos = 1;
    readinfo.errmsg = 0;
    *obj = cairo_image_surface_create_from_png_stream(read_chunk_from_fh,
                                                      &readinfo);
    if (!*obj) {
        lua_pushliteral(L, "error reading PNG file from Lua file handle");
        if (readinfo.errmsg) {
            lua_pushliteral(L, ": ");
            lua_pushstring(L, readinfo.errmsg);
            lua_concat(L, 3);
        }
        return lua_error(L);
    }
    return 1;
}

struct ReadInfoBuffer {
    const unsigned char *data;
    size_t pos, len;
    int not_enough_data;
};

static cairo_status_t
read_chunk_from_buf (void *closure, unsigned char *buf, unsigned int lentoread)
{
    struct ReadInfoBuffer *readinfo = closure;
    size_t bytesleft = readinfo->len - readinfo->pos;

    if (bytesleft < lentoread)
        return CAIRO_STATUS_READ_ERROR; /* not enough data left in buffer */

    memcpy(buf, readinfo->data + readinfo->pos, lentoread);
    readinfo->pos += lentoread;
    return CAIRO_STATUS_SUCCESS;
}

static int
image_surface_create_from_png_string (lua_State *L) {
    struct ReadInfoBuffer readinfo;
    cairo_surface_t **obj;
    obj = lua_newuserdata(L, sizeof(cairo_surface_t *));
    *obj = 0;
    luaL_getmetatable(L, MT_NAME_SURFACE);
    lua_setmetatable(L, -2);

    readinfo.data = (unsigned char *) luaL_checklstring(L, 1, &readinfo.len);
    readinfo.pos = 0;
    readinfo.not_enough_data = 0;
    *obj = cairo_image_surface_create_from_png_stream(read_chunk_from_buf,
                                                      &readinfo);
    if (!*obj) {
        lua_pushliteral(L, "error reading PNG file from Lua string");
        if (readinfo.not_enough_data) {
            lua_pushliteral(L, ": end of string reached");
            lua_concat(L, 2);
        }
        return lua_error(L);
    }
    return 1;
}

static int
pdf_surface_create (lua_State *L) {
    const char *filename;
    lua_Number width, height;
    cairo_surface_t **obj;

    filename = luaL_checkstring(L, 1);
    width = luaL_checknumber(L, 2);
    height = luaL_checknumber(L, 3);
    luaL_argcheck(L, width >= 0, 2, "image width cannot be negative");
    luaL_argcheck(L, height >= 0, 3, "image height cannot be negative");

    obj = lua_newuserdata(L, sizeof(cairo_surface_t *));
    *obj = 0;
    luaL_getmetatable(L, MT_NAME_SURFACE);
    lua_setmetatable(L, -2);
    *obj = cairo_pdf_surface_create(filename, width, height);
    if (cairo_surface_status(*obj) != CAIRO_STATUS_SUCCESS)
        return luaL_error(L, "error creating PDF surface");
    return 1;
}

static int
svg_get_versions (lua_State *L) {
    const cairo_svg_version_t *versions;
    int num_versions, i;
    cairo_svg_get_versions(&versions, &num_versions);

    lua_createtable(L, num_versions, 0);
    for (i = 0; i < num_versions; ++i) {
        lua_pushstring(L, cairo_svg_version_to_string(versions[i]));
        lua_rawseti(L, -2, i + 1);
    }
    return 1;
}

static int
svg_surface_create (lua_State *L) {
    const char *filename;
    lua_Number width, height;
    cairo_surface_t **obj;

    filename = luaL_checkstring(L, 1);
    width = luaL_checknumber(L, 2);
    height = luaL_checknumber(L, 3);
    luaL_argcheck(L, width >= 0, 2, "image width cannot be negative");
    luaL_argcheck(L, height >= 0, 3, "image height cannot be negative");

    obj = lua_newuserdata(L, sizeof(cairo_surface_t *));
    *obj = 0;
    luaL_getmetatable(L, MT_NAME_SURFACE);
    lua_setmetatable(L, -2);
    *obj = cairo_svg_surface_create(filename, width, height);
    if (cairo_surface_status(*obj) != CAIRO_STATUS_SUCCESS)
        return luaL_error(L, "error creating SVG surface");
    return 1;
}

static int
surface_create_similar (lua_State *L) {
    cairo_surface_t **oldobj = luaL_checkudata(L, 1, MT_NAME_SURFACE);
    cairo_surface_t **newobj;
    cairo_content_t content;
    int width, height;

    content = content_values[luaL_checkoption(L, 2, 0, content_names)];
    width = luaL_checkint(L, 3);
    luaL_argcheck(L, width >= 0, 3, "image width cannot be negative");
    height = luaL_checkint(L, 4);
    luaL_argcheck(L, height >= 0, 4, "image height cannot be negative");

    newobj = lua_newuserdata(L, sizeof(cairo_surface_t *));
    *newobj = 0;
    luaL_getmetatable(L, MT_NAME_SURFACE);
    lua_setmetatable(L, -2);
    *newobj = cairo_surface_create_similar(*oldobj, content, width, height);
    return 1;
}

static int
surface_eq (lua_State *L) {
    cairo_surface_t **obj1 = luaL_checkudata(L, 1, MT_NAME_SURFACE);
    cairo_surface_t **obj2 = luaL_checkudata(L, 2, MT_NAME_SURFACE);
    lua_pushboolean(L, *obj1 == *obj2);
    return 1;
}

static int
surface_gc (lua_State *L) {
    cairo_surface_t **obj = luaL_checkudata(L, 1, MT_NAME_SURFACE);
    cairo_surface_destroy(*obj);
    *obj = 0;
    return 0;
}

static int
surface_copy_page (lua_State *L) {
    cairo_surface_t **obj = luaL_checkudata(L, 1, MT_NAME_SURFACE);
    cairo_surface_copy_page(*obj);
    return 0;
}

static int
surface_finish (lua_State *L) {
    cairo_surface_t **obj = luaL_checkudata(L, 1, MT_NAME_SURFACE);
    cairo_surface_finish(*obj);
    return 0;
}

static int
surface_flush (lua_State *L) {
    cairo_surface_t **obj = luaL_checkudata(L, 1, MT_NAME_SURFACE);
    cairo_surface_flush(*obj);
    return 0;
}

static int
surface_get_content (lua_State *L) {
    cairo_surface_t **obj = luaL_checkudata(L, 1, MT_NAME_SURFACE);
    switch (cairo_surface_get_content(*obj)) {
        case CAIRO_CONTENT_COLOR:
            lua_pushliteral(L, "color");
            break;
        case CAIRO_CONTENT_ALPHA:
            lua_pushliteral(L, "alpha");
            break;
        case CAIRO_CONTENT_COLOR_ALPHA:
            lua_pushliteral(L, "color-alpha");
            break;
        default:
            lua_pushliteral(L, "<invalid>");
    }
    return 1;
}

static int
surface_get_device_offset (lua_State *L) {
    cairo_surface_t **obj = luaL_checkudata(L, 1, MT_NAME_SURFACE);
    double x, y;
    cairo_surface_get_device_offset(*obj, &x, &y);
    lua_pushnumber(L, x);
    lua_pushnumber(L, y);
    return 2;
}

static int
surface_get_fallback_resolution (lua_State *L) {
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
    cairo_surface_t **obj = luaL_checkudata(L, 1, MT_NAME_SURFACE);
    double x, y;
    cairo_surface_get_fallback_resolution(*obj, &x, &y);
    cairo_surface_get_fallback_resolution
    lua_pushnumber(L, x);
    lua_pushnumber(L, y);
    return 2;
#else
    return luaL_error(L, "get_fallback_resolution requires Cairo 1.8");
#endif
}

static int
surface_get_format (lua_State *L) {
    cairo_surface_t **obj = luaL_checkudata(L, 1, MT_NAME_SURFACE);
    if (cairo_surface_get_type(*obj) != CAIRO_SURFACE_TYPE_IMAGE)
        return luaL_error(L, "method 'get_format' only works on image surfaces");
    switch (cairo_image_surface_get_format(*obj)) {
        case CAIRO_FORMAT_ARGB32: lua_pushliteral(L, "argb32"); break;
        case CAIRO_FORMAT_RGB24:  lua_pushliteral(L, "rgb24");  break;
        case CAIRO_FORMAT_A8:     lua_pushliteral(L, "a8");     break;
        case CAIRO_FORMAT_A1:     lua_pushliteral(L, "a1");     break;
        default:                  lua_pushliteral(L, "<invalid>");
    }
    return 1;
}

static int
surface_get_height (lua_State *L) {
    cairo_surface_t **obj = luaL_checkudata(L, 1, MT_NAME_SURFACE);
    if (cairo_surface_get_type(*obj) != CAIRO_SURFACE_TYPE_IMAGE)
        return luaL_error(L, "method 'get_height' only works on image surfaces");
    lua_pushnumber(L, cairo_image_surface_get_height(*obj));
    return 1;
}

static int
surface_get_type (lua_State *L) {
    cairo_surface_t **obj = luaL_checkudata(L, 1, MT_NAME_SURFACE);
    const char *s;
    switch (cairo_surface_get_type(*obj)) {
        case CAIRO_SURFACE_TYPE_IMAGE:          s = "image";          break;
        case CAIRO_SURFACE_TYPE_PDF:            s = "pdf";            break;
        case CAIRO_SURFACE_TYPE_PS:             s = "ps";             break;
        case CAIRO_SURFACE_TYPE_XLIB:           s = "xlib";           break;
        case CAIRO_SURFACE_TYPE_XCB:            s = "xcb";            break;
        case CAIRO_SURFACE_TYPE_GLITZ:          s = "glitz";          break;
        case CAIRO_SURFACE_TYPE_QUARTZ:         s = "quartz";         break;
        case CAIRO_SURFACE_TYPE_WIN32:          s = "win32";          break;
        case CAIRO_SURFACE_TYPE_BEOS:           s = "beos";           break;
        case CAIRO_SURFACE_TYPE_DIRECTFB:       s = "directfb";       break;
        case CAIRO_SURFACE_TYPE_SVG:            s = "svg";            break;
        case CAIRO_SURFACE_TYPE_OS2:            s = "os2";            break;
        case CAIRO_SURFACE_TYPE_WIN32_PRINTING: s = "win32-printing"; break;
        case CAIRO_SURFACE_TYPE_QUARTZ_IMAGE:   s = "quartz-image";   break;
        default:                                s = "<invalid>";
    }
    lua_pushstring(L, s);
    return 1;
}

static int
surface_get_width (lua_State *L) {
    cairo_surface_t **obj = luaL_checkudata(L, 1, MT_NAME_SURFACE);
    if (cairo_surface_get_type(*obj) != CAIRO_SURFACE_TYPE_IMAGE)
        return luaL_error(L, "method 'get_width' only works on image surfaces");
    lua_pushnumber(L, cairo_image_surface_get_width(*obj));
    return 1;
}

static int
surface_set_device_offset (lua_State *L) {
    cairo_surface_t **obj = luaL_checkudata(L, 1, MT_NAME_SURFACE);
    cairo_surface_set_device_offset(*obj, luaL_checknumber(L, 2),
                                    luaL_checknumber(L, 3));
    return 0;
}

static int
surface_set_fallback_resolution (lua_State *L) {
    cairo_surface_t **obj = luaL_checkudata(L, 1, MT_NAME_SURFACE);
    cairo_surface_set_fallback_resolution(*obj, luaL_checknumber(L, 2),
                                          luaL_checknumber(L, 3));
    return 0;
}

static int
surface_set_size (lua_State *L) {
    cairo_surface_t **obj = luaL_checkudata(L, 1, MT_NAME_SURFACE);
    if (cairo_surface_get_type(*obj) != CAIRO_SURFACE_TYPE_PDF)
        return luaL_error(L, "method 'set_size' only works on PDF surfaces");
    cairo_pdf_surface_set_size(*obj, luaL_checknumber(L, 2),
                               luaL_checknumber(L, 3));
    return 0;
}

static int
surface_show_page (lua_State *L) {
    cairo_surface_t **obj = luaL_checkudata(L, 1, MT_NAME_SURFACE);
    cairo_surface_show_page(*obj);
    return 0;
}

static int
surface_write_to_png (lua_State *L) {
    cairo_surface_t **obj = luaL_checkudata(L, 1, MT_NAME_SURFACE);
    const char *filename = luaL_checkstring(L, 2);
    if (cairo_surface_write_to_png(*obj, filename) != CAIRO_STATUS_SUCCESS)
        return luaL_error(L, "error writing surface to PNG file '%s'",
                          filename);
    return 0;
}

struct WriteInfoLuaStream {
    lua_State *L;
    int fhpos;
    const char *errmsg;
};

static cairo_status_t
write_chunk_to_fh (void *closure, const unsigned char *buf,
                   unsigned int lentowrite)
{
    struct WriteInfoLuaStream *writeinfo = closure;

    lua_State *L = writeinfo->L;
    lua_getfield(L, writeinfo->fhpos, "write");
    lua_pushvalue(L, writeinfo->fhpos);
    lua_pushlstring(L, (const char *) buf, lentowrite);
    if (lua_pcall(L, 2, 0, 0)) {
        if (lua_isstring(L, -1))
            writeinfo->errmsg = lua_tostring(L, -1);
        return CAIRO_STATUS_WRITE_ERROR;
    }

    return CAIRO_STATUS_SUCCESS;
}

static int
surface_write_to_png_stream (lua_State *L) {
    cairo_surface_t **obj = luaL_checkudata(L, 1, MT_NAME_SURFACE);
    struct WriteInfoLuaStream writeinfo;

    writeinfo.L = L;
    writeinfo.fhpos = 2;
    writeinfo.errmsg = 0;

    if (cairo_surface_write_to_png_stream(*obj, write_chunk_to_fh, &writeinfo)
            != CAIRO_STATUS_SUCCESS)
    {
        lua_pushliteral(L, "error writing PNG file to Lua file handle");
        if (writeinfo.errmsg) {
            lua_pushliteral(L, ": ");
            lua_pushstring(L, writeinfo.errmsg);
            lua_concat(L, 3);
        }
        return lua_error(L);
    }

    return 0;
}

struct WrietInfoBuffer {
    lua_State *L;
    luaL_Buffer b;
};

static cairo_status_t
write_chunk_to_luabuf (void *closure, const unsigned char *buf,
                       unsigned int lentowrite)
{
    struct WrietInfoBuffer *info = closure;
    luaL_checkstack(info->L, 1, "not enough memory for PNG string");
    luaL_addlstring(&info->b, (const char *) buf, lentowrite);
    return CAIRO_STATUS_SUCCESS;
}

static int
surface_write_to_png_string (lua_State *L) {
    cairo_surface_t **obj = luaL_checkudata(L, 1, MT_NAME_SURFACE);
    struct WrietInfoBuffer info;

    info.L = L;
    luaL_buffinit(L, &info.b);

    if (cairo_surface_write_to_png_stream(*obj, write_chunk_to_luabuf, &info)
            != CAIRO_STATUS_SUCCESS)
    {
        luaL_pushresult(&info.b);   /* tidy stack, result ignored */
        luaL_error(L, "error writing PNG file to Lua string");
    }

    luaL_pushresult(&info.b);
    return 1;
}

static const luaL_Reg
surface_methods[] = {
    { "__eq", surface_eq },
    { "__gc", surface_gc },
    { "copy_page", surface_copy_page },
    { "finish", surface_finish },
    { "flush", surface_flush },
    { "get_content", surface_get_content },
    { "get_device_offset", surface_get_device_offset },
    { "get_fallback_resolution", surface_get_fallback_resolution },
    { "get_format", surface_get_format },
    { "get_height", surface_get_height },
    { "get_type", surface_get_type },
    { "get_width", surface_get_width },
    { "set_device_offset", surface_set_device_offset },
    { "set_fallback_resolution", surface_set_fallback_resolution },
    { "set_size", surface_set_size },
    { "show_page", surface_show_page },
    { "write_to_png", surface_write_to_png },
    { "write_to_png_stream", surface_write_to_png_stream },
    { "write_to_png_string", surface_write_to_png_string },
    { 0, 0 }
};

/* vi:set ts=4 sw=4 expandtab: */
