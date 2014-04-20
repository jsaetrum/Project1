#include <windows.h>
#include <commctrl.h>

#include "resource.h"

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;
int lastx,lasty,x,y;		//GLOBAL VARIABLES used in drawing.

HWND hwnd, g_hToolBar;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int iCmdShow)
{
   static TCHAR szAppName[] = TEXT ("Paint") ;//Winddow Class name
   MSG msg ;
   WNDCLASSEX wndClassEx;
   wndClassEx.cbSize = sizeof(WNDCLASSEX);
   wndClassEx.style = CS_HREDRAW | CS_VREDRAW ;
   wndClassEx.lpfnWndProc = WndProc ;
   wndClassEx.cbClsExtra = 0;
   wndClassEx.cbWndExtra = 0;
   wndClassEx.hInstance= hInstance ;
   wndClassEx.hIcon = LoadIcon (NULL, IDI_APPLICATION) ;
   wndClassEx.hCursor= LoadCursor (NULL, IDC_ARROW) ;
	wndClassEx.hbrBackground   = (HBRUSH)(COLOR_3DSHADOW+1);
   wndClassEx.lpszMenuName    = TEXT("MAIN");
   wndClassEx.lpszClassName = szAppName ;
	wndClassEx.hIconSm         = LoadIcon(NULL, IDI_APPLICATION);

   if (!RegisterClassEx (&wndClassEx))
   {
      MessageBox (NULL, TEXT ("This program requires Windows 98!"), 
      szAppName, MB_ICONERROR) ;
      return 0 ;
   }

   hwnd = CreateWindowEx (WS_EX_APPWINDOW, szAppName, // window class name
      TEXT ("Paint in Visual C++."), // window caption
      WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,// window style
      CW_USEDEFAULT,// initial x position
      CW_USEDEFAULT,// initial y position
      CW_USEDEFAULT,// initial x size
      CW_USEDEFAULT,// initial y size
      NULL,// parent window handle
      NULL,// window menu handle
      hInstance, // program instance handle
      NULL) ;// creation parameters
   ShowWindow (hwnd, iCmdShow) ;
   UpdateWindow (hwnd) ;

   g_hToolBar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL,
			WS_CHILD | WS_VISIBLE, 0, 0, 0, 0,
			hwnd, (HMENU)ID_TOOLBAR, hInstance, NULL);
			
   SendMessage(g_hToolBar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

   while (GetMessage (&msg, NULL, 0, 0))
   {
      TranslateMessage (&msg) ;
      DispatchMessage (&msg) ;
   }

   return msg.wParam ;
}

void line(HDC _hdc,int x1,int y1,int x2,int y2)//This function draws line by the given four coordinates.
{
	MoveToEx(_hdc,x1,y1,NULL);
	LineTo(_hdc,x2,y2);
}
LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   HDC hdc;
   PAINTSTRUCT ps;
   RECT rect;
	 	
   switch (message)
   {
	 case WM_LBUTTONDOWN:					//If Left mouse button is pressed
			lastx=LOWORD(lParam);			//Store the x-coordiante in lastx
			lasty=HIWORD(lParam);			//Store the y-coordinate in lasty
			return 0;
   case WM_MOUSEMOVE:						//When mouse is moved on the client area (or form for VB users)
		 hdc = GetDC(hwnd);					//hdc is handle to device context
		 x=LOWORD(lParam);					//Store the current x 
		 y=HIWORD(lParam);					//Store the current y
		 if (wParam & MK_LBUTTON)			//If Left mouse button is down then draw
		 {	
			line(hdc,lastx,lasty,x,y);		//Draw the line frome the last pair of coordiates to current
			lastx=x;						//The current x becomes the lastx for next line to be drawn
			lasty=y;						//The current y becomes the lasty for next line to be drawn
		 }
		 ReleaseDC(hwnd,hdc);
		 return 0;
   case WM_PAINT:
      hdc = BeginPaint (hwnd, &ps) ;
      GetClientRect (hwnd, &rect) ;
		 TextOut(hdc,0,0 , (LPCWSTR)"Programmer :- Niloy Mondal. Email:- niloygk@yahoo.com",53);		 		 		 
		 EndPaint (hwnd, &ps) ;
      return 0 ;
 
   case WM_DESTROY:
      PostQuitMessage (0) ;
      return 0 ;
   }
   return DefWindowProc (hwnd, message, wParam, lParam) ;
}