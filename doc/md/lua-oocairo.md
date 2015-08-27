## lua-oocairo - Lua API for the Cairo vector graphics library

This is a Lua binding for the Cairo graphics library, with an object
oriented style of API.  There is at least one other Cairo binding
available in Lua, but it has a more C-like API and is much less complete.

## Simple Example

The code below is a complete example showing how to load this module and
do some basic drawing with it.  Other features are demonstrated by the
files in the `examples` directory included with the module.

First load the module.  The module doesn't create a global variable,
so you have to assign it to something.



    local Cairo = require "oocairo"

Create a bitmap image `surface` for drawing on, and create a context
object which is used to do the drawing:



    local surface = Cairo.image_surface_create("rgb24", 200, 200)
    local cr = Cairo.context_create(surface)

Fill whole image with white.  The `paint` method will copy the whole of
the `source` to the surface.  In this case the source is a solid colour,
but it can be other things like an image or gradient.  The default source
is solid fully opaque black.  Since the default colour of the image surface
is also black, nothing would happen if we didn't set the source to something
else before painting.



    cr:set_source_rgb(1, 1, 1)
    cr:paint()

Define an arbitrary shape, then fill it in purple.  The `move_to` and
`line_to` methods, among others, define the shape of the path which is
used for drawing operations like filling.



    cr:move_to(0, 0)
    cr:line_to(190, 100)
    cr:line_to(100, 185)
    cr:line_to(200, 200)
    cr:line_to(30, 130)
    cr:close_path()
    cr:set_source_rgb(0.8, 0.4, 1)
    cr:fill()

Another shape, this time both filled and with a line drawn along it's edge.
The path is normally discarded after a drawing operation, so here
`fill_preserve` is used to keep it so that it can also be stoked.
Note that the stroked line won't go all the way around the filled area,
because the path wasn't closed with `close_path`.  The black outline
created by the `stroke` method will be three pixels wide, and will be
centered on the path, half of it inside the filled area.



    cr:move_to(180, 30)
    cr:line_to(100, 20)
    cr:line_to(80, 120)
    cr:set_source_rgb(0.5, 0.7, 0.3)
    cr:fill_preserve()

    cr:set_line_width(3)
    cr:set_source_rgb(0, 0, 0)
    cr:stroke()

Finally, write the output to a file in PNG format.



    surface:write_to_png("simple-example.png")

## Surface constructor functions

The following functions can be accessed from the module table (the table
returned from `require`).  One of them must be used to create a surface
on which to draw on before Cairo can be used for anything interesting.
These all return a surface object, for which the methods are documented
at `lua-oocairo-surface(3)`.  Most drawing is done through a context
object, which should be constructed with a call to the `context_create`
function (also in the module table), passing the surface object as its
only argument.  The methods for context objects are documented at
`lua-oocairo-context(3)`.



### image_surface_create (format, width, height)

Return a surface object for drawing an image in memory.  The `width`
and `height` values must be numbers and are measured in pixels.  The
value of `format` can be any of the following strings:



* argb32

Each pixel is 32 bits, encoding both colour and alpha (transparency) values.

* rgb24

Each pixel is 32 bits with only 24 bits used, encoding a colour value.
All pixels are implicitly 100% opaque.

* a8

Each pixel is a single byte encoding the alpha (transparency) value, with
no colour information.

* a1

Each pixel is a single bit encoding whether a pixel is transparent or opaque,
with no colour information.



The image created will be black by default, and fully transparent if
the pixels have an alpha component.

### image_surface_create_from_data (data, format, width, height, stride)

Creates a new image surface with the size `width` by `height` pixels,
using `format` just as the `image_surface_create` function does, but
the image is initialized using pixel data from the string `data`.
The `stride` value should be whatever is returned from the
`format_stride_for_width` function for the given width and pixel format.
The data should be encoded in the format described for the `surf:get_data()`
method in `lua-oocairo-surface(3)`.

