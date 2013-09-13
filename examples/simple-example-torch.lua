-- A simple example which just draws some arbitrary shapes.

local Cairo = require "oocairo"
require 'image'

local surface = Cairo.image_surface_create("rgb24", 200, 200)
local cr = Cairo.context_create(surface)

cr:set_source_rgb(1, 1, 1)
cr:paint()

cr:move_to(0, 0)
cr:line_to(190, 100)
cr:line_to(100, 185)
cr:line_to(200, 200)
cr:line_to(30, 130)
cr:close_path()
cr:set_source_rgb(0.8, 0.4, 1)
cr:fill()

cr:move_to(180, 30)
cr:line_to(100, 20)
cr:line_to(80, 120)
cr:set_source_rgb(0.5, 0.7, 0.3)
cr:fill_preserve()
cr:set_source_rgb(0, 0, 0)
cr:set_line_width(3)
cr:stroke()

-- The returned image is a ByteTensor with values in the range [0, 255], so
-- convert in order to use with the image package.
local t_img = Cairo.rgb2tensor(surface):float():div(255)

image.savePNG("simple-example-torch.png", t_img)

-- If the alpha channel is not desired switch it off:
local t_img_noalpha = Cairo.rgb2tensor(surface, false):float():div(255)
assert(t_img_noalpha:size(1) == 3)

-- vi:ts=4 sw=4 expandtab
