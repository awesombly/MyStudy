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
		std::cout << "이름 : " << name << ",\t나이 : " << age << std::endl;
	}
	virtual void setParentName(const std::string& _parentName) = 0;
private:

};