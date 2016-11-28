

/*
 
 USC/Viterbi/Computer Science
 "Jello Cube" Assignment 1 starter code
 
 Your name:
Sunoh Yoo
 
 */

#include "jello.h"
#include "Face.h"
#include "input.h"
#include "physics.h"
#include "calcCounter.h"
#include "pic.h"

#include <cmath>
#include "Webcam.h"

#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
//#include "/usr/local/include/opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;
//static calcCounter counter;
vector<float> baseStatus;
float alpha = 0.0;
vector<Vertex> base;
vector<Vertex> delta;
int start_toggle = 1; //1 : start otherwise 0

float rot_angle;
float centerPoint[6];
float transParam=0.0;
// camera parameters
double Theta = pi / 6;
double Phi = pi / 6;
double R = 6;
double angleInRadians;
double prevAngle=0.0;
double curAngle;
// mouse control
static int ccount=0;

int g_iMenuId;
int g_vMousePos[2];
int g_iLeftMouseButton,g_iMiddleMouseButton,g_iRightMouseButton;
string shapefile = "shape_predictor_68_face_landmarks.dat";
dlib::frontal_face_detector detector =dlib::get_frontal_face_detector();
//std::vector<dlib::rectangle> dets;
dlib::shape_predictor sp;
Face *facemodel;
vector<Vertex> finalModel;
vector<Vertex> model[10];
vector<Vertex> target[10];
vector<float> landmarkVect;
vector<vec3> target_delta[10];
vec3 avgDeltaShape[10];
vec3 avgDeltaFrame;
vector<Vertex> morphModel[40];
float morph_weight[10];
std::vector<dlib::full_object_detection> shapes;
dlib::full_object_detection shape;

vector<float> baseFrame;
vector<float> delToTarget;
//vector<glm::vec3> model;
// number of images saved to disk so far
int sprite=0;

// these variables control what is displayed on screen
int shear=0, bend=0, structural=1, ppause=0, viewingMode=0, saveScreenToFile=0;
double fps;
bool mirror = true;
struct world jello;
//Webcam *frame;
CvCapture* g_Capture;
int windowWidth, windowHeight;
int frameWidth, frameHight;
int btn;
glm::ivec2 startMouse;
glm::ivec2 startRot, curRot;
glm::ivec2 startTrans, curTrans;
double angleInDegrees;
// global ambient light
GLfloat aGa[] = { 1.0, 1.0, 0.0, 0.0 };

