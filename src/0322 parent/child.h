#pragma once
#include <string>

class human;
class parent;

class child final :public parent 
{
public:
	child(const int& _age, const std::string& _name)
	{
		age = _age;
		name = _name;
	}
	~child() {}

	void print()
	{
		std::cout << "�̸� : " << name << ",\t���� : " << age << ",\t�θ��̸� : " << parentName << std::endl;
	}

	void setParentName(const std::string& _parentName) override
	{
		parentName = _parentName;
	}
private:
	std::string parentName;
};