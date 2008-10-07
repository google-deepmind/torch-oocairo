require "test-setup"
require "lunit"
local Cairo = require "oocairo"

module("test.font_face", lunit.testcase, package.seeall)

function test_select_get_set_font_face ()
    local surface = Cairo.image_surface_create("rgb24", 23, 45)
    local cr = Cairo.context_create(surface)

    cr:select_font_face("sans", "normal", "normal")
    local sans = cr:get_font_face()
    assert_userdata(sans)
    assert_equal("cairo font face object", sans._NAME)
    assert_equal("toy", sans:get_type())

    cr:select_font_face("serif", "italic", "bold")
    local serif = cr:get_font_face()
    cr:set_font_face(sans)
    assert_equal(sans, cr:get_font_face())
    cr:set_font_face(serif)
    assert_equal(serif, cr:get_font_face())
end

function test_toy_create_and_accessors ()
    if Cairo.toy_font_face_create then
        local font = Cairo.toy_font_face_create("serif", "italic", "bold")
        assert_userdata(font)
        assert_equal("cairo font face object", font._NAME)
        assert_equal("toy", font:get_type())
        assert_equal("serif", font:get_family())
        assert_equal("italic", font:get_slant())
        assert_equal("bold", font:get_weight())
    else
        local surface = Cairo.image_surface_create("rgb24", 23, 45)
        local font = Cairo.context_create(surface):get_font_face()
        assert_nil(Cairo.toy_font_face_create)
        assert_nil(font.get_family)
        assert_nil(font.get_slant)
        assert_nil(font.get_weight)
    end
end

function test_equality ()
    -- Different userdatas, same Cairo object.
    local surface = Cairo.image_surface_create("rgb24", 23, 45)
    local cr = Cairo.context_create(surface)
    local font1 = cr:get_font_face()
    local font2 = cr:get_font_face()
    assert_true(font1 == font2)

    -- Different userdatas, different Cairo objects.
    cr:select_font_face("serif")
    local font3 = cr:get_font_face()
    assert_false(font1 == font3)
end

-- vi:ts=4 sw=4 expandtab
