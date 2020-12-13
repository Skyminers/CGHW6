//
// Created by 刘一辰 on 2020/12/12.
//

#ifndef SOLARSYSTEM_BEZIER_H
#define SOLARSYSTEM_BEZIER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define LEFT_KEY_POINT 2.6
#define HEIGHT 1.9
#define STANDER_HEIGHT 0.9
#define RIGHT_KEY_POINT 5.4
#define LENU 100
#define LENV 100
#define LEN_PATCHES 10
#define DEG 3
#define MAX_CP_POINTS 404

struct Vertex { GLfloat x, y, z; };
struct Vertex carVertices[] = {
        {0.5,0,0},
        {0.5,0,1},
        {0.5,0,2},
        {0.5,0,3},
        {0.5,0.1,0},
        {0.5,0.1,1},
        {0.5,0.1,2},
        {0.5,0.1,3},
        {0.5,0.8,0},
        {0.5,0.8,1},
        {0.5,0.8,2},
        {0.5,0.8,3},
        {1.2,STANDER_HEIGHT,0},
        {1.2,STANDER_HEIGHT,1},
        {1.2,STANDER_HEIGHT,2},
        {1.2,STANDER_HEIGHT,3},
        // 16

        {LEFT_KEY_POINT,STANDER_HEIGHT,0},
        {LEFT_KEY_POINT,STANDER_HEIGHT,1},
        {LEFT_KEY_POINT,STANDER_HEIGHT,2},
        {LEFT_KEY_POINT,STANDER_HEIGHT,3},
        {LEFT_KEY_POINT+0.6,HEIGHT,0.3},
        {LEFT_KEY_POINT+0.6,HEIGHT,1},
        {LEFT_KEY_POINT+0.6,HEIGHT,2},
        {LEFT_KEY_POINT+0.6,HEIGHT,2.7},
        {RIGHT_KEY_POINT-0.05,HEIGHT-0.1,0.3},
        {RIGHT_KEY_POINT-0.05,HEIGHT-0.1,1},
        {RIGHT_KEY_POINT-0.05,HEIGHT-0.1,2},
        {RIGHT_KEY_POINT-0.05,HEIGHT-0.1,2.7},
        {RIGHT_KEY_POINT,STANDER_HEIGHT,0},
        {RIGHT_KEY_POINT,STANDER_HEIGHT,1},
        {RIGHT_KEY_POINT,STANDER_HEIGHT,2},
        {RIGHT_KEY_POINT,STANDER_HEIGHT,3},
        //32

        {RIGHT_KEY_POINT+0.6,STANDER_HEIGHT,0},
        {RIGHT_KEY_POINT+0.6,STANDER_HEIGHT,1},
        {RIGHT_KEY_POINT+0.6,STANDER_HEIGHT,2},
        {RIGHT_KEY_POINT+0.6,STANDER_HEIGHT,3},
        {RIGHT_KEY_POINT+0.85,0.6,0},
        {RIGHT_KEY_POINT+0.85,0.6,1},
        {RIGHT_KEY_POINT+0.85,0.6,2},
        {RIGHT_KEY_POINT+0.85,0.6,3},
        {RIGHT_KEY_POINT+0.85,0.3,0},
        {RIGHT_KEY_POINT+0.85,0.3,1},
        {RIGHT_KEY_POINT+0.85,0.3,2},
        {RIGHT_KEY_POINT+0.85,0.3,3},
        {RIGHT_KEY_POINT+0.8,0,0},
        {RIGHT_KEY_POINT+0.8,0,1},
        {RIGHT_KEY_POINT+0.8,0,2},
        {RIGHT_KEY_POINT+0.8,0,3},
        // 48

        {LEFT_KEY_POINT,0,0},
        {LEFT_KEY_POINT,0.33,0},
        {LEFT_KEY_POINT,0.66,0},
        {LEFT_KEY_POINT, STANDER_HEIGHT, 0},
        {RIGHT_KEY_POINT,0,0},
        {RIGHT_KEY_POINT,0.33,0},
        {RIGHT_KEY_POINT,0.66,0},
        {RIGHT_KEY_POINT,STANDER_HEIGHT,0},
        {LEFT_KEY_POINT,0,3},
        {LEFT_KEY_POINT,0.33,3},
        {LEFT_KEY_POINT,0.66,3},
        {LEFT_KEY_POINT, STANDER_HEIGHT, 3},
        {RIGHT_KEY_POINT,0,3},
        {RIGHT_KEY_POINT,0.33,3},
        {RIGHT_KEY_POINT,0.66,3},
        {RIGHT_KEY_POINT,STANDER_HEIGHT,3},
        //64

