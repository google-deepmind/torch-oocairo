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

static const luaL_Reg
fontface_methods[] = {
    { "__gc", fontface_gc },
    { "get_type", fontface_get_type },
    { 0, 0 }
};

/* vi:set ts=4 sw=4 expandtab: */
