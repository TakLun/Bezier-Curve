// cssample5View.cpp : implementation of the CCssample5View class
//

#include "stdafx.h"
#include "cssample5.h"

#include "cssample5Doc.h"
#include "cssample5View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCssample5View

IMPLEMENT_DYNCREATE(CCssample5View, CView)

BEGIN_MESSAGE_MAP(CCssample5View, CView)
	//{{AFX_MSG_MAP(CCssample5View)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_COMMAND(ID_AS4_SELECT_1, OnSelectControlPoint1)
	ON_COMMAND(ID_AS4_SELECT_2, OnSelectControlPoint2)
	ON_COMMAND(ID_AS4_SELECT_3, OnSelectControlPoint3)
	ON_COMMAND(ID_AS4_SELECT_4, OnSelectControlPoint4)
	ON_COMMAND(ID_AS4_X_DEC, OnXDec)
	ON_COMMAND(ID_AS4_X_INC, OnXInc)
	ON_COMMAND(ID_AS4_Y_DEC, OnYDec)
	ON_COMMAND(ID_AS4_Y_INC, OnYInc)
	ON_COMMAND(ID_AS4_Z_DEC, OnZDec)
	ON_COMMAND(ID_AS4_Z_INC, OnZInc)
	ON_COMMAND(ID_VIEW_FILLED, OnViewFilled)
	ON_COMMAND(ID_VIEW_WIREFRAME, OnViewWireframe)
	ON_COMMAND(ID_VIEW_FLAT, OnViewFlat)
	ON_COMMAND(ID_VIEW_SMOOTH, OnViewSmooth)
	ON_COMMAND(ID_VIEW_LIGHT_YDEC, OnViewLightYdec)
	ON_COMMAND(ID_VIEW_LIGHT_YINC, OnViewLightYinc)
	ON_COMMAND(ID_VIEW_TEXTURE_DISABLE, OnViewTextureDisable)
	ON_COMMAND(ID_VIEW_TEXTURE_ENABLE, OnViewTextureEnable)
	ON_COMMAND(ID_VIEW_POS_DECZ, OnViewPosDecz)
	ON_COMMAND(ID_VIEW_POS_INCZ, OnViewPosIncz)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND(ID_SPECULAR, &CCssample5View::OnSpecular)
	ON_COMMAND(ID_SPECULAR32805, &CCssample5View::OnSpecular32805)
	ON_COMMAND(ID_DIFFUSE, &CCssample5View::OnDiffuse)
	ON_COMMAND(ID_DIFFUSE32810, &CCssample5View::OnDiffuse32810)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCssample5View construction/destruction

CCssample5View::CCssample5View()
{
	m_hDC=NULL;
	m_hRC=NULL;
	w=h=0; //width and height of view area
	control_point=0;
	x_step=0;
	y_step=0;
	z_step=0;

	point_store_x = -1;
	point_store_y = -1;

	flat = 0;
	smooth = 0;

	move_amount = 5.0;

	RedSurface[0]= 1.0f; RedSurface[1]=0.0f; RedSurface[2]=0.0f; RedSurface[3]=1.0f;
	GreenSurface[0]= 0.0f; GreenSurface[1]=1.0f; GreenSurface[2]=0.0f; GreenSurface[3]=1.0f;
	BlueSurface[0]= 0.0f; BlueSurface[1]=0.0f; BlueSurface[2]=1.0f; BlueSurface[3]=1.0f;
	BlackSurface[0]= 0.0f; BlackSurface[1]=0.0f; BlackSurface[2]=0.0f; BlackSurface[3]=1.0f;

	u.x=1.0; u.y=0.0; u.z=0.0;
	v.x=0.0; v.y=1.0; v.z=0.0;
	n.x=0.0; n.y=0.0; n.z=-1.0;
	camera.x = 0.0; camera.y=0.0; camera.z=40.0;
    for(int i=0; i< 4; i++)
    for(int j=0; j< 4; j++) {
		   p[i][j].x = 20.f*j;
		   p[i][j].z = 20.f*i;
		   p[i][j].y = 0.f;
	}

	LightAmbient[0] = 0.0;
	LightAmbient[1] = 0.0;
	LightAmbient[2] = 0.0;
	LightAmbient[3] = 0.0;

	LightDiffuse[0] = 0.7;
	LightDiffuse[1] = 0.7;
	LightDiffuse[2] = 0.7;
	LightDiffuse[3] = 1.0;

	LightSpecular[0] = 1.0;
	LightSpecular[1] = 1.0;
	LightSpecular[2] = 1.0;
	LightSpecular[3] = 1.0;

	ambient_reflection[0] = 0.0;
	ambient_reflection[1] = 1.0;
	ambient_reflection[2] = 0.0;
	ambient_reflection[3] = 1.0;

	diffuse_reflection[0] = 0.0; 
	diffuse_reflection[1] = 1.0; 
	diffuse_reflection[2] = 0.0; 
	diffuse_reflection[3] = 1.0; 

	specular_reflection[0] = 1.0;
	specular_reflection[1] = 1.0;
	specular_reflection[2] = 1.0;
	specular_reflection[3] = 1.0;

	light_position[0] = -50.0;
	light_position[1] = -50.0;
	light_position[2] = -50.0;
	light_position[3] = 0.0;

	shiny[0] = 25.0;

	LightPos.x=50;	LightPos.y=50; 	LightPos.z=50;
	Camera.x=45; Camera.y=45; Camera.z=45;
	WireFrame=1;
	rendering_mode=true; //smooth by default, otherwise flat
	//CalcBezier();
}

