#include "BitMapFile.h"
#include "BitMapManager.h"

BitMapFile::BitMapFile() : m_hMyDC(0), m_hBitmap(0), m_hOldBitmap(0),
						   m_BitInfo(),   m_srcRect()
{}

BitMapFile::~BitMapFile()
{
	SelectObject(m_hMyDC, m_hOldBitmap);
	//DeleteDC(m_hRotateDC);
	ReleaseDC(Window::m_hWnd, m_hMyDC);					// DC ����
}


// �⺻ ��ο�
bool BitMapFile::Draw(const Point& pos, const MyRect& srcRect, const DWORD& type)
{
	int&& posX = (int)(pos.x - (srcRect.getWidth() / 2 + Window::DrawPoint.x));
	int&& posY = (int)(pos.y - (srcRect.getHeight() / 2));
	BitBlt(Window::m_hOffScreenDC, posX, posY, srcRect.getWidth(), srcRect.getHeight(),
		   m_hMyDC, srcRect.getX(), srcRect.getY(), type);
	return true;
}

// ��� ��ũ��
bool BitMapFile::DrawScroll(const MyRect& rect, const MyRect& srcRect, const DWORD& type)
{
	int&& posX = (int)(rect.getX() - Window::DrawPoint.x / 5);
	StretchBlt(Window::m_hOffScreenDC, posX, rect.getY(), rect.getWidth(), rect.getHeight(),
		   m_hMyDC, srcRect.getX(), srcRect.getY(), srcRect.getWidth(), srcRect.getHeight(), type);
	if (posX > 0)
	{
		StretchBlt(Window::m_hOffScreenDC, 0, rect.getY(), posX, rect.getHeight(),
				   m_hMyDC, srcRect.getX() + posX, srcRect.getY(), -posX, srcRect.getHeight(), type);
	}
	else if (posX + rect.getWidth() < Window::getClientRect().right)
	{
		StretchBlt(Window::m_hOffScreenDC, rect.getWidth() + posX, rect.getY(), -posX, rect.getHeight(),
				   m_hMyDC, srcRect.getRight(), srcRect.getY(), posX, srcRect.getHeight(), type);
	}
	return true;
}

// ������ ���� + ��� ����ũ
bool BitMapFile::DrawMask(const MyRect& rect, const MyRect& srcRect, const COLORREF& maskColor)
{
	TransparentBlt(Window::m_hOffScreenDC, rect.getX() - (int)Window::DrawPoint.x, rect.getY(), rect.getWidth(), rect.getHeight(),
				   m_hMyDC, srcRect.getX(), srcRect.getY(), srcRect.getWidth(), srcRect.getHeight(), maskColor);
	return true;
}

// ȭ�� ���� ��ο�
bool BitMapFile::DrawUI(const MyRect& rect, const MyRect& srcRect, const COLORREF& maskColor)
{
	TransparentBlt(Window::m_hOffScreenDC, rect.getX(), rect.getY(), rect.getWidth(), rect.getHeight(),
				   m_hMyDC, srcRect.getX(), srcRect.getY(), srcRect.getWidth(), srcRect.getHeight(), maskColor);
	return true;
}

// + �¿� ����
bool BitMapFile::DrawStretch(const MyRect& rect, const MyRect& srcRect, const DWORD& type, const bool& useReverse)
{
	if (!useReverse)
	{
		StretchBlt(Window::m_hOffScreenDC, rect.getX() - (int)Window::DrawPoint.x, rect.getY(), rect.getWidth(), rect.getHeight(),
				   m_hMyDC, srcRect.getX(), srcRect.getY(), srcRect.getWidth(), srcRect.getHeight(), type);
	}
	else
	{
		StretchBlt(Window::m_hOffScreenDC, rect.getX() - (int)Window::DrawPoint.x, rect.getY(), rect.getWidth(), rect.getHeight(),
				   m_hMyDC, srcRect.getRight(), srcRect.getY(), -srcRect.getWidth(), srcRect.getHeight(), type);
	}
	return true;
}

// �̹��� ȸ����
bool BitMapFile::DrawRotation(HBITMAP* hRotateBitmap, HBITMAP* hRotateOldBitmap, const Point& pos, const DWORD& type, const float& distance)
{
	*hRotateOldBitmap = (HBITMAP)SelectObject(m_hMyDC, *hRotateBitmap);
	
	int&& posX = (int)(pos.x - (distance / 2 + Window::DrawPoint.x));
	int&& posY = (int)(pos.y - distance / 2);
	BitBlt(Window::m_hOffScreenDC, posX, posY, (int)distance, (int)distance, m_hMyDC, 0, 0, type);
	
	SelectObject(m_hMyDC, *hRotateOldBitmap);
	return true;
}

