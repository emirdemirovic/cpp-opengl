#ifndef __OPENGL_H__
#define __OPENGL_H__

#include <memory>
#include "geometry.h"
#include "tgaimage.h"
#include "model.h"

namespace OpenGL
{
    class Render
    {
    public:
        Render() = default;

        class Bresenham
        {
            // Bresenham’s Line Drawing Algorithm
        public:
            Bresenham() = default;
            Bresenham(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) : _x0(x0), _y0(y0), _x1(x1), _y1(y1), _image(image), _color(color) {}
            void draw_line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);

        private:
            int _x0 = 0, _y0 = 0, _x1 = 0, _y1 = 0;
            TGAImage _image;
            TGAColor _color;
        };
        void render_wireframe(const std::unique_ptr<Model> &model, TGAImage &image, const TGAColor &color, Bresenham &bresenham, const int width, const int height);
        void fill_triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color);
        Vec3f barycentric_transform(Vec3f A, Vec3f B, Vec3f C, Vec3f P); 
        void render_filled_triangles(const std::unique_ptr<Model> &model, TGAImage &image, const int width, const int height);

    };
}

#endif //__OPENGL_H__