##  lua-oocairo-scaledfont - Scaled font face object

Scaled font objects represent Cairo font faces which have been scaled to
a particular point size.  They can be created from a font face object with
the `scaled_font_create` function in the main module table (see
`lua-oocairo(3)`), or the `get_scaled_font` method on context objects
(see `lua-oocairo-context(3)`).

Scaled font objects can be compared with the `==` operator.  This will compare
the identity of the objects, so different Lua objects which refer to the
same `cairo_font_face_t` will compare as equal.

## Methods

Scaled font objects provide the following methods.

### font:extents ()

Returns a table containing metrics information about the font.  The return
value is in the same format as that of the `font_extents` method on context
objects (see `lua-oocairo-context(3)`).

### font:get_ctm ()

Returns the coordinate transformation matrix (CTM) associated with the
font.  This maps user coordinates into device coordinates on the surface
with which the font is used.
See `lua-oocairo-matrix(3)` for details of the return value type.

### font:get_font_face ()

Returns an object representing the unscaled font face used to create this
font.  See `lua-oocairo-fontface(3)` for details of its methods.

### font:get_font_matrix ()

Returns the matrix representing the scaling of the font into user coordinates.
See `lua-oocairo-matrix(3)` for details of the return value type.

### cr:get_font_options ()

Returns a font options object (see `lua-oocairo-fontopt(3)`) containing
the rendering options that were supplied when creating this font.

### font:get_scale_matrix ()

Returns a matrix table representing the scaling of the font to the coordinate
space of the surface it is used on.  This is the matrix obtained by
multiplying the font matrix by the surface CTM.  See `lua-oocairo-matrix(3)`
for details of the return value type.
Only available with `Cairo 1.8` or better.

### font:get_type ()

Returns a string indicating which back-end library provides the font face
information.  The return value will be one of the same strings returned
by the `get_type` method on a font face object
(see `lua-oocairo-fontface(3)`).

### font:glyph_extents (glyphs)

Same as `font:text_extents()`, but instead of a string, the `glyphs` value
should be table in the format accepted by the `show_glyphs()` method
on context objects (see `lua-oocairo-context(3)`).

### font:text_extents (text)

Returns a table containing measurements of the given UTF-8 text as it would
be rendered in this font.  The return value is in the same format as that
of the `text_extents` method on context objects (see
`lua-oocairo-context(3)`).

### font:text_to_glyphs (x, y, text)

Given some UTF-8 text in the string `text`, and starting coordinates of
`x` and `y`, this works out which glyphs need to be drawn to represent
the text, and which glyphs correspond to the bytes in `text`.  The method
returns two tables, the first an array of glyphs suitable for supplying to
methods like `show_glyphs` and `glyph_extents`, and the second is a table
of text clusters suitable for supplying to the `show_text_glyphs` method
on a context object.

Only available with `Cairo 1.8` or better.
