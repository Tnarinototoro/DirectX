
#define WINDOW_HEIGHT 900   //window height
#define WINDOW_WIDTH 1920   //window length
#define MAX_LOADSTRING 100  //window name length limit

POINT BLinePoints[9];   //9 default ponits of B curves
HCURSOR mycursor; //icon used in polygong function
HINSTANCE hInst; //window hinstance
WCHAR szTitle[MAX_LOADSTRING] = L"CG";  //window name string
WCHAR szWindowClass[MAX_LOADSTRING]; //window class array
HDC mydc; //global drawing device
POINT start;  //store the left click positon
std::fstream loginfo;  //system log ,unused. 
std::vector<POINT> poly; //vector to store points of the polygon
std::vector<POINT> cutLine;  //存储裁剪线的点
HWND hWnd;  //窗口句柄
ATOM                MyRegisterClass(HINSTANCE hInstance);  //窗口设计函数
BOOL                InitInstance(HINSTANCE, int);   //窗口初始化函数
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM); //消息处理函数
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);  //
INT_PTR CALLBACK   PaintDiamond(HWND dia, UINT u, WPARAM w, LPARAM l);

HDC         hMemDC;                             //兼容DC
HBITMAP     hBmpMem;                            //兼容位图
HBITMAP     hPreBmp;                            //兼容位图
                             

int mode = 0; //控制模式切换的变量
int y_max = 1; //多边形填充的上下边界的下界
int y_min = 2000; //多边形填充的上界
int polygonfinished = 0; //多边形是否绘制完的标志位
std::vector<int> Polygonset[2000]; //扫描线数组
int rectfinished = 0; // 直线裁剪算法中的裁剪多边形是否画完的标志位
HWND status;  //状态栏句柄
int NUMofpoint; //金刚石点的个数
int radius; //半径长度
#define PI 3.14159268  //常数PI
/*
各种消息的定义可以不用深究
*/
#define IDS_APP_TITLE			103
#define IDR_MAINFRAME			128
#define IDD_GRAPHICSCOURSEPROJECT_DIALOG	102
#define IDD_ABOUTBOX			103
#define IDD_PROJECT1BOX         101
#define IDM_ABOUT				104
#define IDM_EXIT				105
#define IDM_DRAW   112
#define IDM_DRAW_POLYGON 113
#define IDI_GRAPHICSCOURSEPROJECT			107
#define IDI_SMALL				108
#define IDC_GRAPHICSCOURSEPROJECT			109
#define IDC_MYICON				2
#define IDM_CLEAR                4
#define IDM_CUTLINE 114
#define IDM_BZEIR 115
#define IDM_BLINE 116
#define ID_X 12
#define ID_RADIUS 20
#define K_MAX 10000
#ifndef IDC_STATIC	
#define IDC_STATIC				-1
#define MODE_DRAW_LINES 13
#define MODE_DRAW_POLYGON 14
#define MODE_CUT_LINES 15
#define MODE_DRAW_BEZIER 16
#define MODE_DRAW_BLINE 17
#define MODE_2D_TRANSFORM 18
#define MODE_DRAWDIAMOND 20
#define IDM_DRAW_LINE 19
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
//绝对值函数
void MakePositive(int& x)
{
	x = x > 0 ? x : (-x);
}
//取得最小值的内联函数
inline int Getmin(int x, int y)
{
	return x > y ? y : x;
}
//取得最大值的内联函数
inline int GetMax(int x, int y)
{
	return x > y ? x : y;
}
//画直线的函数
VOID MyDrawLine(HDC hdc, POINT start, POINT end,COLOR16 pencolor)

