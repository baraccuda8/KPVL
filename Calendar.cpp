#include "pch.h"
//#include "win.h"
#include "main.h"
#include "Calendar.h"

extern HINSTANCE hInstance;
//������������ ����
extern std::string szTitle;

//���������� Instance ���������
extern HINSTANCE hInstance;


//������ �������
extern HBRUSH TitleBrush0;

//����� �������
extern HBRUSH TitleBrush1;

//���������� �������
extern HBRUSH TitleBrush2;

//����������� �������
extern HBRUSH TitleBrush3;

//������� �������
extern HBRUSH TitleBrush4;


#define idCalendar 100          //ID ���� ���������
#define idCalendarOk 101        //ID ������ Ok � ���������
#define idCalendarCansel 102    //ID ������ Cansel � ���������

HWND hwndStatCalendar = NULL;   //��������� ���� ���������
HWND hwndCalendarCal = NULL;    //���� ���������
HWND hwndCalendarCansel = NULL; //������ Cansel � ���������
HWND hwndCalendarOk = NULL;     //������ Ok � ���������
int MinCallendarCX = 0;         //����������� ������ ���� ��������� �� X
int MinCallendarCY = 0;         //����������� ������ ���� ��������� �� Y

SYSTEMTIME CalendarTM;

bool TestData(std::string sd)
{
    boost::match_results<std::string::const_iterator> what;
    boost::regex xRegEx;
    xRegEx = "\\d{4}-\\d{2}-\\d{2}"; //�������� � ��������� �� ����� "ARRAY [{�����������}] OF {���}"

    return boost::regex_match(sd, xRegEx);
}

//������� Ok ���������
LRESULT CommandCalendarOk(HWND hWnd, WPARAM wParam)
{
    MonthCal_GetCurSel(hwndCalendarCal, &CalendarTM);
    WPARAM param = (WPARAM)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    SendMessage(GetWindow(hWnd, GW_OWNER), WM_USER, param, (LPARAM)&CalendarTM);
    DestroyWindow(hWnd);
    return 0;
}

//������� Cansel ���������
LRESULT CommandCalendarCansel(HWND hWnd, WPARAM wParam)
{
    DestroyWindow(hWnd);
    return 0;
}

//������� ���� ���������
LRESULT CommandCalendar(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId = LOWORD(wParam);
    // ��������� ����� � ����:
    if(wmId == idCalendarOk) return CommandCalendarOk(hWnd, HIWORD(wParam));
    if(wmId == idCalendarCansel) return CommandCalendarCansel(hWnd, HIWORD(wParam));
    return DefWindowProc(hWnd, message, wParam, lParam);
}


