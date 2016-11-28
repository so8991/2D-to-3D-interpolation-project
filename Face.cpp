/*
 
 USC/Viterbi/Computer Science
 "Jello Cube" Assignment 1 starter code
 
 */

#include "jello.h"
#include "Face.h"
#include <vector>
#include <iostream>
using namespace std;
using namespace glm;
Face::Face(istream &in)
{
    char* group[4];
    group[0] = new char[100];
    group[1] = new char[100];
    group[2] = new char[100];
    group[3] = new char[100];
    cout<<"object created!"<<endl;
    std::vector< glm::vec4 > positions( 1, glm::vec4( 0, 0, 0, 0 ) );
    std::vector< glm::vec3 > texcoords( 1, glm::vec3( 0, 0, 0 ) );
    std::vector< glm::vec3 > normals( 1, glm::vec3( 0, 0, 0 ) );
    string lineStr;
    while(getline(in,lineStr))
    {
        istringstream lineSS(lineStr);
        string lineType;
        lineSS >> lineType;
        //vertex
        if(lineType=="g")
        {
            //group[3]="";
            lineSS >> group[0] >> group[1] >> group[2] >> group[3];
        }
        if(lineType=="v")
        {
            float x = 0, y = 0, z = 0, w = 1;
            lineSS >> x >> y >>z >> w;
            positions.push_back(glm::vec4(x,y,z,w));
        }
        //texture
        if(lineType=="vt")
        {
            float u = 0, v = 0, w = 0;
            lineSS >> u >> v >> w;
            texcoords.push_back(glm::vec3(u,v,w));
        }
        //normal
        if(lineType=="vn")
        {
            float i=0, j=0, k=0;
            lineSS >> i >> j >> k;
            normals.push_back(normalize(glm::vec3(i,j,k)));
        }
        if(lineType=="c")
        {
            break;
        }
        //polygon
        if(lineType=="f")
        {
            vector<VertRef> refs;
            string refStr;
            while(lineSS >> refStr)
            {
                istringstream ref(refStr);
                string vStr, vtStr, vnStr;
                getline(ref, vStr, '/');
                getline( ref, vtStr, '/' );
                getline( ref, vnStr, '/' );
                int v = atoi( vStr.c_str() );
                int vt = atoi( vtStr.c_str() );
                int vn = atoi( vnStr.c_str() );
                v  = (  v >= 0 ?  v : positions.size() +  v );
                vt = ( vt >= 0 ? vt : texcoords.size() + vt );
                vn = ( vn >= 0 ? vn : normals.size()   + vn );
                refs.push_back( VertRef( v, vt, vn ) );
            }
            if(refs.size()<3)
            {
                continue;
            }
            VertRef* p[3] = {&refs[0],NULL, NULL};
            for( size_t i = 1; i+1 < refs.size(); ++i )
            {
                p[1] = &refs[i+0];
                p[2] = &refs[i+1];
                
                // http://www.opengl.org/wiki/Calculating_a_Surface_Normal
                glm::vec3 U( positions[ p[1]->v ] - positions[ p[0]->v ] );
                glm::vec3 V( positions[ p[2]->v ] - positions[ p[0]->v ] );
                glm::vec3 faceNormal = normalize( cross( U, V ) );
                
                for( size_t j = 0; j < 3; ++j )
                {
                    Vertex vert;
                    vert.position = glm::vec3( positions[ p[j]->v ] );
                    vert.texcoord = glm::vec2( texcoords[ p[j]->vt ] );
                    vert.normal   = ( p[j]->vn != 0 ? normals[ p[j]->vn ] : faceNormal );
                    vert.group[0] = group[0];
                    vert.group[1] = group[1];
                    vert.group[2] = group[2];
                    vert.group[3] = group[3];
                    verts.push_back( vert );
                }
            }
        }
    }
}
vector<Vertex> Face::getModel()
{
   
    return this->verts;
   // return model;
}
vector<Vertex> Face::scaleModel(float a)
{
    vector<Vertex> temp;// = a*(this->verts);
    Vertex v[24];
    for(int i=0;i<24;i++)
    {
       
        glm::vec3 pos(this->verts[i].position.x, this->verts[i].position.y, this->verts[i].position.z);
        glm::vec2 tex(this->verts[i].texcoord.x, this->verts[i].texcoord.y);
        glm::vec3 nor(this->verts[i].normal.x, this->verts[i].normal.y, this->verts[i].normal.z);
        v[i].position = pos;
        v[i].texcoord = tex;
        v[i].normal = nor;
        temp.push_back(v[i]);
    }
    
    /*for(int i=0;i<sizeof(this->verts);i++)
    {
        
        tmpvert.position = glm::vec3(this->verts[i].position.x, this->verts[i].position.y, this->verts[i].position.z);
        tmpvert.texcoord = glm::vec2(this->verts[i].texcoord.x, this->verts[i].texcoord.y);
        tmpvert.normal = glm::vec3(this->verts[i].normal.x, this->verts[i].normal.y, this->verts[i].normal.z);
        temp.push_back(tmpvert);
    }
    */
    return temp;
}
/*
glm::vec3 Face::getCenter()
{
   / float baseVal[3]={0.0};

    for(int i=0;i<30924;i++)
    {
        baseVal[0] += this->verts[i].position.x;
    }
    baseVal[0] /= 30924;
    
    for(int i=0;i<30924;i++)
    {
        baseVal[1] += this->verts[i].position.y;
    }
    baseVal[1] /= 30924;
    
    for(int i=0;i<30924;i++)
    {
        baseVal[2] += this->verts[i].position.y;
    }
    baseVal[2] /= 30924;
    
    //float base = this->verts[0].position.x;
    
    return glm::vec3(baseVal[0], baseVal[1], baseVal[2]);
}*/
vector<vec3> Face::getDelta(vector<Vertex> base)
{
    //vector<vec3> target = getModel();
    //vector<vec3> delta;
    for(int i=0; i<verts.size(); i++)
    {
        delta.push_back(vec3(verts.at(i).position.x-base.at(i).position.x, verts.at(i).position.y-base.at(i).position.y,verts.at(i).position.z-base.at(i).position.z));
    }
    avgDeltaShape.x = 0.0;
    avgDeltaShape.y = 0.0;
    avgDeltaShape.z = 0.0;
    for(int i=0;i<verts.size(); i++)
    {
        avgDeltaShape.x += delta.at(i).x;
        avgDeltaShape.y += delta.at(i).y;
        avgDeltaShape.z += delta.at(i).z;
    }
    avgDeltaShape.x /= (float)verts.size();
    avgDeltaShape.y /= (float)verts.size();
    avgDeltaShape.z /= (float)verts.size();
    //return this->verts;
    return delta;
}
