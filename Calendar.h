#pragma once

#define errordata "�� ��������� ������� ����\n"

bool TestData(std::string sd);

LRESULT CALLBACK WndProcCalendar(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void InitClassMont();
LRESULT CreateCalendar(HWND hWnd, WORD wParam);