#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
static int
toy_font_face_create (lua_State *L) {
    cairo_font_face_t **face = create_fontface_userdata(L);
    *face = cairo_toy_font_face_create(luaL_checkstring(L, 1),
            font_slant_values[luaL_checkoption(L, 2, "normal",
                                               font_slant_names)],
            font_weight_values[luaL_checkoption(L, 3, "normal",
                                                font_weight_names)]);
    return 1;
}
#endif

static int
fontface_eq (lua_State *L) {
    cairo_font_face_t **obj1 = luaL_checkudata(L, 1, MT_NAME_FONTFACE);
    cairo_font_face_t **obj2 = luaL_checkudata(L, 2, MT_NAME_FONTFACE);
    lua_pushboolean(L, *obj1 == *obj2);
    return 1;
}

static int
fontface_gc (lua_State *L) {
    cairo_font_face_t **obj = luaL_checkudata(L, 1, MT_NAME_FONTFACE);
    cairo_font_face_destroy(*obj);
    *obj = 0;
    return 0;
}

static int
fontface_get_type (lua_State *L) {
    cairo_font_face_t **obj = luaL_checkudata(L, 1, MT_NAME_FONTFACE);
    const char *s;
    switch (cairo_font_face_get_type(*obj)) {
        case CAIRO_FONT_TYPE_TOY:    s = "toy";    break;
        case CAIRO_FONT_TYPE_FT:     s = "ft";     break;
        case CAIRO_FONT_TYPE_WIN32:  s = "win32";  break;
        case CAIRO_FONT_TYPE_QUARTZ: s = "quartz"; break;
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
        case CAIRO_FONT_TYPE_USER:   s = "user";   break;
#endif
        default:                                s = "<invalid>";
    }
    lua_pushstring(L, s);
    return 1;
}

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
static int
fontface_get_family (lua_State *L) {
    cairo_font_face_t **obj = luaL_checkudata(L, 1, MT_NAME_FONTFACE);
    if (cairo_font_face_get_type(*obj) != CAIRO_FONT_TYPE_TOY)
        return luaL_error(L, "'get_family' only works on toy font faces");
    lua_pushstring(L, cairo_toy_font_face_get_family(*obj));
    return 1;
}

static int
fontface_get_slant (lua_State *L) {
    cairo_font_face_t **obj = luaL_checkudata(L, 1, MT_NAME_FONTFACE);
    if (cairo_font_face_get_type(*obj) != CAIRO_FONT_TYPE_TOY)
        return luaL_error(L, "'get_slant' only works on toy font faces");
    switch (cairo_toy_font_face_get_slant(*obj)) {
        case CAIRO_FONT_SLANT_NORMAL:  lua_pushliteral(L, "normal");    break;
        case CAIRO_FONT_SLANT_ITALIC:  lua_pushliteral(L, "italic");    break;
        case CAIRO_FONT_SLANT_OBLIQUE: lua_pushliteral(L, "oblique");   break;
        default:                       lua_pushliteral(L, "<invalid>");
    }
    return 1;
}

static int
fontface_get_weight (lua_State *L) {
    cairo_font_face_t **obj = luaL_checkudata(L, 1, MT_NAME_FONTFACE);
    if (cairo_font_face_get_type(*obj) != CAIRO_FONT_TYPE_TOY)
        return luaL_error(L, "'get_weight' only works on toy font faces");
    switch (cairo_toy_font_face_get_weight(*obj)) {
        case CAIRO_FONT_WEIGHT_NORMAL: lua_pushliteral(L, "normal");    break;
        case CAIRO_FONT_WEIGHT_BOLD:   lua_pushliteral(L, "bold");      break;
        default:                       lua_pushliteral(L, "<invalid>");
    }
    return 1;
}
#endif

static const luaL_Reg
fontface_methods[] = {
    { "__eq", fontface_eq },
    { "__gc", fontface_gc },
    { "get_type", fontface_get_type },
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
    { "get_family", fontface_get_family },
    { "get_slant", fontface_get_slant },
    { "get_weight", fontface_get_weight },
#endif
    { 0, 0 }
};

/* vi:set ts=4 sw=4 expandtab: */
