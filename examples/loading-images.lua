require "runlocal"
local Cairo = require "oocairo"

local PIC_FILENAME = "examples/images/wood1.png"
local PIC_WD, PIC_HT = 96, 96

local MARGIN = 10
local IMG_WD, IMG_HT = PIC_WD * 3 + MARGIN * 4, PIC_HT + MARGIN * 2

-- These three functions do the same thing (loading a PNG file and creating
-- a surface object from it), but in different ways.  Loading directly from
-- a filename is convenient, but using a file handle is more flexible.
-- Loading from a string allows images from other libraries like GD to be
-- converted into Cairo objects without going through a temporary file.
function load_pic_from_filename ()
    return Cairo.image_surface_create_from_png(PIC_FILENAME)
end

function load_pic_from_file_handle ()
    local fh = assert(io.open(PIC_FILENAME, "rb"))
    local surface = Cairo.image_surface_create_from_png_stream(fh)
    fh:close()
    return surface
end

function load_pic_from_string ()
    local fh = assert(io.open(PIC_FILENAME, "rb"))
    local picdata = fh:read("*a")
    fh:close()
    return Cairo.image_surface_create_from_png_string(picdata)
end

local surface = Cairo.image_surface_create("rgb24", IMG_WD, IMG_HT)
local cr = Cairo.context_create(surface)

for i, func in ipairs{ load_pic_from_filename,
                       load_pic_from_file_handle,
                       load_pic_from_string }
do
    cr:set_source(func(), MARGIN * i + PIC_WD * (i - 1), MARGIN)
    cr:paint()
end

surface:write_to_png("loading-images.png")

-- vi:ts=4 sw=4 expandtab
