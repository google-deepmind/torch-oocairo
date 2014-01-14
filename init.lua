local pkg = require 'liboocairo'
require 'image'

local channel = {} -- r,g,b,a

for i = 1,#pkg.BYTE_ORDER do
    channel[string.char(string.byte(pkg.BYTE_ORDER, i))] = i
end


--[[ Convert an rgb surface to a tensor

Parameters:

- `surface` (cairo surface)
- `alpha` (optional boolean) use the alpha channel

Returns:

either a 3xNxM or 4xNxM ByteTensor with values in the range [0, 255]

--]]
function pkg.rgb2tensor(surface, alpha)

    surface:flush()
    local data = surface:get_data()
    local h = surface:get_height()
    local w = surface:get_width()
    local img_b = torch.ByteTensor(h, w, 4)
    img_b:storage():string(data)

    local nchannels = 3
    if alpha == nil or alpha == true then
        nchannels = 4
    end
    local output = torch.ByteTensor(nchannels, h, w)

    output[1]:copy(img_b:select(3, channel.r))
    output[2]:copy(img_b:select(3, channel.g))
    output[3]:copy(img_b:select(3, channel.b))
    if nchannels == 4 then
        output[4]:copy(img_b:select(3, channel.a))
    end

    return output
end


--[[ Convert tensor to rgb24 image

Parameters:

- `img` (tensor) either 3xHxW or 1xHxW or HxW. If the input has colour channels the
        order is assumed to be RGB. The image is assumed to be in the range [0,1].
- `opt` (table) options for transforming input image (see image.display), see below.

Optional parameters:

- `opt.min` (number) lower-bound for range
- `opt.max` (number) upper-bound for range
- `opt.saturate` (boolean) saturate (useful when min/max are lower than actual min/max)
- `opt.symmetric` (boolean) if on, images will be displayed using a symmetric dynamic range, useful for drawing filters

Returns:

1. rgb24 (string) image in cairo rgb24 format
2. stride (number) image stride

--]]
function pkg.tensor2rgb24(_img, opt)

    opt = opt or {}

    local h = 0
    local w = 0
    local img = _img
    if img:nDimension() == 2 then
        h = img:size(1)
        w = img:size(2)
        img = _img:resize(1, h, w):expand(3, h, w)
    elseif img:nDimension() == 3 then
        h = img:size(2)
        w = img:size(3)
        if img:size(1) == 1 then
            img = _img:expand(3, h, w)
        elseif img:size(1) == 3 then
            img = _img
        else
            error('oocairo.tensor2rgb24: invalid image format, expect 3xHxW or HxW')
        end
    else
        error('oocairo.tensor2rgb24: invalid image format, expect 3xHxW or HxW')
    end

    local img = image.minmax{tensor=img, min=opt.min, max=opt.max, symm=opt.symmetric, saturate=opt.saturate}
    img:mul(256)

    -- Todo: support pre-allocated output
    local img_b = torch.ByteTensor(h, w, 4)

    -- Todo: set the alpha channel
    img_b:select(3, channel.a):fill(0)
    img_b:select(3, channel.r):copy(img[1])
    img_b:select(3, channel.g):copy(img[2])
    img_b:select(3, channel.b):copy(img[3])

    -- Todo: deal with alpha correctly
    local stride = pkg.format_stride_for_width('rgb24', w)
    return img_b:storage():string(), stride
end


return pkg;
