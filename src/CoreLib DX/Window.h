#pragma once
#include "MyRect.h"


class Window
{
protected:
	MSG				message;					// �޼��� �����̳�
	RECT			m_winRect;					// ������ ũ�� �Ӽ�
	static RECT	    m_clientRect;				// Ŭ���̾�Ʈ ����
	
public:
	static MyVector2		DrawPoint;
	static HWND				m_hWnd;				// ������ �ڵ�
	static HINSTANCE		m_hInstance;		// ���� ��ü �ڵ�
	static Window*			Instance;
public:
	static void ResizeWindow(const MyRect& rect) noexcept;
	static RECT getClientRect();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);							// �޼��� ó�� �ݹ��Լ�
	bool SetWindow(const HINSTANCE& hInstance, const int& nCmdShow, const TCHAR* title = L"SampleWin") noexcept;	// ������ Ȱ��ȭ
	void SetClientRect(const int& x, const int& y, const int& width, const int& height) noexcept;						// ȭ�� ũ�� ����
	bool MessageProcess();					// ������ �޼��� �ؼ�, ó��
	virtual void MsgEvent(const MSG& message) = 0;	// ���콺 �̺�Ʈ �ڵ鸵��
public:
	Window()
	{ Instance = this; }
	virtual ~Window() = default;
};