##  lua-oocairo-fontopt - options controlling text rendering

A font options object contains options that tell Cairo how to render
text on surfaces.  They can be created with the `font_options_create`
function in the main oocairo module (see `lua-oocairo(3)`), and
retrieved from scaled fonts or context objects with `get_font_options`
methods.

Font options objects can be compared with the `==` operator.  Two different
objects with the same options set will be considered equal.

## Methods

The following methods are available on font options objects:

### opt:copy ()

Returns a new font options object, with identical options to `opt`.
The two objects will be completely independent, so you can set them to
have different options.

### opt:get_antialias ()

Get the current antialiasing mode, which will be one of the strings accepted
by the `opt:set_antialias()` method.

### opt:get_hint_metrics ()

Returns the current hint metrics option, which will be one of the same strings
accepted by the `opt:set_hint_metrics()` method.

### opt:get_hint_style ()

Get the kind of font hinting done when rendering text.  Returns one of
the strings accepted by the `opt:set_hint_style()` method.

### opt:get_subpixel_order ()

Returns the current ordering of colour components in pixels used for
subpixel rendering, which will be one of the same strings
accepted by the `opt:set_subpixel_order()` method.

### opt:hash ()

Return a number which can be used as a hash key identifying the particular
options in `opt`.  That is, if you wanted to cache fonts according to
the options set on them, you could store them in a table with this hash
value as the key.  Two font options objects with the same options will
have the same hash value.

### opt:merge (srcopt)

Add any options which are set to a non-default value in `srcopt` to `opt`
if that still has the default value from them.  The options in `srcopt`
will be unchanged.  Nothing is returned.

### opt:set_antialias (antialiastype)

Set the antialiasing mode used for drawing.  The argument must be a string
or boolean value accepted by the `set_antialias` method on context objects
(see `lua-oocairo-context(3)`).

### opt:set_hint_metrics (hintmetrics)

Set the kind of hinting to be applied to the metrics, as opposed to outlines,
of the glyphs in a font.  The `hintmetrics` value must be one of the
following strings:

* `default`

* `off`

* `on`

### opt:set_hint_style (hintstyle)

Set the kind of font hinting done when rendering text.  The `hintstyle`
value must be one of the following strings:

* `default`

* `none`

* `slight`

* `medium`

* `full`

### opt:set_subpixel_order (order)

Used to inform Cairo of the arrangement of colour components in the pixels
of a display device like a monitor.  Only applies if subpixel antialiasing
is used.  The `order` value must be one of the following strings:

* `default`

* `rgb`

* `bgr`

* `vrgb`

* `vbgr`


The last two options refer to vertical arrangements of colours.
