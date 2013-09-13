local pkg = require 'liboocairo'

function pkg.rgb2tensor(out, surface)
    surface:flush()
    local data = surface:get_data()
    local size = surface:get_width()
    assert(size == surface:get_height())
    local img_b = torch.ByteTensor(size, size, 4)
    img_b:storage():string(data)
    return out:copy(img_b:select(3,2))
end

return pkg;
