# Project: 
Custom OpenGL renderer


## Build:
```bash
cmake -configure .
sudo make all
```
## Run:
```bash
./opengl
```
## Outputs:

### render_wireframe (Bersenham's line algorithm):
![render_wireframe](https://github.com/emirdemirovic/cpp-opengl/blob/main/images/render_wireframe.png?raw=true)

### render_filled_triangles (Back-face culling + Gouraud shading, no Z-Buffer yet):
![render_filled_triangles_no_z](https://github.com/emirdemirovic/cpp-opengl/blob/main/images/render_filled_triangles_no_z.png?raw=true)


### render_filled_triangles (fixed clipping issues with Z-Buffer):
![render_filled_triangles_with_z](https://github.com/emirdemirovic/cpp-opengl/blob/main/images/render_filled_triangles_with_z.png?raw=true)