CCssample5View::~CCssample5View()
{
}

BOOL CCssample5View::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCssample5View drawing

void CCssample5View::OnDraw(CDC* pDC)
{
	CCssample5Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CRect rcClient;
	GetClientRect(rcClient);
	
	w=rcClient.Width();
	h=rcClient.Height();

	wglMakeCurrent(m_hDC,m_hRC);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);		
	
	glColor4f(1.0, 1.0, 1.0, 0.0);

	glViewport(0, 0, (int)(w/1.0), (int)(h/1.0));

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)w/(float)h, 1, 256);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Camera.x, Camera.y, Camera.z, 0, 0, 0, 0, 1, 0);

	point_store_x = -1;
	point_store_y = -1;
		
	glPushMatrix();

		glTranslatef(-30, 0, -30);//move the axis for better view area
		glDisable(GL_LIGHTING);
		DrawXYZaxis();
		//bezier_plane();
		for (float u = 0.00; u <=1.05; u = u + 0.05){
			point_store_x +=1;
			point_store_y = -1;
			for (float v = 0.00; v <=1.05; v = v + 0.05){
				point_store_y +=1;
				setAllNormals(u, v, point_store_x, point_store_y);
			}
		}

		if (WireFrame == 0){
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, WhiteSurface);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
		} else {
		glDisable(GL_LIGHTING);
		}

		//if (WireFrame == 1){
		glColor4f(0.0, 1.0, 0.0, 0.0);
		for (int x = 0; x < 20; ++x){
			for (int y = 0; y < 20; ++y){
				Vector a = m[x][y];
				Vector b = m[x][y+1];
				Vector c = m[x+1][y+1];

				//float normal_x = ((b.y - a.y) * (c.z - a.z)) - ((c.y - a.y) * (b.z - a.z));
				//float normal_y = ((b.z - a.z) * (c.x - a.x)) - ((c.z - a.z) * (b.x - a.x));
				//float normal_z = ((b.x - a.x) * (c.y - a.y)) - ((c.x - a.x) * (b.y - a.y));

				float matrix1_x = b.x - a.x;
				float matrix1_y = b.y - a.y;
				float matrix1_z = b.z - a.z;

				float matrix2_x = c.x - a.x;
				float matrix2_y = c.y - a.y;
				float matrix2_z = c.z - a.z;
					
				//float normal_x = ((a.y * b.z) - (a.z * b.y));
				//float normal_y = ((a.z * b.x) - (a.x * b.z));
				//float normal_z = ((a.x * b.y) - (a.y * b.x));

				float normal_x = (matrix1_y * matrix2_z) - (matrix1_z * matrix2_y);
				float normal_y = (matrix1_z * matrix2_x) - (matrix1_x * matrix2_z);
				float normal_z = (matrix1_x * matrix2_y) - (matrix1_y * matrix2_x);

				int length = sqrt((normal_x * normal_x) + (normal_y * normal_y) + (normal_z * normal_z));

				normal_x = normal_x/length;
				normal_y = normal_y/length;
				normal_z = normal_z/length;

				float dot_product1 = (-1*Camera.x) * normal_x;
				float dot_product2 = (-1*Camera.y) * normal_y;
				float dot_product3 = (-1*Camera.z) * normal_z;

				float dot_product = dot_product1 + dot_product2 + dot_product3;

				if (WireFrame == 1){
					if (dot_product <= 0){
						glBegin(GL_LINES);
						glVertex3d(m[x][y].x, m[x][y].y, m[x][y].z);
						glVertex3d(m[x][y+1].x, m[x][y+1].y, m[x][y+1].z);

						glVertex3d(m[x][y+1].x, m[x][y+1].y, m[x][y+1].z);
						glVertex3d(m[x+1][y+1].x, m[x+1][y+1].y, m[x+1][y+1].z);

						glVertex3d(m[x+1][y+1].x, m[x+1][y+1].y, m[x+1][y+1].z);
						glVertex3d(m[x][y].x, m[x][y].y, m[x][y].z);
						glEnd();
					}
				} else if (WireFrame == 0){
					if (flat == 1){
						glShadeModel(GL_FLAT);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						glNormal3f((-1*normal_x), (-1*normal_y), (-1*normal_z));
						glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, GreenSurface);
						glBegin(GL_TRIANGLES);
						glVertex3d(m[x][y].x, m[x][y].y, m[x][y].z);
						//glVertex3d(m[x][y+1].x, m[x][y+1].y, m[x][y+1].z);

						glVertex3d(m[x][y+1].x, m[x][y+1].y, m[x][y+1].z);
						//glVertex3d(m[x+1][y+1].x, m[x+1][y+1].y, m[x+1][y+1].z);

						glVertex3d(m[x+1][y+1].x, m[x+1][y+1].y, m[x+1][y+1].z);
						//glVertex3d(m[x][y].x, m[x][y].y, m[x][y].z);
						glEnd();
					} else if (smooth == 1){
						glShadeModel(GL_SMOOTH);
					}
				}

				//float dot_product = 
				/*if (dot_product <= 0){
					if (WireFrame == 0){
						if (flat == 1){
							glShadeModel(GL_FLAT);
						} else if (smooth == 1){
							glShadeModel(GL_SMOOTH);
						}
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glNormal3f((-1*normal_x), (-1*normal_y), (-1*normal_z));
					glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, GreenSurface);
					glBegin(GL_TRIANGLES);
						glVertex3d(m[x][y].x, m[x][y].y, m[x][y].z);
						//glVertex3d(m[x][y+1].x, m[x][y+1].y, m[x][y+1].z);

						glVertex3d(m[x][y+1].x, m[x][y+1].y, m[x][y+1].z);
						//glVertex3d(m[x+1][y+1].x, m[x+1][y+1].y, m[x+1][y+1].z);

						glVertex3d(m[x+1][y+1].x, m[x+1][y+1].y, m[x+1][y+1].z);
						//glVertex3d(m[x][y].x, m[x][y].y, m[x][y].z);
					glEnd();
					} else {
						glBegin(GL_LINES);
						glVertex3d(m[x][y].x, m[x][y].y, m[x][y].z);
						glVertex3d(m[x][y+1].x, m[x][y+1].y, m[x][y+1].z);

						glVertex3d(m[x][y+1].x, m[x][y+1].y, m[x][y+1].z);
						glVertex3d(m[x+1][y+1].x, m[x+1][y+1].y, m[x+1][y+1].z);

						glVertex3d(m[x+1][y+1].x, m[x+1][y+1].y, m[x+1][y+1].z);
						glVertex3d(m[x][y].x, m[x][y].y, m[x][y].z);
					glEnd();
					}
				}*/

				c = m[x][y];
				b = m[x+1][y];
				a = m[x+1][y+1];

				//normal_x = ((b.y - a.y) * (c.z - a.z)) - ((c.y - a.y) * (b.z - a.z));
				//normal_y = ((b.z - a.z) * (c.x - a.x)) - ((c.z - a.z) * (b.x - a.x));
				//normal_z = ((b.x - a.x) * (c.y - a.y)) - ((c.x - a.x) * (b.y - a.y));

				matrix1_x = b.x - a.x;
				matrix1_y = b.y - a.y;
				matrix1_z = b.z - a.z;

				matrix2_x = c.x - a.x;
				matrix2_y = c.y - a.y;
				matrix2_z = c.z - a.z;

				//normal_x = ((a.y * b.z) - (a.z * b.y));
				//normal_y = ((a.z * b.x) - (a.x * b.z));
				//normal_z = ((a.x * b.y) - (a.y * b.x));

				normal_x = (matrix1_y * matrix2_z) - (matrix1_z * matrix2_y);
				normal_y = (matrix1_z * matrix2_x) - (matrix1_x * matrix2_z);
				normal_z = (matrix1_x * matrix2_y) - (matrix1_y * matrix2_x);

				length = sqrt((normal_x * normal_x) + (normal_y * normal_y) + (normal_z * normal_z));

				normal_x = normal_x/length;
				normal_y = normal_y/length;
				normal_z = normal_z/length;

				dot_product1 = (-1*Camera.x) * normal_x;
				dot_product2 = (-1*Camera.y) * normal_y;
				dot_product3 = (-1*Camera.z) * normal_z;

				dot_product = dot_product1 + dot_product2 + dot_product3;

				if (WireFrame == 1){
					if (dot_product <= 0){
						glBegin(GL_LINES);
						glVertex3d(m[x][y].x, m[x][y].y, m[x][y].z);
						glVertex3d(m[x+1][y].x, m[x+1][y].y, m[x+1][y].z);

						glVertex3d(m[x+1][y].x, m[x+1][y].y, m[x+1][y].z);
						glVertex3d(m[x+1][y+1].x, m[x+1][y+1].y, m[x+1][y+1].z);

						glVertex3d(m[x+1][y+1].x, m[x+1][y+1].y, m[x+1][y+1].z);
						glVertex3d(m[x][y].x, m[x][y].y, m[x][y].z);
					glEnd();
					}
				} else if (WireFrame == 0){
					if (flat == 1){
						glShadeModel(GL_FLAT);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glNormal3f((-1*normal_x), (-1*normal_y), (-1*normal_z));
					glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, GreenSurface);
					glBegin(GL_TRIANGLES);
						//glVertex3d(m[x][y].x, m[x][y].y, m[x][y].z);
						glVertex3d(m[x+1][y].x, m[x+1][y].y, m[x+1][y].z);

						//glVertex3d(m[x+1][y].x, m[x+1][y].y, m[x+1][y].z);
						glVertex3d(m[x][y].x, m[x][y].y, m[x][y].z);

						//glVertex3d(m[x+1][y+1].x, m[x+1][y+1].y, m[x+1][y+1].z);
						glVertex3d(m[x+1][y+1].x, m[x+1][y+1].y, m[x+1][y+1].z);
					glEnd();
					} else if (smooth == 1){
						glShadeModel(GL_SMOOTH);
					}
				}

				/*if (dot_product <= 0){
					if (WireFrame == 0){
						if (flat == 1){
							glShadeModel(GL_FLAT);
						} else if(smooth == 1){
							glShadeModel(GL_SMOOTH);
						}
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glNormal3f((-1*normal_x), (-1*normal_y), (-1*normal_z));
					glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, GreenSurface);
					glBegin(GL_TRIANGLES);
						//glVertex3d(m[x][y].x, m[x][y].y, m[x][y].z);
						glVertex3d(m[x+1][y].x, m[x+1][y].y, m[x+1][y].z);

						//glVertex3d(m[x+1][y].x, m[x+1][y].y, m[x+1][y].z);
						glVertex3d(m[x][y].x, m[x][y].y, m[x][y].z);

						//glVertex3d(m[x+1][y+1].x, m[x+1][y+1].y, m[x+1][y+1].z);
						glVertex3d(m[x+1][y+1].x, m[x+1][y+1].y, m[x+1][y+1].z);
					glEnd();
					} else {
						glBegin(GL_LINES);
						glVertex3d(m[x][y].x, m[x][y].y, m[x][y].z);
						glVertex3d(m[x+1][y].x, m[x+1][y].y, m[x+1][y].z);

						glVertex3d(m[x+1][y].x, m[x+1][y].y, m[x+1][y].z);
						glVertex3d(m[x+1][y+1].x, m[x+1][y+1].y, m[x+1][y+1].z);

						glVertex3d(m[x+1][y+1].x, m[x+1][y+1].y, m[x+1][y+1].z);
						glVertex3d(m[x][y].x, m[x][y].y, m[x][y].z);
					glEnd();
					}
				}*/
			}
		}
		
