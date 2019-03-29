#pragma once
#include <string>

class Buffer
{
public:
	Buffer();
	~Buffer();
	
	HANDLE m_hBuffer[2];
	bool m_nScreenIndex;

	void BufferWrite(const char* string, const short& x, const short& y);
	void BufferFlipping();
	void BufferClear();
	void GotoXY(const short& x, const short& y);
private:
};

