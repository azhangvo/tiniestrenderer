//
// Created by endian on 5/29/22.
//

#include "model.h"
#include <iostream>
#include <fstream>
#include <sstream>

Model::Model(const char *filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line.find("v ") == 0) {
            std::istringstream iss(line);
            std::string s;
            Vec3f v;
            iss >> s >> v.raw[0] >> v.raw[1] >> v.raw[2];
            vertices.push_back(v);
        } else if (line.find("f ") == 0) {
            std::istringstream iss(line);
            int i;
            char c;
            std::vector<int> face;
            iss >> std::noskipws;
            iss >> c;
            while(iss >> c >> i) {
                if(c == ' ') {
                    face.push_back(i-1);
                }
            }
            faces.push_back(face);
        }
    }
}