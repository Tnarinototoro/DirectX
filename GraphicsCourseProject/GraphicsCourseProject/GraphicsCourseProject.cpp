
#include "stdafx.h"
#include "GraphicsCourseProject.h"
HCURSOR bziercursor; //icon of bezier funciton
HCURSOR blinecur; //icon of b curves function
int id; //bline part lines id from 0 to 8
std::fstream file("log.txt", std::ios::out);  //error log file,annonated 
std::vector<MyPointF> BezierLine; //
std::vector<MyLine> polyLineForm; //polygon lineform 
int polyfillfinished = 0; //bool variable of state of polygon
/*main function*/
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	//announce unused variable
    UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GRAPHICSCOURSEPROJECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
	//init window hinstance
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
	//load acceltable
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GRAPHICSCOURSEPROJECT));

    MSG msg;

    // 主消息循环: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}
/*
window  class design function
*/
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GRAPHICSCOURSEPROJECT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName =  MAKEINTRESOURCEW(109);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}
/*
window hinstance initial function
*/
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // store the hinstance in global variable

	 hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      0, 0, WINDOW_WIDTH	, WINDOW_HEIGHT, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

/*
message processing function
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

	case WM_CREATE:
	{
		//creation of state bar
		status = CreateWindowEx(
			0L,                              
			L"msctls_statusbar32",                 
			L"",                              
			WS_CHILD | WS_BORDER | WS_VISIBLE,  
			100, 100, 10, 10,              // x, y, cx, cy
			hWnd,                            
			(HMENU)100,                      
			hInst,                           
			NULL);                           
		if (status == NULL)
			MessageBox(NULL, L"Status Bar not created!", NULL, MB_OK);
		break;
	}
	/*
	here begins the main parts of responding to the message
	*/
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // menu processing: 
            switch (wmId)
            {
				
            case IDM_ABOUT:  //about dialogue processing
				
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);//pop up aboutbox
                break; 
				
            case IDM_EXIT:   //exit message
                DestroyWindow(hWnd);
                break;
			case IDM_DRAW:   //diamond dialogue
				DialogBox(hInst, MAKEINTRESOURCE(IDD_PROJECT1BOX), hWnd, PaintDiamond);
				break;
			case IDM_CLEAR:   //clear the paiting
				/*
				reset data
				*/
				InvalidateRect(hWnd, NULL, 1);  
				mode = 0;
				start.x = 0;
				start.y = 0;
				break;
			case IDM_DRAW_LINE:   
				mode = MODE_DRAW_LINES;  //switch to the Drawline function
				MessageBox(hWnd, L"PRESS L BUTTON TO DRAW AND RELEASE", L"BRAVO", 0); 
				break;
			case IDM_DRAW_POLYGON:   
				 mycursor = LoadCursor(NULL,IDC_CROSS);  
				 mode = MODE_DRAW_POLYGON; //switch to the polygon function
				break;
			case IDM_CUTLINE:    
			 mode = MODE_CUT_LINES; //switch to cutline mode
			 InvalidateRect(hWnd, NULL, true);
			 MessageBox(hWnd, L"L click to draw line", L"R click to cut!", 1);
			 break;
			case IDM_BLINE:   
				mode = MODE_DRAW_BLINE; //switch to b curves mode
				/*
				init of 9 points
				*/
				BLinePoints[0].x = 204;
				BLinePoints[0].y = 231;

				BLinePoints[1].x = 204;
				BLinePoints[1].y = 231;

					BLinePoints[2].x = 286;
					BLinePoints[2].y = 362;

					BLinePoints[3].x = 363;
					BLinePoints[3].y = 145;

					BLinePoints[4].x = 472;
					BLinePoints[4].y = 527;

					BLinePoints[5].x = 548;
					BLinePoints[5].y = 228;

					BLinePoints[6].x = 662;
					BLinePoints[6].y = 40;

					BLinePoints[7].x = 830;
					BLinePoints[7].y = 450;

					BLinePoints[8].x = 930;
					BLinePoints[8].y = 350;
					MessageBox(hWnd, L"L click to choose node", L"grab it and release to change the graphics", 1);
					PaintBline();
				break;
				/*
				Brzier curves responding!
				*/
			case IDM_BZEIR:
				mode = MODE_DRAW_BEZIER; //switch to bezier mode
				bziercursor = LoadCursor(NULL, IDC_CROSS); 
				MessageBox(hWnd, L"Try not to overwhelm 10 points,or it will be extremely slow!", L"L to set point R to draw", 0);
				break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
		
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
             mydc = BeginPaint(hWnd, &ps);
			 if (mode==MODE_DRAWDIAMOND)   
			 {
				 MakeDiamod(NUMofpoint, radius, mydc);
				 mode = 0;

			 }
			 if (mode == MODE_CUT_LINES)  
			 {
				 POINT p1, p2, p3, p4;
				 p1.x = 400;
				 p2.x = 800;
				 p3.x = 400;
				 p4.x = 800;
				 p1.y = 300;
				 p2.y = 300;
				 p3.y = 800;
				 p4.y = 800;
				 MyDrawLine(mydc, p1, p2, RGB(255, 0, 0));
				 MyDrawLine(mydc, p2, p4, RGB(255, 0, 0));
				 MyDrawLine(mydc, p4, p3, RGB(255, 0, 0));
				 MyDrawLine(mydc, p3, p1, RGB(255, 0, 0));
				 rectfinished = 1;
			 }
			 if (mode == MODE_DRAW_BLINE)
			 {
				 PaintBline();   
			 }
            EndPaint(hWnd, &ps);
			break;
        }
	
	case WM_LBUTTONDOWN:
	{
		
		if (mode == MODE_DRAW_LINES)    
		{
			GetCursorPos(&start);
			ScreenToClient(hWnd, &start);
		}
		if (mode == MODE_CUT_LINES)
		{
			GetCursorPos(&start);
			ScreenToClient(hWnd, &start);
		}
		if (mode == MODE_DRAW_BEZIER)
		{
			SetCursor(bziercursor);
			
			

			mydc = GetDC(hWnd);
			GetCursorPos(&start);
			ScreenToClient(hWnd, &start);
			SetAlittleRect(start.x, start.y, mydc, RGB(0, 0, 255), 5);
			MyPointF myp;
			myp.x = start.x;
			myp.y = start.y;
			BezierLine.push_back(myp);
			std::vector<MyPointF>::iterator itr = BezierLine.end();
			if (BezierLine.size()> 1)
			{
				POINT p1, p2;
				p1.x = (*(itr - 1)).x;
				p1.y = (*(itr - 1)).y;
				p2.x = (*(itr - 2)).x;
				p2.y = (*(itr - 2)).y;
				MyDrawLine(mydc,p1 ,p2, RGB(0, 0, 255));
				
				
			}

		}
		if (mode == MODE_DRAW_POLYGON)
		{
			    mydc = GetDC(hWnd);
				SetCursor(mycursor);
				GetCursorPos(&start);
				ScreenToClient(hWnd, &start);
				if (start.y >= y_max)
				{
					y_max = start.y;//get min y of the points
				}
				if (start.y <= y_min)//get max y of the points
				{
					y_min = start.y;
				}
				poly.push_back(start);
				std::vector<POINT>::iterator itr = poly.end();
				MyLine myline;
				if (poly.size()> 1)
				{
					//compute the attribute of every line
					MyDrawLine(mydc, *(itr-1), *(itr - 2), RGB(255, 0, 0));
					myline.start = *(itr - 2);
					myline.end = *(itr - 1);
					myline.lineup = GetMax(myline.start.y, myline.end.y);
					myline.linebotttom = Getmin(myline.start.y, myline.end.y);
					if (myline.start.y == myline.end.y)
					{
						myline.flat = TRUE;
						myline.kx = 0;
						myline.b = 0;
					}
					if (myline.start.y != myline.end.y)
					{
						myline.flat = FALSE;
					myline.kx = (float)(myline.start.x - myline.end.x) / (float)(myline.start.y - myline.end.y);
						myline.b = myline.start.x - myline.kx * myline.start.y;
					}
					polyLineForm.push_back(myline);//insert into the line form
				}
				
		}
		if (mode == MODE_DRAW_BLINE)
		{
			
			
				start.x = LOWORD(lParam);
				start.y = HIWORD(lParam);
			

		}
		break;
	}
	//process mousemoving message
	case WM_MOUSEMOVE:
	{
		//output the coordinate information of the mouse 
		wchar_t st[20];
		wsprintf(st, L"X:%d Y:%d", LOWORD(lParam), HIWORD(lParam));
		SendMessage(status, SB_SETTEXT, 0, (LPARAM)(LPSTR)st);
		/*
		modes below
		*/
		if (mode == MODE_DRAW_LINES)
		{
			mydc = GetDC(hWnd);
			POINT pCurrent;
			GetCursorPos(&pCurrent);
			ScreenToClient(hWnd, &pCurrent);
			ReleaseDC(hWnd, mydc);
		}
		if (mode == MODE_DRAW_BEZIER)
		{
			;
			
		}
		if (mode == MODE_CUT_LINES)
		{
			mydc = GetDC(hWnd);
			POINT pCurrent;
			GetCursorPos(&pCurrent);
			ScreenToClient(hWnd, &pCurrent);


			ReleaseDC(hWnd, mydc);
		}
		//fullfill the polygon  function
		if (mode == MODE_DRAW_POLYGON)
		{
			POINT pCurrent;
			GetNowCursorPos(hWnd, &pCurrent);
			if (poly.size() >= 3)
			{
				int Dis_X = pCurrent.x - poly.front().x;
				int Dis_Y = pCurrent.y - poly.front().y;
				MakePositive(Dis_X);
				MakePositive(Dis_Y);
				if (Dis_X <= 4 && Dis_Y <=4&&polyfillfinished==0)
				{
					mydc = GetDC(hWnd);


					hMemDC = CreateCompatibleDC(mydc);

					hBmpMem = CreateCompatibleBitmap(mydc, WINDOW_WIDTH, WINDOW_HEIGHT);

					hPreBmp = (HBITMAP)SelectObject(hMemDC, hBmpMem);


					HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));    

					HPEN hOldPen = (HPEN)SelectObject(hMemDC, hPen);       
					//when near to the last drawn point ,draw the last line
					MyLine myline;
					myline.start = poly.back();
					myline.end = poly.front();
					myline.lineup = GetMax(myline.start.y, myline.end.y);
					myline.linebotttom = Getmin(myline.start.y, myline.end.y);
					if (myline.start.y == myline.end.y)
					{
						myline.flat = TRUE;
						myline.kx = 0;
						myline.b = 0;
					}
					if (myline.start.y != myline.end.y)
					{
						myline.flat = FALSE;
						myline.kx = (float)(myline.start.x - myline.end.x) / (float)(myline.start.y - myline.end.y);
						myline.b = myline.start.x - myline.kx * myline.start.y;
					}
					polyLineForm.push_back(myline);//complete setting the lineform
					POINT p1, p2;
					MyDrawLine(mydc, poly.front(), poly.back(), RGB(255, 0, 0));
					//test
					//for (auto p : polyLineForm)
					//{
						//file << "Line: start x=" << p.start.x << " start y=" << p.start.y << " end x=" << p.end.x << " end y=" <<p.end.y << " b=" << p.kx << std::endl;
					//}
					//start fullfiling
					for (int y = y_min; y <= y_max; y++)
					{
                      //from the first scanning line
						p1.y = y;
						p2.y = y;
						int siz;

						//traverse every line;
						for (int i=0;i<polyLineForm.size();i++)
						{
							MyLine nowline = polyLineForm[i];
							MyLine next;
							if (i == polyLineForm.size() - 1)
							{
								next = polyLineForm[0];
							}
							else
							{
								next = polyLineForm[i + 1];
							}
							if (y <= nowline.lineup&&y >= nowline.linebotttom)
							{
								if (nowline.start.y == nowline.end.y)
								{
									Polygonset[y].push_back(nowline.start.x);
									Polygonset[y].push_back(nowline.end.x);
								}
								if (nowline.start.y != nowline.end.y)
								{

									int x = nowline.kx*y + nowline.b;
									if (y == nowline.end.y)
									{
										int m = (y - nowline.start.y)*(y - next.end.y);
										if (m >= 0)
										{
											Polygonset[y].push_back(x);
										}
									}
									else
									{
										Polygonset[y].push_back(x);
									}
									
								}
								
							}
						}
						

						siz = Polygonset[y].size();
						if (siz >= 2)
						{
							std::sort(Polygonset[y].begin(), Polygonset[y].end());
							for (int j = 0; j < siz - 1; j+=2)
							{
								p1.x = Polygonset[y][j];
								p2.x = Polygonset[y][j + 1];
								MoveToEx(hMemDC, p1.x, y, NULL);                        

								LineTo(hMemDC, p2.x, y);

							}
						}
    

					}
					polyfillfinished = 1;
					BitBlt(mydc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hMemDC, 0, 0, SRCINVERT);
					DeleteObject(hPen);
					DeleteObject(hBmpMem);                                
					DeleteDC(hMemDC);

				}
				
				
				
			}
			ReleaseDC(hWnd, mydc);
		}
		if (mode == MODE_DRAW_BLINE)
		{
			mydc = GetDC(hWnd);
			POINT Pcur;
			Pcur.x = LOWORD(lParam);
			Pcur.y = HIWORD(lParam);
			if (InWhichNode(Pcur.x, Pcur.y) < 9)
			{
				id = InWhichNode(Pcur.x, Pcur.y);
				//BLinePoints[id].x = Pcur.x;
				//BLinePoints[id].y = Pcur.y;
				//InvalidateRect(hWnd, NULL, 1);
				//PaintBline();
				
				SetCursor(LoadCursor(NULL, IDC_HAND));
			}
			else
			{
				start.x = LOWORD(lParam);
				start.y = HIWORD(lParam);
				
			}

		}
		break;
	}
	//lbutton pressing message responding
	case WM_LBUTTONUP:
	{
		if (mode == MODE_DRAW_LINES)
		{
			mydc = GetDC(hWnd);
			POINT pCurrent;
			GetCursorPos(&pCurrent);
			ScreenToClient(hWnd, &pCurrent);

			MyDrawLine(mydc, start, pCurrent, RGB(255, 0, 0));

			ReleaseDC(hWnd, mydc);
		}
		if (mode == MODE_CUT_LINES)
		{
			mydc = GetDC(hWnd);
			POINT pCurrent;
			GetCursorPos(&pCurrent);
			ScreenToClient(hWnd, &pCurrent);

			MyDrawLine(mydc, start, pCurrent, RGB(255, 0, 0));
			ReleaseDC(hWnd, mydc);
		}
		if (mode == MODE_DRAW_BLINE)
		{
			BLinePoints[id].x = start.x;
			BLinePoints[id].y = start.y;
			InvalidateRect(hWnd, NULL, 1);
			
		}
		break;
	}

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
	case WM_RBUTTONDOWN:  
	{
		if (mode == MODE_CUT_LINES)   
		{
			mydc = GetDC(hWnd);
			for (auto p : cutLine)
			{
				SetPixel(mydc, p.x, p.y, RGB(255, 255, 255));

			}
		}
		if (mode == MODE_DRAW_POLYGON)
		{
			
			for (int y = y_min; y <= y_max; y++)
			{
				file << "Line y=" << y << "x serial:";
				for (auto m : Polygonset[y])
				{
					file << m << " ";
				}
				file << std::endl;

			}
		}
		if (mode == MODE_DRAW_BEZIER)
		{
			POINT p;
			std::vector<POINT> mypo;
			int siz = BezierLine.size();
			for (float t = 0.00; t <= 1.00; t += 0.001)
			{
				for (int i = 0; i <= siz - 1; ++i)
				{
					int j = siz - 1;
					while (j != i)
					{
						BezierLine[j].x = BezierLine[j].x + t * (BezierLine[j - 1].x - BezierLine[j].x);
						BezierLine[j].y = BezierLine[j].y + t * (BezierLine[j - 1].y - BezierLine[j].y);
						j--;
					}
				}
				p.x = BezierLine[siz - 1].x;
				p.y = BezierLine[siz - 1].y;
				mypo.push_back(p);
			}
			std::vector<POINT>::iterator itr = mypo.end()-1;
			if (mypo.size()>1)
			{
				for(;itr!=mypo.begin()+1;itr--)
				MyDrawLine(mydc, *(itr - 1), *(itr - 2), RGB(255, 0, 0));

			}
			
		}
		if (mode == MODE_DRAW_BLINE)
		{
			;
		}
		break;
	}
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
//about message proc
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
//diamond dialogue
INT_PTR CALLBACK   PaintDiamond(HWND dia, UINT u, WPARAM w, LPARAM l)
{
	UNREFERENCED_PARAMETER(l);
	switch (u)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		int info = LOWORD(w);
		if (info== IDOK || info== IDCANCEL)
		{
			NUMofpoint= GetDlgItemInt(dia, ID_X, NULL, true); //get the number of points
			radius = GetDlgItemInt(dia, ID_RADIUS, NULL, true); //get the radius
			//MakeDiamod(NUMofpoint, radius, mydc);
			mode = MODE_DRAWDIAMOND;//mode switch
			InvalidateRect(hWnd,NULL,true);
			EndDialog(dia, LOWORD(w));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}