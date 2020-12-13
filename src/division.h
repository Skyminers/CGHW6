//
// Created by 刘一辰 on 2020/12/13.
//

#ifndef SOLARSYSTEM_DIVISION_H
#define SOLARSYSTEM_DIVISION_H

#include <iostream>
#include <cstdio>
#include <string>
#include <vector>

const float PI = acos(-1);

using namespace std;
struct Point {float x,y,z;};
struct Face {int order[3];};

struct HalfEdge {
    int ori;
    HalfEdge* nxt, *cvt;
};

void doSubdivision(char *fromObj, char *toObj,int times);
void subdivision();
void readOBJ(const char* filename);
void writeOBJ(const char *filename);
void init();


vector<Point> points;
vector<Face> faces;
vector<HalfEdge*> edges;
int e_num, n_node = 0, n_face = 0, n_edge = 0;

void doSubdivision(char *fromObj, char *toObj,int times){
    readOBJ(fromObj);
    init();
    while(times--) subdivision();
    writeOBJ(toObj);
}

void subdivision() {
    vector<Point> points_new;
    vector<Face> faces_new;
    vector<HalfEdge*> edges_new;


    int n;
    float p_sumx,p_sumy,p_sumz;
    float px,py,pz;
    float u;
    for (int i = 0; i < n_node; ++i) {
        HalfEdge *he = NULL;
        for(int k = 0; k < n_edge; ++k) {
            if (edges[k]->ori == i) {
                he = edges[k];
                break;
            }
        }

        if(he != NULL) {
            n = 0;
            p_sumx = 0;
            p_sumy = 0;
            p_sumz = 0;
            HalfEdge* e = he->nxt;
            for (int p0 = e->ori; ; e = e->nxt->cvt->nxt) {
                n++;
                p_sumx += points[e->nxt->ori].x;
                p_sumy += points[e->nxt->ori].y;
                p_sumz += points[e->nxt->ori].z;
                if (e->nxt->ori == p0) break;
            }
            if (n == 3) {
                u = 3.0 / 16;
            } else {
                u = 3.0 / (8 * n);
            }
            px=(1 - n * u) * points[i].x + u * p_sumx;
            py=(1 - n * u) * points[i].y + u * p_sumy;
            pz=(1 - n * u) * points[i].z + u * p_sumz;

            points_new.push_back(Point{px, py, pz});
        }
    }

    int** map1 = new int*[n_node];
    for (int i = 0; i < n_node; ++i) {
        map1[i] = new int[n_node];
        for (int j = 0; j < n_node; ++j) {
            map1[i][j] = 0;
        }
    }
    for (int i = 0; i < n_edge; ++i) {
        if (!map1[edges[i]->ori][edges[i]->nxt->ori]) {
            int p1 = edges[i]->ori;
            int p2 = edges[i]->nxt->ori;
            int p3 = edges[i]->nxt->nxt->ori;
            int p4 = edges[i]->cvt->nxt->nxt->ori;
            px = 0.375 * (points[p1].x + points[p2].x) + 0.125 * (points[p3].x + points[p4].x);
            py = 0.375 * (points[p1].y + points[p2].y) + 0.125 * (points[p3].y + points[p4].y);
            pz = 0.375 * (points[p1].z + points[p2].z) + 0.125 * (points[p3].z + points[p4].z);
            points_new.push_back(Point{px, py, pz});

            map1[edges[i]->ori][edges[i]->nxt->ori] = points_new.size() - 1;
            map1[edges[i]->nxt->ori][edges[i]->ori] = points_new.size() - 1;
        }
    }
    for (int i = 0; i < n_face; ++i) {
        int a, b, c, d, e, f;
        a = faces[i].order[0];
        b = faces[i].order[1];
        c = faces[i].order[2];
        d = map1[a][b];
        e = map1[b][c];
        f = map1[a][c];
        faces_new.push_back(Face{a, d, f});
        faces_new.push_back(Face{d, b, e});
        faces_new.push_back(Face{d, e, f});
        faces_new.push_back(Face{f, e, c});
    }
    for (int i = 0; i < n_node; ++i) delete[] map1[i];
    delete[] map1;
    n_face = faces_new.size();
    n_node = points_new.size();
    int** map2 = new int*[n_node];
    for (int i = 0; i < n_node; ++i) {
        map2[i] = new int[n_node];
        for (int j = 0; j < n_node; ++j) {
            map2[i][j] = -1;
        }
    }
    e_num = 0;
    for (int i = 0; i < n_face; ++i) {
        HalfEdge* edge4 = new HalfEdge();
        HalfEdge* edge5 = new HalfEdge();
        HalfEdge* edge6 = new HalfEdge();

        edge4->ori = faces_new[i].order[0];
        edge5->ori = faces_new[i].order[1];
        edge6->ori = faces_new[i].order[2];

        edge4->nxt = edge5;
        edge5->nxt = edge6;
        edge6->nxt = edge4;

        HalfEdge* temp;
        if (map2[faces_new[i].order[1]][faces_new[i].order[0]] != -1) {
            temp = edges_new[map2[faces_new[i].order[1]][faces_new[i].order[0]]];
            edge4->cvt = temp;
            temp->cvt = edge4;
        } else {
            edge4->cvt = NULL;
            map2[faces_new[i].order[0]][faces_new[i].order[1]] = e_num;
        }
        e_num++;
        if (map2[faces_new[i].order[2]][faces_new[i].order[1]] != -1) {
            temp = edges_new[map2[faces_new[i].order[2]][faces_new[i].order[1]]];
            edge5->cvt = temp;
            temp->cvt = edge5;
        } else {
            edge5->cvt = NULL;
            map2[faces_new[i].order[1]][faces_new[i].order[2]] = e_num;
        }
        e_num++;
        if (map2[faces_new[i].order[0]][faces_new[i].order[2]] != -1) {
            temp = edges_new[map2[faces_new[i].order[0]][faces_new[i].order[2]]];
            edge6->cvt = temp;
            temp->cvt = edge6;
        } else {
            edge6->cvt = NULL;
            map2[faces_new[i].order[2]][faces_new[i].order[0]] = e_num;
        }
        e_num++;

        edges_new.push_back(edge4);
        edges_new.push_back(edge5);
        edges_new.push_back(edge6);
    }
    for (int i = 0; i < n_node; ++i) delete[] map2[i];
    delete[] map2;
    n_edge = edges_new.size();
    points.assign(points_new.begin(),points_new.end());
    faces.assign(faces_new.begin(),faces_new.end());
    edges.assign(edges_new.begin(),edges_new.end());
}

