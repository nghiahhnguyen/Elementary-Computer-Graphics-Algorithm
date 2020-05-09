## Rendering
- In order to keep a faster and easier interface for drawing pixels, I used a bitmap, represented by a matrix of unsigned char for keeping track the colors of the pixels on the screen. At every display rendering callback, I only render the buffer, instead of each shape.
- In order to draw a pixel, you can set the color of the corresponding element in the bitmap.

## Color Filling Algorithms

### Flood Filling
- The problem with flood filling is that its recursive nature is prone to stack overflow error. In order to solve this problem, I simulate the algorithm's DFS strategy with a stack:
```
static void floodFillColoring(int x, int y)
	{
		RGBColor borderColor(0, 0, 0);
		stack<pair<int, int>> S;
		S.push({x, y});
    
    
        // while stack is not empty
		while (!S.empty()) {
    
            // pop from the top
			pair<int, int> top = S.top();
			S.pop();
			x = top.first, y = top.second;

			if (bitMap[x][y] == fillingColor || bitMap[x][y] == borderColor) {
				continue;
			}

			bitMap[x][y] = fillingColor;

			int dx[] = {1, 0, -1, 0};
			int dy[] = {0, 1, 0, -1};
      
            // Starts filling adjacent pixels
			for (int i = 0; i < 4; ++i) {
				int newX = x + dx[i],
					newY = y + dy[i];
				if (1 <= newX && newX <= WIDTH && 1 <= newY && newY <= 
				HEIGHT && bitMap[newX][newY] != fillingColor && bitMap[newX][newY] != borderColor) {
					S.push({newX, newY});
				}
			}
		}
	}
```

### Scan Line Filling
Scan Line is highly dependent on the properties of the shape, so each shape gets its own implementation

#### Circle + Ellipse
Iterate a point (x, y) along the circumference from 0 to 180 degree. At each step, draw a line from the (xt - x, yt - y) to (xt + x, xt + y) with xt, yt being the coordinates of the center.

#### Rectangle
Iterate a point (x, y) along the left side of the rectangle. At each step draw a line from the (x, y) to (x2, y), with x2 being the maximum x coordinate of the rectangle.


#### Polygon
