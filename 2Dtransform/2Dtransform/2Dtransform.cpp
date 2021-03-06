//2Dtransform.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include "2Dtransform.h"
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY2DTRANSFORM, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY2DTRANSFORM));

	MSG msg = { 0 };	
	//定义并初始化msg
	while (msg.message != WM_QUIT)		//使用while循环，如果消息不是WM_QUIT消息，就继续循环
	{
		t_now = GetTickCount();   //获取当前系统时间
		if (t_now - t_pre >= 20)        //当此次循环运行与上次绘图时间相差0.1秒时再进行重绘操作
			Mypaint(hWnd);
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))   //查看应用程序消息队列，有消息时将队列中的消息派发出去。
		{
			TranslateMessage(&msg);		//将虚拟键消息转换为字符消息
			DispatchMessage(&msg);			//分发一个消息给窗口程序。
		}
		else
		{
			
		}

	}

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY2DTRANSFORM));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY2DTRANSFORM);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

    hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      0,0,WINDOW_WIDTH,WINDOW_HEIGHT, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
 
   
   ShowWindow(hWnd, nCmdShow);
  /*
  绘画初始化
  */
   mydc = GetDC(hWnd);
   // 设置初始中心点位置
   centerpoint.x = 200; 
   centerpoint.y = 300;
	  UpdateWindow(hWnd);
	  //输出提示信息
	  wchar_t a[100]=TEXT("增减顶点个数");
	  TextOut(mydc, rightline + 260, 0, a, lstrlenW(a));
	  wchar_t b[100] = TEXT("增减大小");
	  TextOut(mydc, rightline + 260, 80, b, lstrlenW(b));
	  wchar_t c[100] = TEXT("X轴方向平移");
	  TextOut(mydc, rightline + 260, 160, c, lstrlenW(c));
	  wchar_t d[100] = TEXT("Y轴方向平移");
	  TextOut(mydc, rightline + 260, 240, d, lstrlenW(d));
	  wchar_t e[100] = TEXT("逆时针《-》顺时针");
	  TextOut(mydc, rightline + 260, 320, e, lstrlenW(e));
   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
	{
		//创建5个 scrollbar来进行参数调整

		Bar_numofpoint = CreateWindow(TEXT("scrollbar"), NULL, WS_CHILD|WS_VISIBLE| SBS_HORZ , rightline+60, 0, 200, 40, hWnd, NULL, hInst, NULL);
		SetScrollRange(Bar_numofpoint, SB_CTL, 3, 40, FALSE);
		SetScrollPos(Bar_numofpoint, SB_CTL, 3, FALSE);

		Bar_Scale = CreateWindow(TEXT("scrollbar"), NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ, rightline + 60, 80, 200, 40, hWnd, HMENU(0), hInst, NULL);
		SetScrollRange(Bar_Scale, SB_CTL, 50, 200, FALSE);
		SetScrollPos(Bar_Scale, SB_CTL, 50, FALSE);

		Bar_XVconstr = CreateWindow(TEXT("scrollbar"), NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ, rightline + 60, 160, 200, 40, hWnd, HMENU(0), hInst, NULL);
		SetScrollRange(Bar_XVconstr, SB_CTL, 1, 10, FALSE);
		SetScrollPos(Bar_XVconstr, SB_CTL, 1, FALSE);

		Bar_YVconstr = CreateWindow(TEXT("scrollbar"), NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ, rightline + 60, 240, 200, 40, hWnd, HMENU(0), hInst, NULL);
		SetScrollRange(Bar_YVconstr, SB_CTL, 1, 10, FALSE);
		SetScrollPos(Bar_YVconstr, SB_CTL, 1, FALSE);
	
		Bar_RotateConstr = CreateWindow(TEXT("scrollbar"), NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ, rightline + 60, 320, 200, 40, hWnd, HMENU(0), hInst, NULL);
		SetScrollRange(Bar_RotateConstr, SB_CTL, 1, 10, FALSE);
		SetScrollPos(Bar_RotateConstr, SB_CTL, 1, FALSE);
	}
	break;
	//横向scrollbar的消息
	case WM_HSCROLL:
	{
		DWORD pos = GetMessagePos(); //获取消息位置
		WORD x = LOWORD(pos);
		WORD y = HIWORD(pos);
		POINT p;
		p.x = x;
		p.y = y;
		ScreenToClient(hWnd, &p);
		//使用y坐标进行信息区分以做出不同的响应
		if (p.y<=40)
		{
			int nMin = 0;
			int nMax = 0;
			GetScrollRange(Bar_numofpoint, SB_CTL, &nMin, &nMax);
			switch (LOWORD(wParam))
			{
			case SB_THUMBPOSITION:
			{
				num_ofpoint = HIWORD(wParam);

				break;
			}
			default:
				break;
			}
			if (num_ofpoint >= nMin && num_ofpoint <= nMax)
			{
				SetScrollPos(Bar_numofpoint, SB_CTL, num_ofpoint, false);
			}
			if (num_ofpoint > nMax)
			{
				num_ofpoint = nMax;
			}
			if (num_ofpoint < nMin)
			{
				num_ofpoint = nMin;
			}

		}
		if (p.y<=120&&p.y>=80)
		{
			int nMin = 0;
			int nMax = 0;
			GetScrollRange(Bar_Scale, SB_CTL, &nMin, &nMax);
			switch (LOWORD(wParam))
			{
			case SB_THUMBPOSITION:
			{
				radius = HIWORD(wParam);

				break;
			}
			default:
				break;
			}
			if (radius >= nMin && radius <= nMax)
			{
				SetScrollPos(Bar_Scale, SB_CTL, radius, true);
			}
			if (radius > nMax)
			{
				radius = nMax;
			}
			if (radius< nMin)
			{
				radius = nMin;
			}

		}
		if (p.y <= 200 && p.y >= 160)
		{
			int nMin = 0;
			int nMax = 0;
			GetScrollRange(Bar_XVconstr, SB_CTL, &nMin, &nMax);
			switch (LOWORD(wParam))
			{
			case SB_THUMBPOSITION:
			{
				vx = HIWORD(wParam);

				break;
			}
			default:
				break;
			}
			if (vx>= nMin && vx <= nMax)
			{
				SetScrollPos(Bar_XVconstr, SB_CTL, vx, true);
			}
			if (vx > nMax)
			{
				vx= nMax;
			}
			if (vx< nMin)
			{
				vx = nMin;
			}

		}
		if (p.y <= 280 && p.y >= 240)
		{
			int nMin = 0;
			int nMax = 0;
			GetScrollRange(Bar_YVconstr, SB_CTL, &nMin, &nMax);
			switch (LOWORD(wParam))
			{
			case SB_THUMBPOSITION:
			{
				vy= HIWORD(wParam);

				break;
			}
			default:
				break;
			}
			if (vy >= nMin && vy <= nMax)
			{
				SetScrollPos(Bar_YVconstr, SB_CTL, vy, true);
			}
			if (vy > nMax)
			{
				vy = nMax;
			}
			if (vy< nMin)
			{
				vy = nMin;
			}

		}
		if (p.y <= 360 && p.y >= 320)
		{
			int nMin = 0;
			int nMax = 0;
			GetScrollRange(Bar_RotateConstr, SB_CTL, &nMin, &nMax);
			switch (LOWORD(wParam))
			{
			case SB_THUMBPOSITION:
			{
				rate= HIWORD(wParam);

				break;
			}
			default:
				break;
			}
			if (rate >= nMin && rate <= nMax)
			{
				SetScrollPos(Bar_RotateConstr, SB_CTL, rate, true);
			}
			if (rate > nMax)
			{
				rate = nMax;
			}
			if (rate< nMin)
			{
				rate = nMin;
			}
			
		

		}
	}
	break;
	//命令消息
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择: 
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			//Mypaint(hWnd);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

