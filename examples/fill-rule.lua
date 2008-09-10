-- Demonstrate the effect of different fill rules.

require "runlocal"
local Cairo = require "oocairo"

local RADIUS, NUM_POINTS, MARGIN = 100, 5, 30
local IMG_WD, IMG_HT = RADIUS * 4 + MARGIN * 3, RADIUS * 2 + MARGIN * 2
local PI = math.asin(1) * 2

local surface = Cairo.image_surface_create("rgb24", IMG_WD, IMG_HT)
local cr = Cairo.context_create(surface)

-- White background.
cr:set_source_rgb(1, 1, 1)
cr:paint()

local function draw_star (x, y)
    local ang_step = (2 * PI) / NUM_POINTS
    cr:move_to(x + RADIUS, y)
    local ang = 2 * ang_step
    for _ = 1, NUM_POINTS - 1 do
        cr:line_to(x + RADIUS * math.cos(ang), y + RADIUS * math.sin(ang))
        ang = ang + 2 * ang_step
    end
    cr:close_path()
    cr:set_source_rgb(0, 0, 0)
    cr:fill()
end

cr:set_fill_rule("winding")     -- this is the default anyway
draw_star(MARGIN + RADIUS, MARGIN + RADIUS)
cr:set_fill_rule("even-odd")
draw_star(2 * MARGIN + 3 * RADIUS, MARGIN + RADIUS)

-- As a test of different ways to write the output, this particular example
-- program writes the finished PNG file into a string, before writing that
-- data out into an actual file.  The same technique could be used in
-- conjunction with the 'gd' module to convert the output into a JPEG or GIF.
local data = surface:write_to_png_string()
local fh = assert(io.open("fill-rule.png", "wb"))
fh:write(data)
fh:close()

-- vi:ts=4 sw=4 expandtab
