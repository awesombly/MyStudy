// 0322 parent.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"

using namespace std;


int main()
{
	parent* pParent = new child(50, "부모");
	pParent->setParentName(pParent->name);
	pParent->print();

	child* pChild = new child(20, "자식");
	pChild->setParentName(pParent->name);
	pChild->print();

	while (1);
    return 0;
}

