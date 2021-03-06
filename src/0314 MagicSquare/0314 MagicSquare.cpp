// 0314 MagicSquare.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"

void SettingArray(int* pArray, int size) {
	int x = size / 2, y = 0;
	int index = 1, total = size * size;

	//memset(pArray, 0, total);

	while (index <= total)
	{
		*(pArray + (y * size + x)) = index++;

		if (y - 1 < 0 && x + 1 >= size)
		{
			y++;
			continue;
		}
		y = y - 1 <		0 ? size - 1 : y - 1;
		x = x + 1 >= size ?		   0 : x + 1;

		if (*(pArray + (y * size + x)) != 0)
		{
			y += 2;
			x--;
			if (y < 0)		y = size - 1;
			if (x >= size)	x = 0;
		}
	}
}

void PrintArray(int* pArray, int size) {
	int sum = 0;

	for (int row = 0; row < size; row++)
	{
		for (int col = 0; col < size; col++)
		{
			printf("\t %d", *(pArray + (row * size + col)));
			sum += *(pArray + (row * size + col));
		}
		printf("\t[ %d ]\n", sum);
		sum = 0;
	}
	

	for (int i = size - 1; i >= 0; i--)
	{
		sum += *(pArray + (i * size + (size - 1 - i)));
	}
	printf("[ %d ]\t", sum);
	sum = 0;

	for (int col = 0; col < size; col++)
	{
		for (int row = 0; row < size; row++)
		{
			sum += *(pArray + (row * size + col));
		}
		printf("[ %d ]\t", sum);
		sum = 0;
	}

	for (int i = 0; i < size; i++)
	{
		sum += *(pArray + (i * size + i));
	}
	printf("[ %d ]", sum);
	sum = 0;
}

int main()
{
	int size = 0;
	do
	{
		printf("크기를 입력하시오~(3이상, 홀수) : ");
		scanf_s(" %d", &size);
	} while (size < 3 || size % 2 == 0);

	// calloc(count, size);		할당과 초기화를 한번에 해쥼
	int* pArray = (int*)calloc(size * size, sizeof(int));

	SettingArray(pArray, size);
	PrintArray(pArray, size);

	free(pArray);
	getchar();	getchar();
    return 0;
}