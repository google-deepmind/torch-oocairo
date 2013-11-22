-- Drawing with Cairo on an SDL window.

sdl = require 'sdl2'
cairo = require 'oocairo'

require 'util'

sdl.init(sdl.INIT_VIDEO)
local window = sdl.createWindow('Test Window', sdl.WINDOWPOS_CENTERED, sdl.WINDOWPOS_CENTERED, 200, 200, sdl.WINDOW_RESIZABLE)
local SDLsurface = sdl.getWindowSurface(window)

local surface = cairo.image_surface_create_for_data(SDLsurface.pixels, "rgb24", SDLsurface.w, SDLsurface.h, SDLsurface.pitch)
local cr = cairo.context_create(surface)

function paint(i)
    sdl.lockSurface(SDLsurface)

    cr:set_source_rgb(1, 1, 1)
    cr:paint()

    cr:move_to(0, 0)
    cr:line_to(150+i, 100)
    cr:line_to(100, 185)
    cr:line_to(200, 200)
    cr:line_to(30, 100+i)
    cr:close_path()
    cr:set_source_rgb(0.8, 0.4, 1)
    cr:fill()

    cr:move_to(180, 30+i)
    cr:line_to(100, 20+i)
    cr:line_to(80, 120+i)
    cr:set_source_rgb(0.5, 0.7, 0.3)
    cr:fill_preserve()
    cr:set_source_rgb(0, 0, 0)
    cr:set_line_width(3)
    cr:stroke()

    sdl.unlockSurface(SDLsurface)
    sdl.updateWindowSurface(window)
end

for j = 1,1 do
    for i = 0,50 do
        paint(i)
        util.sleep(0.05)
    end
    for i = 50,0,-1 do
        paint(i)
        util.sleep(0.05)
    end
end

sdl.quit()
