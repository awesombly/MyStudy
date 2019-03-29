#pragma once
#include <Windows.h>
#include <tchar.h>
#include <process.h>
#include <iostream>
#include <vector>
#define MAX_THREAD_COUNT 5
#define MAX_READ_SIZE 4096 * 4

using namespace std;

class IOCP
{
protected:
	uintptr_t m_hWorkThread[MAX_THREAD_COUNT] = { 0, };
	// 핸들들
	HANDLE m_hReadFile = nullptr;
	HANDLE m_hWriteFile = nullptr;
	HANDLE m_hIOCP = nullptr;

	HANDLE m_hEventKillThread = nullptr;
	// 비동기용 구조체
	OVERLAPPED m_hReadOV  = { 0, };
	OVERLAPPED m_hWriteOV = { 0, };
	 
	LARGE_INTEGER m_fileSize;
	char m_ReadBuffer[MAX_READ_SIZE];
public:
	static UINT WINAPI WorkerThread(LPVOID param);
	bool WaitForRead();
	bool DispatchRead(DWORD trans);
	bool DispatchWrite(DWORD trans);

	bool Init();
	bool Frame();
	bool Release();

	IOCP() = default;
	virtual ~IOCP() = default;
};