## lua-oocairo-context - Drawing context objects

A context object is used to perform all the drawing operations on a
Cairo surface.  Context objects can be created by calling the
`context_create` function in the module table returned from `require`
and passing in a surface object.

## Methods

The following methods are available on context objects:



### cr:append_path (path)

Add the path instructions from `path` to the current path of `cr`.
The `path` value should be an object returned from the `cr:copy_path()`
or `cr:copy_path_flat()` method.

### cr:arc (cx, cy, radius, angle1, angle2)

Add a curved segment to the current path which approximates a section
of a circle centered at point (`cx`,`nbsp``cy`) with a radius of `radius`.
The two angles determine which part of the circle to draw.  They are both
measured in radians.  For a complete circle `angle1` should be zero, and
`angle2` should be 2`pi`.

    -- Draw a complete circle.
    local PI = 2 * math.asin(1)
    local x, y, radius = 50, 100, 23
    cr:arc(x, y, radius, 0, 2 * PI)
    cr:stroke()

All five arguments to the method must be numbers.

A straight line will be added to the path from the current point to the
start of the arc, if there is a current point.  To avoid this, call
`cr:new_sub_path()` before adding the arc.

### cr:arc_negative (cx, cy, radius, angle1, angle2)

Same as `cr:arc()`, but goes the opposite way round the circle.

### cr:clip ()

From now on clip anything outside the current path, preventing it from
being drawn, and then discard the path.

### cr:clip_extents ()

Returns four numbers in two pairs, the `x` and `y` coordinates of the
corners of a rectangle which encloses the area of the surface where drawing
can still take place when clipping is taken in to account.

### cr:clip_preserve ()

Same as `cr:clip()` but keeps the current path.

### cr:close_path ()

