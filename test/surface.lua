require "runlocal"
require "lunit"
local Cairo = require "oocairo"

module("test.surface", lunit.testcase, package.seeall)

teardown = clean_up_temp_files

function test_image_surface_create ()
    for format, content in pairs({
        rgb24 = "color", argb32 = "color-alpha",
        a8 = "alpha", a1 = "alpha"
    }) do
        local surface = Cairo.image_surface_create(format, 23, 45)
        assert_userdata(surface, "got userdata for " .. format)
        assert_equal("cairo surface object", surface._NAME,
                     "got surface object for " .. format)
        assert_equal("image", surface:get_type())
        assert_equal(format, surface:get_format())
        assert_equal(content, surface:get_content(), "content for " .. format)
        local wd, ht = surface:get_width(), surface:get_height()
        assert_equal(23, wd, "width for " .. format)
        assert_equal(45, ht, "height for " .. format)
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

function test_not_image_surface ()
    local surface = Cairo.svg_surface_create(tmpname(), 300, 200)
    assert_error("get_width on non-image surface",
                 function () surface:get_width() end)
    assert_error("get_height on non-image surface",
                 function () surface:get_height() end)
    assert_error("get_format on non-image surface",
                 function () surface:get_format() end)
end

function test_not_pdf_surface ()
    local surface = Cairo.image_surface_create("rgb24", 30, 20)
    assert_error("set_size on non-PDF surface",
                 function () surface:set_size(40, 50) end)
end

function test_write_to_png_stream ()
    local surface = Cairo.image_surface_create("rgb24", 23, 45)
    local filename = tmpname()
    local fh = assert(io.open(filename, "wb"))
    surface:write_to_png_stream(fh)
    fh:close()
    fh = assert(io.open(filename, "rb"))
    local data = fh:read("*a")
    fh:close()
    assert_match("^\137PNG\13\10", data)
end

function test_write_to_png_string ()
    local surface = Cairo.image_surface_create("rgb24", 23, 45)
    local data = surface:write_to_png_string()
    assert_match("^\137PNG\13\10", data)
end

function test_equality ()
    -- Create two userdatas containing the same pointer value (different
    -- objects in Lua, but the same objects in C, so should be equal).
    local surface1 = Cairo.image_surface_create("rgb24", 23, 45)
    local cr = Cairo.context_create(surface1)
    local surface2 = cr:get_target()
    assert_true(surface1 == surface2)

    -- Create a new, completely separate object, which should be distinct
    -- from any other.
    local surface3 = Cairo.image_surface_create("rgb24", 23, 45)
    assert_false(surface1 == surface3)
end

-- vi:ts=4 sw=4 expandtab