// ��Ʈ�� ȸ����Ŵ
HBITMAP BitMapFile::SetRotationBitMap(HDC* hRotateDC, HBITMAP* hRotateBitmap, HBITMAP* hRotateOldBitmap, const MyRect& srcRect, const float& angle, const float& distance)
{
	if (*hRotateBitmap == NULL)
	{
		// ȸ���� ��Ʈ�� ����, ����a
		*hRotateBitmap = CreateCompatibleBitmap(Window::m_hScreenDC, (int)distance, (int)distance);
		SelectObject(*hRotateDC, BitMapManager::m_backBrush);
	}
	*hRotateOldBitmap = (HBITMAP)SelectObject(*hRotateDC, *hRotateBitmap);

	// ����ũ������ ��� ĥ��
	PatBlt(*hRotateDC, 0, 0, (int)distance, (int)distance, PATCOPY);

	// ��ǥ�� ��ȯ
	int oldMode = SetGraphicsMode(*hRotateDC, GM_ADVANCED);

	// ȸ���� ���
	float radian = MyRect::getRadian(angle);
	float cosine = cos(radian);
	float sine = sin(radian);
	XFORM xForm;
	xForm.eM11 = cosine;			xForm.eM12 = sine;
	xForm.eM21 = -sine;				xForm.eM22 = cosine;
	xForm.eDx = distance / 2;		xForm.eDy = distance / 2;

	// DC�� ��İ��� ���� ȸ����Ŵ
	if (SetWorldTransform(*hRotateDC, &xForm) == false)
		return false;

	// �̹��� �Ѹ�
	BitBlt(*hRotateDC, -(srcRect.getWidth() / 2), -(srcRect.getHeight() / 2),
		   srcRect.getWidth(), srcRect.getHeight(),
		   m_hMyDC, srcRect.getX(), srcRect.getY(), SRCCOPY);

	// ȸ��DC �� ��ǥ�� �ʱ�ȭ
	//SelectObject(*hRotateDC, *hRotateOldBitmap);
	//xForm.eM11 = 1;	xForm.eM12 = 0;
	//xForm.eM21 = 0;	xForm.eM22 = 1;
	//xForm.eDx = 0;	xForm.eDy = 0;
	//SetWorldTransform(*hRotateDC, &xForm);
	SetGraphicsMode(*hRotateDC, oldMode);

	return *hRotateBitmap;
}

// �̸����� �ҷ�����
bool BitMapFile::Load(wstring bitName)
{
	bitName = L"../../data/bitmap/" + bitName + L".bmp";
	// HDC LoadImage(��ü�ڵ�, ���ϰ��, ��������, x, y, ȣ��ɼ�);
	m_hBitmap = (HBITMAP)LoadImage(Window::m_hInstance, bitName.c_str(), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	if (m_hBitmap == NULL)
		return false;
	GetObject(m_hBitmap, sizeof(BITMAP), &m_BitInfo);				// ��Ʈ�� ���� ���(m_BitInfo)
	m_hMyDC = CreateCompatibleDC(Window::m_hScreenDC);				// ȣȯ�Ǵ� DC ����(���DC)
	//m_hRotateDC = CreateCompatibleDC(Window::m_hScreenDC);
	m_hOldBitmap = (HBITMAP)SelectObject(m_hMyDC, m_hBitmap);		// ������ �޸𸮿� ��Ʈ�� ����
	m_srcRect.setRect(0, 0, m_BitInfo.bmWidth, m_BitInfo.bmHeight);	// �⺻ �ҽ� ũ��
	return true;
}

//// ���� ��η� �ҷ�����
//bool BitMapFile::Load(const TCHAR* fileUrl)
//{
//	// HDC LoadImage(��ü�ڵ�, ���ϰ��, ��������, x, y, ȣ��ɼ�);
//	m_hBitmap = (HBITMAP)LoadImage(Window::m_hInstance, fileUrl, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
//	GetObject(m_hBitmap, sizeof(BITMAP), &m_BitInfo);				// ��Ʈ�� ���� ���(m_BitInfo)
//	m_hMyDC = CreateCompatibleDC(Window::m_hScreenDC);				// ȣȯ�Ǵ� DC ����(���DC)
//	m_hRotateDC = CreateCompatibleDC(Window::m_hScreenDC);	
//	m_hOldBitmap = (HBITMAP)SelectObject(m_hMyDC, m_hBitmap);		// ������ �޸𸮿� ��Ʈ�� ����
//	m_srcRect.setRect(0, 0, m_BitInfo.bmWidth, m_BitInfo.bmHeight);	// �⺻ �ҽ� ũ��
//	return true;
//}
//
//// ��Ʈ��ID�� �ҷ�����
//bool BitMapFile::Load(DWORD bitmapID)
//{
//	// ��Ʈ�� ��Ʈ�ʺҷ�����(��ü�ڵ�, ���ҽ�ID->�ڵ�ȭ(���);
//	m_hBitmap = LoadBitmap(Window::m_hInstance, MAKEINTRESOURCE(bitmapID));
//	GetObject(m_hBitmap, sizeof(BITMAP), &m_BitInfo);				 // ��Ʈ�� ���� ���(m_BitInfo)
//	m_hMyDC = CreateCompatibleDC(Window::m_hScreenDC);				 // ȣȯ�Ǵ� DC ����(���DC)
//	m_hRotateDC = CreateCompatibleDC(Window::m_hScreenDC);
//	m_hOldBitmap = (HBITMAP)SelectObject(m_hMyDC, m_hBitmap);		 // MyDc�� ������Ʈ�� Bitmap���� �ٲٰ� ���� ��Ʈ���� OldBitmap�� ���� ���� ��
//	m_srcRect.setRect(0, 0, m_BitInfo.bmWidth, m_BitInfo.bmHeight);  // �⺻ �ҽ� ũ��
//	return true;
//}

void BitMapFile::setSrcRect(const int& sx, const int& sy, const int& width, const int& height)
{
	m_srcRect.setRect(sx, sy, width, height);
}

MyRect BitMapFile::getSrcRect() const
{
	return m_srcRect;
}

const BITMAP& BitMapFile::getBitmapInfo() const
{
	return m_BitInfo;
}
