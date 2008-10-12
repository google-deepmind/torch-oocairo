static int
image_surface_create (lua_State *L) {
    cairo_format_t fmt;
    int width, height;
    SurfaceUserdata *surface;

    fmt = format_from_lua(L, 1);
    width = luaL_checkint(L, 2);
    luaL_argcheck(L, width >= 0, 2, "image width cannot be negative");
    height = luaL_checkint(L, 3);
    luaL_argcheck(L, height >= 0, 3, "image height cannot be negative");

    surface = create_surface_userdata(L);
    surface->surface = cairo_image_surface_create(fmt, width, height);
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
    struct ReadInfoLuaStream *info = closure;
    const char *s;
    size_t len;

    lua_State *L = info->L;
    lua_getfield(L, info->fhpos, "read");
    lua_pushvalue(L, info->fhpos);
    lua_pushnumber(L, lentoread);
    if (lua_pcall(L, 2, 1, 0)) {
        if (lua_isstring(L, -1))
            info->errmsg = lua_tostring(L, -1);
        return CAIRO_STATUS_READ_ERROR;
    }

    s = lua_tolstring(L, -1, &len);
    if (!s) {
        info->errmsg = "'read' method on file handle didn't return string";
        return CAIRO_STATUS_READ_ERROR;
    }
    if (len != lentoread) {
        info->errmsg = "'read' method on file handle returned wrong amount"
                           " of data";
        return CAIRO_STATUS_READ_ERROR;
    }

    memcpy(buf, s, len);
    lua_pop(L, 1);
    return CAIRO_STATUS_SUCCESS;
}

#if CAIRO_HAS_PNG_FUNCTIONS
static int
image_surface_create_from_png (lua_State *L) {
    SurfaceUserdata *surface = create_surface_userdata(L);

    if (lua_isstring(L, 1)) {
        const char *filename = luaL_checkstring(L, 1);
        surface->surface = cairo_image_surface_create_from_png(filename);
        switch (cairo_surface_status(surface->surface)) {
            case CAIRO_STATUS_FILE_NOT_FOUND:
                return luaL_error(L, "PNG file '%s' not found", filename);
            case CAIRO_STATUS_READ_ERROR:
                return luaL_error(L, "error reading PNG file '%s'", filename);
            default:;
        }
    }
    else {
        struct ReadInfoLuaStream info;

        info.L = L;
        info.fhpos = 1;
        info.errmsg = 0;
        surface->surface = cairo_image_surface_create_from_png_stream(
                                    read_chunk_from_fh, &info);
        if (!surface->surface) {
            lua_pushliteral(L, "error reading PNG file from Lua file handle");
            if (info.errmsg) {
                lua_pushliteral(L, ": ");
                lua_pushstring(L, info.errmsg);
                lua_concat(L, 3);
            }
            return lua_error(L);
        }
    }

    return 1;
}
#endif

/* This function is used for several types of surface which all have the
 * same type of construction, and are all a bit complicated because they
 * can write their output to a Lua file handle. */
typedef cairo_surface_t * (*SimpleSizeCreatorFunc) (const char *,
                                                    double, double);
typedef cairo_surface_t * (*StreamSizeCreatorFunc) (cairo_write_func_t, void *,
                                                    double, double);
static void
surface_create_with_size (lua_State *L, SimpleSizeCreatorFunc simplefunc, StreamSizeCreatorFunc streamfunc)
{
    double width, height;
    SurfaceUserdata *surface;
    int filetype;

    width = luaL_checknumber(L, 2);
    height = luaL_checknumber(L, 3);
    luaL_argcheck(L, width >= 0, 2, "image width cannot be negative");
    luaL_argcheck(L, height >= 0, 3, "image height cannot be negative");

    surface = create_surface_userdata(L);

    filetype = lua_type(L, 1);
    if (filetype == LUA_TSTRING || filetype == LUA_TNUMBER) {
        const char *filename = lua_tostring(L, 1);
        surface->surface = simplefunc(filename, width, height);
        if (cairo_surface_status(surface->surface) != CAIRO_STATUS_SUCCESS)
            luaL_error(L, "error creating surface for filename '%s'", filename);
    }
    else if (filetype == LUA_TUSERDATA || filetype == LUA_TTABLE) {
        lua_pushvalue(L, 1);
        surface->fhref = luaL_ref(L, LUA_REGISTRYINDEX);

        surface->surface = streamfunc(write_chunk_to_fh, surface,
                                      width, height);
        if (cairo_surface_status(surface->surface) != CAIRO_STATUS_SUCCESS) {
            lua_pushliteral(L, "error writing surface output file to Lua"
                            " file handle");
            if (surface->errmsg) {
                lua_pushliteral(L, ": ");
                lua_pushstring(L, surface->errmsg);
                lua_concat(L, 3);
            }
            lua_error(L);
        }
    }
    else
        luaL_typerror(L, 1, "filename or file handle object");
}

