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

	std::cout << "������ ��� ��ȣ�� �Է��ϼ���. :";
	std::cin >> index;
	if (index > list.getCount() || index <= 0)
		throw exception("�ش� ��尡 �����ϴ�.");

	if (list.DeleteIndex(index))
		cout << "��带 �����Ͽ����ϴ�." << endl;
	else
		throw exception("�ش� ��尡 �����ϴ�.");
}

int DataManager::SearchName() const
{
	string name;
	do
	{
		std::cout << "�˻��� �̸��� �Է��ϼ��� : ";
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
	std::cout << "��ȣ\t�̸�\t����\t����\t����\t����\n";

	int cnt = 0;
	for (MyNode<Data>* iter = list.Begin(); iter != list.End(); iter = iter->m_pNext)
	{
		iter->m_pData->Print(++cnt);
	}

	std::cout << "\n�� " << cnt << "���� �����Ͱ� �ֽ��ϴ�.\n";
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

	fclose(pFile);			// ���� ������ ����
}
