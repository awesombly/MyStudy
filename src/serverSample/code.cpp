#include "iocp.h"


void main()
{
	IOCP iocp;

	iocp.Init();
	iocp.Frame();
	iocp.Release();
}