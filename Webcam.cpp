
#include "Webcam.h"
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <opencv2/video/tracking.hpp>
//#include "/usr/local/include/opencv2/opencv.hpp"


//#include "opencv/cv.h"
//#include "opencv/highgui.h"
using namespace std;
using namespace cv;
Webcam::Webcam(int w, int h)
{
    cout<<"Webcam constructor created"<<endl;
    this->w = w;
    this->h = h;
    ////capture = cvCaptureFromCAM( -1 );
    this->start_seconds = current_time_in_seconds();
    this->g_Capture = cvCaptureFromCAM(0);
    if(this->g_Capture==NULL){
        std::cerr<<"!!! error: vCaptureFromCam\n";
    }
    //IplImage *src = NULL;
    assert(this->g_Capture);
   // glutIdleFunc(idle);
    //VideoCapture cap;
}
double Webcam::current_time_in_seconds()
{
    timeval timer;
    gettimeofday(&timer, NULL);
    double seconds = 1e-6 * timer.tv_usec + timer.tv_sec;
    return seconds;
}

void Webcam::showFrame()
{
    
    this->g_Capture = cvCaptureFromCAM(0);
    if(this->g_Capture==NULL){
        std::cerr<<"!!! error: vCaptureFromCam\n";
    }
    //IplImage *src = NULL;
    assert(this->g_Capture);
    image = cvQueryFrame(this->g_Capture);
    char * buffer = new char[image->width*image->height*image->nChannels];
    int step     = image->widthStep;
    int height   = image->height;
    int width    = image->width;
    int channels = image->nChannels;
    char * data  = (char *)image->imageData;
    for(int i=0;i<height;i++)
    {
        memcpy(&buffer[i*width*channels],&(data[i*step]),width*channels);
    }
    
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
    
    
    // Clean up buffer
    delete[] buffer;
    
    // Update display
    glutPostRedisplay();
    
    double stop_seconds = current_time_in_seconds();
    //fps = 0.9*fps + 0.1*1.0/(stop_seconds-start_seconds);

    double w = this->w;//glutGet( GLUT_WINDOW_WIDTH );
    double h = this->h;//glutGet( GLUT_WINDOW_HEIGHT );
    double ar = w / h;
    // Draw a textured quad
    glBegin(GL_QUADS);
    
    glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(w, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(w, h);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, h);
    glEnd();
    ///
    
}