/*
		glBegin(GL_LINES);
		for (int x = 0; x < 21; ++x){
			for (int y = 0; y < 20; ++y){
				glVertex3d(m[x][y].x, m[x][y].y, m[x][y].z);
				glVertex3d(m[x][y+1].x, m[x][y+1].y, m[x][y+1].z);
			}
		}
		for (int x = 0; x < 21; ++x){
			for (int y = 0; y < 20; ++y){
				glVertex3d(m[y][x].x, m[y][x].y, m[y][x].z);
				glVertex3d(m[y+1][x].x, m[y+1][x].y, m[y+1][x].z);
			}
		}

		//Triangle Patch
		for (int x = 0; x < 20; ++x){
			for (int y = 0; y < 20; ++y){
				glVertex3d(m[x][y].x, m[x][y].y, m[x][y].z);
				glVertex3d(m[x+1][y+1].x, m[x+1][y+1].y, m[x+1][y+1].z);
			}
		}
		glEnd();
*/
/*
		switch (control_point){
			case 1:
				glColor4f(1.0, 0.0, 0.0, 0.0);
				glBegin(GL_POINTS);
					glPointSize(5.0f);
					glVertex3d(p[2][1].x, p[2][1].y, p[2][1].z);
				glEnd();
				glColor4f(1.0, 1.0, 1.0, 0.0);
				break;
		}
*/
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	glFlush();

