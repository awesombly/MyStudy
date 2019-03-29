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
		std::cout << "이름 : " << name << ",\t나이 : " << age << ",\t부모이름 : " << parentName << std::endl;
	}

	void setParentName(const std::string& _parentName) override
	{
		parentName = _parentName;
	}
private:
	std::string parentName;
};