require "runlocal"
require "lunit"
local Cairo = require "oocairo"

module("test.svg_surface", lunit.testcase, package.seeall)

teardown = clean_up_temp_files

function test_svg_versions ()
    local versions = Cairo.svg_get_versions()
    assert_table(versions)
    for k, v in pairs(versions) do
        assert_number(k)
        assert_string(v)
    end
end

-- Arbitrary drawing just to make sure there's something in the SVG file.
local function draw_stuff (surface)
    local cr = Cairo.context_create(surface)
    cr:move_to(50, 50)
    cr:line_to(250, 150)
    cr:set_line_width(5)
    cr:set_source_rgb(1, 0, 0.5)
    cr:stroke()
end

function test_create ()
    local filename_normal = tmpname()
    local surface = Cairo.svg_surface_create(filename_normal, 300, 200)
    assert_userdata(surface)
    assert_equal("cairo surface object", surface._NAME)
    assert_equal("svg", surface:get_type())

    draw_stuff(surface)
    surface:finish()
    local fh = assert(io.open(filename_normal, "rb"))
    local svg = fh:read("*a")
    assert_match("<svg", svg)
end

-- vi:ts=4 sw=4 expandtab