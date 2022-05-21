#include "opengl.h"
#include <memory>

int main(int argc, char **argv)
{
    std::unique_ptr<Model> model = nullptr;
    if (2 == argc)
    {
        model = std::make_unique<Model>(argv[1]);
    }
    else
    {
        model = std::make_unique<Model>("obj/head.obj");
    }

    const int width = 800;
    const int height = 800;
    TGAImage image(width, height, TGAImage::RGB);
    const TGAColor white(255, 255, 255, 255);
    OpenGL::Render render;
    // OpenGL::Render::Bresenham bresenham;
    // render.render_wireframe(model, image, white, bresenham, width, height);
    render.render_filled_triangles(model, image, width, height);

    image.flip_vertically();
    image.write_tga_file("output.tga");

    return 0;
}
