#pragma once


namespace LoginDlg
{
	bool LoadConnect();
	void SaveConnect();
	DLLRESULT CALLBACK bagSave(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};