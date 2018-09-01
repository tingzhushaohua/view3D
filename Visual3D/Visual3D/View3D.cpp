#define _CRT_SECURE_NO_WARNINGS
#include "View3D.h"
#include <vector>

GLfloat g_rotx = 0.0, g_roty = 0.0, g_rotz = 0.0, g_angle=0.0;
GLfloat g_modelPos[3] = {0,0,-240};
GLfloat g_scale = 0.3;//初始化缩放参数
GLfloat maxz=20.0;
//char g_strFileName[] = "E:\\vs2010Projects\\TestUSB980Camera\\TestStereoCamera\\盆栽1.txt";
//char g_strFileName[] = "testfile.txt";
int g_nPointsNumber;//Number of all triangle faces

typedef struct VERTEX_3D
{
	double x;
	double y;
	double z;
} VERTEX3D;

typedef struct  GROUND_INFO
{
	double c;
	double g;
}GROUNDINFO;

typedef struct _TRIANGLE
{
	VERTEX3D A;//Index of vertex 0
	VERTEX3D B;//Index of vertex 1
	VERTEX3D C;//Index of vertex 2
	VERTEX3D N; //Normal of triangle
} TRIANGLE;
VERTEX_3D *g_ver;//Pointer to store vertices

View3D::View3D(void)
{
}
View3D::~View3D(void)
{
}
const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

void GetMinMax(VERTEX3D &vMin, VERTEX3D &vMax, VERTEX3D v)
{
	if(v.x < vMin.x ) vMin.x = v.x;
	if(v.y < vMin.y ) vMin.y = v.y;
	if(v.z < vMin.z ) vMin.z = v.z;
	if(v.x > vMax.x ) vMax.x = v.x;
	if(v.y > vMax.y ) vMax.y = v.y;
	if(v.z > vMax.z ) vMax.z = v.z;
	//maxz=vMax.z;
	//printf("%d",maxz);
}

void FormatGeometry(VERTEX3D &v, VERTEX3D vMin, VERTEX3D vMax, double scale)
{
	v.x = scale*(v.x - 0.5*(vMin.x + vMax.x));
	v.y = scale*(v.y - 0.5*(vMin.y + vMax.y));
	v.z = scale*(v.z - 0.5*(vMin.z + vMax.z));
}

//显示点云
void DrawPointCloud()
{
	int i;    
	glDisable(GL_LIGHTING);
	glPointSize(1);
	glBegin(GL_POINTS);
	for (i=0; i<g_nPointsNumber; i++){
		//glColor3d(1-(float)g_ver[i].z/maxz,1-(float)fabs((g_ver[i].z-5)/5),1-(float)(maxz-g_ver[i].z)/maxz);
		//glColor3d(1-(float)g_ver[i].z*3/maxz,(float)g_ver[i].z/100,(float)g_ver[i].z/100);
		glColor3d(1,1,1);
		glVertex3d(g_ver[i].x, g_ver[i].y, g_ver[i].z);
	}
	glEnd();
	glPointSize(1);
}

static void resize(int width, int height)
{
	const float ar = (float) width / (float) height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(10, ar, 1.0, 100000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity() ;
}

static void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity() ;
	glTranslatef(g_modelPos[0], g_modelPos[1], g_modelPos[2]);
	glRotatef(g_rotx, 1.0, 0.0, 0.0);
	glRotatef(g_roty, 0.0, 1.0, 0.0);
	glRotatef(g_rotz, 0.0, 0.0, 1.0);
	DrawPointCloud();

	glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27 :
		exit(0);
		break;
	case 'w'://GLUT_KEY_LEFT
		g_rotx += 5;
		break;
	case 's':
		g_rotx -= 5;
		break;
	case 'a':
		g_roty += 5;
		break;
	case 'd':
		g_roty -= 5;
		break;
	case 'q':
		g_rotz += 5;
		break;
	case 'e':
		g_rotz -= 5;
		break;
	case 'z':
		g_modelPos[2] += 1;
		break;
	case 'x':
		g_modelPos[2] -= 1;
		break;
	}

	glutPostRedisplay();
}

static void idle(void)
{
	glutPostRedisplay();
}

void LoadPointCloudPLY(char *filename)
{
	int i;
	double x, y, z ;//c, g;
	VERTEX3D vMin, vMax;

	FILE* fp=fopen(filename,"r");
	if (fp==NULL)
	{
		printf("ERROR: unable to open point cloud [%s]!\n",filename);
		exit(0);
	}
	
	// Read the number of points
	g_nPointsNumber = 0;
	while (fscanf(fp, "%lf", &x) && !feof(fp)) { g_nPointsNumber++; }
	g_nPointsNumber = g_nPointsNumber/3;
	printf("Number of points: %d\n", g_nPointsNumber);
	fseek(fp, 0L, SEEK_SET);
	
	//Allocate the points array
	g_ver = new VERTEX3D[g_nPointsNumber];
	
	//Read in the points
	for (i=0; i<g_nPointsNumber; i++)
	{
		fscanf(fp, "%lf %lf %lf", &x, &y, &z);
		g_ver[i].x = x;        
		g_ver[i].y = y;       
		g_ver[i].z = z;
		if(i==0)
		{
			vMin.x = vMax.x = g_ver[i].x;
			vMin.y = vMax.y = g_ver[i].y;
			vMin.z = vMax.z = g_ver[i].z;
		}
		else
			GetMinMax(vMin, vMax, g_ver[i]);
	}

	for (i=0; i<g_nPointsNumber; i++)
	{
		FormatGeometry(g_ver[i], vMin, vMax, g_scale);
	}

	fclose(fp);
}

//显示3d视图
void View3D::view3dview(char *g_strFileName){
	glutInitWindowSize(640,480);
	glutInitWindowPosition(10,10);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow("Point cloud display");

	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);

	glClearColor(0,0,0,1);
	glDisable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);

	glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glShadeModel(GL_SMOOTH);

	LoadPointCloudPLY(g_strFileName);
	glutMainLoop();
}


//读入显示的点云数据
//void View3D::LoadPointCloudPLY(vector<CvPoint3D32f> points,float fa,float ne,float scale)
//{
//	double x, y, z, c, g;
//	VERTEX3D vMin, vMax;	
//	g_nPointsNumber = points.size();
//	g_ver = new VERTEX3D[g_nPointsNumber];
//
//	for (int i=0; i<g_nPointsNumber; i++)
//	{
//		CvPoint3D32f point=points[i];
//		if((point.z<fa)&&(point.z>ne)){		
//			g_ver[i].x = point.x/scale;        
//			g_ver[i].y = point.y/scale;       
//			g_ver[i].z = point.z/scale;
//
//			if(i==0)
//			{
//				vMin.x = vMax.x = g_ver[i].x;
//				vMin.y = vMax.y = g_ver[i].y;
//				vMin.z = vMax.z = g_ver[i].z;
//			}
//			else
//				GetMinMax(vMin, vMax, g_ver[i]);
//
//		}else{
//			return;
//		}	
//	}
//
//	for (int i=0; i<g_nPointsNumber; i++)
//	{
//		FormatGeometry(g_ver[i], vMin, vMax, g_scale);
//	}
//
//}
//
void View3D::init3dview()
{
	glutInitWindowSize(640,480);
	glutInitWindowPosition(10,10);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow("Point cloud display");

	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);

	glClearColor(0,0,0,1);
	glDisable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);

	glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glShadeModel(GL_SMOOTH);
}