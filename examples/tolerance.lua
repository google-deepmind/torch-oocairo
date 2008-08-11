-- Draw curves using different 'tolerance' values.

require "runlocal"
local Cairo = require "oocairo"

local DEMO_WD, MARGIN = 140, 50
local IMG_WD, IMG_HT = DEMO_WD * 3 + MARGIN * 4, 300

local surface = Cairo.image_surface_create("rgb24", IMG_WD, IMG_HT)
local cr = Cairo.context_create(surface)

-- White background.
cr:set_source_rgb(1, 1, 1)
cr:paint()

local x, y = MARGIN, IMG_HT - MARGIN
for _, tolerance in ipairs{ 0.1, 2.5, 10 } do
    cr:set_tolerance(tolerance)
    cr:move_to(x, y)
    cr:curve_to(x + 0.1*DEMO_WD, y - 2.5*DEMO_WD,
                x + 0.7*DEMO_WD, y - 1.5*DEMO_WD,
                x + DEMO_WD, y)
    cr:set_source_rgb(0, 0, 0)
    cr:stroke()
    x = x + DEMO_WD + MARGIN
end

surface:write_to_png("tolerance.png")

-- vi:ts=4 sw=4 expandtab
