static int
pattern_create_linear (lua_State *L) {
    cairo_pattern_t **obj;
    double x0 = luaL_checknumber(L, 1), y0 = luaL_checknumber(L, 2),
           x1 = luaL_checknumber(L, 3), y1 = luaL_checknumber(L, 4);
    obj = lua_newuserdata(L, sizeof(cairo_pattern_t *));
    *obj = 0;
    luaL_getmetatable(L, MT_NAME_PATTERN);
    lua_setmetatable(L, -2);
    *obj = cairo_pattern_create_linear(x0, y0, x1, y1);
    return 1;
}

static int
pattern_create_radial (lua_State *L) {
    cairo_pattern_t **obj;
    double cx0 = luaL_checknumber(L, 1), cy0 = luaL_checknumber(L, 2),
           radius0 = luaL_checknumber(L, 3),
           cx1 = luaL_checknumber(L, 4), cy1 = luaL_checknumber(L, 5),
           radius1 = luaL_checknumber(L, 6);
    obj = lua_newuserdata(L, sizeof(cairo_pattern_t *));
    *obj = 0;
    luaL_getmetatable(L, MT_NAME_PATTERN);
    lua_setmetatable(L, -2);
    *obj = cairo_pattern_create_radial(cx0, cy0, radius0, cx1, cy1, radius1);
    return 1;
}

static int
pattern_gc (lua_State *L) {
    cairo_pattern_t **obj = luaL_checkudata(L, 1, MT_NAME_PATTERN);
    cairo_pattern_destroy(*obj);
    *obj = 0;
    return 0;
}

static int
pattern_get_extend (lua_State *L) {
    cairo_pattern_t **obj = luaL_checkudata(L, 1, MT_NAME_PATTERN);
    switch (cairo_pattern_get_extend(*obj)) {
        case CAIRO_EXTEND_NONE:    lua_pushliteral(L, "none");      break;
        case CAIRO_EXTEND_REPEAT:  lua_pushliteral(L, "repeat");    break;
        case CAIRO_EXTEND_REFLECT: lua_pushliteral(L, "reflect");   break;
        case CAIRO_EXTEND_PAD:     lua_pushliteral(L, "pad");       break;
        default:                   lua_pushliteral(L, "<invalid>"); break;
    }
    return 1;
}

static int
pattern_get_filter (lua_State *L) {
    cairo_pattern_t **obj = luaL_checkudata(L, 1, MT_NAME_PATTERN);
    switch (cairo_pattern_get_filter(*obj)) {
        case CAIRO_FILTER_FAST:     lua_pushliteral(L, "fast");      break;
        case CAIRO_FILTER_GOOD:     lua_pushliteral(L, "good");      break;
        case CAIRO_FILTER_BEST:     lua_pushliteral(L, "best");      break;
        case CAIRO_FILTER_NEAREST:  lua_pushliteral(L, "nearest");   break;
        case CAIRO_FILTER_BILINEAR: lua_pushliteral(L, "bilinear");  break;
        case CAIRO_FILTER_GAUSSIAN: lua_pushliteral(L, "gaussian");  break;
        default:                    lua_pushliteral(L, "<invalid>"); break;
    }
    return 1;
}

static int
pattern_get_linear_points (lua_State *L) {
    cairo_pattern_t **obj = luaL_checkudata(L, 1, MT_NAME_PATTERN);
    double x0, y0, x1, y1;
    if (cairo_pattern_get_linear_points(*obj, &x0, &y0, &x1, &y1)
            == CAIRO_STATUS_PATTERN_TYPE_MISMATCH)
        luaL_error(L, "pattern is not a linear gradient");
    lua_pushnumber(L, x0);
    lua_pushnumber(L, y0);
    lua_pushnumber(L, x1);
    lua_pushnumber(L, y1);
    return 4;
}

static int
pattern_get_radial_circles (lua_State *L) {
    cairo_pattern_t **obj = luaL_checkudata(L, 1, MT_NAME_PATTERN);
    double x0, y0, r0, x1, y1, r1;
    if (cairo_pattern_get_radial_circles(*obj, &x0, &y0, &r0, &x1, &y1, &r1)
            == CAIRO_STATUS_PATTERN_TYPE_MISMATCH)
        luaL_error(L, "pattern is not a radial gradient");
    lua_pushnumber(L, x0);
    lua_pushnumber(L, y0);
    lua_pushnumber(L, r0);
    lua_pushnumber(L, x1);
    lua_pushnumber(L, y1);
    lua_pushnumber(L, r1);
    return 6;
}

static int
pattern_set_extend (lua_State *L) {
    cairo_pattern_t **obj = luaL_checkudata(L, 1, MT_NAME_PATTERN);
    cairo_pattern_set_extend(*obj,
            extend_values[luaL_checkoption(L, 2, 0, extend_names)]);
    return 0;
}

static int
pattern_set_filter (lua_State *L) {
    cairo_pattern_t **obj = luaL_checkudata(L, 1, MT_NAME_PATTERN);
    cairo_pattern_set_filter(*obj,
            filter_values[luaL_checkoption(L, 2, 0, filter_names)]);
    return 0;
}

static const luaL_Reg
pattern_methods[] = {
    { "__gc", pattern_gc },
    { "get_extend", pattern_get_extend },
    { "get_filter", pattern_get_filter },
    { "get_linear_points", pattern_get_linear_points },
    { "get_radial_circles", pattern_get_radial_circles },
    { "set_extend", pattern_set_extend },
    { "set_filter", pattern_set_filter },
    { 0, 0 }
};

/* vi:set ts=4 sw=4 expandtab: */
