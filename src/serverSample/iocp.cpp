#include "iocp.h"



#define SAFE_CLOSE_HANDLE(handle) if (handle != INVALID_HANDLE_VALUE) CloseHandle(handle)

UINT WINAPI IOCP::WorkerThread(LPVOID param)
{
	IOCP* pIOCP = (IOCP*)param;

	DWORD transferByte = 0;
	ULONG_PTR keyValue = 0;
	OVERLAPPED* ov = nullptr;
	bool ret;
	while (true)
	{
		// 신호 받을시 쓰레드 종료
		if (WaitForSingleObject(pIOCP->m_hEventKillThread, 1) == WAIT_OBJECT_0)
		{
			break;
		}
		// IOCP의 큐에 완료된 작업이 있다면 가져옴
		ret = GetQueuedCompletionStatus(pIOCP->m_hIOCP, &transferByte, &keyValue, &ov, INFINITE);
		if (ret)
		{
			if (transferByte != 0)
			{
				if (keyValue != 0 && ov != nullptr)
				{
					// 정상적 완료
					if (keyValue == 1234)
					{
						pIOCP->DispatchRead(transferByte);
					}
					if (keyValue == 9999)
					{
						pIOCP->DispatchWrite(transferByte);
					}
				}
			}
			else
			{
				//SetEvent(pIOCP->m_hEventKillThread);
			}
		}
		else
		{
			DWORD lastError = GetLastError();
			if (lastError == WAIT_TIMEOUT)
			{
				continue;
			}
			else
			{
				if (ov != nullptr)
				{
					if (lastError == ERROR_HANDLE_EOF)
					{
						//SetEvent(pIOCP->m_hEventKillThread);
					}
					else
					{
						std::cout << std::endl << "Error!!" << std::endl;
					}
				}
			}
		}
	}

	return 0;
}

bool IOCP::Init()
{
	m_hReadFile = CreateFile(L"../../pool.7z", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);
	m_hWriteFile = CreateFile(L"../../copy.7z", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);
	GetFileSizeEx(m_hReadFile, &m_fileSize);

	UINT threadId = 0;
	for (auto& iter : m_hWorkThread)
		iter = _beginthreadex(nullptr, 0, IOCP::WorkerThread, (void*)this, 0, &threadId);
	m_hEventKillThread = CreateEvent(0, true, false, 0);

	// Input/Output Completion Port 핸들 생성
	m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	// IOCP 핸들에 바인딩, 다른 쓰레드의 비동기 작업을 확인 가능(대상, iocp, 완료키, 0)
	CreateIoCompletionPort(m_hReadFile, m_hIOCP, 1234, 0);
	CreateIoCompletionPort(m_hWriteFile, m_hIOCP, 9999, 0);


	//ZeroMemory(&m_hReadOV, sizeof(OVERLAPPED));
	//ZeroMemory(&m_hWriteOV, sizeof(OVERLAPPED));

	return true;
}

bool IOCP::WaitForRead()
{
	DWORD readByte = 0;
	bool ret = ReadFile(m_hReadFile, m_ReadBuffer, MAX_READ_SIZE, &readByte, &m_hReadOV);


	DWORD lastError = GetLastError();
	if (!ret)
	{
		// 진행중일시
		switch (lastError)
		{
		case ERROR_IO_PENDING:
		{
			return true;
		} break;
		case ERROR_HANDLE_EOF:
		{
			return false;
		} break;
		}
		return false;
	}
	else
	{
		if (readByte == 0 || lastError == ERROR_HANDLE_EOF)
			return false;
	}
	return true;
}

bool IOCP::DispatchRead(DWORD trans)
{
	DWORD writeByte = 0;
	WriteFile(m_hWriteFile, m_ReadBuffer, trans, &writeByte, &m_hWriteOV);

	LARGE_INTEGER largeByte;
	largeByte.QuadPart = trans;

	m_hReadOV.Offset += largeByte.LowPart;
	m_hReadOV.OffsetHigh += largeByte.HighPart;
	//std::cout << m_hReadOV.OffsetHigh << "," << m_hReadOV.Offset << " 바이트 리드~" << std::endl;
	if (WaitForRead() == false)
	{
		//SetEvent(m_hEventKillThread);
		return false;
	}
	return true;
}

bool IOCP::DispatchWrite(DWORD trans)
{
	//DWORD writeByte = 0;
	//ReadFile(m_hReadFile, m_ReadBuffer, trans, &writeByte, &m_hReadOV);

	static LARGE_INTEGER largeByte = { 0, };
	largeByte.QuadPart += trans;

	m_hWriteOV.Offset = largeByte.LowPart;
	m_hWriteOV.OffsetHigh = largeByte.HighPart;
	std::cout << largeByte.QuadPart << " 바이트 라이트~(" << ((float)largeByte.QuadPart / m_fileSize.QuadPart) * 100 << "%)" <<  std::endl;
	//if (WaitForRead() == false)
	//{
	//	SetEvent(m_hEventKillThread);
		//return false;
	//}
	return true;
}

bool IOCP::Frame()
{
	WaitForRead();
	WaitForMultipleObjects(MAX_THREAD_COUNT, (HANDLE*)m_hWorkThread, true, INFINITE);
	getchar();
	return true;
}

bool IOCP::Release()
{
	SAFE_CLOSE_HANDLE(m_hReadFile);
	SAFE_CLOSE_HANDLE(m_hWriteFile);
	SAFE_CLOSE_HANDLE(m_hIOCP);
	SAFE_CLOSE_HANDLE(m_hEventKillThread);

	return true;
}