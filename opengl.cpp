#include "opengl.h"
#include <vector>
#include <memory>
#include <limits>

void OpenGL::Render::render_wireframe(const std::unique_ptr<Model> &model, TGAImage &image, const TGAColor &color, OpenGL::Render::Bresenham &bresenham, const int width, const int height)
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

void OpenGL::Render::fill_triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color)
{
    if (t0.y == t1.y && t0.y == t2.y)
        return; // ignore degenerate triangles
    if (t0.y > t1.y)
        std::swap(t0, t1);
    if (t0.y > t2.y)
        std::swap(t0, t2);
    if (t1.y > t2.y)
        std::swap(t1, t2);
    int total_height = t2.y - t0.y;
    for (int i = 0; i < total_height; i++)
    {
        bool second_half = i > t1.y - t0.y || t1.y == t0.y;
        int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;
        float alpha = static_cast<float>(i) / total_height;
        float beta = static_cast<float>(i - (second_half ? t1.y - t0.y : 0)) / segment_height; // caution: no division by zero here
        Vec2i A = t0 + (t2 - t0) * alpha;
        Vec2i B = second_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;
        if (A.x > B.x)
            std::swap(A, B);
        for (int j = A.x; j <= B.x; j++)
        {
            image.set(j, t0.y + i, color);
        }
    }
}

Vec3f OpenGL::Render::barycentric_transform(Vec3f A, Vec3f B, Vec3f C, Vec3f P)
{
    Vec3f s[2];
    for (int i = 2; i--;)
    {
        s[i][0] = C[i] - A[i];
        s[i][1] = B[i] - A[i];
        s[i][2] = A[i] - P[i];
    }
    Vec3f u = cross(s[0], s[1]);
    if (std::abs(u[2]) > 1e-2) // u[2] is integer (if it is zero then triangle ABC is degenerate)
        return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
    return Vec3f(-1, 1, 1); // in this case generate negative coordinates, it will be thrown away by the rasterizator
}

void OpenGL::Render::render_filled_triangles(const std::unique_ptr<Model> &model, TGAImage &image, const int width, const int height)
{
    std::vector<float> zbuffer(width*height);
    for (int i = width * height; i--; zbuffer[i] = -std::numeric_limits<float>::max());
    Vec3f light_dir(0, 0, -1);
    for (int i = 0; i < model->nfaces(); i++)
    {
        std::vector<int> face = model->face(i);
        Vec2i screen_coords[3];
        Vec3f world_coords[3];
        Vec3f pts[3];
        for (int i = 0; i < 3; i++)
        {
            pts[i] = Vec3f(static_cast<int>((model->vert(face[i]).x + 1.) * width / 2. + .5), static_cast<int>((model->vert(face[i]).y + 1.) * height / 2. + .5), model->vert(face[i]).z);
            Vec3f v = model->vert(face[i]);
            screen_coords[i] = Vec2i((v.x + 1.) * width / 2., (v.y + 1.) * height / 2.);
            world_coords[i] = v;
        }
        Vec3f n = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);
        n.normalize();
        float intensity = n * light_dir;
        if (intensity > 0)
        {
            TGAColor color(intensity * 255, intensity * 255, intensity * 255, 255);
            Vec2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
            Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
            Vec2f clamp(image.get_width() - 1, image.get_height() - 1);
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    bboxmin[j] = std::max(0.f, std::min(bboxmin[j], pts[i][j]));
                    bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], pts[i][j]));
                }
            }
            Vec3f P;
            for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++)
            {
                for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++)
                {
                    Vec3f bc_screen = barycentric_transform(pts[0], pts[1], pts[2], P);
                    if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0)
                        continue;
                    P.z = 0;
                    for (int i = 0; i < 3; i++)
                        P.z += pts[i][2] * bc_screen[i];
                    if (zbuffer[static_cast<int>(P.x + P.y * width)] < P.z)
                    {
                        zbuffer[static_cast<int>(P.x + P.y * width)] = P.z;
                        image.set(P.x, P.y, color);
                    }
                }
            }
        }
    }
}

void OpenGL::Render::Bresenham::draw_line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
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
