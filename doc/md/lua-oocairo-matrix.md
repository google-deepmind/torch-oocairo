## lua-oocairo-matrix - Transformation matrix table

Matrix objects are used in Cairo to represent affine transformations
like rotation and scaling.  They are used for transforming both fonts
and paths drawn on surfaces.

This binding uses normal Lua tables for matrices, with each matrix
being an array of six numbers.  You can use any such table when supplying
a matrix to a method, but the ones returned by this module have a metatable
set so that the methods listed below can be called on it.

The `matrix_create` function in the module table can be used to create
a new identity matrix.

## Methods

The following methods can be called on a matrix object.  Unless otherwise
stated they do not return any values.  The transformations alter the matrix
you supply by adjusting the numbers in the table, rather than creating a
new table.

### mat:invert ()

Set `mat` to its inverse, which will result in a transformation which
reverses what it did originally.  Not all matrices have an inverse, and if
none can be found then an exception will be thrown.

### mat1:multiply (mat2)

Multiply `mat1` by `mat2`, leaving `mat2` unchanged.  This has the effect
of combining the transformations represented by the two matrices.

### mat:rotate (ang)

Apply a rotation by `ang` radians to `mat`.

### mat:scale (sx, sy)

Apply a scaling transformation to `mat`.

### mat:transform_distance (x, y)

Transform the distance given, using `mat`, and return two numbers
giving the resulting distance.  This will have the same effect as
`mat:transform_point()` but without any translation taking place.

### mat:transform_point (x, y)

Transform the position of the coordinates, which must be numbers, to the
matrix, and return two numbers giving the resulting position.

### mat:translate (x, y)

Apply a translation to `mat`.
