-- Test some of the 'operator' types.  This example is a translation of a C
-- program from a Cairo tutorial here:
--     http://zetcode.com/tutorials/cairographicstutorial/compositing/

local Cairo = require "oocairo"

local IMG_WD, IMG_HT = 510, 120

local function draw (cr, x, w, h, op)
    local first = Cairo.surface_create_similar(cr:get_target(), "color-alpha",
                                               w, h)
    local second = Cairo.surface_create_similar(cr:get_target(), "color-alpha",
                                                w, h)

    local first_cr = Cairo.context_create(first)
    first_cr:set_source_rgb(0, 0, 0.4)
    first_cr:rectangle(x, 20, 50, 50)
    first_cr:fill()

    local second_cr = Cairo.context_create(second)
    second_cr:set_source_rgb(0.5, 0.5, 0)
    second_cr:rectangle(x + 10, 40, 50, 50)
    second_cr:fill()

    first_cr:set_operator(op)
    first_cr:set_source(second, 0, 0)
    first_cr:paint()

    cr:set_source(first, 0, 0)
    cr:paint()
end

local surface = Cairo.image_surface_create("rgb24", IMG_WD, IMG_HT)
local cr = Cairo.context_create(surface)

-- White background.
cr:set_source_rgb(1, 1, 1)
cr:paint()

local x, y = 20, 20
local ops = { "dest-over", "dest-in", "out", "add", "atop", "dest-atop" }
for _, op in ipairs(ops) do
    draw(cr, x, IMG_WD, IMG_HT, op)
    x = x + 80
end

surface:write_to_png("compositing.png")

-- vi:ts=4 sw=4 expandtab
