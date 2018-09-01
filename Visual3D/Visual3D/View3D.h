#pragma once

//#include "stdafx.h"
//#include <windows.h>


#define GLUT_DISABLE_ATEXIT_HACK
#include "glut.h"


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
#include <vector>

//using namespace cv;
using namespace std;

#if 0 //glut32
#pragma comment(lib,"freeglut.lib")
#pragma comment(lib,"glut.lib")
#endif // 0 //glut32

#pragma comment(lib,"glut32.lib")

class View3D
{
public:
	View3D(void);
	~View3D(void);
	void init3dview();

	//void LoadPointCloudPLY(vector<CvPoint3D32f> points, float fa, float ne, float scale);
	void view3dview(char *g_strFileName);

};

