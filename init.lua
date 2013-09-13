local pkg = require 'liboocairo'


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
    for i = 1,nchannels do
        output[i] = img_b:select(3, i)
    end

    return output
end


return pkg;
