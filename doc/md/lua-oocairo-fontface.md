## lua-oocairo-fontface - Unscaled font face object

Cairo font face objects are returned by the `get_font_face` method
on context objects (see `lua-oocairo-context(3)`).  They represent
a font selection, but not including the scaling used to create a
font of a particular size.  See `lua-oocairo-scaledfont(3)` for details
about fonts at a particular size, which can be created by scaling one of
these font face objects.

Font face objects can be compared with the `==` operator.  This will compare
the identity of the objects, so different Lua objects which refer to the
same `cairo_font_face_t` will compare as equal.

## Methods

Font face objects provide the following methods.  Some of these will not
be defined if the module was compiled against a version of Cairo too old
to support them.

### fontface:get_family ()

Returns a string containing the name of the family selected with the
`select_font_face` method.  Throws an exception if called on a font face
which wasn't created with the ‘toy’ API.
Only available with `Cairo 1.8` or better.

### fontface:get_slant ()

Returns the slant style of the font face created with the toy API, or
throws an exception for other font faces.  The return value will be a string,
with one of the values accepted by the `select_font_face` method.
Only available with `Cairo 1.8` or better.

### fontface:get_type ()

Returns a string indicating what backend library handles this font.
The return value will be one of the following:

* toy

Created using Cairo's ‘toy’ font API, probably selected with
the `select_font_face` method.

* ft

Created with the FreeType API.

* win32

A Windows native font.

* quartz

A MacOS X native font.

* user

A font created with custom code.

### fontface:get_weight ()

Returns the weight of the font face created with the toy API, or
throws an exception for other font faces.  The return value will be a string,
with one of the values accepted by the `select_font_face` method.
Only available with `Cairo 1.8` or better.
