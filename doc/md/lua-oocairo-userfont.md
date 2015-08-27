## lua-oocairo-userfonts

User fonts are created by calling the `user_font_face_create` function
in the main module hash, and passing in a table of callbacks which Cairo
will call when necessary.  The `render_glyph` callback function must be
supplied, but all the others are optional.

A user font can be created and set to be used like this:



    local function render_glyph (...) end

    local font = Cairo.user_font_face_create({
        init = function () print("initializing") end,
        render_glyph = render_glyph,
    })
    cr:set_font_face(font)
    cr:set_font_size(12)

## Callbacks

The following callbacks can be supplied (the names here being the keys
you should use in the table when creating the font).  The `font`
value is always the scaled font object representing the user font at
the size it's being used at.



### init (font, cr, extents)

This is called once, and can be used to set up the font and provide Cairo
with metrics information, by filling in the font `extents` table supplied.
The extents information is in the same format as that returned by the
`font_extents` method on context objects (see `lua-oocairo-context(3)`),
except that all measurements are in font units, which will be scaled
according to the font size.

The `font` value isn't fully created when this function is called, and
using it is likely to break things.

Return values are ignored.

### render_glyph (font, glyph, cr, extents)

This callback is required, and will be called for each glyph which needs
to be displayed.  It should draw the glyph using `cr`, which is a special
Cairo context object which will be scaled to font coordinates.  The
callback must not change the source on `cr` (so multi-coloured fonts are
out of the question, and rendering bitmap images from another surface needs
to be done with the `mask` method).

The value of the `glyph` parameter will be the glyph index number of
the glyph which Cairo wants drawn.  This will be whatever is determined
to be appropriate by the other callbacks, or the Unicode codepoint number
as a default.

Glyph images are cached, so you can't have a font where each occurrence
of the letter ‘e’ is slightly different, for example.

The callback should set the `x_advance` value in the `extents`
table to an appropriate value (measured in font coordinates).
Return values are ignored.

This example draws all glyphs as a simple triangle shape, except
for the space character (assuming that doesn't end up with some
other glyph index):



    local function render_glyph (font, glyph, cr, extents)
        extents.x_advance = 1   -- distance to move to next glyph

        if glyph == 32 then return end  -- space, draw nothing

        cr:move_to(0, 0)
        cr:line_to(1, 0)
        cr:line_to(0, 1)
        cr:fill()
    end

### text_to_glyphs (font, text, want_clusters)

Called to translate text into glyphs.  The `text` parameter is UTF-8
encoded text.  The callback should return a table of glyphs which would
be appropriate for rendering it, including appropriate `x` and `y`
coordinates for each one, in the same format as accepted by the
`show_glyphs` and `show_text_glyphs` methods on context objects.
The coordinates should be in font coordinates.

If the `want_clusters` parameter is true then the font is being used on
a surface which can make use of text cluster information (mapping bytes
in `text` to glyphs returned).  In this case the callback can optionally
return a second value, an array of pairs in the same format as accepted
by the `show_text_glyphs` method on context objects.  If `want_clusters`
is false then any text clusters information returned will just be
ignored.

### unicode_to_glyph (font, unicode)

Called when there is no `text_to_glyphs` callback, or it doesn't provide
any glyphs.  The `unicode` parameter is a Unicode codepoint for a character
being drawn, and the callback should return the glyph index appropriate
for rendering it.  With this simpler callback each Unicode character has to
map to exactly one glyph.