        {LEFT_KEY_POINT+1, STANDER_HEIGHT,0},
        {RIGHT_KEY_POINT-1,STANDER_HEIGHT,0},
        {LEFT_KEY_POINT+1, STANDER_HEIGHT, 3},
        {RIGHT_KEY_POINT-1,STANDER_HEIGHT,3},

};
GLuint carPatches[][DEG + 1][DEG + 1] = {
        {{1,  2,  3,  4},  {5,  6,  7,  8},  {9,  10, 11, 12}, {13, 14, 15, 16}}, // 前弧
        {{17, 18, 19, 20}, {21, 22, 23, 24}, {25, 26, 27, 28}, {29, 30, 31, 32}}, // 车顶
        {{13, 14, 15, 16}, {13, 14, 15, 16}, {17, 18, 19, 20}, {17, 18, 19, 20}}, // 前盖
        {{29, 30, 31, 32}, {29, 30, 31, 32}, {33, 34, 35, 36}, {33, 34, 35, 36}}, // 后盖
        {{33, 34, 35, 36}, {37, 38, 39, 40}, {41, 42, 43, 44}, {45, 46, 47, 48}}, // 后狐
        {{1,  2,  3,  4},  {1,  2,  3,  4},  {45, 46, 47, 48}, {45, 46, 47, 48}}, // 底板
        {{1,  5,  9,  13}, {49, 50, 51, 52}, {53, 54, 55, 56}, {45, 41, 37, 33}}, // 右车门
        {{4,  8,  12, 16}, {57, 58, 59, 60}, {61, 62, 63, 64}, {48, 44, 40, 36}}, // 左车门
        {{52, 65, 66, 56}, {52, 65, 66, 56}, {17, 21, 25, 29}, {17, 21, 25, 29}}, // 右挡风
        {{60, 67, 68, 64}, {60, 67, 68, 64}, {20, 24, 28, 32}, {20, 24, 28, 32}}, // 左挡风
};
GLfloat patchesColor[][3] = {
        {204, 102, 0},
        {255, 153, 51},
        {155, 153, 102},
        {155, 153, 102},
        {255, 102, 102},
        {0,   102, 0},
        {255, 80,  80},
        {255, 80,  80},
        {133, 153, 53},
        {133, 153, 53},
};

struct Vertex realVertices[LEN_PATCHES * LENU * LENV];
GLfloat realColors[LEN_PATCHES * LENU * LENV * 3];
GLuint realElements[LEN_PATCHES * (LENU - 1) * (LENV - 1) * 2 * 3];

GLfloat CPColors[MAX_CP_POINTS * 3];
GLuint CPElements[LEN_PATCHES][DEG + 1][DEG + 1];

inline int fact(int n) {
    int result = 1;
    for (int i = n; i > 1; i--) result *= i;
    return result;
}

inline float C(int i, int n) { return 1.0f * fact(n) / (fact(i) * fact(n - i)); }

inline float B(int i, int n, float u) { return C(i, n) * powf(u, i) * powf(1 - u, n - i); }

void Bezier() {
    static struct Vertex CP[DEG + 1][DEG + 1];
    // Vertices
    for (int p = 0; p < LEN_PATCHES; p++) {
        for (int i = 0; i <= DEG; i++)
            for (int j = 0; j <= DEG; j++)
                CP[i][j] = carVertices[carPatches[p][i][j] - 1];

        for (int ru = 0; ru <= LENU - 1; ru++) {
            float u = 1.0 * ru / (LENU - 1);
            for (int rv = 0; rv <= LENV - 1; rv++) {
                float v = 1.0 * rv / (LENV - 1);

                Vertex &res = realVertices[p * LENU * LENV + ru * LENV + rv];
                res = {0.0, 0.0, 0.0};
                for (int i = 0; i <= DEG; i++) {
                    float poly_i = B(i, DEG, u);
                    for (int j = 0; j <= DEG; j++) {
                        float poly_j = B(j, DEG, v);
                        res.x += poly_i * poly_j * CP[i][j].x;
                        res.y += poly_i * poly_j * CP[i][j].y;
                        res.z += poly_i * poly_j * CP[i][j].z;
                    }
                }

                realColors[p * LENU * LENV * 3 + ru * LENV * 3 + rv * 3 + 0] = patchesColor[p][0] / 255;
                realColors[p * LENU * LENV * 3 + ru * LENV * 3 + rv * 3 + 1] = patchesColor[p][1] / 255;
                realColors[p * LENU * LENV * 3 + ru * LENV * 3 + rv * 3 + 2] = patchesColor[p][2] / 255;
            }
        }
    }

    // Elements
    int n = 0;
    for (int p = 0; p < LEN_PATCHES; p++)
        for (int ru = 0; ru < LENU - 1; ru++)
            for (int rv = 0; rv < LENV - 1; rv++) {
                realElements[n++] = p * LENU * LENV + ru * LENV + rv;
                realElements[n++] = p * LENU * LENV + ru * LENV + (rv + 1);
                realElements[n++] = p * LENU * LENV + (ru + 1) * LENV + (rv + 1);
                realElements[n++] = p * LENU * LENV + (ru + 1) * LENV + (rv + 1);
                realElements[n++] = p * LENU * LENV + (ru + 1) * LENV + rv;
                realElements[n++] = p * LENU * LENV + ru * LENV + rv;
            }

    for (float &CPColor : CPColors) {
        CPColor = 1;
    }

    for (int p = 0; p < LEN_PATCHES; p++)
        for (int i = 0; i < (DEG + 1); i++)
            for (int j = 0; j < (DEG + 1); j++)
                CPElements[p][i][j] = carPatches[p][i][j] - 1;
}

#endif //SOLARSYSTEM_BEZIER_H
