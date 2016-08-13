# Librecog
Description of used algorithms

## Overview 

Image type determinited by vector of the 2-dimensional points.
Points should describe exactly one shape.

Main algorithm idea is to build all shapes (line, rectangle and ellipse) and check how much input points belonging to each.
Answer is shape with maximum value.

On each stage input points corrected to make continious contour for more precious recognition. 
For example two points `(0, 10)` and `(10,10)` will be treated like continious line.

## Line detection

Line detection implemented through [Hough transformation](https://en.wikipedia.org/wiki/Hough_transform) without any performance improvements.

Line described by equation: `x*cos(th) + y*sin(th) = r`, where `th` denotes angle and `r` - radius

Hough field equation: `F(x, y, th, r) = x*cos(th) + y*sin(th) - r = 0`

By default library uses hough field with these dimensions:
- Angle (th) - 480
- Radius (r) - 2048

After processing input points, value in hough matrix cell mean number of points belonging to line, so most big value describe desired line parameters.

Complexity: `O(c*thD)`, where `c` - number of input points, `thD` - angle axis of the hough matrix (480 by default).

Used references:
* [Hough transformation algorithm description (ru)](http://cgm.computergraphics.ru/content/view/36)
* [Rosetta code samples in various languages](https://rosettacode.org/wiki/Hough_transform)

## Rectangle detection

Rectangle detection implemented in the same way like line one, with following steps:

1. Detect line on image as described above
2. Remove all points, belonging to that line
3. Step 1 (3 times more)

On Step 2 there might be euristic performed: remove all points in small around each point, belonging to line. Thats help avoid repeated line detection in same place.

After detecting 4 lines, some invariants check performed:
* Make sure that its intersects exactly in 4 vertexes
* Check that two of it has angle near 0 or 180 degrees and two near 90 degrees
* Check that rectangle vertexes not too close, that might be wrong detection

If all checks passed, final rectangle score caclulated like sum of 4 lines scores.

Complexity almost the same: `O(4*c*thD)`

## Ellipse detection

Unfortunately hough transform not suitable for ellipse detection.
Even without rotation, there 4 parameters: `(x - x0)/a + (y - y0)/b = 1` - `x0,y0,a,b`, hought transformtion over this equation will take too long.

Therefore for ellipse detection following algorithm used:

1. Calculate outer contour over input points
2. Calculate raw and central moments of image
3. Determine ellipse parameters: `x0, y0, a, b, th` using image moments
4. Count number of input points matching ellipse contour

Image raw moments calculated through all points inside contour - `m10, m01, m11, m00, m20, m02` (See references).

Ellipse center coordinates, through image raw moments: `x = m10 / m00`, `y = m01 / m00`

Image central moments:
* `u20 = m20 / m00 - x * x`
* `u02 = m02 / m00 - y * y`
* `u11 = m11 / m00 - x * y`

Where x,y - coordinates of ellipse center.

Rotation angle in radians: `th = 0.5 * arctan(2 * m11 / (u20 - u02))`

Squares of half axes: 
* `a = (u20 + u02 + sqrt(4 * u11 * u11 + du * du)) / 2.0`
* `b = (u20 + u02 - sqrt(4 * u11 * u11 + du * du)) / 2.0`

To count number of points belonging to ellipse, used equation, taken [from stackoverflow](http://stackoverflow.com/questions/7946187/point-and-ellipse-rotated-position-test-algorithm), with slight changes to allow small deviation from exact ellipse contour.



References:
* [Image moments documentation on Wikipedia](https://en.wikipedia.org/wiki/Image_moment)

