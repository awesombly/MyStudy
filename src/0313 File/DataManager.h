#pragma once
#include "Data.h"
#include "LinkedList.h"

using namespace std;

class DataManager
{
public:
	LinkedList<Data> list;

public:
	DataManager() = default;
	~DataManager() = default;

	Data* DataInput();
	void DeleteData();
	void PrintAllData() const;
	int SearchName() const;
	void FileSave() const;
	void FileLoad();
};


Data* DataManager::DataInput()
{
	Data* data = new Data();
	data->Input();
	return data;
}

void DataManager::DeleteData()
{
	int index = 0;

	std::cout << "삭제할 노드 번호를 입력하세요. :";
	std::cin >> index;
	if (index > list.getCount() || index <= 0)
		throw exception("해당 노드가 없습니다.");

	if (list.DeleteIndex(index))
		cout << "노드를 삭제하였습니다." << endl;
	else
		throw exception("해당 노드가 없습니다.");
}

int DataManager::SearchName() const
{
	string name;
	do
	{
		std::cout << "검색할 이름을 입력하세요 : ";
		std::cin >> name;
	} while (name.empty());

	int searchCnt = 0, index = 0;
	for (auto iter = list.Begin();
		 iter != list.End();
		 iter = iter->m_pNext)
	{
		index++;
		if (iter->m_pData->Search(name))
		{
			iter->m_pData->Print(index);
			searchCnt++;
		}
	}
	return searchCnt;
}

void DataManager::PrintAllData() const
{
	system("cls");
	std::cout << "번호\t이름\t나이\t국어\t영어\t수학\n";

	int cnt = 0;
	for (MyNode<Data>* iter = list.Begin(); iter != list.End(); iter = iter->m_pNext)
	{
		iter->m_pData->Print(++cnt);
	}

	std::cout << "\n총 " << cnt << "건의 데이터가 있습니다.\n";
}

void DataManager::FileSave() const
{
	FILE* pFile = nullptr;
	fopen_s(&pFile, "data", "wb");

	for (MyNode<Data>* iter = list.Begin(); iter != list.End(); iter = iter->m_pNext)
	{
		fwrite(iter->m_pData, sizeof(Data), 1, pFile);
	}

	fclose(pFile);			
}

void DataManager::FileLoad()
{
	FILE* pFile = nullptr;
	fopen_s(&pFile, "data", "rb");

	list.Clear();

	fseek(pFile, 0, SEEK_END);
	int cnt = ftell(pFile) / sizeof(Data);
	fseek(pFile, 0, SEEK_SET);

	Data* pNode = nullptr;
	for (int i = 0; i < cnt; i++)
	{
		pNode = new Data();
		fread(pNode, sizeof(Data), 1, pFile);
		list.AddNode(pNode);
	}

	fclose(pFile);			// 파일 포인터 해제
}
