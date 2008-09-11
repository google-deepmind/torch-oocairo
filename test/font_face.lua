require "runlocal"
require "lunit"
local Cairo = require "oocairo"

module("test.font_face", lunit.testcase, package.seeall)

function test_type ()
    local surface = Cairo.image_surface_create("rgb24", 23, 45)
    local cr = Cairo.context_create(surface)
    cr:select_font_face("sans", "normal", "normal")
    local font = cr:get_font_face()
    assert_userdata(font)
    assert_equal("cairo font face object", font._NAME)
    assert_equal("toy", font:get_type())
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