Note that since Lua strings are immutable, a copy of the data is made and
used as the live buffer. Use `image_surface_create_for_data` to avoid this
behaviour.

This binds the native Cairo function `cairo_image_surface_create_for_data`,
but has a slightly different name because the data you provide is only used
at construction time, not kept around for drawing.

### image_surface_create_for_data (data, format, width, height, stride)

Same as `image_surface_create_from_data` but using the memory pointed by data
as the buffer instead of making a copy of it, replicating the original
behaviour of the native Cairo function `cairo_image_surface_create_for_data`.

### image_surface_create_from_png (file/filename)

Creates a new image surface containing the image in a PNG file.  The
file is read from the filename or file handle specified.  See below
for details about what kind of file handles can be used.

### pdf_surface_create (file/filename, width, height)

Create a surface which writes drawing instructions out to a PDF file,
at the given filename or through a file handle.  The `width` and `height`
values must be numbers and are measured in points.

### ps_surface_create (file/filename, width, height)

Create a surface which writes drawing instructions out to an Adobe`trade`
file, at the given filename or through a file handle.  The `width` and
`height` values must be numbers and are measured in points.

### surface_create_similar (surface, content, width, height)

Create a new surface of the same type as `surface`, but with the width
and height specified.  The `content` value indicates whether the surface
should include colour and/or transparency information, and must be one of
the following strings:



* color

Surface will have only colour content.

* alpha

Surface will have only transparency content.

* color-alpha

Surface will have colour and transparency content.



### svg_surface_create (file/filename, width, height)

Create a surface which writes drawing instructions out to an SVG file,
at the given filename or through a file handle.  The `width` and `height`
values must be numbers and are measured in points.



## Other top-level functions

The following additional methods are available.  The are rarely needed
except for `context_create` and the gradient constructors.



### BYTE_ORDER

Not actually a function, this is a string indicating which order colour
components are stored in pixels in raw image data on this platform.
This will be `argb` on big-endian systems and `bgra` on little-endian
ones.  This is the order in which the bytes for each pixel are stored
on `rgb24` and `argb32` image surfaces.  For `a1` it indicates the
order the pixels are encoded as bits.  It can be ignored when the image
format is `a8`.

### context_create (surface)

Return a new context object for drawing on `surface`.
See `lua-oocairo-context(3)` for methods which can be called on this object.

### context_create_gdk (drawable)

Return a new context object for drawing on the GdkDrawable object `drawable`,
which must be from the Lua-Gnome library.

Fails unless Lua-Gnome has been loaded and the `gtk` module is available
as a global variable.

See example programs `gtk-image.lua` and `gtk-drawing.lua` for examples
of how to use this.

### font_options_create ()

Returns a new Cairo font options object, with all its options set to
default values.  See `lua-oocairo-fontopt(3)` for information about
what you can do with it.

### format_stride_for_width (format, width)

Returns the number of bytes Cairo will used for storing a single row of
pixels in the raw data of an image surface.  This can be used as the
`stride` value for the `image_surface_create_from_data()` function
and the image surface method `get_data()`.  The `format` value should
be one of the pixel format strings such as `rgb24`, and the width should
be a number.

### matrix_create ()

Return a new copy of the identity matrix.  All transformation matrices
returned to Lua are in the format of a table of six numbers.
See `lua-oocairo-matrix(3)` for methods which can be called on these.

### pattern_create_for_surface (surface)

Return a new pattern object representing the image on the surface.
See `lua-oocairo-pattern(3)` for methods which can be called on pattern
objects.

### pattern_create_linear (x1, y1, x2, y2)

Return a new pattern object representing a linear gradient, where the
offset numbers go from zero at the point specified by the two numbers
(`x1`,`nbsp``y1`) to the point (`x2`,`nbsp``y2`).

### pattern_create_radial (x1, y1, r1, x2, y2, r2)

