#pragma once
#include <iostream>
#include <string>

class human;

class parent :public human
{
public:
	parent() = default;
	virtual ~parent() {}
	void print()
	{
		std::cout << "�̸� : " << name << ",\t���� : " << age << std::endl;
	}
	virtual void setParentName(const std::string& _parentName) = 0;
private:

};