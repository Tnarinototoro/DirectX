//{{NO_DEPENDENCIES}}
// Microsoft Visual C++ generated include file.
// 供 2Dtransform.rc 使用
//
#define MAX_LOADSTRING 100 //标题栏最大长度
#define PI 3.14159268 //常数PI
#define rightline 1200 //小球运动区域右边界
#define topline 940 //小球运动区域的下边界
int vx = 7; //x方向速度
int vy = 4;  //y方向速度
int radius = 100; //缩放大小
float g_angle = 2*PI/20;  //旋转度数常量
HPEN oldpen;  //画笔
HPEN usepen; //兼容画笔
HBITMAP oldmap; //位图
HBITMAP usemap;  //兼容位图
POINT centerpoint; //图形中心点
std::vector<POINT> PolyPointsset;  //图形的点集合
int num_ofpoint = 10;
int rate = -5;
HWND hWnd;//window handle
HWND Bar_numofpoint;//顶点数控制
HWND Bar_XVconstr;//x方向速度控制
HWND Bar_YVconstr;//y方向速度控制
HWND Bar_RotateConstr;//角速度变化
HWND Bar_Scale;//大小变化
HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];
HDC memDc; //兼容DC
HDC mydc;  
DWORD t_now = 0; //记录当前时间
DWORD t_pre = 0; //记录上次绘画时间
#define WINDOW_WIDTH 1700   
#define WINDOW_HEIGHT 1000
/*
各种标识位声明很多事系统自动生成的无需我们去了解
*/
#define IDS_APP_TITLE			103
#define IDR_MAINFRAME			128
#define IDD_MY2DTRANSFORM_DIALOG	102
#define IDD_ABOUTBOX			103
#define IDM_ABOUT				104
#define IDM_EXIT				105
#define IDI_MY2DTRANSFORM			107
#define IDI_SMALL				108
#define IDC_MY2DTRANSFORM			109
#define IDC_MYICON				2
#define IDC_NUMSCROLLBAR 15
#ifndef IDC_STATIC
#define IDC_STATIC				-1
#endif
// 新对象的下一组默认值
//
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS

#define _APS_NO_MFC					130
#define _APS_NEXT_RESOURCE_VALUE	129
#define _APS_NEXT_COMMAND_VALUE		32771
#define _APS_NEXT_CONTROL_VALUE		1000
#define _APS_NEXT_SYMED_VALUE		110
#endif
#endif


// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void MakePositive(int& x) //绝对值函数
{
	x = x > 0 ? x : (-x);
}
//绘制多边形函数
VOID MakePolygon(int NUM_OF_POINT, int Radius, HDC hdc)
{
	std::vector<POINT> x;
	float angle = (2 * PI) / NUM_OF_POINT;
	//先进行旋转变化
	for (int i = 0; i < NUM_OF_POINT; i++)
	{
		
		POINT p;
		p.x = Radius * cosf(i*angle+g_angle*0.1*(rate-5)) + centerpoint.x;
		p.y = Radius * sinf(i*angle+g_angle*0.1*(rate-5)) + centerpoint.y;
		x.push_back(p);
	}
	int size = x.size();
	POINT m;
	if (size >= 2)
	{
		for (int i = 0; i < size ; i++)
		{
			if (i < size - 1)
			{
				MoveToEx(hdc, x[i].x, x[i].y, NULL);
				LineTo(hdc, x[i + 1].x, x[i + 1].y);
				LineTo(hdc, centerpoint.x, centerpoint.y);
			}
			else
			{
				MoveToEx(hdc, x[i].x, x[i].y, NULL);
				LineTo(hdc, centerpoint.x, centerpoint.y);
				LineTo(hdc, x[0].x, x[0].y);
			}
		
		}
		MoveToEx(hdc, x.front().x, x.front().y,NULL);
		LineTo(hdc, x.back().x, x.back().y);
	}
}

VOID Mypaint(HWND hwnd)
{
	mydc = GetDC(hwnd);
	oldpen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	memDc = CreateCompatibleDC(mydc);
	oldmap = CreateCompatibleBitmap(mydc, WINDOW_WIDTH, WINDOW_HEIGHT);
	usemap = (HBITMAP)SelectObject(memDc, oldmap);
	usepen = (HPEN)SelectObject(memDc, oldpen);
	//SetPixel(mydc, GetTickCount() % 400, GetTickCount() % 400 + 20, RGB(255, 0, 0));
	//MoveToEx(mydc, GetTickCount() % 400, GetTickCount() % 400 + 20,NULL);
	//MoveToEx(memDc, rightline, 0, NULL);
	//LineTo(memDc, rightline, topline);
	//MoveToEx(memDc, 0, topline, NULL);
	//LineTo(memDc, rightline, topline);
	int t = GetTickCount() % 400;
	//再进行平移变换 x=x+Vt
	centerpoint.x = centerpoint.x + vx * 1;
	centerpoint.y = centerpoint.y + vy * 1;
	//碰撞检测
	if ((rightline - (centerpoint.x + radius)) <= 3 || (centerpoint.x - radius) <= 3)
	{
		if (centerpoint.x < radius)
		{
			centerpoint.x = radius;
		}
		if (centerpoint.x >(rightline - radius))
		{
			centerpoint.x = (rightline - radius);
		}
		vx = (-vx);
	}
	if ((topline - (centerpoint.y + radius)) <= 3 || (centerpoint.y-radius)<=3)
	{
		if (centerpoint.y < radius)
		{
			centerpoint.y = radius;
		}
		if (centerpoint.y >(topline - radius))
		{
			centerpoint.x = (topline - radius);
		}
		vy = (-vy);
	}
	//旋转角度增加
	g_angle = g_angle + 0.9;
	MakePolygon(num_ofpoint, radius, memDc);
	BitBlt(mydc, 0, 0, rightline, topline, memDc, 0, 0, SRCCOPY); //复制进显示器缓存
	//删除设备和Dc的绑定操作
	DeleteObject(memDc);
	DeleteObject(oldpen);
	DeleteObject(usepen);
	DeleteObject(oldmap);
	DeleteObject(usepen);
		//Rectangle(mydc, x, y , x+40, y+50); 测试用

		t_pre = GetTickCount(); //记录当前 绘图时间
}