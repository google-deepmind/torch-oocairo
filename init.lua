local pkg = require 'liboocairo'


function pkg.rgb2tensor(surface, alpha)

    surface:flush()
    local data = surface:get_data()
    local size = surface:get_width()
    assert(size == surface:get_height())
    local img_b = torch.ByteTensor(size, size, 4)
    img_b:storage():string(data)
    local nchannels = 3
    if alpha == nil or alpha == true then
        nchannels = 4
    end
    local output = torch.ByteTensor(nchannels, size, size)
    for i = 1,nchannels do
        output[i] = img_b:select(3, i)
    end

    return output
end


return pkg;