//	wglMakeCurrent(m_hDC,NULL);
	wglMakeCurrent(m_hDC,m_hRC);
    ::SwapBuffers(m_hDC);
}

/////////////////////////////////////////////////////////////////////////////
// CCssample5View printing

BOOL CCssample5View::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CCssample5View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{}

void CCssample5View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{}

/////////////////////////////////////////////////////////////////////////////
// CCssample5View diagnostics

#ifdef _DEBUG
void CCssample5View::AssertValid() const
{
	CView::AssertValid();
}

void CCssample5View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCssample5Doc* CCssample5View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCssample5Doc)));
	return (CCssample5Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCssample5View message handlers

int CCssample5View::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	int nPixelFormat;				
	m_hDC = ::GetDC(m_hWnd);			

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),	
		1,									
		PFD_DRAW_TO_WINDOW |			
		PFD_SUPPORT_OPENGL |			
		PFD_DOUBLEBUFFER,				
		PFD_TYPE_RGBA, 24, 0, 0, 0, 0, 0, 0,					
		0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 
                PFD_MAIN_PLANE, 0, 0, 0, 0 
	};

    nPixelFormat = ::ChoosePixelFormat(m_hDC, &pfd);
    ::SetPixelFormat(m_hDC, nPixelFormat, &pfd);
    m_hRC = ::wglCreateContext(m_hDC);
    ::wglMakeCurrent(m_hDC,m_hRC);
    SetupGL();
    ::wglMakeCurrent(m_hDC,NULL);
	
	return 0;
}

