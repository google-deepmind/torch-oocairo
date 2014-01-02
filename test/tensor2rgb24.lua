local Cairo = require 'oocairo'
require 'image'


-- create an RGB image
do
    local img = image.lena():mul(2)
    local data, stride = Cairo.tensor2rgb24(img)
    local surface = Cairo.image_surface_create_from_data(data, 'rgb24', img:size(2), img:size(3), stride)
    surface:write_to_png('lena_colour.png')
end


-- create a grayscale image from green channel
do
    local img = image.lena()
    local data, stride = Cairo.tensor2rgb24(img:narrow(1,2,1), {})
    local surface = Cairo.image_surface_create_from_data(data, 'rgb24', img:size(2), img:size(3), stride)
    surface:write_to_png('lena_green_3.png')
end


-- create a grayscale image from green channel
do
    local img = image.lena()
    local data, stride = Cairo.tensor2rgb24(img[2], {})
    local surface = Cairo.image_surface_create_from_data(data, 'rgb24', img:size(2), img:size(3), stride)
    surface:write_to_png('lena_green_2.png')
end
