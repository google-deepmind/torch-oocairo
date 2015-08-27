##  lua-oocairo-path - Path object

A path object represents a series of path instructions like `move_to`
and `line_to`, as supplied to a context object when defining shapes for
drawing.  They can be created by issuing instructions on a context object
and then calling `cr:copy_path()` or `cr:copy_path_flat()` (see
`lua-oocairo-context(3)`).  A path can then be added back to the
current path by calling `cr:append_path()`.

## Methods

Currently only the following method can be called on a path object:

### path:each ()

Returns an iterator function and initial values needed to iterate over
the instructions in the path using a Lua `for` loop.  This can be used
to translate the Cairo path piece by piece, perhaps to encode it in some
unusual vector graphics format, or to translate it into calls to another
API like OpenGL.

The iterator function will return three values each time it is called.
The first is a number, which should be ignored (but is needed to keep track
of the current position in the list of instructions).  The second value is
a string naming the instruction to be performed, and the last value is either
nil or a table (array) of numbers which are the arguments of the instruction.

This example simply prints out the instructions in the order they were
issued on the context object `cr`:



    for _, instr, args in cr:copy_path():each() do
        if args then
            print(instr, unpack(args))
        else
            print(instr)
        end
    end

The following instructions can be included:


* move-to

Set current point.  Comes with a table of two numbers.

* line-to

Draw straight line.  Comes with a table of two numbers.

* curve-to

Draw curved line.  Comes with a table of two numbers, the two
control points first and then the destination point.

* close-path

Draw line back to start point.  The extra arguments value will be nil.
