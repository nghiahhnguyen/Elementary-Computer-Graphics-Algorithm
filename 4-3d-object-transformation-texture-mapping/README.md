## Object transformation and Texture Mapping

### Object transformation

- Circle: Iterate through points on the circumference of the circle at a constant step. After that, the primitives to draw are triangles between a pair of adjacent points and the center.

- Cone: Iterate through points on the circumference of the circle at the bottom at a constant step. After that, the primitives to draw are triangles between a pair of adjacent points and the top of the cone. The circle at the bottom can be drawn like above.

- Cube: First identify the vertices at the 8 corners. After that draw quads based on the suitable combinations of size 4 of them for faces.

- Cylinder: Iterate throught points on the circumferense of the the bottom and top circles. After that, draw quads for the corresponding pairs of two adjacent points on the top and bottom circles.

- Hyperboloid: Loop thourgh each sector, then on each sector, draw the set of points based on the parametric equations of a  hyperboloid:
    - x = a * cosh(v) * cos(theta)
    - y = c * sinh(v)
	- z = b * cosh(v) * sin(theta)
	
- Paraboloid: Do the same as above, only the parametric equations is changed:
    - x = r * cosf(theta)
    - z = r * sinf(theta)
    - y = a * x * x + b * z * z
    
- Sphere: Divide the sphere into tiles based on sectors and stacks. Then draw 2 triangles for each tile.

- Torus: For each flat ring on the torus, find the set of points on that ring. After that, for each 4 adjacents points draw a quad.

### Texture Mapping
- I load the 7 bitmap files in the folder ./img to the memory as textures. The order of the files are random.
- For each primitive (quad/triangle), I map a rando texture to that primitive.
