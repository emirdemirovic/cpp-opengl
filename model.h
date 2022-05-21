#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"

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

#endif // __MODEL_H__