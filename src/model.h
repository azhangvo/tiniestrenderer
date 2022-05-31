//
// Created by endian on 5/29/22.
//

#ifndef TINIESTRENDERER_MODEL_H
#define TINIESTRENDERER_MODEL_H

#include <vector>
#include "geometry.h"

class Model {
public:
    explicit Model(const char* filename);
    std::vector<int> face(int n) { return faces[n]; }
    Vec3f vertex(int n) { return vertices[n]; }
    int n_faces() { return faces.size(); }
    int n_verts() { return vertices.size(); }
private:
    std::vector<Vec3f> vertices;
    std::vector<std::vector<int>> faces;
};


#endif //TINIESTRENDERER_MODEL_H