// light 's ambient, diffuse, specular
GLfloat lKa0[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat lKd0[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat lKs0[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat lKa1[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat lKd1[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat lKs1[] = { 1.0, 0.0, 0.0, 1.0 };

GLfloat lKa2[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat lKd2[] = { 1.0, 1.0, 0.0, 1.0 };
GLfloat lKs2[] = { 1.0, 1.0, 0.0, 1.0 };

GLfloat lKa3[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat lKd3[] = { 0.0, 1.0, 1.0, 1.0 };
GLfloat lKs3[] = { 0.0, 1.0, 1.0, 1.0 };

GLfloat lKa4[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat lKd4[] = { 0.0, 0.0, 1.0, 1.0 };
GLfloat lKs4[] = { 0.0, 0.0, 1.0, 1.0 };

GLfloat lKa5[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat lKd5[] = { 1.0, 0.0, 1.0, 1.0 };
GLfloat lKs5[] = { 1.0, 0.0, 1.0, 1.0 };

GLfloat lKa6[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat lKd6[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat lKs6[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat lKa7[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat lKd7[] = { 0.0, 1.0, 1.0, 1.0 };
GLfloat lKs7[] = { 0.0, 1.0, 1.0, 1.0 };

// light positions and directions
GLfloat lP0[] = { -1.999, -1.999, -1.999, 1.0 };
GLfloat lP1[] = { 1.999, -1.999, -1.999, 1.0 };
GLfloat lP2[] = { 1.999, 1.999, -1.999, 1.0 };
GLfloat lP3[] = { -1.999, 1.999, -1.999, 1.0 };
GLfloat lP4[] = { -1.999, -1.999, 1.999, 1.0 };
GLfloat lP5[] = { 1.999, -1.999, 1.999, 1.0 };
GLfloat lP6[] = { 1.999, 1.999, 1.999, 1.0 };
GLfloat lP7[] = { -1.999, 1.999, 1.999, 1.0 };

// jelly material color

GLfloat mKa[] = { 0.3, 0.0, 0.3, 1.0 };
GLfloat mKd[] = { 0.3, 0.3, 0.3, 1.0 };
GLfloat mKs[] = { 0.3, 0.0, 0.3, 1.0 };
GLfloat mKe[] = { 0.3, 0.0, 0.3, 1.0 };

double distance_to_Line(cv::Point line_start, cv::Point line_end, cv::Point point)
{
    cv::Point line; //direction of the line
    line.x = line_end.x - line_start.x;
    line.y = line_end.y - line_start.y;
    cv::Point point_to_start; //vector from the point to the start of the line
    point_to_start.x = line_start.x - point.x;
    point_to_start.y = line_start.y - point.y;
    int dot = point_to_start.x * line.x + point_to_start.y * line.y; //dot product of point_to_start * line
    cv::Point distance; //shortest distance vector from point to line
    distance.x = point_to_start.x - dot * line.x;
    distance.y = point_to_start.y - dot * line.y;
    double output = sqrt((double)(distance.x * distance.x + distance.y * distance.y));
    return output;
}

double current_time_in_seconds()
{
    timeval timer;
    gettimeofday(&timer,NULL);
    double seconds = 1e-6 * timer.tv_usec + timer.tv_sec;
    return seconds;
}

void mouse(int button, int state, int x, int y )
{
    if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
    {
        btn = button;
        startMouse = glm::ivec2( x, glutGet( GLUT_WINDOW_HEIGHT ) - y );
        startRot = curRot;
    }
    if( button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN )
    {
        btn = button;
        startMouse = glm::ivec2( x, glutGet( GLUT_WINDOW_HEIGHT ) - y );
        startTrans = curTrans;
    }
    glutPostRedisplay();
}

void motion( int x, int y )
{
    glm::ivec2 curMouse( x, glutGet( GLUT_WINDOW_HEIGHT ) - y );
    if( btn == GLUT_LEFT_BUTTON )
    {
        curRot = startRot + ( curMouse - startMouse );
    }
    else if( btn == GLUT_RIGHT_BUTTON )
    {
        curTrans = startTrans + ( curMouse - startMouse );
    }
    glutPostRedisplay();
}


void myinit()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(90.0,1.0,0.01,1000.0);
    
    // set background color to grey
    glClearColor(0.5, 0.5, 0.5, 0.0);
    
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    
    glShadeModel(GL_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    
    return;
}

void reshape(int w, int h)
{
    // Prevent a divide by zero, when h is zero.
    // You can't make a window of zero height.
    glViewport(0, 0, w, h);
    if(h == 0)
        h = 1;
    
    glViewport(0, 0, w, h);
    
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Set the perspective
    double aspectRatio = 1.0 * w / h;
    gluPerspective(60.0f, aspectRatio, 0.01f, 1000.0f);
    //gluPerspective(45,double(w)/double(h),1, 100 );
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    windowWidth = w;
    windowHeight = h;
    cout<<"reshape"<<endl;
    glutPostRedisplay();
}
void drawMesh2(glm::ivec2 curTrans, glm::ivec2 curRot, int index)
{
    //cout<<model[0].position.x<<endl;
    cout<<"draw mesh!"<<endl;
    glPushMatrix();
    //glScalef(2.5,2.5,2.5);
    glScalef(0.07,0.07,0.07);
    glTranslatef(3, 5, -8.0);
    glColor3ub( 255, 0, 0 );
    
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    glEnableClientState( GL_NORMAL_ARRAY );
    
    glVertexPointer( 3, GL_FLOAT, sizeof(Vertex), &model[index][0].position );
    glTexCoordPointer( 2, GL_FLOAT, sizeof(Vertex), &model[index][0].texcoord );
    glNormalPointer( GL_FLOAT, sizeof(Vertex), &model[index][0].normal);
    
    
    glDrawArrays( GL_TRIANGLES, 0, model[index].size() );
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    glDisableClientState( GL_NORMAL_ARRAY );
    
    glPopMatrix();
    
}

void drawMesh(glm::ivec2 curTrans, glm::ivec2 curRot)
{
    //cout<<model[0].position.x<<endl;
    cout<<"draw mesh!"<<endl;
    glPushMatrix();
    //glScalef(2.5,2.5,2.5);
    glScalef(0.07,0.07,0.07);
    glTranslatef(3, 5, -8.0);
    //cout<<"center : "<<centerPoint<<endl;
    glColor3ub( 255, 0, 0 );
    
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    glEnableClientState( GL_NORMAL_ARRAY );
    
    glVertexPointer( 3, GL_FLOAT, sizeof(Vertex), &finalModel[0].position );
    glTexCoordPointer( 2, GL_FLOAT, sizeof(Vertex), &finalModel[0].texcoord );
    glNormalPointer( GL_FLOAT, sizeof(Vertex), &finalModel[0].normal);
    

    glDrawArrays( GL_TRIANGLES, 0, finalModel.size() );
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    glDisableClientState( GL_NORMAL_ARRAY );
    
    glPopMatrix();
    
}
float vecLength(float X, float Y, float Z){
    return sqrt(X * X + Y * Y + Z * Z);
}
void getLandmarkPoints(IplImage* frame)
{
   
    int percent = 20;
    IplImage *destination = cvCreateImage ( cvSize((int)((frame->width*percent)/100) , (int)((frame->height*percent)/100) ), frame->depth, frame->nChannels );
    cout<<destination->width<<" and hiehgt: "<<destination->height<<endl;
    cvResize(frame, destination);
    dlib::cv_image<dlib::bgr_pixel> img(destination);

    std::vector<dlib::rectangle> dets = detector(img);
    
    cout << "Number of faces detected: " << dets.size() << endl;
    
    if(dets.size()==1)
    {
        //if face detected!

        for (unsigned long j = 0; j < 1; ++j)
        {
        
            shape = sp(img, dets[j]);
            shapes.push_back(shape);
            
        }
        
        if(start_toggle==1)
        {
            landmarkVect.push_back(vec3(shape.part(36).x(), shape.part(36).y(), 0.0).length());
            landmarkVect.push_back(vec3(shape.part(39).x(), shape.part(39).y(), 0.0).length());
            landmarkVect.push_back(vec3(shape.part(42).x(), shape.part(42).y(), 0.0).length());
            landmarkVect.push_back(vec3(shape.part(45).x(), shape.part(45).y(), 0.0).length());
            landmarkVect.push_back(vec3(shape.part(37).x() - shape.part(41).x(), shape.part(37).y() - shape.part(41).y(), 0.0).length());
            landmarkVect.push_back(vec3(shape.part(43).x() - shape.part(47).x(), shape.part(43).y() - shape.part(47).y(), 0.0).length());
            landmarkVect.push_back(vec3(shape.part(18).x(), shape.part(18).y(), 0.0).length());
            landmarkVect.push_back(vec3(shape.part(25).x(), shape.part(25).y(), 0.0).length());
            landmarkVect.push_back(vec3(shape.part(51).x() - shape.part(57).x(), shape.part(51).y() - shape.part(57).y(), 0.0).length());
            landmarkVect.push_back(vec3(shape.part(48).x() - shape.part(54).x(), shape.part(48).y() - shape.part(54).y(), 0.0).length());
            landmarkVect.push_back(vec3(shape.part(30).x() - shape.part(27).x(), shape.part(30).y() - shape.part(27).y(), 0.0).length());
            baseFrame= landmarkVect;
        }
        else
        {
            landmarkVect.at(0)=(vec3(shape.part(36).x(), shape.part(36).y(), 0.0).length());
            landmarkVect.at(1)=(vec3(shape.part(39).x(), shape.part(39).y(), 0.0).length());
            landmarkVect.at(2)=(vec3(shape.part(42).x(), shape.part(42).y(), 0.0).length());
            landmarkVect.at(3)=(vec3(shape.part(45).x(), shape.part(45).y(), 0.0).length());
            landmarkVect.at(4)=(vec3(shape.part(37).x() - shape.part(41).x(), shape.part(37).y() - shape.part(41).y(), 0.0).length());
            landmarkVect.at(5)=(vec3(shape.part(43).x() - shape.part(47).x(), shape.part(43).y() - shape.part(47).y(), 0.0).length());
            landmarkVect.at(6)=(vec3(shape.part(18).x(), shape.part(18).y(), 0.0).length());
            landmarkVect.at(7)=(vec3(shape.part(25).x(), shape.part(25).y(), 0.0).length());
            landmarkVect.at(8)=(vec3(shape.part(51).x() - shape.part(57).x(), shape.part(51).y() - shape.part(57).y(), 0.0).length());
            landmarkVect.at(9)=(vec3(shape.part(48).x() - shape.part(54).x(), shape.part(48).y() - shape.part(54).y(), 0.0).length());
            landmarkVect.at(10)=(vec3(shape.part(30).x() - shape.part(27).x(), shape.part(30).y() - shape.part(27).y(), 0.0).length());
        }
        //start_toggle = 0;
        delToTarget = landmarkVect;
        for(int i= 0; i<11 ; i++)
        {
            delToTarget.at(i) = delToTarget.at(i) - baseFrame.at(i);
            cout<<delToTarget.at(i)<<endl;
            //delToTarget.push_back(vec3((float)landmarkVect.at(i).x - (float)baseFrame.at(i).x, (float)landmarkVect.at(i).y - (float)baseFrame.at(i).y, 0.0));
        }
    }
}
void display()
{
    cout<<"display"<<endl;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    
    //===>...lighting
    // show the cube
    //showCube(&jello);
    
    // show the bounding box
    //showBoundingBox();
    glEnable(GL_TEXTURE_2D);
    // These are necessary if using glTexImage2D instead of gluBuild2DMipmaps
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    
    gluOrtho2D(0, frameWidth*3, frameHight*3, 0);
    
    // Switch to Model View Matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //frame->showFrame();
    double w = glutGet( GLUT_WINDOW_WIDTH );
    double h = glutGet( GLUT_WINDOW_HEIGHT );
    double ar = w / h;
    // Draw a textured quad
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(frameWidth, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(frameWidth, frameHight);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, frameHight);
    glEnd();
    glFlush();
    glDisable(GL_TEXTURE_2D);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, aGa);
    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
    
    // set up cube color
    glMaterialfv(GL_FRONT, GL_AMBIENT, mKa);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mKd);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mKs);
    glMaterialfv(GL_FRONT, GL_EMISSION, mKe);
    glMaterialf(GL_FRONT, GL_SHININESS, 120);
    
    // macro to set up light i
#define LIGHTSETUP(i)\
glLightfv(GL_LIGHT##i, GL_POSITION, lP##i);\
glLightfv(GL_LIGHT##i, GL_AMBIENT, lKa##i);\
glLightfv(GL_LIGHT##i, GL_DIFFUSE, lKd##i);\
glLightfv(GL_LIGHT##i, GL_SPECULAR, lKs##i);\
glEnable(GL_LIGHT##i)
    
    LIGHTSETUP (0);
    LIGHTSETUP (1);
    LIGHTSETUP (2);
    LIGHTSETUP (3);
    LIGHTSETUP (4);
    LIGHTSETUP (5);
    LIGHTSETUP (6);
    LIGHTSETUP (7);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    drawMesh(curTrans, curRot);
    
    for(int index=1;index<6;index++)
    {
        drawMesh2(curTrans, curRot, index);
    }

    glutSwapBuffers();
}
IplImage *rotate_image(IplImage *image, float angle)
{
    IplImage *rotated;
    
    //if(_90_degrees_steps_anti_clockwise != 2)
    rotated = cvCreateImage(cvSize(image->height, image->width), image->depth, image->nChannels);
    cvTranspose(image, rotated);
    //cvFlip(image, image, )
    //return img;
    
    return rotated;
}
vec3 project(vec3 a, vec3 b)
{
    //projection a to b
    float costheta = a.x * b.x + a.y * b.y + a.z * b.z;
    return (costheta*b);
}

void doIdle()
{
    // start timer
    double start_seconds = current_time_in_seconds();
    IplImage * image = cvQueryFrame(g_Capture);
    getLandmarkPoints(image);
    // demonstrate filtering the video before making the texture
    if(mirror)
    {
        cvFlip(image, NULL, 1);
    }
    char * buffer = new char[image->width*image->height*image->nChannels];
    int step     = image->widthStep;
    int height   = image->height;
    int width    = image->width;
    int channels = image->nChannels;
    char * data  = (char *)image->imageData;
    
    vector<float> currentStatus;
    currentStatus.push_back(shape.part(37).y() - shape.part(41).y()); //left eye
    currentStatus.push_back(shape.part(43).y() - shape.part(47).y()); //right eye
    currentStatus.push_back(shape.part(48).x()); //left mouth
    currentStatus.push_back(shape.part(48).y());
    currentStatus.push_back(shape.part(54).x()); //right mouth
    currentStatus.push_back(shape.part(54).x() - shape.part(48).x()); //mouth (x)
    currentStatus.push_back(shape.part(51).y() - shape.part(57).y()); //mouth
    if(start_toggle==1)
    {
        baseStatus = currentStatus;
    }
    start_toggle=0;
    vector<float> deltaStatus;
    for(int i=0;i<7;i++)
    {
        deltaStatus.push_back(currentStatus[i] - baseStatus[i]);
    }
    // eyes...
    if(deltaStatus[0]<0)
    {
        if(morph_weight[7]<6)
        {
            morph_weight[7] += 2;
            morph_weight[8] += 2;
        }
        
    }
    else if(deltaStatus[0]>0)
    {
        if(morph_weight[7]>0)
        {
            morph_weight[7]-= 1.0;
            morph_weight[8] -= 2;
        }
    }
    // mouth...
    if(deltaStatus[6]<0)
    {
        if(morph_weight[3]<3)
        {
            morph_weight[3] += 1/4.0;
        }
    }else if(deltaStatus[6]>0){
        if(morph_weight[3]>0){
            morph_weight[3]-=1/4.0;
        }
    }
    // mouth...
    if(deltaStatus[5]<0)
    {
        if(morph_weight[3]<2){
            morph_weight[3]=1/4.0;
            morph_weight[2] += 1/4.0;
        
    }else if(deltaStatus[5]>0){
        if(morph_weight[2]>0)
        {
            
            morph_weight[2] -= 1/4.0;
        }
        }
    }
    
    // mouth...
    if(deltaStatus[2]<0 && deltaStatus[3]<0)
    {
        if(morph_weight[1]<2)
        {
            morph_weight[1] += 1/4.0;
        }
        if(morph_weight[4]>0)
        {
            morph_weight[1] -= 1/4.0;
        }
    }else if(deltaStatus[2]>=0 && deltaStatus[3]>=0){
        if(morph_weight[1]>0)
        {
            morph_weight[1] -= 1/4.0;
        }
        if(morph_weight[4]<1)
        {
            morph_weight[4] += 1/16.0;
        }
    }
    
    ////
    for(int i=0;i<height;i++)
    {
        memcpy(&buffer[i*width*channels],&(data[i*step]),width*channels);
    }
    //cout<<data<<endl;
    // Create Texture
    glTexImage2D(
                 GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 image->width,
                 image->height,
                 0,
                 GL_BGR,
                 GL_UNSIGNED_BYTE,
                 buffer);
    

    vec3 d = avgDeltaShape[0];//target_delta[0].at(i);
    //d = glm::normalize(d);
    vec3 projResult;

    finalModel = model[0];
    float sum1=0.0;
    for(int i=0;i<9;i++)
    {
        if(morph_weight[i]<0)
            morph_weight[i] = 0;
    }
    for(int i=0;i<6;i++)
    {
       sum1 += morph_weight[i];
    }
    for(int i=0;i<6;i++)
    {
        morph_weight[i] /= sum1;
    }
    
    float sum2=0.0;
    for(int i=6;i<9;i++)
    {
        sum2 += morph_weight[i];
    }
    for(int i=6;i<9;i++)
    {
        morph_weight[i] /= sum2;
    }
    
    for(int j=0;j<9;j++)
    {
        for(int i=0;i<target[j].size();i++)
        {

            finalModel.at(i).position.x = finalModel.at(i).position.x + (target[j].at(i).position.x-model[0].at(i).position.x)*morph_weight[j];
            finalModel.at(i).position.y = finalModel.at(i).position.y + (target[j].at(i).position.y-model[0].at(i).position.y)*morph_weight[j];
            finalModel.at(i).position.z = finalModel.at(i).position.z + (target[j].at(i).position.z-model[0].at(i).position.z)*morph_weight[j];
            
        }
    }
    
    char s[20]="picxxxx.ppm";
    // save screen to file
    s[3] = 48 + (sprite / 1000);
    s[4] = 48 + (sprite % 1000) / 100;
    s[5] = 48 + (sprite % 100 ) / 10;
    s[6] = 48 + sprite % 10;
    
    //if ((int)(10000*counter.GetElapsedTime())%2 ==0)
    if (ccount % 2 == 0)
    {
        saveScreenshot(640, 480, s);
        sprite++;
    }
    if (sprite >= 300) // allow only 300 snapshots
    {
        exit(0);
    }
    
    
    ccount++;

    glutPostRedisplay();
    
}
vec3 Slerp(vec3 p0, vec3 p1, float t)
{
    float dotp = dot(normalize(p0), normalize(p1));
    if ((dotp > 0.9999) || (dotp<-0.9999))
    {
        if (t<=0.5)
            return p0;
        return p1;
    }
    float theta = acos(dotp * 3.14159/180.0);
    vec3 P = ((p0*sin((1-t)*theta) + p1*sin(t*theta)) / sin(theta));
    return P;
}

int main (int argc, char ** argv)
{
    
    char* chr = strdup(shapefile.c_str());
    dlib::deserialize(shapefile) >> sp;
    //cout<<&sp<<endl;
    g_Capture = cvCaptureFromCAM(0);
    assert(g_Capture);
    frameWidth = (int)cvGetCaptureProperty(g_Capture,CV_CAP_PROP_FRAME_WIDTH);
    frameHight = (int)cvGetCaptureProperty(g_Capture, CV_CAP_PROP_FRAME_HEIGHT);
    Face *morphTarget[40];
    /* Load all base models */
    ifstream ifile("base_head/original_head.obj" );
    if(&ifile==NULL) cout<<"error!"<<endl;
    morphTarget[0] = new Face(ifile);
    model[0] = morphTarget[0]->verts;
    
    ifstream ifile1("base_head/lt_eye.obj" );
    if(&ifile1==NULL) cout<<"error!"<<endl;
    morphTarget[1] = new Face(ifile1);
    model[1] = morphTarget[1]->verts;
    
    ifstream ifile2("base_head/rt_eye.obj" );
    if(&ifile2==NULL) cout<<"error!"<<endl;
    morphTarget[2] = new Face(ifile2);
    model[2] = morphTarget[2]->verts;
    
    ifstream ifile3("base_head/original_lower_teeth.obj" );
    if(&ifile3==NULL) cout<<"error!"<<endl;
    morphTarget[3] = new Face(ifile3);
    model[3] = morphTarget[3]->verts;
    
    ifstream ifile4("base_head/original_tongue.obj" );
    if(&ifile4==NULL) cout<<"error!"<<endl;
    morphTarget[4] = new Face(ifile4);
    model[4] = morphTarget[4]->verts;
    
    ifstream ifile5("base_head/shirt.obj" );
    if(&ifile5==NULL) cout<<"error!"<<endl;
    morphTarget[5] = new Face(ifile5);
    model[5] = morphTarget[5]->verts;
    
    ifstream ifile6("base_head/upper_teeth.obj" );
    if(&ifile6==NULL) cout<<"error!"<<endl;
    morphTarget[6] = new Face(ifile6);
    model[6] = morphTarget[6]->verts;
    /* */
    
    /* manipulating mouth */
    ifstream ifile7("morphs/O.obj" );
    if(&ifile7==NULL) cout<<"error!"<<endl;
    morphTarget[7] = new Face(ifile7);
    target[0] = morphTarget[7]->verts;
    target_delta[0] = morphTarget[7]->getDelta(model[0]);
    morph_weight[0] = 1/7.0;
    
    ifstream ifile8("morphs/smile_mouth.obj" );
    if(&ifile8==NULL) cout<<"error!"<<endl;
    morphTarget[8] = new Face(ifile8);
    target[1] = morphTarget[8]->verts;
    target_delta[1] = morphTarget[8]->getDelta(model[0]);
    avgDeltaShape[1] = morphTarget[8]->avgDeltaShape;
    morph_weight[1] = 1/7.0;
    
    ifstream ifile9("morphs/Oo.obj" );
    if(&ifile9==NULL) cout<<"error!"<<endl;
    morphTarget[9] = new Face(ifile9);
    target[2] = morphTarget[9]->verts;
    target_delta[2] = morphTarget[9]->getDelta(model[0]);
    avgDeltaShape[2] = morphTarget[9]->avgDeltaShape;
    morph_weight[2] = 1/7.0;
    
    ifstream ifile10("morphs/E.obj" );
    if(&ifile10==NULL) cout<<"error!"<<endl;
    morphTarget[10] = new Face(ifile10);
    target[3] = morphTarget[10]->verts;
    target_delta[3] = morphTarget[10]->getDelta(model[0]);
    avgDeltaShape[3] = morphTarget[10]->avgDeltaShape;
    morph_weight[3] = 1/7.0;
    
    ifstream ifile11("morphs/F_V.obj" );
    if(&ifile11==NULL) cout<<"error!"<<endl;
    morphTarget[11] = new Face(ifile11);
    target[4] = morphTarget[11]->verts;
    target_delta[4] = morphTarget[11]->getDelta(model[0]);
    avgDeltaShape[4] = morphTarget[11]->avgDeltaShape;
    morph_weight[4] = 1/7.0;
    
    ifstream ifile12("morphs/Mm.obj" );
    if(&ifile12==NULL) cout<<"error!"<<endl;
    morphTarget[12] = new Face(ifile12);
    target[5] = morphTarget[12]->verts;
    target_delta[5] = morphTarget[12]->getDelta(model[0]);
    avgDeltaShape[5] = morphTarget[12]->avgDeltaShape;
    morph_weight[5] = 1/7.0;
    
    /* manipulating eyes */
    ifstream ifile13("morphs/a_smile_eyes.obj" );
    if(&ifile13==NULL) cout<<"error!"<<endl;
    morphTarget[13] = new Face(ifile13);
    target[6] = morphTarget[13]->verts;
    target_delta[6] = morphTarget[13]->getDelta(model[0]);
    avgDeltaShape[6] = morphTarget[13]->avgDeltaShape;
    morph_weight[6] = 1/4.0;
    
    ifstream ifile14("morphs/a_right_blink.obj" );
    if(&ifile14==NULL) cout<<"error!"<<endl;
    morphTarget[14] = new Face(ifile14);
    target[7] = morphTarget[14]->verts;
    target_delta[7] = morphTarget[14]->getDelta(model[0]);
    avgDeltaShape[7] = morphTarget[14]->avgDeltaShape;
    morph_weight[7] = 1/4.0;
    
    ifstream ifile15("morphs/a_left_blink.obj" );
    if(&ifile15==NULL) cout<<"error!"<<endl;
    morphTarget[15] = new Face(ifile15);
    target[8] = morphTarget[15]->verts;
    target_delta[8] = morphTarget[15]->getDelta(model[0]);
    avgDeltaShape[8] = morphTarget[15]->avgDeltaShape;
    morph_weight[8] = 1/4.0;
    
    /////////////////////////////
    
    
    finalModel = model[0];
    
    glutInit(&argc,argv);
    glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );
    windowWidth = 640;
    windowHeight = 480;
    glutInitWindowSize (windowWidth, windowHeight);
    glutInitWindowPosition (0,0);
    glutCreateWindow ("Final project");
    /* tells glut to use a particular display function to redraw */
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    /* replace with any animate code */
    glutIdleFunc(doIdle);
    
    glutMainLoop();
    return(0);
}
