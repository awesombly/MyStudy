#pragma once
#pragma once
#include "Data.h"
#include <list>

using namespace std;

class DataManager
{
	list<Data> m_list;

public:
	DataManager() = default;
	~DataManager() = default;

	Data& DataInput();
	void DeleteData();
	void PrintAllData();
	void SearchIndex(int index);
	void SearchName() const;
	void FileSave();
	void FileLoad();
};

void DataManager::SearchIndex(int index)
{
	list<Data>::iterator itor = m_list.begin();

	advance(itor, index - 1);

	if (itor != m_list.end())
		itor->Print(index);
	else
		throw exception("�ش� ��尡 �����ϴ�.");
}

Data& DataManager::DataInput()
{
	Data* data = new Data();
	data->Input();
	return *data;
}

void DataManager::DeleteData()
{
	int index = 0;

	std::cout << "������ ��� ��ȣ�� �Է��ϼ���. :";
	std::cin >> index;
	if (index > (int)m_list.size() || index <= 0)
		throw exception("�ش� ��尡 �����ϴ�.");

	
	list<Data>::iterator itor = m_list.begin();
	//while (--index > 0)
	//	itor++;
	advance(itor, index - 1);

	if (m_list.erase(itor) != m_list.end())
		cout << "��带 �����Ͽ����ϴ�." << endl;
	else
		throw exception("�ش� ��尡 �����ϴ�.");
}

void DataManager::SearchName() const
{
	string name;
	do
	{
		std::cout << "�˻��� �̸��� �Է��ϼ��� : ";
		std::cin >> name;
	} while (name.empty());

	//int searchCnt = 0, index = 0;

	//auto itor = find(m_list.begin(), m_list.end(), );
	//m_list.begin() == name;
	auto itor = find(m_list.begin(), m_list.end(), Data(name));
	itor->Print(1);
	/*for (auto iter = m_list.begin();
		 iter != m_list.end();
		 iter++)
	{
		index++;
		if (iter->Search(name))
		{
			iter->Print(index);
			searchCnt++;
		}
	}*/
	//return searchCnt;
}

void DataManager::PrintAllData()
{
	system("cls");
	std::cout << "��ȣ\t�̸�\t����\t����\t����\t����\n";

	int cnt = 0;
	for (list<Data>::iterator iter = m_list.begin();
		 iter != m_list.end();
		 iter++)
	{
		iter->Print(++cnt);
	}

	std::cout << "\n�� " << cnt << "���� �����Ͱ� �ֽ��ϴ�.\n";
}

void DataManager::FileSave()
{
	FILE* pFile = nullptr;
	fopen_s(&pFile, "data", "w");

	for (list<Data>::iterator iter = m_list.begin();
		 iter != m_list.end();
		 iter++)
	{
		/*char name[5] = { 0 };
		int i = 0;
		for (string::iterator itor = iter->m_name.begin(); itor != iter->m_name.end(); itor++)
		{
			name[i++] = *itor;
		}
		name[4] = '\0';*/
		datadata* _data = new datadata(iter->m_name, iter->m_age, iter->m_kor, iter->m_eng, iter->m_math);
		fwrite(_data, sizeof(datadata), 1, pFile);
		//fprintf(pFile, "%5s%3d%3d%3d%3d", name, iter->m_age, iter->m_kor, iter->m_eng, iter->m_math);
		//fprintf(pFile, "%5s %d %d %d %d", "aaa", 12, 11, 11, 33);
		delete _data;
	}

	fclose(pFile);
}

void DataManager::FileLoad()
{
	FILE* pFile = nullptr;
	fopen_s(&pFile, "data", "r");

	m_list.clear();

	fseek(pFile, 0, SEEK_END);
	int cnt = ftell(pFile) / sizeof(datadata);
	fseek(pFile, 0, SEEK_SET);
	
	Data* pNode = nullptr;
	for (int i = 0; i < cnt; i++)
	{
		datadata* _data = new datadata();
		fread(_data, sizeof(datadata), 1, pFile);
		pNode = new Data(_data->m_name, _data->m_age, _data->m_kor, _data->m_eng, _data->m_math);
		

		//*pNode = *_data;
		//char name[5];
		//fscanf_s(pFile, "%5s %3d %3d %3d %3d", name, &pNode->m_age, &pNode->m_kor, &pNode->m_eng, &pNode->m_math);
		/*fread(name, sizeof(char[5]), 1, pFile);
		fread(&pNode->m_age, 3, 1, pFile);
		fread(&pNode->m_kor, 3, 1, pFile);
		fread(&pNode->m_eng, 3, 1, pFile);
		fread(&pNode->m_math, 3, 1, pFile);*/
		//name[4] = '\0';
		//pNode->m_name = name;
		m_list.emplace_back(*pNode);
		//delete _data;
	}

	fclose(pFile);			// ���� ������ ����
}
