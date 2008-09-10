require "runlocal"
require "lunit"
local Cairo = require "oocairo"

module("test.svg_surface", lunit.testcase, package.seeall)

local _tmp_filenames = {}
local function tmpname ()
    local filename = os.tmpname()
    _tmp_filenames[#_tmp_filenames + 1] = filename
    return filename
end
function teardown ()
    for _, filename in ipairs(_tmp_filenames) do
        assert(os.remove(filename))
    end
    _tmp_filenames = {}
end

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

function test_not_image_surface ()
    local surface = Cairo.svg_surface_create(tmpname(), 300, 200)
    assert_error("get_width on non-image surface",
                 function () surface:get_width() end)
    assert_error("get_height on non-image surface",
                 function () surface:get_height() end)
    assert_error("get_format on non-image surface",
                 function () surface:get_format() end)
end

-- vi:ts=4 sw=4 expandtab
