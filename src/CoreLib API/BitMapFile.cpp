#include "BitMapFile.h"
#include "BitMapManager.h"

BitMapFile::BitMapFile() : m_hMyDC(0), m_hBitmap(0), m_hOldBitmap(0),
						   m_BitInfo(),   m_srcRect()
{}

BitMapFile::~BitMapFile()
{
	SelectObject(m_hMyDC, m_hOldBitmap);
	//DeleteDC(m_hRotateDC);
	ReleaseDC(Window::m_hWnd, m_hMyDC);					// DC 해제
}


// 기본 드로우
bool BitMapFile::Draw(const Point& pos, const MyRect& srcRect, const DWORD& type)
{
	int&& posX = (int)(pos.x - (srcRect.getWidth() / 2 + Window::DrawPoint.x));
	int&& posY = (int)(pos.y - (srcRect.getHeight() / 2));
	BitBlt(Window::m_hOffScreenDC, posX, posY, srcRect.getWidth(), srcRect.getHeight(),
		   m_hMyDC, srcRect.getX(), srcRect.getY(), type);
	return true;
}

// 배경 스크롤
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

// 사이즈 변경 + 배경 마스크
bool BitMapFile::DrawMask(const MyRect& rect, const MyRect& srcRect, const COLORREF& maskColor)
{
	TransparentBlt(Window::m_hOffScreenDC, rect.getX() - (int)Window::DrawPoint.x, rect.getY(), rect.getWidth(), rect.getHeight(),
				   m_hMyDC, srcRect.getX(), srcRect.getY(), srcRect.getWidth(), srcRect.getHeight(), maskColor);
	return true;
}

// 화면 고정 드로우
bool BitMapFile::DrawUI(const MyRect& rect, const MyRect& srcRect, const COLORREF& maskColor)
{
	TransparentBlt(Window::m_hOffScreenDC, rect.getX(), rect.getY(), rect.getWidth(), rect.getHeight(),
				   m_hMyDC, srcRect.getX(), srcRect.getY(), srcRect.getWidth(), srcRect.getHeight(), maskColor);
	return true;
}

// + 좌우 반전
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

// 이미지 회전시
bool BitMapFile::DrawRotation(HBITMAP* hRotateBitmap, HBITMAP* hRotateOldBitmap, const Point& pos, const DWORD& type, const float& distance)
{
	*hRotateOldBitmap = (HBITMAP)SelectObject(m_hMyDC, *hRotateBitmap);
	
	int&& posX = (int)(pos.x - (distance / 2 + Window::DrawPoint.x));
	int&& posY = (int)(pos.y - distance / 2);
	BitBlt(Window::m_hOffScreenDC, posX, posY, (int)distance, (int)distance, m_hMyDC, 0, 0, type);
	
	SelectObject(m_hMyDC, *hRotateOldBitmap);
	return true;
}

// 비트맵 회전시킴
HBITMAP BitMapFile::SetRotationBitMap(HDC* hRotateDC, HBITMAP* hRotateBitmap, HBITMAP* hRotateOldBitmap, const MyRect& srcRect, const float& angle, const float& distance)
{
	if (*hRotateBitmap == NULL)
	{
		// 회전용 비트맵 생성, 선택a
		*hRotateBitmap = CreateCompatibleBitmap(Window::m_hScreenDC, (int)distance, (int)distance);
		SelectObject(*hRotateDC, BitMapManager::m_backBrush);
	}
	*hRotateOldBitmap = (HBITMAP)SelectObject(*hRotateDC, *hRotateBitmap);

	// 마스크용으로 흰색 칠함
	PatBlt(*hRotateDC, 0, 0, (int)distance, (int)distance, PATCOPY);

	// 좌표계 변환
	int oldMode = SetGraphicsMode(*hRotateDC, GM_ADVANCED);

	// 회전용 행렬
	float radian = MyRect::getRadian(angle);
	float cosine = cos(radian);
	float sine = sin(radian);
	XFORM xForm;
	xForm.eM11 = cosine;			xForm.eM12 = sine;
	xForm.eM21 = -sine;				xForm.eM22 = cosine;
	xForm.eDx = distance / 2;		xForm.eDy = distance / 2;

	// DC를 행렬값에 따라 회전시킴
	if (SetWorldTransform(*hRotateDC, &xForm) == false)
		return false;

	// 이미지 뿌림
	BitBlt(*hRotateDC, -(srcRect.getWidth() / 2), -(srcRect.getHeight() / 2),
		   srcRect.getWidth(), srcRect.getHeight(),
		   m_hMyDC, srcRect.getX(), srcRect.getY(), SRCCOPY);

	// 회전DC 및 좌표계 초기화
	//SelectObject(*hRotateDC, *hRotateOldBitmap);
	//xForm.eM11 = 1;	xForm.eM12 = 0;
	//xForm.eM21 = 0;	xForm.eM22 = 1;
	//xForm.eDx = 0;	xForm.eDy = 0;
	//SetWorldTransform(*hRotateDC, &xForm);
	SetGraphicsMode(*hRotateDC, oldMode);

	return *hRotateBitmap;
}

