static int
image_surface_create (lua_State *L) {
    cairo_format_t fmt;
    int width, height;
    cairo_surface_t **obj;

    fmt = format_option_values[luaL_checkoption(L, 1, 0, format_option_names)];
    width = luaL_checkint(L, 2);
    height = luaL_checkint(L, 3);

    obj = lua_newuserdata(L, sizeof(cairo_surface_t *));
    *obj = 0;
    luaL_getmetatable(L, MT_NAME_SURFACE);
    lua_setmetatable(L, -2);
    *obj = cairo_image_surface_create(fmt, width, height);
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
    height = luaL_checkint(L, 4);

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
surface_write_to_png (lua_State *L) {
    cairo_surface_t **obj = luaL_checkudata(L, 1, MT_NAME_SURFACE);
    const char *filename = luaL_checkstring(L, 2);
    cairo_surface_write_to_png(*obj, filename);
    return 0;
}

static const luaL_Reg
surface_methods[] = {
    { "__gc", surface_gc },
    { "write_to_png", surface_write_to_png },
    { 0, 0 }
};

/* vi:set ts=4 sw=4 expandtab: */