//������ ���������� ���� ���������
LRESULT SizeCalendar(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int cx = LOWORD(lParam);
    int cy = HIWORD(lParam);
    SetWindowPos(hwndCalendarCal, 0, 0, 0, cx, cy - 30, 0);
    SetWindowPos(hwndCalendarOk, 0, cx - 220, cy - 25, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    SetWindowPos(hwndCalendarCansel, 0, cx - 110, cy - 25, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    return 0;
}

//����������� ������ ���� ���������
LRESULT GetMinMaxInfoCalendar(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    MINMAXINFO* MI = (MINMAXINFO*)lParam;
    if(MI)
    {
        MI->ptMinTrackSize.x = MinCallendarCX;
        MI->ptMinTrackSize.y = MinCallendarCY;
    }
    return LRESULT(0);
};

//��������� WM_CREATE ��� ���� ���������
LRESULT CloseCalendar(HWND hWnd)
{
    EnableWindow(GetWindow(hWnd, GW_OWNER), TRUE);
    SetActiveWindow(GetWindow(hWnd, GW_OWNER));

    return 0;
}
//��������� WM_DESTROY ��� ���� ���������
LRESULT CloseOpen(HWND hWnd)
{
    EnableWindow(GetWindow(hWnd, GW_OWNER), FALSE);
    return 0;
}

//��������� ����� ���������
LRESULT CALLBACK WndProcCalendar(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(message == WM_CREATE) return 	CloseOpen(hWnd);
    if(message == WM_DESTROY)return 	CloseCalendar(hWnd);
    if(message == WM_GETMINMAXINFO) return GetMinMaxInfoCalendar(hWnd, message, wParam, lParam);
    if(message == WM_SIZE) return SizeCalendar(hWnd, message, wParam, lParam);
    if(message == WM_COMMAND) return CommandCalendar(hWnd, message, wParam, lParam);
    //if(message == WM_DRAWITEM) { return DrawItem((LPDRAWITEMSTRUCT)lParam); }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

#define szMont "Montch"

void InitClassMont()
{
    WNDCLASSEX cex ={0};
    cex.cbSize = sizeof(WNDCLASSEX);
    cex.style          = CS_HREDRAW | CS_VREDRAW;
    cex.lpfnWndProc    = WndProcCalendar;
    cex.cbClsExtra     = 0;
    cex.cbWndExtra     = 0;
    cex.hInstance      = hInstance;
    cex.hIcon          = NULL;
    cex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    cex.hbrBackground  = TitleBrush2;
    cex.lpszMenuName   = NULL;
    cex.lpszClassName  = szMont;
    cex.hIconSm        = NULL;
    if(cex.hInstance && !RegisterClassExA(&cex))
        throw std::exception((std::string("������ ����������� ������ ����: " + std::string(szMont)).c_str()));

}
//�������� ���� ���������
LRESULT CreateCalendar(HWND hWnd, WORD wParam)
{
    //if(wParam != BN_CLICKED) return 0;
    //hWndAnswer = hWnd;
    //param = wParam;

    DWORD flag = WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN;

    hwndStatCalendar = CreateWindow(szMont, "", flag, 0, 0, 0, 0, hWnd, (HMENU)0, hInstance, NULL);
    if(!hwndStatCalendar)
        throw std::exception("������ �������� ���������: hwndStatCalendar");

    hwndCalendarCal  = CreateWindowEx(0, MONTHCAL_CLASS, "", WS_CHILD | WS_VISIBLE | MCS_DAYSTATE, 0, 0, 0, 0, hwndStatCalendar, (HMENU)idCalendar, hInstance, NULL);
    if(!hwndCalendarCal)
        throw std::exception("������ �������� ���������: !hwndCalendarCal");

    hwndCalendarCansel  = CreateWindow("Button", "��������", WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT, 0, 0, 100, 20, hwndStatCalendar, (HMENU)idCalendarCansel, hInstance, NULL);
    if(!hwndCalendarCansel)
        throw std::exception("������ �������� ���������: !hwndCalendarCansel");

    hwndCalendarOk      = CreateWindow("Button", "�������", WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT, 0, 0, 100, 20, hwndStatCalendar, (HMENU)idCalendarOk, hInstance, NULL);
    if(!hwndCalendarOk)
        throw std::exception("������ �������� ���������: !hwndCalendarOk");

    SetWindowLongPtr(hwndStatCalendar, GWLP_USERDATA, wParam);

    RECT rc;
    MonthCal_GetMinReqRect(hwndCalendarCal, &rc);

    //rc.right *= 3;
    //rc.bottom *= 3;

    AdjustWindowRectEx (&rc, flag, false, 0);

    MinCallendarCX = rc.right - rc.left;
    MinCallendarCY = rc.bottom - rc.top + 30;
    if(MinCallendarCX < 240) MinCallendarCX = 240;

    std::tm TM;
    SYSTEMTIME st;

    time_t timer = time(NULL);
    //timer = (time_t)difftime(timer, 2635200); // ����� 1 �����
    //timer = (time_t)difftime(timer, 60 * 60 * 24); // ����� 1 ����
    localtime_s(&TM, &timer);

    st.wYear = TM.tm_year + 1900;
    st.wMonth = TM.tm_mon + 1;
    st.wDay = TM.tm_mday;

    MonthCal_SetCurSel(hwndCalendarCal, &st);

    SetWindowPos(hwndStatCalendar, HWND_TOP, 0, 0, MinCallendarCX, MinCallendarCY, 0);
    CenterWindow(hwndStatCalendar, GlobalWindow);

    ShowWindow(hwndStatCalendar, SW_SHOWNORMAL);

    //MonthCal_GetMonthRange(MonthCalendar1.Handle, GMR_DAYSTATE, @times[0]);
    return 0;
}