void readOBJ(const char* filename){
    FILE *f = fopen(filename, "r");

    if(f == NULL) {
        cout << "Error: failed to open file!" << endl;
        exit(0);
    }

    char buffer[1024];
    char splitter[] = " ,\t\n";
    while (true) {
        if (!fgets(buffer, 1024, f)) break;
        char *str = strtok(buffer, splitter);

        if (!str) continue;

        if (!strcmp(str, "v")) {
            float p[3];
            for (int i = 0; i < 3; ++i) {
                str = strtok(NULL, splitter);
                p[i] = atof(str);
            }
            points.push_back(Point{p[0], p[1], p[2]});
            n_node++;
        }
        if (!strcmp(str, "f")) {
            int p[3];
            for (int i = 0; i < 3; ++i) {
                str = strtok(NULL, splitter);
                p[i] = atoi(str) - 1;
            }
            faces.push_back(Face{p[0], p[1], p[2]});
            n_face++;
        }
    }
}

void writeOBJ(const char *filename) {
    FILE *fp = fopen(filename, "w");
    for (auto it = points.begin(); it != points.end(); ++it) {
        fprintf(fp, "v %g %g %g\n", it->x, it->y, it->z);
    }
    for (auto it = faces.begin(); it != faces.end(); ++it) {
        fprintf(fp, "f %d %d %d\n", it->order[0] + 1, it->order[1] + 1, it->order[2] + 1);
    }
    fclose(fp);
}

void init() {
    int **map = new int*[n_node];
    for (int i = 0; i < n_node; ++i) {
        map[i] = new int[n_node];
        for(int j = 0; j < n_node; ++j) {
            map[i][j] = -1;
        }
    }
    e_num=0;
    for (int i = 0; i < n_face; ++i) {
        HalfEdge* edge1 = new HalfEdge();
        HalfEdge* edge2 = new HalfEdge();
        HalfEdge* edge3 = new HalfEdge();

        edge1->ori=faces[i].order[0];
        edge2->ori=faces[i].order[1];
        edge3->ori=faces[i].order[2];

        edge1->nxt=edge2;
        edge2->nxt=edge3;
        edge3->nxt=edge1;

        HalfEdge* temp;
        if(map[faces[i].order[1]][faces[i].order[0]] != -1) {
            temp = edges[map[faces[i].order[1]][faces[i].order[0]]];
            edge1->cvt = temp;
            temp->cvt = edge1;
        } else {
            edge1->cvt = NULL;
            map[faces[i].order[0]][faces[i].order[1]] = e_num;
        }
        e_num++;
        if (map[faces[i].order[2]][faces[i].order[1]] != -1) {
            temp = edges[map[faces[i].order[2]][faces[i].order[1]]];
            edge2->cvt = temp;
            temp->cvt = edge2;
        } else {
            edge2->cvt = NULL;
            map[faces[i].order[1]][faces[i].order[2]] = e_num;
        }
        e_num++;
        if(map[faces[i].order[0]][faces[i].order[2]] != -1) {
            temp = edges[map[faces[i].order[0]][faces[i].order[2]]];
            edge3->cvt = temp;
            temp->cvt = edge3;
        } else {
            edge3->cvt = NULL;
            map[faces[i].order[2]][faces[i].order[0]] = e_num;
        }
        e_num++;

        edges.push_back(edge1);
        edges.push_back(edge2);
        edges.push_back(edge3);
    }
    n_edge = edges.size();
    for (int i = 0; i < n_node; ++i) delete[] map[i];
    delete[] map;
}

#endif //SOLARSYSTEM_DIVISION_H