Return a new pattern object representing a radial gradient, where offsets
move from the circle defined by the three numbers (`x1`,`nbsp``y1`,`nbsp``r1`),
where `r1` is the radius, to the circle (`x2`,`nbsp``y2`,`nbsp``r2`).

### pattern_create_rgb (r, g, b)

Return a new pattern object representing a solid colour.  The red, green,
and blue values specified should all be numbers in the range zero to one
inclusive.

### pattern_create_rgba (r, g, b, a)

Same as `pattern_create_rgb`, but accepts an alpha value, so the solid
colour can be semitransparent.

### ps_get_levels ()

Return a table containing a list of strings indicating what levels of
PostScript are supported by Cairo.

### scaled_font_create (face, font_matrix, ctm, options)

Creates a new scaled font object, representing a scaled version of `face`.
See `lua-oocairo-scaledfont(3)` for information about these objects.

The `font_matrix` and `ctm` values must be matrices (any table containing
six numbers, see `lua-oocairo-matrix(3)` for details).  The `options`
argument can be nil for the default options, or a font options object
as returned by the `font_options_create` function.

### svg_get_versions ()

Return a table containing a list of strings indicating what versions of
SVG are supported by Cairo.

### toy_font_face_create (family, slant, weight)

Create and return a toy font face object (see `lua-oocairo-fontface(3)`).
See the context method `select_font_face` for details of the arguments.
This does the same, but returns the object instead of applying it to a
context.
Only available with `Cairo 1.8` or better, otherwise this method won't
exist.

### user_font_face_create (callbacks)

Returns a font face object which uses the supplied callbacks for rendering
and such.  This is intended to be used for creating special fancy font
effects or for providing access to obscure font formats which aren't
supported by Cairo.  The `callbacks` value must be a table, with string
keys for the callbacks.  The `render_glyph` callback is required, the
others optional.

See `lua-oocairo-userfont(3)` for details of how to write the callbacks.
Only available with `Cairo 1.8` or better.



## I/O through file handles

Functions in this module which are advertised as accepting a `file/filename`
value can accept a filename (a simple string) or a Lua file handle.  They
will also work with any other object (a userdata value or table) which
provides the appropriate input or output method.

For files which are to be used for output, a file handle object needs to
provide a method called `write`, which will be called with a single
string every time more output is available.  Any return values from
the method are ignored, but it can throw an exception if there's an error.

For files which are used for input, a file handle needs to provide a
method called `read`, which will be called with a number indicating the
number of bytes it should read.  It should return a string containing
exactly that number of bytes.

The `memoryfile` module available from LuaForge can be used to write
output into a buffer in memory which can then be accessed as a Lua string.
This can be useful as a way to get an image into another graphics library
such as GD, where it can be written in other formats other than PNG.

## Feature flags

When the module is compiled, it will only enable support for the features
which were compiled into your copy of the Cairo library.  For example, if
you built Cairo without the PDF backend, then the module will not include
the functions for creating a PDF surface.

There are flags in the module table which indicate which features are
available.  These are all wither `true` or `false`:



* HAS_PDF_SURFACE

Support for creating a surface which writes drawing instructions out to
an SVG file.

* HAS_PNG_FUNCTIONS

Support for loading a PNG bitmap and creating an image surface from it,
or for writing the contents of an image surface out to a PNG.

* HAS_PS_SURFACE

Support for creating a surface which writes to a PostScript or EPS file.

* HAS_SVG_SURFACE

Support for creating a surface which writes to a PDF file.

* HAS_USER_FONT

Support for custom fonts defined by callback functions.  If true then
the `user_font_face_create` function should be available in the main
module table.

## Using with Torch

Surfaces can be exported as tensors using the function rgb2tensor with the
following prototype:

    rgb2tensor(surface, [useAlpha])

This returns either a 3xNxM or 4xNxM ByteTensor with values in the range [0,
255].

The inverse operation can be achieved with the function tensor2rgb2.
