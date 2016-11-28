

#ifndef _WEBCAM_H_
#define _WEBCAM_H_
#include <vector>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
//#include <glm/glm.hpp>
#include "/usr/local/include/glm/glm.hpp"
#include "jello.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
using namespace cv;
//#include "/usr/local/include/opencv2/opencv.hpp"
//#include "/usr/local/include/opencv2/core/core.hpp"
//#include "/usr/local/include/opencv2/highgui/highgui.hpp"
class Webcam
{
public:
    //double current_time_in_seconds();
    //GLvoid idle();
    Webcam(int w, int h);
    void showFrame();
    double current_time_in_seconds();
    ~Webcam();
private:
    //cv::VideoCapture capture;
    int w;
    int h;
    CvCapture* g_Capture;
    double start_seconds;
    IplImage *image;
};
#endif