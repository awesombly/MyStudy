// chellenge.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include <iostream>
#include <cmath>
#include <bitset>

using namespace std;

int main()
{
	int N = 3;
	int count = pow(2, N);
	for (int i = 0; i < count; ++i)
	{
		cout << (unsigned int)bitset<8>(i) << endl;
	}




	while (1);
    return 0;
}
