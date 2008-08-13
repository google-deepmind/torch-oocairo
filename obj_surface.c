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
    /* TODO - throw exception if this is not an image surface */
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
surface_get_size (lua_State *L) {
    cairo_surface_t **obj = luaL_checkudata(L, 1, MT_NAME_SURFACE);
    /* TODO - throw exception if this is not an image surface */
    lua_pushnumber(L, cairo_image_surface_get_width(*obj));
    lua_pushnumber(L, cairo_image_surface_get_height(*obj));
    return 2;
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
surface_show_page (lua_State *L) {
    cairo_surface_t **obj = luaL_checkudata(L, 1, MT_NAME_SURFACE);
    cairo_surface_show_page(*obj);
    return 0;
}

static int
surface_write_to_png (lua_State *L) {
    cairo_surface_t **obj = luaL_checkudata(L, 1, MT_NAME_SURFACE);
    const char *filename = luaL_checkstring(L, 2);
    cairo_surface_write_to_png(*obj, filename);
    return 0;
}

static const luaL_Reg
surface_methods[] = {
    { "__gc", surface_gc },
    { "copy_page", surface_copy_page },
    { "get_content", surface_get_content },
    { "get_device_offset", surface_get_device_offset },
    { "get_fallback_resolution", surface_get_fallback_resolution },
    { "get_format", surface_get_format },
    { "get_size", surface_get_size },
    { "get_type", surface_get_type },
    { "set_device_offset", surface_set_device_offset },
    { "set_fallback_resolution", surface_set_fallback_resolution },
    { "show_page", surface_show_page },
    { "write_to_png", surface_write_to_png },
    { 0, 0 }
};

/* vi:set ts=4 sw=4 expandtab: */
