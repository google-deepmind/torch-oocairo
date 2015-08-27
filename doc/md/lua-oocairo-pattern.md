##  lua-oocairo-pattern - Cairo pattern objects

A pattern object is any one of several things that can be used as the
B`source` for a Cairo context, defining what colours and transparencies
will be used for drawing.  These objects can be created explicitly with
the various `pattern_create_*` functions in the module table (see
`lua-oocairo(3)` for details), or implicitly by calling methods like
`cr:set_source_rgb()` on a context object (see `lua-oocairo-context(3)`).

Pattern objects can be compared with the `==` operator.  This will compare
the identity of the objects, so different Lua objects which refer to the
same `cairo_pattern_t` will compare as equal.

## Methods

### pat:add_color_stop_rgb (offset, r, g, b)

Add a new colour stop to a gradient (either linear or radial, it doesn't
matter).  The offset is a number from zero (the start of the line or the
first circle of the gradient) to one (the end of the line or the second
circle).  The colour values must also be numbers from zero to one.
The alpha level is set to fully transparent.

This will throw an exception if called on a pattern which isn't a gradient.

### pat:add_color_stop_rgba (offset, r, g, b, a)

Same as `pat:add_color_stop_rgb()` above, but the alpha value can be
set to less than one to give some transparency to the gradient.  This
can be used for gradients which fade away into nothing for example.

### pat:get_color_stops ()

Returns the color stops defined on a linear or radial gradient.
Throws an exception if called on any other type of pattern.

The return value will be a table, an array with one entry per color stop.
Each entry will be a table containing five numbers, in the following
order: offset, red, green, blue, alpha.

### pat:get_extend ()

Return the current method of extending the pattern beyond its limits.
The return value will be one of the strings accepted by the
`pat:set_extend()` method.

### pat:get_filter ()

Return the current filter method, which will be one of the strings
accepted by the `pat:set_filter()` method.

### pat:get_linear_points ()

Return the start and end points of a linear gradient as four numbers.
Throws an exception if called on any other type of pattern.

### pat:get_matrix ()

Return the current transformation matrix used for the pattern.
See `lua-oocairo-matrix(3)`.

### pat:get_radial_circles ()

Return the start and end circles of a radial gradient as six numbers.
Throws an exception if called on any other type of pattern.

### pat:get_rgba ()

Return the colour value and alpha level for a solid colour pattern.
Throws an exception for any other type of pattern.  The return values
will be four numbers, in the following order: red, green, blue, alpha.

### pat:get_surface ()

Return the surface object (see `lua-oocairo-surface(3)`) used for a
surface pattern.  Throws an exception for any other kind of pattern.

### pat:get_type ()

Returns a string indicating what kind of pattern object this is.
The return value will be one of the following:

* `linear`

* `radial`

* `solid`

* `surface`


### pat:set_extend (extendtype)

Set the way in which the pattern should be extended beyond its defined
limits.  The argument must be one of the following strings:

* none

Everything outside the area defined by the pattern is fully transparent.

* repeat

Pattern tiled by endlessly repeating it.

* reflect

Pattern tiled by reflecting it at its edges.

* pad

Whatever is at the edge of the pattern is extended outwards.


### pat:set_filter (filtertype)

Set the filter method used for picking colours from the pattern when
it is being used to render something.  The argument must be one of the
following strings:


* fast

Fast interpolation that is likely to be low quality in some cases (but
probably OK for a smooth gradient).

* good

Reasonably good quality interpolation.

* best

The highest quality interpolation available, but likely very slow.

* nearest

Pick colour of nearest pixel.

* bilinear

Interpolate between pixels.

* gaussian

Gaussian blur, but this isn't implemented yet.

### pat:set_matrix (matrix)

Set the transformation matrix used for the pattern, as a table of six
numbers.  See `lua-oocairo-matrix(3)`.
