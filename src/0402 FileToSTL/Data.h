#pragma once
#include "header.h"

class datadata {
public:
	char m_name[5];
	int m_age;
	int m_math;
	int m_eng;
	int m_kor;

	datadata() {};
	datadata (std::string& name, int& age, int& kor, int& eng, int& math)
	{
		memset(m_name, 0, 5);
		int i = 0;
		for (std::string::iterator itor = name.begin(); itor != name.end(); itor++)
		{
			m_name[i++] = *itor;
		}
		m_name[4] = '\0';
		
		m_age = age;
		m_math = math;
		m_eng = eng;
		m_kor = kor;
	}
};

class Data
{
public:
	std::string m_name;
	int m_age;
	int m_math;
	int m_eng;
	int m_kor;
public:
	void Input();
	bool Print(const int& index) const;
	bool Search(const std::string& str) const;
	static Data* Create();

	Data();
	Data(std::string name)
	{
		m_name = name;
		m_age = 0;
		m_kor = 0;
		m_eng = 0;
		m_math = 0;
	}
	Data(const std::string& name, const int& age, const int& kor, const int& eng, const int& math);
	~Data() = default;

	bool operator == (std::string name)
	{
		return m_name == name;
	}
	bool operator ==(Data data) const
	{
		return this->m_name == data.m_name;
	}
};


Data::Data(const std::string& name, const int& age, const int& kor, const int& eng, const int& math)
{
	m_name = name;
	m_age = age;
	m_kor = kor;
	m_eng = eng;
	m_math = math;
}

Data::Data()
{
	m_name[0] = '\0';
	m_age = 0;
	m_kor = 0;
	m_eng = 0;
	m_math = 0;
}

void Data::Input()
{
	std::cout << std::endl << "이름 입력 : ";
	std::cin >> m_name;
	std::cout << "나이 입력 : ";
	std::cin >> m_age;
	std::cout << "국어 입력 : ";
	std::cin >> m_kor;
	std::cout << "영어 입력 : ";
	std::cin >> m_eng;
	std::cout << "수학 입력 : ";
	std::cin >> m_math;
}

bool Data::Print(const int& index) const
{
	std::cout << index << "\t" << m_name << "\t" << m_age << "\t"
		<< m_kor << "\t" << m_eng << "\t" << m_math << std::endl;
	return true;
}

bool Data::Search(const std::string& str) const
{
	if (str.find(m_name) != std::string::npos)
	{
		return true;
	}

	return false;
}

Data* Data::Create()
{
	char name[4] = { (int)rand() % 26 + 65 , (int)rand() % 26 + 97 , (int)rand() % 26 + 97, '\0' };
	Data* Node = new Data(name, rand() % 20 + 10,
						  rand() % 100, rand() % 100, rand() % 100);
	return Node;
}