void CCssample5View::OnDestroy() 
{
	CView::OnDestroy();
	// Clean up rendering context stuff
    ::wglDeleteContext(m_hRC);
	::ReleaseDC(m_hWnd,m_hDC);	
}

void CCssample5View::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
		
    ::wglMakeCurrent(m_hDC,m_hRC);
    SizeGL1(cx, cy);
    ::wglMakeCurrent(NULL,NULL);

}

// Setup GL view/projection matrices for window size.
void CCssample5View::SizeGL1(int w, int h)
{
        // Correct for zero dimensions.
        h = h ? h : 1;
        w = w ? w : 1;
 
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, w, 0, h);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glFlush();
}

void CCssample5View::SetupGL()
{
        glDepthFunc(GL_LEQUAL);
	    glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
}

void CCssample5View::SetupLightAndMaterial()
{
	//your implementation here
	//....
}

void CCssample5View::DrawXYZaxis()
{
	  glColor4f(1.0, 0.0, 0.0, 0.0);
	  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, RedSurface);
	  glBegin(GL_LINES);
		glVertex3f(-100, 0, 0);
		glVertex3f(100,  0, 0);
	  glEnd();

      glColor4f(0.0, 1.0, 0.0, 0.0);
	  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, GreenSurface);
      glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 100,  0);
	  glEnd();

      glColor4f(0.0, 0.0, 1.0, 0.0);
	  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, BlueSurface);
      glBegin(GL_LINES);
		glVertex3f(0, 0, -100);
		glVertex3f(0, 0,  100);
	  glEnd();
      glColor4f(1.0, 1.0, 1.0, 0.0);
}

