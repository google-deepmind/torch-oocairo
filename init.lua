local pkg = require 'liboocairo'

local channel = {} -- r,g,b,a

for i = 1,#pkg.BYTE_ORDER do
    channel[string.char(string.byte(pkg.BYTE_ORDER, i))] = i
end



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


return pkg;
