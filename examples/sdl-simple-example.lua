-- A simple example which just draws some arbitrary shapes on an SDL surface.

sdl = require 'sdl2'
cairo = require 'oocairo'

sdl.init(sdl.INIT_VIDEO)
SDLsurface = sdl.createRGBSurface(0, 200, 200, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0);
sdl.lockSurface(SDLsurface)

surface = cairo.image_surface_create_for_data(SDLsurface.pixels, "rgb24", SDLsurface.w, SDLsurface.h, SDLsurface.pitch)
cr = cairo.context_create(surface)

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

sdl.unlockSurface(SDLsurface)
sdl.saveBMP(SDLsurface, "sdl-simple-example.bmp")
sdl.quit()
