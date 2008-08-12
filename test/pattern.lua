require "runlocal"
require "lunit"
local Cairo = require "oocairo"

module("test.pattern", lunit.testcase, package.seeall)

function test_linear_gradient ()
    local g = Cairo.pattern_create_linear(3, 4, 8, 9)
    assert_userdata(g)
    assert_equal("cairo pattern object", g._NAME)

    local x0, y0, x1, y1 = g:get_linear_points()
    assert_equal(3, x0)
    assert_equal(4, y0)
    assert_equal(8, x1)
    assert_equal(9, y1)
end

function test_radial_gradient ()
    local g = Cairo.pattern_create_radial(3, 4, 5, 8, 9, 10)
    assert_userdata(g)
    assert_equal("cairo pattern object", g._NAME)

    local x0, y0, r0, x1, y1, r1 = g:get_radial_circles()
    assert_equal(3, x0)
    assert_equal(4, y0)
    assert_equal(5, r0)
    assert_equal(8, x1)
    assert_equal(9, y1)
    assert_equal(10, r1)
end

function test_wrong_pattern_type ()
    local pat = Cairo.pattern_create_radial(3, 4, 5, 8, 9, 10)
    assert_error("not linear gradient, get points", function ()
        pat:get_linear_points()
    end)

    pat = Cairo.pattern_create_linear(3, 4, 8, 9)
    assert_error("not radial gradient, get circles", function ()
        pat:get_radial_circles()
    end)
end

function test_extend ()
    local pat = Cairo.pattern_create_linear(3, 4, 8, 9)
    assert_error("bad value", function () pat:set_extend("foo") end)
    assert_equal("pad", pat:get_extend(), "default intact after error")
    for _, v in ipairs{ "none", "repeat", "reflect", "pad" } do
        pat:set_extend(v)
        assert_equal(v, pat:get_extend())
    end
end

function test_filter ()
    local pat = Cairo.pattern_create_linear(3, 4, 8, 9)
    assert_error("bad value", function () pat:set_filter("foo") end)
    assert_equal("good", pat:get_filter(), "default intact after error")
    for _, v in ipairs{
        "fast", "good", "best", "nearest", "bilinear", "gaussian"
    } do
        pat:set_filter(v)
        assert_equal(v, pat:get_filter())
    end
end

-- vi:ts=4 sw=4 expandtab
