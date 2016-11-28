/*
 
 USC/Viterbi/Computer Science
 "Jello Cube" Assignment 1 starter code
 
 */


#ifndef _SHOWCUBE_H_
#define _SHOWCUBE_H_
#include <vector>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
//#include <glm/glm.hpp>
#include "/usr/local/include/glm/glm.hpp"
#include "jello.h"
using namespace std;
using namespace glm;
struct Vertex
{
    glm::vec3 position;
    glm::vec2 texcoord;
    glm::vec3 normal;
    char* group[4];
    
};
struct VertRef
{
    VertRef(int v, int vt, int vn)
    :v(v), vt(vt), vn(vn)
    { }
    int v, vt, vn;
};
class Face
{
public:
    vector<Vertex> getModel();
    glm::vec3 getCenter();
    vector<vec3> getDelta(vector<Vertex> base);
    Face(istream& in);
    vector<Vertex> scaleModel(float a);
//private:
    vector<Vertex> verts;
    vector<vec3> delta;
    //vector<char*> group[4];
    char *objType;  // type of the object
    glm::vec3 centerVert;
    vec3 avgDeltaShape;

};

#endif