void CCssample5View::OnSelectControlPoint1() 
{
	control_point = 1;
	//InvalidateRect(NULL, FALSE);
}

void CCssample5View::OnSelectControlPoint2() 
{
	control_point = 2;
}

void CCssample5View::OnSelectControlPoint3() 
{
	control_point = 3;
}

void CCssample5View::OnSelectControlPoint4() 
{
	control_point = 4;
}

void CCssample5View::OnXDec() 
{

	switch (control_point){
		case 1:
			p[2][1].x -= move_amount;
			break;
		case 2:
			p[1][1].x -=move_amount;
			break;
		case 3:
			p[1][2].x -= move_amount;
			break;
		case 4:
			p[2][2].x -= move_amount;
			break;
	}
	InvalidateRect(NULL, FALSE);
}

void CCssample5View::OnXInc() 
{

	switch (control_point){
		case 1:
			p[2][1].x += move_amount;
			break;
		case 2:
			p[1][1].x +=move_amount;
			break;
		case 3:
			p[1][2].x += move_amount;
			break;
		case 4:
			p[2][2].x += move_amount;
			break;
	}
	InvalidateRect(NULL, FALSE);
}

void CCssample5View::OnYDec() 
{
	
	switch (control_point){
		case 1:
			p[2][1].y -= move_amount;
			break;
		case 2:
			p[1][1].y -= move_amount;
			break;
		case 3:
			p[1][2].y -= move_amount;
			break;
		case 4:
			p[2][2].y -= move_amount;
			break;
	}
    InvalidateRect(NULL, FALSE);
}

void CCssample5View::OnYInc() 
{

	switch (control_point){
		case 1:
			p[2][1].y += move_amount;
			break;
		case 2:
			p[1][1].y +=move_amount;
			break;
		case 3:
			p[1][2].y += move_amount;
			break;
		case 4:
			p[2][2].y += move_amount;
			break;
	}
	InvalidateRect(NULL, FALSE);
}

void CCssample5View::OnZDec() 
{

	switch (control_point){
		case 1:
			p[2][1].z -= move_amount;
			break;
		case 2:
			p[1][1].z -=move_amount;
			break;
		case 3:
			p[1][2].z -= move_amount;
			break;
		case 4:
			p[2][2].z -= move_amount;
			break;
	}
	InvalidateRect(NULL, FALSE);
}

void CCssample5View::OnZInc() 
{

	switch (control_point){
		case 1:
			p[2][1].z += move_amount;
			break;
		case 2:
			p[1][1].z +=move_amount;
			break;
		case 3:
			p[1][2].z += move_amount;
			break;
		case 4:
			p[2][2].z += move_amount;
			break;
	}
	InvalidateRect(NULL, FALSE);
}

float CCssample5View::Bezier3(int n, int k)
{
	int n_temp = factorial(n);
	int k_temp = factorial(k);
	int n_k_temp = factorial(n-k);

	float c = (n_temp)/((k_temp)*(n_k_temp));
	
	return c;
}

int CCssample5View::factorial(int f){

	int number;

	if (f <= 1){
		return 1;
	}

	number = f * factorial(f - 1);
	return number;

}

float CCssample5View::CalcBezier(float u, int k, int n)
{

	float c = Bezier3(n, k);
	float bez = c * pow(u, k) * pow(1-u, n-k);

	return bez;

}

void CCssample5View::setAllNormals(float u, float v, int m_index_x, int m_index_y)
{	
	int j = 0;
	int k = 0;

	float x = 0;
	float y = 0;
	float z = 0;

	for (j = 0; j < 4; ++j){
		for (k = 0; k < 4; ++k){
			x += p[j][k].x * CalcBezier(v, j, 3) * CalcBezier(u, k, 3);
			y += p[j][k].y * CalcBezier(v, j, 3) * CalcBezier(u, k, 3);
			z += p[j][k].z * CalcBezier(v, j, 3) * CalcBezier(u, k, 3);
		}
	}

	
	/*glBegin(GL_POINTS);
		glVertex3f(x, y, z);
	glEnd();*/
	
	
	m[m_index_x][m_index_y].x = x;
	m[m_index_x][m_index_y].y = y;
	m[m_index_x][m_index_y].z = z;

}

