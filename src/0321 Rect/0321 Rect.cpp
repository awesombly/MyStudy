// 0321 Rect.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include <iostream>
using namespace std;

int main()
{
	Rectangle* rect1 = new Rectangle(2, 2, 5, 5);
	Rectangle* rect2 = new Rectangle(5, 5, 10, 10);

	cout << "rect1 =\t";
	rect1->DrawRect();

	cout << "rect2 =\t";

	rect1->DrawRect(*rect1->Intersection(*rect2));
	
	cout << "Union =\t";
	rect1->DrawRect(*rect1->Union(*rect2));

/*
	Rectangle* rect3 = new Rectangle(0, 0, 0, 0);

	*rect3 = *rect1 + *rect2;
	*rect3 = *rect1 * *rect2;
	*rect3 = *rect1 + 10;
	*rect3 = *rect1 - 10;
	*rect3 = 10 + *rect1;
	*rect3 = 10 - *rect2;
*/
	while (1);
    return 0;
}

