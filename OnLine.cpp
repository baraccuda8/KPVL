#include "pch.h"

#include "main.h"
#include "OnLine.h"

HWND OnLineWindow = NULL;

std::string strTitleOnLine = "OnLine";
std::string strClassOnLine = "ClassOnLine";


LRESULT CALLBACK WinProcOnLine(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(message == WM_CLOSE)OnLineWindow = NULL;
    return DefMDIChildProc(hWnd, message, wParam, lParam);
}


void OnLineInitApplication()
{
    WNDCLASSEX wc;

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = (WNDPROC)WinProcOnLine;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = strClassOnLine.c_str();
    wc.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wc.hIconSm        = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    RegisterClassEx(&wc);

}


void OnLineInitInstance()
{
    OnLineInitApplication();

    if(OnLineWindow)
    {
        //WM_MDIACTIVATE;
        //WM_MDIRESTORE;
        //SendMessage(MidiClientWindow, WM_MDIACTIVATE, (WPARAM)OnLineWindow, 0);
        //if(OnLineWindow !=(HWND)SendMessage(MidiClientWindow, WM_MDIGETACTIVE, 0, 0))

        SendMessage(MidiClientWindow, WM_MDIRESTORE, (WPARAM)OnLineWindow, 0);
        return;
    }
    MDICREATESTRUCT mcs;

    mcs.szTitle = strTitleOnLine.c_str();
    mcs.szClass = strClassOnLine.c_str();
    mcs.hOwner  = hInstance;
    mcs.x = mcs.cx = CW_USEDEFAULT;
    mcs.y = mcs.cy = CW_USEDEFAULT;
    mcs.style = MDIS_ALLCHILDSTYLES;

    OnLineWindow = (HWND)SendMessage(MidiClientWindow, WM_MDICREATE, 0, (LPARAM)&mcs);

    if(!OnLineWindow)
        throw std::exception(std::string("Ошибка создания окна : OnLineWindow").c_str());
}