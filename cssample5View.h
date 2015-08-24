// cssample5View.h : interface of the CCssample5View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CSsample5VIEW_H__72070A70_2B2E_4625_A38B_3C3967F61AFB__INCLUDED_)
#define AFX_CSsample5VIEW_H__72070A70_2B2E_4625_A38B_3C3967F61AFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCssample5View : public CView
{
protected: // create from serialization only
	CCssample5View();
	DECLARE_DYNCREATE(CCssample5View)

// Attributes
public:
	CCssample5Doc* GetDocument();

// Operations
public:

	int m_GLPixelIndex;	
	HDC m_hDC;
	HGLRC m_hRC;

	GLdouble m_ytheta;
	GLdouble m_xtheta;
	CPoint m_LDownPos;
	BOOL m_LButtonDown;

	int w, h;
	int angle, rangle;
	int control_point;
	int point_store_x;
	int point_store_y;

	float move_amount;

	typedef struct Vec{
		float x;
		float y;
		float z;
	}Vector;
	Vector p[4][4];
	Vector m[21][21];
	Vector t[21][42];
	Vector step;
	Vector u, v, n;
	Vector camera;
	int x_step;
	int y_step;
	int z_step;
	float normal[40][40][3];
	float vnormal[21][21][3];
	int flat, smooth;

	GLfloat RedSurface[4];
	GLfloat GreenSurface[4];
	GLfloat YellowSurface[4];
	GLfloat WhiteSurface[4];
	GLfloat BlueSurface[4];
	GLfloat BlackSurface[4];

	GLfloat LightAmbient[4];
	GLfloat LightDiffuse[4];
	GLfloat LightSpecular[4];

	GLfloat ambient_reflection[4];
	GLfloat diffuse_reflection[4];
	GLfloat specular_reflection[4];

	GLfloat light_position[4];

	GLfloat shiny[1];
	
	// Setup GL view/projection matrices for window size.
    void SizeGL1(int w, int h);
    // Setup default params (lighting, etc...) for GL.
    void SetupGL();
    // Render scene using OpenGL.
    void DrawCylinder(int base_radius, int top_radius, int height);

	void DrawXYZaxis();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCssample5View)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	void setAllNormals(float u, float v, int m_index_x, int m_index_y);
	void SetTexture();
	GLuint ImgName;
	BITMAP texImage;
	BOOL LoadImage(LPCTSTR fileName);
	Vector Camera;
	Vector LightPos;
	float CalcBezier(float u, int k, int n);
	BOOL rendering_mode;
	double dotproduct(float* v1, double v2[]);
	int WireFrame;
	void SetupLightAndMaterial(void);
	void CalcNormal(Vector p1, Vector p2, Vector p3, float *n);
	void bezier_plane(void);
	float Bezier3(int n, int k);
	int factorial(int f);
	virtual ~CCssample5View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCssample5View)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelectControlPoint1();
	afx_msg void OnSelectControlPoint2();
	afx_msg void OnSelectControlPoint3();
	afx_msg void OnSelectControlPoint4();
	afx_msg void OnXDec();
	afx_msg void OnXInc();
	afx_msg void OnYDec();
	afx_msg void OnYInc();
	afx_msg void OnZDec();
	afx_msg void OnZInc();
	afx_msg void OnViewFilled();
	afx_msg void OnViewWireframe();
	afx_msg void OnViewFlat();
	afx_msg void OnViewSmooth();
	afx_msg void OnViewLightYdec();
	afx_msg void OnViewLightYinc();
	afx_msg void OnViewTextureDisable();
	afx_msg void OnViewTextureEnable();
	afx_msg void OnViewPosDecz();
	afx_msg void OnViewPosIncz();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSpecular();
	afx_msg void OnSpecular32805();
	afx_msg void OnDiffuse();
	afx_msg void OnDiffuse32810();
};

#ifndef _DEBUG  // debug version in cssample5View.cpp
inline CCssample5Doc* CCssample5View::GetDocument()
   { return (CCssample5Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSsample5VIEW_H__72070A70_2B2E_4625_A38B_3C3967F61AFB__INCLUDED_)
