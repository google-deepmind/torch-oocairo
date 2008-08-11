static int
context_create (lua_State *L) {
    cairo_surface_t **surface = luaL_checkudata(L, 1, MT_NAME_SURFACE);
    cairo_t **obj;
    obj = lua_newuserdata(L, sizeof(cairo_t *));
    *obj = 0;
    luaL_getmetatable(L, MT_NAME_CONTEXT);
    lua_setmetatable(L, -2);
    *obj = cairo_create(*surface);
    return 1;
}

static int
cr_gc (lua_State *L) {
    cairo_t **obj = luaL_checkudata(L, 1, MT_NAME_CONTEXT);
    cairo_destroy(*obj);
    *obj = 0;
    return 0;
}

static int
cr_arc (lua_State *L) {
    cairo_t **obj = luaL_checkudata(L, 1, MT_NAME_CONTEXT);
    cairo_arc(*obj, luaL_checknumber(L, 2), luaL_checknumber(L, 3),
              luaL_checknumber(L, 4), luaL_checknumber(L, 5),
              luaL_checknumber(L, 6));
    return 0;
}

static int
cr_close_path (lua_State *L) {
    cairo_t **obj = luaL_checkudata(L, 1, MT_NAME_CONTEXT);
    cairo_close_path(*obj);
    return 0;
}

static int
cr_curve_to (lua_State *L) {
    cairo_t **obj = luaL_checkudata(L, 1, MT_NAME_CONTEXT);
    cairo_curve_to(*obj, luaL_checknumber(L, 2), luaL_checknumber(L, 3),
                   luaL_checknumber(L, 4), luaL_checknumber(L, 5),
                   luaL_checknumber(L, 6), luaL_checknumber(L, 7));
    return 0;
}

static int
cr_fill (lua_State *L) {
    cairo_t **obj = luaL_checkudata(L, 1, MT_NAME_CONTEXT);
    cairo_fill(*obj);
    return 0;
}

static int
cr_fill_preserve (lua_State *L) {
    cairo_t **obj = luaL_checkudata(L, 1, MT_NAME_CONTEXT);
    cairo_fill_preserve(*obj);
    return 0;
}

static int
cr_in_fill (lua_State *L) {
    cairo_t **obj = luaL_checkudata(L, 1, MT_NAME_CONTEXT);
    lua_pushboolean(L,
        cairo_in_fill(*obj, luaL_checknumber(L, 2), luaL_checknumber(L, 3)));
    return 1;
}

static int
cr_in_stroke (lua_State *L) {
    cairo_t **obj = luaL_checkudata(L, 1, MT_NAME_CONTEXT);
    lua_pushboolean(L,
        cairo_in_stroke(*obj, luaL_checknumber(L, 2), luaL_checknumber(L, 3)));
    return 1;
}

static int
cr_line_to (lua_State *L) {
    cairo_t **obj = luaL_checkudata(L, 1, MT_NAME_CONTEXT);
    cairo_line_to(*obj, luaL_checknumber(L, 2), luaL_checknumber(L, 3));
    return 0;
}

static int
cr_move_to (lua_State *L) {
    cairo_t **obj = luaL_checkudata(L, 1, MT_NAME_CONTEXT);
    cairo_move_to(*obj, luaL_checknumber(L, 2), luaL_checknumber(L, 3));
    return 0;
}

static int
cr_new_path (lua_State *L) {
    cairo_t **obj = luaL_checkudata(L, 1, MT_NAME_CONTEXT);
    cairo_new_path(*obj);
    return 0;
}

static int
cr_new_sub_path (lua_State *L) {
    cairo_t **obj = luaL_checkudata(L, 1, MT_NAME_CONTEXT);
    cairo_new_sub_path(*obj);
    return 0;
}

static int
cr_paint (lua_State *L) {
    cairo_t **obj = luaL_checkudata(L, 1, MT_NAME_CONTEXT);
    cairo_paint(*obj);
    return 0;
}

static int
cr_paint_with_alpha (lua_State *L) {
    cairo_t **obj = luaL_checkudata(L, 1, MT_NAME_CONTEXT);
    cairo_paint_with_alpha(*obj, luaL_checknumber(L, 2));
    return 0;
}

static int
cr_rectangle (lua_State *L) {
    cairo_t **obj = luaL_checkudata(L, 1, MT_NAME_CONTEXT);
    cairo_rectangle(*obj, luaL_checknumber(L, 2), luaL_checknumber(L, 3),
                    luaL_checknumber(L, 4), luaL_checknumber(L, 5));
    return 0;
}

static int
cr_set_line_cap (lua_State *L) {
    cairo_t **obj = luaL_checkudata(L, 1, MT_NAME_CONTEXT);
    cairo_set_line_cap(*obj,
            linecap_values[luaL_checkoption(L, 2, 0, linecap_names)]);
    return 0;
}

static int
cr_get_line_cap (lua_State *L) {
    cairo_t **obj = luaL_checkudata(L, 1, MT_NAME_CONTEXT);
    switch (cairo_get_line_cap(*obj)) {
        case CAIRO_LINE_CAP_BUTT:   lua_pushliteral(L, "butt");      break;
        case CAIRO_LINE_CAP_ROUND:  lua_pushliteral(L, "round");     break;
        case CAIRO_LINE_CAP_SQUARE: lua_pushliteral(L, "square");    break;
        default:                    lua_pushliteral(L, "<invalid>"); break;
    }
    return 1;
}

static int
cr_set_line_width (lua_State *L) {
    cairo_t **obj = luaL_checkudata(L, 1, MT_NAME_CONTEXT);
    cairo_set_line_width(*obj, luaL_checknumber(L, 2));
    return 0;
}

static int
cr_set_source_rgb (lua_State *L) {
    cairo_t **obj = luaL_checkudata(L, 1, MT_NAME_CONTEXT);
    cairo_set_source_rgb(*obj, luaL_checknumber(L, 2), luaL_checknumber(L, 3),
                         luaL_checknumber(L, 4));
    return 0;
}

static int
cr_set_source_rgba (lua_State *L) {
    cairo_t **obj = luaL_checkudata(L, 1, MT_NAME_CONTEXT);
    cairo_set_source_rgba(*obj, luaL_checknumber(L, 2), luaL_checknumber(L, 3),
                          luaL_checknumber(L, 4), luaL_checknumber(L, 5));
    return 0;
}

static int
cr_stroke (lua_State *L) {
    cairo_t **obj = luaL_checkudata(L, 1, MT_NAME_CONTEXT);
    cairo_stroke(*obj);
    return 0;
}

static const luaL_Reg
context_methods[] = {
    { "__gc", cr_gc },
    { "arc", cr_arc },
    { "close_path", cr_close_path },
    { "curve_to", cr_curve_to },
    { "fill", cr_fill },
    { "fill_preserve", cr_fill_preserve },
    { "get_line_cap", cr_get_line_cap },
    { "in_fill", cr_in_fill },
    { "in_stroke", cr_in_stroke },
    { "line_to", cr_line_to },
    { "move_to", cr_move_to },
    { "new_path", cr_new_path },
    { "new_sub_path", cr_new_sub_path },
    { "paint", cr_paint },
    { "paint_with_alpha", cr_paint_with_alpha },
    { "rectangle", cr_rectangle },
    { "set_line_cap", cr_set_line_cap },
    { "set_line_width", cr_set_line_width },
    { "set_source_rgb", cr_set_source_rgb },
    { "set_source_rgba", cr_set_source_rgba },
    { "stroke", cr_stroke },
    { 0, 0 }
};

/* vi:set ts=4 sw=4 expandtab: */
