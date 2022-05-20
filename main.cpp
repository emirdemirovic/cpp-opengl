#include "opengl.h"
#include <memory>

void render_wireframe(const std::unique_ptr<OpenGL::Model> &model, TGAImage &image, const TGAColor &color, OpenGL::Bresenham &bresenham, const int width, const int height)
{

    for (int i = 0; i < model->nfaces(); i++)
    {
        std::vector<int> face = model->face(i);
        for (int j = 0; j < 3; j++)
        {
            Vec3f v0 = model->vert(face[j]);
            Vec3f v1 = model->vert(face[(j + 1) % 3]);
            int x0 = (v0.x + 1.) * width / 2.;
            int y0 = (v0.y + 1.) * height / 2.;
            int x1 = (v1.x + 1.) * width / 2.;
            int y1 = (v1.y + 1.) * height / 2.;
            bresenham.draw_line(x0, y0, x1, y1, image, color);
        }
    }
}

int main(int argc, char **argv)
{
    std::unique_ptr<OpenGL::Model> model = nullptr;
    if (2 == argc)
    {
        model = std::make_unique<OpenGL::Model>(argv[1]);
    }
    else
    {
        model = std::make_unique<OpenGL::Model>("obj/head.obj");
    }

    const int width = 800;
    const int height = 800;
    TGAImage image(width, height, TGAImage::RGB);
    const TGAColor white(255, 255, 255, 255);
    OpenGL::Bresenham bresenham;
    render_wireframe(model, image, white, bresenham, width, height);
    image.flip_vertically();
    image.write_tga_file("output.tga");

    return 0;
}