#if CAIRO_HAS_PDF_SURFACE
static int
pdf_surface_create (lua_State *L) {
    surface_create_with_size(L, cairo_pdf_surface_create,
                             cairo_pdf_surface_create_for_stream);
    return 1;
}
#endif

#if CAIRO_HAS_PS_SURFACE
static int
ps_surface_create (lua_State *L) {
    surface_create_with_size(L, cairo_ps_surface_create,
                             cairo_ps_surface_create_for_stream);
    return 1;
}
#endif

#if CAIRO_HAS_SVG_SURFACE
static int
svg_surface_create (lua_State *L) {
    surface_create_with_size(L, cairo_svg_surface_create,
                             cairo_svg_surface_create_for_stream);
    return 1;
}
#endif

#if CAIRO_HAS_PS_SURFACE
static int
ps_get_levels (lua_State *L) {
    const cairo_ps_level_t *levels;
    int num_levels, i;
    cairo_ps_get_levels(&levels, &num_levels);

    lua_createtable(L, num_levels, 0);
    for (i = 0; i < num_levels; ++i) {
        lua_pushstring(L, cairo_ps_level_to_string(levels[i]));
        lua_rawseti(L, -2, i + 1);
    }
    return 1;
}
#endif

#if CAIRO_HAS_SVG_SURFACE
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
#endif

