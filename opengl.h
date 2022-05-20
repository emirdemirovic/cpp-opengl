#ifndef __OPENGL_H__
#define __OPENGL_H__

#include <vector>
#include "geometry.h"
#include "tgaimage.h"

namespace OpenGL
{
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

    class Model
    {

    public:
        Model(const char *filename);
        ~Model();
        int nverts() const;
        int nfaces() const;
        Vec3f vert(int i);
        std::vector<int> face(int idx);
    private:
        bool open_file(const char *filename, std::ifstream &in);
        void parse_obj_file(std::ifstream &in);
        void obj_file_summary(const int verts, const int faces);
        std::vector<Vec3f> verts_;
        std::vector<std::vector<int>> faces_;
    };
}

#endif