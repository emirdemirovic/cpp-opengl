#include "model.h"
#include <string>
#include <fstream>
#include <sstream>


Model::Model(const char *filename) : verts_(), faces_()
{
    std::ifstream in;
    if (open_file(filename, in))
    {
        parse_obj_file(in);
        obj_file_summary(verts_.size(), faces_.size());
    }
}

bool Model::open_file(const char *filename, std::ifstream &in)
{

    in.open(filename, std::ifstream::in);
    return !in.fail();
}

void Model::parse_obj_file(std::ifstream &in)
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
                iss >> v[i];
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

void Model::obj_file_summary(const int verts, const int faces)
{
    std::cerr << "# v# " << verts << " f# " << faces << std::endl;
}

Model::~Model()
{
}

int Model::nverts() const
{
    return static_cast<int>(verts_.size());
}

int Model::nfaces() const
{
    return static_cast<int>(faces_.size());
}

std::vector<int> Model::face(int idx)
{
    return faces_[idx];
}

Vec3f Model::vert(int i)
{
    return verts_[i];
}