void CCssample5View::bezier_plane()
{

	//Z-axis patch
	glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(60, 0, 0);

		glVertex3f(0, 0, 20);
		glVertex3f(60, 0, 20);

		glVertex3f(0, 0, 40);
		glVertex3f(60, 0, 40);

		glVertex3f(0, 0, 60);
		glVertex3f(60, 0, 60);

		//glVertex3f(0, 0, 80);
		//glVertex3f(80, 0, 80);
	glEnd();

	//X-axis patch
	glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 60);
		
		glVertex3f(20, 0, 0);
		glVertex3f(20, 0, 60);

		glVertex3f(40, 0, 0);
		glVertex3f(40, 0, 60);

		glVertex3f(60, 0, 0);
		glVertex3f(60, 0, 60);

		//glVertex3f(80, 0, 0);
		//glVertex3f(80, 0, 80);
	glEnd();

}

void CCssample5View::CalcNormal(Vector p1, Vector p2, Vector p3, float *n)
{
	//your implementation here
	//....
}

double CCssample5View::dotproduct(float *v1, double v2[])
{
	//your implementation here
	//....
	return 0;
}

void CCssample5View::OnViewFilled() 
{
	WireFrame = 0;
	flat = 1;
	smooth = 0;
	InvalidateRect(NULL, FALSE);
}

void CCssample5View::OnViewWireframe() 
{
	WireFrame = 1;
	flat = 0;
	smooth = 0;
	InvalidateRect(NULL, FALSE);
}

void CCssample5View::OnViewFlat() 
{
	flat = 1;
	smooth = 0;
	InvalidateRect(NULL, FALSE);
}

void CCssample5View::OnViewSmooth() 
{
	smooth = 1;
	flat = 0;
	InvalidateRect(NULL, FALSE);
}


void CCssample5View::OnViewLightYdec() 
{
	light_position[1]-=5.0;
	InvalidateRect(NULL, FALSE);
}

void CCssample5View::OnViewLightYinc() 
{
	light_position[1]+=5.0;
	InvalidateRect(NULL, FALSE);
}

BOOL CCssample5View::LoadImage(LPCTSTR fileName)
{
	HBITMAP hBmp = (HBITMAP) ::LoadImage (NULL,(LPCTSTR) fileName, 
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE |LR_CREATEDIBSECTION);

	if (hBmp == NULL){
		AfxMessageBox("Error: unable to load bitmap file");
        return FALSE;
    }

    // Get bitmap info.
	::GetObject (hBmp, sizeof(texImage), &texImage);

    // Must be 24 bit
    if (texImage.bmBitsPixel != 24){
		AfxMessageBox("Error: bitmap must be 24 bit");
        return FALSE;
    }
        
    glBindTexture(GL_TEXTURE_2D, ImgName);
    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, texImage.bmWidth, texImage.bmHeight, 
			GL_BGR_EXT, GL_UNSIGNED_BYTE, texImage.bmBits );

    return TRUE;
}

void CCssample5View::SetTexture()
{
	glDrawBuffer(GL_BACK);
	glGenTextures(1, &ImgName);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	LoadImage("flower.bmp");
}

void CCssample5View::OnViewTextureDisable() 
{
	//your implementation here
	//....	
}

void CCssample5View::OnViewTextureEnable() 
{
	//your implementation here
	//....
}

void CCssample5View::OnViewPosDecz() 
{
	Camera.z-=5;
	InvalidateRect(NULL, FALSE);
}

void CCssample5View::OnViewPosIncz() 
{
	Camera.z+=5;
	InvalidateRect(NULL, FALSE);
}
void CCssample5View::OnSpecular()
{
	// TODO: Add your command handler code here
}

void CCssample5View::OnSpecular32805()
{
	// TODO: Add your command handler code here
}

void CCssample5View::OnDiffuse()
{
	// TODO: Add your command handler code here
}

void CCssample5View::OnDiffuse32810()
{
	// TODO: Add your command handler code here
}