{
	if (end.x == start.x)
	{
		SetPixel(hdc, start.x, start.y, pencolor);
		
		for (int i = Getmin(end.y, start.y)+1; i <= GetMax(end.y, start.y); i++)
		{
			SetPixel(hdc, start.x, i, pencolor);
			if (mode == MODE_CUT_LINES&&rectfinished==1)
			{

				POINT p;
				p.x = start.x;
				p.y = i;
				if (p.x <= 400 || p.x >= 800 || p.y <= 300 || p.y >= 800)
					cutLine.push_back(p);
			}
		}
		return;
	}
	else if (end.y == start.y)
	{
		
		for (int i = Getmin(end.x, start.x); i <= GetMax(end.x, start.x); i++)
		{
			SetPixel(hdc, i, end.y, pencolor);
			
			if (mode == MODE_CUT_LINES && rectfinished == 1)
			{

				POINT p;
				p.x = i;
				p.y = end.y;
				if (p.x <= 400 || p.x >= 800 || p.y <= 300 || p.y >= 800)
					cutLine.push_back(p);
			}
			
		}
		return;
	}
	else
	{

		if (start.x>end.x)
		{
			POINT temp = start;
			start = end;
			end = temp;
		}
		double k = (double)(end.y - start.y) / (end.x - start.x);
		if (end.y>start.y)
		{
			if (k <= 1.0)
			{
				SetPixel(hdc, start.x, start.y, pencolor);
				if (mode == MODE_CUT_LINES && rectfinished == 1)
				{

					POINT p;
					p.x = start.x;
					p.y = start.y;
					if (p.x <= 400 || p.x >= 800 || p.y <= 300 || p.y >= 800)
						cutLine.push_back(p);
				}
				
				double d = 0.5 - k;
				int	j = start.y;
				for (int i = start.x + 1; i <= end.x; i++)
				{
					if (d <0.0)
					{
						j++;
						
						d += (1 - k);
					}
					else
					{
						d -= k;
					}
					SetPixel(hdc, i, j, pencolor);
					if (mode == MODE_CUT_LINES && rectfinished == 1)
					{

						POINT p;
						p.x = i;
						p.y = j;
						if (p.x <= 400 || p.x >= 800 || p.y <= 300 || p.y >= 800)
							cutLine.push_back(p);
					}

				}
			}
			else
			{
				SetPixel(hdc, start.x, start.y, pencolor);
				if (mode == MODE_CUT_LINES && rectfinished == 1)
				{

					POINT p;
					p.x = start.x;
					p.y = start.y;
					if (p.x <= 400 || p.x >= 800 || p.y <= 300 || p.y >= 800)
						cutLine.push_back(p);
				}
				double d = 1 - 0.5*k;
				int	j = start.x;
				
				for (int i = start.y + 1; i <= end.y; i++)
				{

					if (d <0.0)
					{
						j;
						d += 1;
					}
					else
					{
						j++;
						d -= (k - 1);
					}
					SetPixel(hdc, j, i, pencolor);
					if (mode == MODE_CUT_LINES && rectfinished == 1)
					{

						POINT p;
						p.x = j;
						p.y = i;
						if (p.x <= 400 || p.x >= 800 || p.y <= 300 || p.y >= 800)
							cutLine.push_back(p);
					}


				}
			}




		}
		else
		{
			k = -k;
			if (k <= 1.0)
			{
				SetPixel(hdc, start.x, start.y, pencolor);
				if (mode == MODE_CUT_LINES)
				{

					POINT p;
					p.x = start.x;
					p.y = start.y;
					if (p.x <= 400 || p.x >= 800 || p.y <= 300 || p.y >= 800)
						cutLine.push_back(p);
				}
				
				double d = 0.5 - k;
				int	j = start.y;
				for (int i = start.x + 1; i <= end.x; i++)
				{
					if (d <0.0)
					{
						j--;
						d += (1 - k);
					}
					else
					{
						d -= k;
					}
					if (mode == MODE_CUT_LINES)
					{

						POINT p;
						p.x = i;
						p.y = j;
						if (p.x <= 400 || p.x >= 800 || p.y <= 300 || p.y >= 800)
							cutLine.push_back(p);
					}
					SetPixel(hdc, i, j, pencolor);



				}
			}
			else
			{
				SetPixel(hdc, start.x, start.y, pencolor);
				if (mode == MODE_CUT_LINES)
				{

					POINT p;
					p.x = start.x;
					p.y = start.y;
					if (p.x <= 400 || p.x >= 800 || p.y <= 300 || p.y >= 800)
						cutLine.push_back(p);
				}
				
				double d = 1 - 0.5*k;
				int	j = start.x;
				for (int i = start.y + 1; i >= end.y; i--)
				{
					if (d <0.0)
					{
						j;
						d += 1;
					}
					else
					{
						j++;
						d -= (k - 1);
					}
					SetPixel(hdc, j, i, pencolor);
					if (mode == MODE_CUT_LINES)
					{

						POINT p;
						p.x = j;
						p.y = i;
						if (p.x < 400 || p.x >800 || p.y < 300 || p.y > 800)
							cutLine.push_back(p);
					}


				}
			}


		}
	}
}
//画金刚石的函数
VOID MakeDiamod(int NUM_OF_POINT,int Radius,HDC hdc)
{
	std::vector<POINT> x;
	float angle = (2 * PI) / NUM_OF_POINT;
	for (int i = 0; i < NUM_OF_POINT; i++)
	{
		POINT p;
		p.x = Radius * cosf(angle*i)+500;
		p.y = Radius * sinf(angle*i)+400;
		x.push_back(p);
	}
	int size = 1;
	POINT m;
	while (size!=0)
	{
		size = x.size();
		if (!x.empty())
		{
			m = x.back();
			x.pop_back();
		}
		for (auto l : x)
		{
			MyDrawLine(hdc, m, l, RGB(255,0,0));
		}

	}
}
//自定义的浮点类型点 只是为了在画曲线时保存中间结果，避免经过多次乘法后精度丢失太多
struct MyPointF
{
	float x;
	float y;
};
//该函数的作用是为了在bezier以及B样条曲线的约束点处画一个粗大的点以更加醒目而已
VOID SetAlittleRect(int x,int y, HDC hdc, COLOR16 pencolor,int wide)
{
	for (int i = x - wide; i <= x+wide; i++)
	{
		for (int j = y - wide; j <= y + wide; ++j)
		{
			SetPixel(hdc, i, j, pencolor);
		}
	}
}
//获得当前客户区鼠标的位置坐标
VOID GetNowCursorPos(HWND hwnd,POINT *p)
{
	GetCursorPos(p);
	ScreenToClient(hwnd, p);
}
//边表中边的数据结构
struct MyLine
{
	POINT start; //起点
	POINT end; //终点
	int lineup; //线段的上边界
	int linebotttom; //线段的下边界
	float kx; //斜率的倒数 即dx/dy 便于直接计算我们需要的x的值
	float b; //在x轴的截距
	BOOL flat; //是否是平行于x轴的直线
};
//B样条曲线的基函数
float Basefuction(float t, int num)
{
	switch (num)
	{
	case 0:
		return (3 * t*t - 3 * t + 1 - t * t*t) / 6;
		break;
	case 1:
		return (3 * t*t*t - 6 * t*t + 4) / 6;
			break;
	case 2:
		return (3 * t*t + 3 * t + 1 - 3 * t*t*t) / 6;
	case 3:
		return (t * t*t )/ 6;
	default:
		break;
	}
}
//绘画B样条曲线的函数
VOID PaintBline()
{
	mydc = GetDC(hWnd);
	MyPointF pf;
	float f0;
	float f1;
	float f2;
	float f3;
	for (int i = 0; i < 9; ++i)
	{
		
		SetAlittleRect(BLinePoints[i].x, BLinePoints[i].y, mydc, RGB(255, 0, 0), 3); //粗大化9个控制点
		if (i <= 7)
		{
			MyDrawLine(mydc, BLinePoints[i], BLinePoints[i + 1], RGB(0, 0, 0));   //画出控制线
		}
		if (i <= 5)
		{
			for (float t = 0; t <= 1.00; t += 0.001)
			{
				f0 = Basefuction(t, 0);
				f1 = Basefuction(t, 1);
				f2 = Basefuction(t, 2);
				f3 = Basefuction(t, 3);

				pf.x = BLinePoints[i].x*f0
					+ BLinePoints[i + 1].x*f1
					+ BLinePoints[i + 2].x*f2
					+ BLinePoints[i + 3].x*f3;
				pf.y = BLinePoints[i].y*f0
					+ BLinePoints[i + 1].y*f1
					+ BLinePoints[i + 2].y*f2
					+ BLinePoints[i + 3].y*f3;
				SetPixel(mydc, pf.x, pf.y, RGB(255, 0, 0)); //画出b样条线的每一个点
			}
		}
	}
}
//检测鼠标落在哪一个控制点的粗大点的范围中，并在此时将光标变为手形，返回所在的节点的id
int InWhichNode(int x, int y)
{
	for (int id = 0; id < 9; ++id)
	{
		if (x >= (BLinePoints[id].x - 3) && x <= (BLinePoints[id].x + 3) && y >= (BLinePoints[id].y - 3) && y <= (BLinePoints[id].y + 3))
		{
			return id;
		}
	}
	return 100;
}