static int
surface_create_similar (lua_State *L) {
    cairo_surface_t **oldobj = luaL_checkudata(L, 1, MT_NAME_SURFACE);
    cairo_content_t content;
    int width, height;
    SurfaceUserdata *surface;

    content = content_from_lua(L, 2);
    width = luaL_checkint(L, 3);
    luaL_argcheck(L, width >= 0, 3, "image width cannot be negative");
    height = luaL_checkint(L, 4);
    luaL_argcheck(L, height >= 0, 4, "image height cannot be negative");

    surface = create_surface_userdata(L);
    surface->surface = cairo_surface_create_similar(*oldobj, content,
                                                    width, height);
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
    SurfaceUserdata *ud = luaL_checkudata(L, 1, MT_NAME_SURFACE);
    free_surface_userdata(ud);
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
    return content_to_lua(L, cairo_surface_get_content(*obj));
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

#if CAIRO_HAS_PS_SURFACE
static int
surface_get_eps (lua_State *L) {
    cairo_surface_t **obj = luaL_checkudata(L, 1, MT_NAME_SURFACE);
    if (cairo_surface_get_type(*obj) != CAIRO_SURFACE_TYPE_PS)
        return luaL_error(L, "method 'get_eps' only works on PostScript"
                          " surfaces");
    lua_pushboolean(L, cairo_ps_surface_get_eps(*obj));
    return 1;
}
#endif

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
static int
surface_get_fallback_resolution (lua_State *L) {
    cairo_surface_t **obj = luaL_checkudata(L, 1, MT_NAME_SURFACE);
    double x, y;
    cairo_surface_get_fallback_resolution(*obj, &x, &y);
    lua_pushnumber(L, x);
    lua_pushnumber(L, y);
    return 2;
}
#endif

static int
surface_get_format (lua_State *L) {
    cairo_surface_t **obj = luaL_checkudata(L, 1, MT_NAME_SURFACE);
    if (cairo_surface_get_type(*obj) != CAIRO_SURFACE_TYPE_IMAGE)
        return luaL_error(L, "method 'get_format' only works on image surfaces");
    return format_to_lua(L, cairo_image_surface_get_format(*obj));
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
    return surface_type_to_lua(L, cairo_surface_get_type(*obj));
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

#if CAIRO_HAS_PS_SURFACE
static int
surface_set_eps (lua_State *L) {
    cairo_surface_t **obj = luaL_checkudata(L, 1, MT_NAME_SURFACE);
    if (cairo_surface_get_type(*obj) != CAIRO_SURFACE_TYPE_PS)
        return luaL_error(L, "method 'set_eps' only works on PostScript"
                          " surfaces");
    cairo_ps_surface_set_eps(*obj, lua_toboolean(L, 2));
    return 0;
}
#endif

static int
surface_set_fallback_resolution (lua_State *L) {
    cairo_surface_t **obj = luaL_checkudata(L, 1, MT_NAME_SURFACE);
    cairo_surface_set_fallback_resolution(*obj, luaL_checknumber(L, 2),
                                          luaL_checknumber(L, 3));
    return 0;
}

#if CAIRO_HAS_PDF_SURFACE || CAIRO_HAS_PS_SURFACE
static int
surface_set_size (lua_State *L) {
    cairo_surface_t **obj = luaL_checkudata(L, 1, MT_NAME_SURFACE);
    cairo_surface_type_t type = cairo_surface_get_type(*obj);
    double width = luaL_checknumber(L, 2), height = luaL_checknumber(L, 3);
#if CAIRO_HAS_PDF_SURFACE
    if (type == CAIRO_SURFACE_TYPE_PDF)
        cairo_pdf_surface_set_size(*obj, width, height);
#endif
#if CAIRO_HAS_PDF_SURFACE && CAIRO_HAS_PS_SURFACE
    else
#endif
#if CAIRO_HAS_PS_SURFACE
    if (type == CAIRO_SURFACE_TYPE_PS)
        cairo_ps_surface_set_size(*obj, width, height);
#endif
    else
        return luaL_error(L, "method 'set_size' only works on PostScript and"
                          " PDF surfaces");
    return 0;
}
#endif

static int
surface_show_page (lua_State *L) {
    cairo_surface_t **obj = luaL_checkudata(L, 1, MT_NAME_SURFACE);
    cairo_surface_show_page(*obj);
    return 0;
}

#if CAIRO_HAS_PNG_FUNCTIONS
static int
surface_write_to_png (lua_State *L) {
    cairo_surface_t **obj = luaL_checkudata(L, 1, MT_NAME_SURFACE);
    int filetype = lua_type(L, 2);

    if (filetype == LUA_TSTRING || filetype == LUA_TNUMBER) {
        const char *filename = lua_tostring(L, 2);
        if (cairo_surface_write_to_png(*obj, filename) != CAIRO_STATUS_SUCCESS)
            return luaL_error(L, "error writing surface to PNG file '%s'",
                              filename);
    }
    else if (filetype == LUA_TUSERDATA || filetype == LUA_TTABLE) {
        SurfaceUserdata info;
        init_surface_userdata(L, &info);
        lua_pushvalue(L, 2);
        info.fhref = luaL_ref(L, LUA_REGISTRYINDEX);

        if (cairo_surface_write_to_png_stream(*obj, write_chunk_to_fh, &info)
                != CAIRO_STATUS_SUCCESS)
        {
            lua_pushliteral(L, "error writing PNG file to Lua file handle");
            if (info.errmsg) {
                lua_pushliteral(L, ": ");
                lua_pushstring(L, info.errmsg);
                lua_concat(L, 3);
            }
            free_surface_userdata(&info);
            return lua_error(L);
        }

        free_surface_userdata(&info);
    }
    else
        return luaL_typerror(L, 1, "filename or file handle object");

    return 0;
}
#endif

static const luaL_Reg
surface_methods[] = {
    { "__eq", surface_eq },
    { "__gc", surface_gc },
    { "copy_page", surface_copy_page },
    { "finish", surface_finish },
    { "flush", surface_flush },
    { "get_content", surface_get_content },
    { "get_device_offset", surface_get_device_offset },
#if CAIRO_HAS_PS_SURFACE
    { "get_eps", surface_get_eps },
#endif
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
    { "get_fallback_resolution", surface_get_fallback_resolution },
#endif
    { "get_format", surface_get_format },
    { "get_height", surface_get_height },
    { "get_type", surface_get_type },
    { "get_width", surface_get_width },
    { "set_device_offset", surface_set_device_offset },
#if CAIRO_HAS_PS_SURFACE
    { "set_eps", surface_set_eps },
#endif
    { "set_fallback_resolution", surface_set_fallback_resolution },
    { "set_size", surface_set_size },
    { "show_page", surface_show_page },
#if CAIRO_HAS_PNG_FUNCTIONS
    { "write_to_png", surface_write_to_png },
#endif
    { 0, 0 }
};

/* vi:set ts=4 sw=4 expandtab: */
