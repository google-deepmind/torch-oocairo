## lua-oocairo-surface - Cairo surface objects

All Cairo drawing operations are targeted at a `surface`, which represents
a backend writing drawing instructions out to a vector graphics file (SVG,
PDF, etc.) or drawing to a bitmap image.  Surfaces are created with one of
the top-level constructor functions listed in `lua-oocairo(3)`.

Surface objects can be compared with the `==` operator.  This will compare
the identity of the objects, so different Lua objects which refer to the
same `cairo_surface_t` will compare as equal.

## Methods

The following methods are available on surface objects.

### surf:copy_page ()

Same as `surf:show_page()`, but keeps whatever has been drawn on the current
page for additional drawing on the next page.

### surf:finish ()

Finish any drawing to the surface and disconnect from any external resources
it uses, such as closing a file handle if it's writing output to a file.
No more drawing can be done with this surface after calling this method.

### surf:flush ()

Finish any drawing work currently in progress.

### surf:get_content ()

Returns a string indicating what kind of content the surface has (colours
and/or transparency).  The return value will be one of the strings
accepted by the `surface_create_similar` function (see `lua-oocairo(3)`).

### surf:get_data ()

Returns the raw data for an image surface as a string, so that you can
pass it to some other library or get the exact colours of each pixel.
Returns nothing for surfaces which aren't image surfaces.

See the example program `get-data.lua` included with the distribution
for some code to extract the colour values from pixels in the image data.

There are actually two values returned from this method, the data itself
and the `stride` for it.
The stride is the number of bytes of the string used for each
line of the image.  This is at least the width of the image multiplied by
the number of bytes needed for each pixel, although it might be larger
(in which case the extra bytes at the end of each line of pixels should
be ignored).  The length of the data will always be equal to the stride
multipled by the image's height.

The data string will contain rows of pixels starting with the top row.
Each row contains the pixels one after another, with no padding in between,
starting from the left hand side.

The way individual pixels are stored depends on the image format, which
can be queried with the `surf:get_format()` method.  For colour images,
that is ones with a format of `rgb24` or `argb32`, each pixel will be
four bytes in size.  The alpha, red, green, and blue components will be
in the order given by the string `Cairo.BYTE_ORDER`.  On `rgb24`
images the alpha value should be ignored, although it will still be present
(that is, the pixels will still be stored in `32 bits` each, not `24 bits`).

If the format is `a8` then each pixel will be one byte.  The `a1` format
stores each pixel as one bit, in groups of 32 bits ordered according to the
endianness of the platform (so if the last value returned from the method
is `argb` then the first pixel will be the most significant bit of the 32
bits, otherwise the reverse).

### surf:get_device_offset ()

Returns two numbers, whichever ones were set with `surf:set_device_offset()`.

### surf:get_eps ()

Returns a boolean value matching whatever was last set with
`surf:set_eps()`, or throws an exception if `surf` is not a PostScript
surface.

### surf:get_fallback_resolution ()

Returns two numbers, the values set by `surf:set_fallback_resolution()`.
Only available with `Cairo 1.8` or better.

### surf:get_font_options ()

Returns a new font options object containing the preferred options for
rendering text on this surface.  These options are used automatically
when rendering text, so can usually be ignored, but might be useful if
you want to render some text to an image internally and then present it
on a surface on screen, in a widget of some sort.

See `lua-oocairo-fontopt(3)` for more about font options objects.

### surf:get_format ()

Return the pixel format for an image surface.  Throws an exception for
other surface types.  The return value will be one of the strings accepted
by the `image_surface_create` function (see `lua-oocairo(3)`).

### surf:get_gdk_pixbuf ()

Creates a GdkPixbuf object containing the image currently held on an image
surface.  The return value is suitable for use with the Lua-Gnome library,
for example as an icon.  See the example program `gtk-drawing.lua` for
an example showing that use.

Must be called on an image surface, otherwise it throws an exception.  Also,
it currently only supports the `rgb24` and `argb32` formats.

Requires the Lua-Gnome package to have been installed and the `gtk` module
to be loaded.

### surf:get_height ()

Returns the height in pixels of an image surface, or throws an exception
for other types.

### surf:get_type ()

Returns a string indicating what type of surface object `surf` is.
The value will be one of the following:


* image

A bitmap image stored in a memory buffer.

* pdf

Vector drawing into a Adobe`trade` PDF file.

* ps

Vector drawing into a PostScript file.

* svg

Vector drawing into an SVG file.

* xlib

An X window in a graphical user interface such as Gtk+.



Other values are theoretically possible, but there isn't yet any way to
create them with this binding of Cairo.

### surf:get_width ()

Returns the width in pixels of an image surface, or throws an exception
for other types.

### surf:has_show_text_glyphs ()

Returns true if this surface can use text cluster information to improve
the quality of text information in the output.  If true then it may be
beneficial to use the `show_text_glyphs` method on a context object instead
of the `show_glyphs` method, otherwise that will make no difference.

Only available with `Cairo 1.8` or better.

### surf:set_device_offset (x, y)

Set two numbers which are added to the `x` and `y` coordinates used for
drawing after the current transformation matrix has been applied.

### surf:set_eps (eps)

On a PostScript surface this sets whether the output should be encapsulated
PostScript or not.  The value of `eps` is interpreted as boolean.
Throws an exception if `surf` is not a PostScript surface.

### surf:set_fallback_resolution (x, y)

Set the resolution at which vector graphics are rendered to bitmaps when
drawing something on a vector-based surface type which can't be directly
represented.  Has no effect on surfaces where the output is bitmapped
anyway.  The values of `x` and `y` must be numbers.

### surf:set_size (width, height)

Set the size in points of the next page of PostScript or PDF output.
This should be called before any drawing has been done on the new page
(probably just after creating the surface or just after the last
`surf:show_page()` call).  The values of `width` and `height` should
be numbers.  Throws an exception if `surf` isn't a PostScript or PDF
surface.

### surf:show_page ()

Starts a new page on surfaces which support that (such as PDF and PostScript).

### surf:write_to_png (file/filename)

Write the bitmap data from a surface out to the specified file in PNG
format.  The argument can be a filename or file handle.
