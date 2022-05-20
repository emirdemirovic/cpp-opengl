#include "opengl.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

void OpenGL::Bresenham::draw_line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1))
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1 - x0;
    int dy = y1 - y0;
    float derror = std::abs(dy / static_cast<float>(dx));
    float error = 0;
    int y = y0;
    for (int x = x0; x <= x1; x++)
    {
        if (steep)
        {
            image.set(y, x, color);
        }
        else
        {
            image.set(x, y, color);
        }
        error += derror;
        if (error > .5)
        {
            y += (y1 > y0 ? 1 : -1);
            error -= 1.;
        }
    }
}

OpenGL::Model::Model(const char *filename) : verts_(), faces_()
{
    std::ifstream in;
    if (open_file(filename, in))
    {
        parse_obj_file(in);
        obj_file_summary(verts_.size(), faces_.size());
    }
}

bool OpenGL::Model::open_file(const char *filename, std::ifstream &in)
{
    
    in.open(filename, std::ifstream::in);
    return in.fail();
}

void OpenGL::Model::parse_obj_file(std::ifstream &in)
{
    std::string line;
    while (!in.eof())
    {
        
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v "))
        {
            iss >> trash;
            Vec3f v;
            for (int i = 0; i < 3; i++)
                iss >> v.raw[i];
            verts_.push_back(v);
        }
        else if (!line.compare(0, 2, "f "))
        {
            std::vector<int> f;
            int itrash, idx;
            iss >> trash;
            while (iss >> idx >> trash >> itrash >> trash >> itrash)
            {
                idx--; // in wavefront obj all indices start at 1, not zero
                f.push_back(idx);
            }
            faces_.push_back(f);
        }
    }
}

void OpenGL::Model::obj_file_summary(const int verts, const int faces)
{
    std::cerr << "# v# " << verts << " f# " << faces << std::endl;
}

OpenGL::Model::~Model()
{
}

int OpenGL::Model::nverts() const
{
    return static_cast<int>(verts_.size());
}

int OpenGL::Model::nfaces() const
{
    return static_cast<int>(faces_.size());
}

std::vector<int> OpenGL::Model::face(int idx)
{
    return faces_[idx];
}

Vec3f OpenGL::Model::vert(int i)
{
    return verts_[i];
}
