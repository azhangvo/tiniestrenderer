//
// Created by azhan on 5/27/2022.
//

#include <iostream>
#include "tgaimage.h" // tga management library
#include "model.h" // model management library

#define width 200 // width of image
#define height 200 // height of image

const TGAColor white = TGAColor(255, 255, 255, 255); // pre-defined colors
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);

/*
 * Draws a line between two points given an image and a color
 *
 * @param image - the image to draw on
 * @param x0 - the x coordinate of the first point
 * @param y0 - the y coordinate of the first point
 * @param x1 - the x coordinate of the second point
 * @param y1 - the y coordinate of the second point
 * @param color - the color to draw the line with
 */
void line(int x0, int y0, int x1, int y1, TGAImage &image, const TGAColor &color) {
    bool transpose = false;
    if (std::abs(y1 - y0) > std::abs(x1 - x0)) { // if the line is more vertical than horizontal swap the coordinates
        std::swap(y1, x1);
        std::swap(y0, x0);
        transpose = true;
    }
    if (x0 > x1) { // if the first point is to the right of the second point swap the x coordinates
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dx = x1 - x0;
    int dy = y1 - y0;
    int derror = std::abs(dy) * 2;
    int error = 0;
    int y = y0;
    for (int x = x0; x <= x1; x++) { // for each pixel in the line find the correct y coordinate based on the slope and draw the pixel
        if (transpose)
            image.set(y, x, color);
        else
            image.set(x, y, color);
        error += derror; // increment the error
        if (error > dx) { // if the error is greater than 1 increment the y coordinate
            y += (y1 > y0 ? 1 : -1);
            error -= 2 * dx;
        }
    }
}

void triangle(Vec2i v0, Vec2i v1, Vec2i v2, TGAImage &image, const TGAColor &color) {
    if (v0.y > v1.y) {
        std::swap(v0, v1);
    }
    if (v1.y > v2.y) {
        std::swap(v1, v2);
    }
    if (v0.y > v1.y) {
        std::swap(v0, v1);
    }

    bool transpose01 = false;
    bool transpose02 = false;
    bool transpose12 = false;
    if (std::abs(v1.y - v0.y) < std::abs(v1.x - v0.x)) {
        transpose01 = true;
    }
    if (std::abs(v2.y - v1.y) < std::abs(v2.x - v1.x)) {
        transpose12 = true;
    }
    if (std::abs(v2.y - v0.y) < std::abs(v2.x - v0.x)) {
        transpose02 = true;
    }

    int x01 = v0.x;
    int dx01 = std::abs(v1.x - v0.x);
    int dy01 = std::abs(v1.y - v0.y);
    int derror01y = 2 * dy01;
    int derror01x = 2 * dx01;
    int error01 = 0;

    int x02 = v0.x;
    int dx02 = std::abs(v2.x - v0.x);
    int dy02 = std::abs(v2.y - v0.y);
    int derror02y = 2 * dy02;
    int derror02x = 2 * dx02;
    int error02 = 0;

    for (int y = v0.y; y <= v1.y; y++) {
        for(int k = std::min(x01, x02)+1; k <= std::max(x01, x02)-1; k++) {
            image.set(k, y, color);
        }

        if (transpose01) {
            while (error01 < dx01 && x01 != v1.x) {
                image.set(x01, y, color);
                error01 += derror01y;
                x01 += (v1.x > v0.x ? 1 : -1);
            }
            error01 -= 2 * dx01;
        } else {
            image.set(x01, y, color);
            error01 += derror01x;
            if (error01 > dy01) {
                x01 += (v1.x > v0.x ? 1 : -1);
                error01 -= 2 * dy01;
            }
        }
        if(y == v1.y) {
            break;
        }
        if(transpose02) {
            while(error02 < dx02 && x02 != v2.x) {
                image.set(x02, y, color);
                error02 += derror02y;
                x02 += (v2.x > v0.x ? 1 : -1);
            }
            error02 -= 2 * dx02;
        } else {
            image.set(x02, y, color);
            error02 += derror02x;
            if(error02 > dy02) {
                x02 += (v2.x > v0.x ? 1 : -1);
                error02 -= 2 * dy02;
            }
        }
    }


    int x12 = v1.x;
    int dx12 = std::abs(v2.x - v1.x);
    int dy12 = std::abs(v2.y - v1.y);
    int derror12y = 2 * dy12;
    int derror12x = 2 * dx12;
    int error12 = 0;

    for (int y = v1.y; y <= v2.y; y++) {
        for (int k = std::min(x12, x02) + 1; k <= std::max(x12, x02) - 1; k++) {
            image.set(k, y, color);
        }

        if (transpose12) {
            while (error12 < dx12 && x12 != v1.x) {
                image.set(x12, y, color);
                error12 += derror12y;
                x12 += (v2.x > v1.x ? 1 : -1);
            }
            error12 -= 2 * dx12;
        } else {
            image.set(x12, y, color);
            error12 += derror12x;
            if (error12 > dy12) {
                x12 += (v2.x > v1.x ? 1 : -1);
                error12 -= 2 * dy12;
            }
        }
        if (transpose02) {
            while (error02 < dx02 && x02 != v2.x) {
                image.set(x02, y, color);
                error02 += derror02y;
                x02 += (v2.x > v0.x ? 1 : -1);
            }
            error02 -= 2 * dx02;
        } else {
            image.set(x02, y, color);
            error02 += derror02x;
            if (error02 > dy02) {
                x02 += (v2.x > v0.x ? 1 : -1);
                error02 -= 2 * dy02;
            }
        }
    }
}

void triangle2(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color) {
    // sort the vertices, t0, t1, t2 lower−to−upper (bubblesort yay!)
    if (t0.y>t1.y) std::swap(t0, t1);
    if (t0.y>t2.y) std::swap(t0, t2);
    if (t1.y>t2.y) std::swap(t1, t2);

    line(t0.x, t0.y, t1.x, t1.y, image, white);
    line(t1.x, t1.y, t2.x, t2.y, image, white);
    line(t2.x, t2.y, t0.x, t0.y, image, white);

    int total_height = t2.y-t0.y;
    for (int y=t0.y; y<=t1.y; y++) {
        int segment_height = t1.y-t0.y+1;
        float alpha = (float)(y-t0.y)/total_height;
        float beta  = (float)(y-t0.y)/segment_height; // be careful with divisions by zero
        Vec2i A = t0 + (t2-t0)*alpha;
        Vec2i B = t0 + (t1-t0)*beta;
        if (A.x>B.x) std::swap(A, B);
        for (int j=A.x; j<=B.x; j++) {
            image.set(j, y, color); // attention, due to int casts t0.y+i != A.y
        }
    }
    for (int y=t1.y; y<=t2.y; y++) {
        int segment_height =  t2.y-t1.y+1;
        float alpha = (float)(y-t0.y)/total_height;
        float beta  = (float)(y-t1.y)/segment_height; // be careful with divisions by zero
        Vec2i A = t0 + (t2-t0)*alpha;
        Vec2i B = t1 + (t2-t1)*beta;
        if (A.x>B.x) std::swap(A, B);
        for (int j=A.x; j<=B.x; j++) {
            image.set(j, y, color); // attention, due to int casts t0.y+i != A.y
        }
    }
}

int main(int argc, char **argv) {
    TGAImage image(width, height, TGAImage::RGB); // initialize image
    Model model = Model("african_head.obj"); // import model

    Vec2i t0[3] = {Vec2i(10, 70), Vec2i(50, 160), Vec2i(70, 80)};
    Vec2i t1[3] = {Vec2i(180, 50), Vec2i(150, 1), Vec2i(70, 180)};
    Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};
//    triangle(t0[0], t0[1], t0[2], image, red);
//    triangle(t1[0], t1[1], t1[2], image, white);
//    triangle(t2[0], t2[1], t2[2], image, green);
    triangle2(t0[0], t0[1], t0[2], image, blue);
    triangle2(t1[0], t1[1], t1[2], image, blue);
    triangle2(t2[0], t2[1], t2[2], image, blue);

//    for (int i = 0; i < model.n_faces(); i++) { // iterate through faces
//        std::vector<int> face = model.face(i);
//        for (int j = 0; j < face.size(); j++) { // iterate through
//            int v1 = face[j];
//            int v2 = face[(j + 1) % face.size()];
//            Vec3f p1 = model.vertex(v1);
//            Vec3f p2 = model.vertex(v2);
//            line((p1.y + 0.5) * width / 2, (p1.z + 1) * height / 2, (p2.y + 0.5) * width / 2, (p2.z + 1) * height / 2, image,
//                 white);
//        }
//    }

//    line(20, 13, 40, 80, image, red);
//    line(80, 40, 13, 20, image, red);
//    line(13, 20, 80, 40, image, white);
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga"); // write image to file
    return 0;
}