// 이름으로 불러오기
bool BitMapFile::Load(wstring bitName)
{
	bitName = L"../../data/bitmap/" + bitName + L".bmp";
	// HDC LoadImage(객체핸들, 파일경로, 파일종류, x, y, 호출옵션);
	m_hBitmap = (HBITMAP)LoadImage(Window::m_hInstance, bitName.c_str(), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	if (m_hBitmap == NULL)
		return false;
	GetObject(m_hBitmap, sizeof(BITMAP), &m_BitInfo);				// 비트맵 정보 얻기(m_BitInfo)
	m_hMyDC = CreateCompatibleDC(Window::m_hScreenDC);				// 호환되는 DC 생성(대상DC)
	//m_hRotateDC = CreateCompatibleDC(Window::m_hScreenDC);
	m_hOldBitmap = (HBITMAP)SelectObject(m_hMyDC, m_hBitmap);		// 생성된 메모리에 비트맵 적용
	m_srcRect.setRect(0, 0, m_BitInfo.bmWidth, m_BitInfo.bmHeight);	// 기본 소스 크기
	return true;
}

//// 파일 경로로 불러오기
//bool BitMapFile::Load(const TCHAR* fileUrl)
//{
//	// HDC LoadImage(객체핸들, 파일경로, 파일종류, x, y, 호출옵션);
//	m_hBitmap = (HBITMAP)LoadImage(Window::m_hInstance, fileUrl, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
//	GetObject(m_hBitmap, sizeof(BITMAP), &m_BitInfo);				// 비트맵 정보 얻기(m_BitInfo)
//	m_hMyDC = CreateCompatibleDC(Window::m_hScreenDC);				// 호환되는 DC 생성(대상DC)
//	m_hRotateDC = CreateCompatibleDC(Window::m_hScreenDC);	
//	m_hOldBitmap = (HBITMAP)SelectObject(m_hMyDC, m_hBitmap);		// 생성된 메모리에 비트맵 적용
//	m_srcRect.setRect(0, 0, m_BitInfo.bmWidth, m_BitInfo.bmHeight);	// 기본 소스 크기
//	return true;
//}
//
//// 비트맵ID로 불러오기
//bool BitMapFile::Load(DWORD bitmapID)
//{
//	// 비트맵 비트맵불러오기(객체핸들, 리소스ID->코드화(대상);
//	m_hBitmap = LoadBitmap(Window::m_hInstance, MAKEINTRESOURCE(bitmapID));
//	GetObject(m_hBitmap, sizeof(BITMAP), &m_BitInfo);				 // 비트맵 정보 얻기(m_BitInfo)
//	m_hMyDC = CreateCompatibleDC(Window::m_hScreenDC);				 // 호환되는 DC 생성(대상DC)
//	m_hRotateDC = CreateCompatibleDC(Window::m_hScreenDC);
//	m_hOldBitmap = (HBITMAP)SelectObject(m_hMyDC, m_hBitmap);		 // MyDc의 오브젝트를 Bitmap으로 바꾸고 이전 비트맵을 OldBitmap에 저장 시켜 둠
//	m_srcRect.setRect(0, 0, m_BitInfo.bmWidth, m_BitInfo.bmHeight);  // 기본 소스 크기
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
