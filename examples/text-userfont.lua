local Cairo = require "oocairo"

if not Cairo.user_font_face_create then
    error("User font example requires module built with Cairo 1.8", 0)
end

local BIT_WIDTH, BIT_HEIGHT, CHAR_GAP = 0.3, 1 / 8, 0.15

function make_bit_font ()
    local function render_glyph (glyph, cr, extents)
        if glyph == 32 then return end      -- space, just default advance

        -- Draw a diamond shape for each bit which is set within the codepoint
        -- number, with eight bits per column.
        local n = glyph
        local x = 0
        while n ~= 0 do
            local byte = n % 0x100
            n = (n - byte) / 0x100
            for i = 0, 7 do
                local bit = byte % 2
                byte = (byte - bit) / 2
                if bit ~= 0 then
                    cr:move_to(x, (i + 0.5) * BIT_HEIGHT)
                    cr:rel_line_to(BIT_WIDTH / 2, BIT_HEIGHT / 2)
                    cr:rel_line_to(BIT_WIDTH / 2, -BIT_HEIGHT / 2)
                    cr:rel_line_to(-BIT_WIDTH / 2, -BIT_HEIGHT / 2)
                    cr:close_path()
                    cr:fill()
                end
            end
            x = x + BIT_WIDTH
        end

        -- Box round edge, 1 pixel wide and aligned on a pixel in device
        -- coordinates (so that it's sharp, and as thin as possible).
        local xpix, ypix = cr:device_to_user_distance(1, 1)
        cr:rectangle(xpix / 2, ypix / 2, x - xpix, 1 - ypix)
        cr:set_line_width(xpix)
        cr:stroke()
        extents.x_advance = x + CHAR_GAP
    end

    return Cairo.user_font_face_create({ render_glyph = render_glyph })
end

local surface = Cairo.image_surface_create("rgb24", 600, 260)
local cr = Cairo.context_create(surface)

-- White background.
cr:set_source_rgb(1, 1, 1)
cr:paint()

cr:set_font_face(make_bit_font())
cr:set_font_size(50)

-- Draw lines of text, each in a different colour.
local x, y = 30, 30
for _, info in ipairs{
    { "The quick brown fox", { .5, 0, 0 } },
    { "jumped over the lazy dog…", { 0, .5, 0 } },
    -- Last two characters in here will have three columns of bits:
    { "Cuneiform: 𒀽𒍬", { 0, 0, .5 } },
} do
    local line, color = unpack(info)
    cr:move_to(x, y)
    cr:set_source_rgb(unpack(color))
    cr:show_text(line)
    y = y + 70
end

surface:write_to_png("text-userfont.png")

-- vi:ts=4 sw=4 expandtab
