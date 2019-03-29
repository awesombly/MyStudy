#include "header.h"

namespace Server {

	void ErrorMessage(const string_view& msg)
	{
#ifdef _DEBUG
		// ����
		static stringstream ostr;
		ostr.str("");
		ostr << endl << msg << " ";
		OutputDebugStringA(ostr.str().c_str());
#endif // DEBUG
	}


	char* WCharToChar(wchar_t* str)
	{
		//��ȯ�� char* ���� ����
		char* pStr;
		//�Է¹��� wchar_t ������ ���̸� ����
		int strSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
		//char* �޸� �Ҵ�
		pStr = new char[strSize];
		//�� ��ȯ 
		WideCharToMultiByte(CP_ACP, 0, str, -1, pStr, strSize, 0, 0);
		return pStr;
	}

	wchar_t* CharToWChar(char* str)
	{
		//wchar_t�� ���� ����
		wchar_t* pStr;
		//��Ƽ ����Ʈ ũ�� ��� ���� ��ȯ
		int strSize = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, NULL);
		//wchar_t �޸� �Ҵ�
		pStr = new WCHAR[strSize];
		//�� ��ȯ
		MultiByteToWideChar(CP_ACP, 0, str, (int)strlen(str) + 1, pStr, strSize);
		return pStr;
	}


}