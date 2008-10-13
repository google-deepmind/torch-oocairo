require "test-setup"
require "lunit"
local Cairo = require "oocairo"

module("test.scaled_font", lunit.testcase, package.seeall)

local function mk_surface_cr ()
    local surface = Cairo.image_surface_create("rgb24", 23, 45)
    local cr = Cairo.context_create(surface)
    return surface, cr
end

local function mk_scaled_font (face, size)
    local font_mat = Cairo.matrix_create()
    font_mat:scale(size, size)
    local ctm = Cairo.matrix_create()
    return Cairo.scaled_font_create(face, font_mat, ctm)
end

function test_create ()
    local surface, cr = mk_surface_cr()
    cr:select_font_face("sans", "normal", "normal")
    local face = cr:get_font_face()
    local font = mk_scaled_font(face, 23)
    assert_userdata(font)
    assert_equal("cairo scaled font object", font._NAME)
    assert_true(face == font:get_font_face())
    assert_string(font:get_type())
end

function test_get_set_on_context ()
    local surface, cr = mk_surface_cr()
    local origfont = cr:get_scaled_font()
    assert_userdata(origfont)
    assert_equal("cairo scaled font object", origfont._NAME)

    local newfont = mk_scaled_font(cr:get_font_face(), 23)
    cr:set_scaled_font(newfont)
    assert_true(cr:get_scaled_font() ~= origfont)
end

function test_font_extents ()
    local surface, cr = mk_surface_cr()
    local font = mk_scaled_font(cr:get_font_face(), 23)
    check_font_extents(font:extents())
end

function test_text_extents ()
    local surface, cr = mk_surface_cr()
    local font = mk_scaled_font(cr:get_font_face(), 23)
    check_text_extents(font:text_extents("foo bar quux"))
end

function test_glyph_extents ()
    local surface, cr = mk_surface_cr()
    local font = mk_scaled_font(cr:get_font_face(), 23)
    local x, y = 10, 20
    local glyphs = { {73,10,20}, {82,30,40}, {91,50,60} }
    check_text_extents(font:glyph_extents(glyphs))
end

function test_matrix ()
    local surface, cr = mk_surface_cr()
    local font = mk_scaled_font(cr:get_font_face(), 23)

    local m = font:get_font_matrix()
    check_matrix_elems(m)
    assert_equal("cairo matrix object", m._NAME)

    m = font:get_ctm()
    check_matrix_elems(m)
    assert_equal("cairo matrix object", m._NAME)

    if font.get_scale_matrix then
        m = font:get_scale_matrix()
        check_matrix_elems(m)
        assert_equal("cairo matrix object", m._NAME)
    end
end

-- vi:ts=4 sw=4 expandtab