Add a straight line segment to the path from the current point to the
point where the path started from (the first `cr:move_to()` position.

### cr:copy_path ()

Returns an object containing details of the instructions which define
the current path on `cr` at the time this is called, as defined by any
calls to methods like `cr:move_to()` and `cr:line_to()` made so far.
The return value can be passed to `cr:append_path()`.  See
`lua-oocairo-path(3)` for details about what else can be done with
a path object.

### cr:copy_path_flat ()

Same as `cr:copy_path()` except that the path returned will not contain
any curves.  If there are curves in the current path they will first be
converted into a series of straight lines.

### cr:curve_to (c1x, c1y, c2x, c2y, x, y)

Add a curved segment to the current path, from the current point to
(`x`,`nbsp``y`), with the control points (`c1x`,`nbsp``c1y`) and
(`c2x`,`nbsp``c2y`).  All six arguments must be numbers.

### cr:device_to_user (x, y)

Returns two numbers, the position given by the numbers `x` and `y`
converted from device coordinates to user coordinates.

### cr:device_to_user_distance (x, y)

Returns two numbers, the distance given by the numbers `x` and `y`
converted from device coordinates to user coordinates.

### cr:fill ()

Fill in the inside of the current path using colour from the current source.

### cr:fill_extents ()

Returns four numbers giving two corners of the rectangular area which would
be affected by a call to `cr:fill()` with the current path.

### cr:fill_preserve ()

Same as `cr:fill()` but the current path is left intact for use in
further drawing operations.

### cr:font_extents ()

Return a table containing metrics for the currently selected font at its
current size, in user-space coordinates (so `not` scaled etc. according to
the current transformation matrix).  The table will contain the following
entries:



* ascent

Distance font extends above baseline.

* descent

Distance font extends below baseline.

* height

Recommended line height (vertical distance from one baseline to the next).

* max_x_advance

Largest distance any glyph will advance the text horizontally.

* max_y_advance

Largest distance any glyph will advance the text vertically.



### cr:get_antialias ()

Get the current antialiasing mode, which will be one of the strings accepted
by the `cr:set_antialias()` method.

### cr:get_current_point ()

Return two numbers, the coordinates of the current point.  Returns no values
if there is no current point.

### cr:get_dash ()

Return two values, a table and a number, which describe the current dash
pattern and offset in the same format as accepted by `cr:set_dash()`.

### cr:get_fill_rule ()

Return the current fill rule as one of the strings accepted by the
`cr:set_fill_rule()` method.

### cr:get_font_face ()

Get the currently selected font face (the unscaled font).  See
`lua-oocairo-fontface(3)` for details on font face objects.

### cr:get_font_matrix ()

Returns the transformation matrix used for the current font.

### cr:get_font_options ()

Returns a new font options object containing the options set for font
rendering with this context.  The options won't reflect any defaults
provided by the surface, just the options set with the
`cr:set_font_options()` method.

See `lua-oocairo-fontopt(3)` for more about font options objects.

### cr:get_group_target ()

Returns the surface which is currently the destination of drawing
operations.  This is usually the same as `cr:get_target()`, except
when `cr:push_group()` has created a new temporary surface.

### cr:get_line_cap ()

Returns one of the strings accepted by `cr:set_line_cap()`.

### cr:get_line_join ()

Returns one of the strings accepted by `cr:set_line_join()`.

### cr:get_line_width ()

Returns a number, the current line width in pixels.

### cr:get_matrix ()

Returns the current transformation matrix as a table of six numbers.

### cr:get_miter_limit ()

Returns the current miter limit as a number.

### cr:get_operator ()

Returns a string indicating the current compositing operator, as set
by the `cr:set_operator()` method.

### cr:get_source ()

Return the pattern object current being used as the source.  See
`lua-oocairo-pattern(3)` for methods available on pattern objects.

### cr:get_target ()

Returns the surface on which this context will draw.

### cr:get_tolerance ()

Return a single number, as set by `cr:set_tolerance()`.

### cr:glyph_extents (glyphs)

Same as `cr:text_extents()`, but instead of a string, the `glyphs` value
should be table in the format accepted by `cr:show_glyphs()`.

### cr:glyph_path (glyphs)

Same as `cr:text_path()`, but instead of string takes a table of glyphs
like `cr:show_glyphs()`.

### cr:has_current_point ()

Returns a boolean value, true if there is a current point (as defined with
`cr:move_to()`).

### cr:identity_matrix ()

Set the current transformation matrix to the identity matrix, resetting
any transformations which had previously been applied.

### cr:in_fill (x, y)

Return a boolean value, true if the coordinates are inside the area which
would be filled by the `cr:fill()` method if it was called with the
current path.

### cr:in_stroke (x, y)

Return a boolean value, true if the coordinates are inside the area which
would be painted by the `cr:stroke()` method if it was called with the
current path and line drawing options.

### cr:line_to (x, y)

Add a straight line segment to the path, from the current point to the
coordinates given.

### cr:mask (pattern)

### cr:mask (surface, x, y)

Both of these set a mask which will be used to filter all drawing.  The
parts of the mask which are transparent will still be drawn, but the opaque
parts will not be, and semitransparent pixels work proportionately.

This method will accept either a pattern object or a surface object,
just as for the `cr:set_source()` method.

The mask only has an alpha channel, so any colour information in the
pattern or surface is ignored.

### cr:move_to (x, y)

Set the current point to the coordinates given.

### cr:new_path ()

Discard the current path.

### cr:new_sub_path ()

Disconnect from the current path, so anything added to it will form a
separate shape.  Mainly used to avoid the extra straight line you get
with the `cr:arc()` method.

### cr:paint ()

Copy the source to the surface across the whole surface.  This is the same
as filling a rectangle enclosing the whole area of the surface.

### cr:paint_with_alpha (alpha)

Same as `cr:paint()` except the source is blended at the proportion given
by `alpha`.  An alpha value of one means complete opacity (same as
`cr:paint()`), and zero means complete transparency (no effect).

### cr:path_extents ()

Returns four numbers giving two corners of the rectangular bounding box
of the current path.

### cr:pop_group ()

Finish with the temporary surface created by `cr:push_group()` and
discard it.

### cr:pop_group_to_source ()

Finish with the temporary surface created by `cr:push_group()` and
set it as the source image, as if with a call to `cr:set_source()`.

### cr:push_group ([content])

Begin drawing on a new temporary surface.  If `content` is specified
then it can limit what the temporary surface can store.  It should be
one of the values accepted by the `surface_create_similar` function
defined in the module table (see `lua-oocairo(3)`).

### cr:rectangle (x, y, width, height)

Add the outline of a rectangle to the current path.  All four arguments
must be numbers.  The rectangle path will be closed at the end.

### cr:rel_curve_to (c1x, c1y, c2x, c2y, x, y)

Same as `cr:curve_to()` but with coordinates relative to the current point.

### cr:rel_line_to (x, y)

Same as `cr:line_to()` but with coordinates relative to the current point.

### cr:rel_move_to (x, y)

Same as `cr:move_to()` but with coordinates relative to the current point.

### cr:reset_clip ()

Reset there to be no clipping.

### cr:restore ()

Restore the state previously saved with a call to `cr:save()`.

### cr:rotate (angle)

Apply a rotation to the current transformation matrix, by `angle`
radians.

### cr:save ()

Save the current settings for this context on a stack, so that they can
be altered and then restored to their previous state with a call to
`cr:restore()`.  Multiple saves and restores can be nested.

### cr:scale (sx, sy)

Adjust the current transformation matrix to scale everything by the
amount specified by the numbers `sx` and `sy`.

### cr:select_font_face (familyname, slant, weight)

Set the font used by methods such as `cr:show_text()` to the font
family named by the string `familyname` with the specified styling.

The `slant` and `weight` values are both optional.

If specified, `slant` must be one of the following strings:



* `normal` (the default)

* `italic`

* `oblique`



If specified, `weight` must be one of the following strings:



* `normal` (the default)

* `bold`



### cr:set_antialias (antialiastype)

Set the antialiasing mode used for drawing.  The argument must be one
of the following strings:



* `default`

* `none`

* `gray`

* `subpixel`



Additionally, the argument can be one of the Lua boolean values (`true`
or `false`, but not `nil`), which will simply turn AA on or off by
setting it to `default` or `none`.

### cr:set_dash (dashpattern, offset)

Set the dash pattern used for stroking lines.  The `dashpattern`
value must be a table, which is an array of numbers defining how long
each dash or gap should be.  If there is only one number then it will
be treated as if the same number were repeated a second time.

The `offset` must be a number giving the distance into the pattern
at which the stroke should start being drawn.

### cr:set_fill_rule (fillrule)

Choose the algorithm used for deciding what is inside or outside of a
path when filling its inside.  The argument must be one of the following
strings:



* `winding`

* `even-odd`



### cr:set_font_face (font)

Set the selected font to the one given, or to the default font if `font`
is nil.  See `lua-oocairo-fontface(3)` for details on font face objects.

### cr:set_font_matrix (matrix)

Set the transformation matrix used for fonts.  Can be used for rotating
text or slanting the font in unusual ways for example.  The `matrix`
must be a table of six numbers.

### cr:set_font_options (options)

Set the options used for rendering fonts.  The value of `options` must
be a font options object (see `lua-oocairo-fontopt(3)`).  The current
options can be retrieved with the `cr:get_font_options()` method.

### cr:set_font_size (size)

Set the font size to use for methods like `cr:show_text()`.
The `size` value must be a number.

### cr:set_line_cap (style)

Set the style with which the ends of a path are stroked.  The `style`
value must be one of the following strings:



* butt

Line ends exactly at end point of path.

* round

Line is extended in a circle round the end of the path.

* square

Line is extended in a square round the end of the path.



### cr:set_line_join (style)

Set the style with which corners of a path are stroked.  The `style`
value must be one of the following strings:



* miter

Outside edge of stroke goes straight on to a sharp point.

* round

Outside edge of line curves in a circle.

* bevel

Outside edge of line is flattened off.



### cr:set_line_width (width)

Set the width of lines drawn with the `cr:stroke()` method to the
number given, in pixels.  The default is two pixels wide.

### cr:set_matrix (matrix)

Set the current transformation matrix to `matrix`, which must be a table
containing six numbers.

### cr:set_miter_limit (limit)

Set the miter limit to the number given.

### cr:set_operator (operator)

Change the operator used for compositing new paint onto what has already
been drawn.  The value must be one of the following strings:



* `add`

* `atop`

* `clear`

* `dest`

* `dest-atop`

* `dest-in`

* `dest-out`

* `dest-over`

* `in`

* `out`

* `over` (this is the default)

* `saturate`

* `source`

* `xor`



### cr:set_source (pattern)

### cr:set_source (surface, x, y)

Set the source to a ready-made pattern object, or to a surface object with
the given offset.  Setting a pattern object is useful for applying a gradient.
Setting a surface is useful for drawing the contents of one surface onto
another.  The `x` and `y` values indicate what position on `surface`
should be drawn at the origin.

### cr:set_source_gdk_color (c, [alpha])

Set the source to a solid colour given by a GdkColor value from the
Lua-Gnome library, and optionally an alpha value using the same range
of numbers.  This method is exactly the same as calling `set_source_rgb`
(or `set_source_rgba` if `alpha` is present), except that the red,
green, and blue components are retrieved from `c` by treating it as a
table, and all the numbers are treated as `16 bit` unsigned integers
and scaled to the range used by Cairo.

If present and not nil, the opacity value `alpha` must be a number and
is expected to range from 0 for fully transparent to 0xFFFF (65535) for
completely opaque.

See the example program `gtk-drawing.lua` for one use of this method
in setting a source colour and opacity from a GtkColorButton widget.

### cr:set_source_pixbuf (pixbuf, x, y)

### cr:set_source_pixmap (pixmap, x, y)

Set the source to an image given as a Gdk object.  The value of `pixbuf`
must be a GdkPixbuf object from the Lua-Gnome library, and that of `pixmap`
must be a GdkPixmap object.

The `x` and `y` coordinates are the same as in `cr:set_source()`.

Usually, you'll want to use `set_source_pixbuf`, because pixbufs can
include transparency information.

See example program `gtk-image.lua` for an example of how to use these
methods.

### cr:set_source_rgb (r, g, b)

Set the source to a solid colour.  The three values must be numbers from
zero to one.

### cr:set_source_rgba (r, g, b, a)

Set the source to a solid colour with a level of opacity.  The four values
must be numbers from zero to one.  A value of one for `a` indicates full
opacity.

### cr:set_tolerance (tolerance)

Set a number which determines how finely curves are spliced up into straight
lines before being drawn.  Higher numbers can increase the quality of the
output in some situations, but will slow down rendering.

### cr:show_glyphs (glyphs)

Draw glyphs from a font, but instead of specifying a string of characters,
this method expects the caller to have already worked out which glyphs in
the current font to use, and the exact positions at which they should be
placed.  The `glyphs` value should be a table containing an array of
tables, each of which should contain three numbers:



* `1` The index number of the glyph.  The exact meaning of this depends on
which font is selected.

* `2` The `x` coordinate at which to draw the glyph on the surface.

* `3` The `y` coordinate.



### cr:show_text (text)

Draw the text given in the string `text` (which must be encoded as
UTF-8), using the current font face and size, and the current source.
This draws immediately, it doesn't leave a path for further drawing.
See the `cr:text_path()` method for drawing outlined text.

### cr:show_text_glyphs (text, glyphs, clusters)

Draw the glyphs given by the `glyphs` table, in the same way as the
`cr:show_glyphs()` method, but if the surface supports it, store the
correspondence between the glyphs and the characters in the UTF-8
string `text` represented by `clusters`.

The clusters value must be a table.  It may contain a key called `backward`,
whose value will be a boolean value, true if the clusters are listed in
reverse order.  It should also contain an array part containing pairs
of values; that is, the `clusters` entries with keys starting with the
number one should each be a table containing two numbers, a number of bytes
in the `text` string and a number of glyphs in the `glyphs` array, which
are meant to correspond to each other.

Only available with `Cairo 1.8` or better.

### cr:stroke ()

Draw a line along the current path using colour from the current source,
with other settings such line width taken in to account.

### cr:stroke_extents ()

Returns four numbers giving two corners of the rectangular area which would
be affected by a call to `cr:stroke()` with the current path.

### cr:stroke_preserve ()

Same as `cr:stroke()` but the current path is left intact for use in
further drawing operations.

### cr:text_extents (text)

Returns a table of metrics describing the how the text in the string `text`
will appear when rendered with the current font.  The table will contain
the following keys:



* x_bearing

* y_bearing

Distance from origin to the top left of the box which will be affected
by drawing the text.

*  width

*  height

Size of bounding box which will be affected by drawing the text.

*  x_advance

*  y_advance

Distance the current point will be moved by showing the text.  In other
words, the relative position where the next piece of text should go.



### cr:text_path (text)

Set the current path to the outline of the text given in the string
`text` (which should be encoded as UTF-8), using the glyph shapes from
the current font.  This can be used to draw text with an outline style.



    -- Draw some text in a big outlined font, in two
    -- shades of pink.
    cr:set_font_size(100)
    cr:move_to(50, 150)
    cr:text_path("Foo!")

    -- At this point the path has been set, so we can
    -- draw it like any other shape.
    cr:set_source_rgb(0.6, 0, 0.3)
    cr:fill_preserve()
    cr:set_source_rgb(1, 0.7, 1)
    cr:stroke()

### cr:transform (matrix)

Apply the transformation encoded in `matrix`, by multiplying the current
transformation matrix by it.

### cr:translate (x, y)

Apply a translation transformation, moving the coordinate system by the
amount specified by the two numbers given.

### cr:user_to_device (x, y)

Returns two numbers, the position given by the numbers `x` and `y`
converted from user coordinates to device coordinates.

### cr:user_to_device_distance (x, y)

Returns two numbers, the distance given by the numbers `x` and `y`
converted from user coordinates to device coordinates.
