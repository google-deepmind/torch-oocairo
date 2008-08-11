require "runlocal"
require "lunit"
local Cairo = require "oocairo"

module("test.surface", lunit.testcase, package.seeall)

function test_image_surface_create ()
    for format, content in pairs({
        rgb24 = "color", argb32 = "color-alpha",
        a8 = "alpha", a1 = "alpha"
    }) do
        local surface = Cairo.image_surface_create(format, 23, 45)
        assert_userdata(surface, "got userdata for " .. format)
        assert_equal("cairo surface object", surface._NAME,
                     "got surface object for " .. format)
        assert_equal(content, surface:get_content(), "content for " .. format)
    end
end

function test_image_surface_create_bad ()
    assert_error("bad format", function ()
        Cairo.image_surface_create("foo", 23, 45)
    end)
    assert_error("bad width type", function ()
        Cairo.image_surface_create("rgb24", "x", 23)
    end)
    assert_error("negative width value", function ()
        Cairo.image_surface_create("rgb24", -23, 45)
    end)
    assert_error("bad height type", function ()
        Cairo.image_surface_create("rgb24", 23, "x")
    end)
    assert_error("negative height value", function ()
        Cairo.image_surface_create("rgb24", 23, -45)
    end)
end

function test_surface_create_similar ()
    local base = assert(Cairo.image_surface_create("rgb24", 23, 45))
    for _, v in ipairs({ "color", "alpha", "color-alpha" }) do
        local surface = Cairo.surface_create_similar(base, v, 23, 45)
        assert_userdata(surface, "got userdata for " .. v)
        assert_equal("cairo surface object", surface._NAME,
                     "got surface object for " .. v)
        assert_equal(v, surface:get_content(), "right content")
    end
end

function test_surface_create_similar_bad ()
    local base = assert(Cairo.image_surface_create("rgb24", 23, 45))
    assert_error("bad format", function ()
        Cairo.surface_create_similar(base, "foo", 23, 45)
    end)
    assert_error("bad width type", function ()
        Cairo.surface_create_similar(base, "color", "x", 23)
    end)
    assert_error("negative width value", function ()
        Cairo.surface_create_similar(base, "color", -23, 45)
    end)
    assert_error("bad height type", function ()
        Cairo.surface_create_similar(base, "color", 23, "x")
    end)
    assert_error("negative height value", function ()
        Cairo.surface_create_similar(base, "color", 23, -45)
    end)
end

function test_device_offset ()
    local surface = Cairo.image_surface_create("rgb24", 23, 45)
    local x, y = surface:get_device_offset()
    assert_equal(0, x)
    assert_equal(0, y)
    surface:set_device_offset(-5, 3.2)
    x, y = surface:get_device_offset()
    assert_equal(-5, x)
    assert_equal(3.2, y)
end

function test_fallback_resolution ()
    if Cairo._CAIRO_VERSION > "1.8" then
        local surface = Cairo.image_surface_create("rgb24", 23, 45)
        local x, y = surface:get_fallback_resolution()
        assert_equal(300, x)
        assert_equal(300, y)
        surface:set_fallback_resolution(123, 456)
        x, y = surface:get_fallback_resolution()
        assert_equal(123, x)
        assert_equal(456, y)
    end
end

function test_type ()
    local surface = Cairo.image_surface_create("rgb24", 23, 45)
    assert_equal("image", surface:get_type())
end

-- vi:ts=4 sw=4 expandtab
