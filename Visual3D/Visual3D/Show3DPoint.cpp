#include "View3D.h"

char g_strFileName[] = "testfile.txt";

int main(int argc, char *argv[])
{
	View3D showPoint;

	showPoint.init3dview();
	showPoint.view3dview(g_strFileName);
	return 0;
}