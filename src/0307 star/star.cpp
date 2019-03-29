#include <stdio.h>

void DrawStar(int length) {

	int spaceCnt = length / 2;
	int starCnt = 1;
	bool isEven = length % 2 == 0 ? 1 : 0;
	
	for (int i = 1; i <= length; i++)
	{
		for (int j = 1; j <= spaceCnt; j++)
		{
			if (isEven && i == 1 && j == 1)
			{
				printf(" ");
				continue;
			}
			printf("  ");
		}

		for (int k = starCnt; k >= 1; k--) {
			printf("* ");
		}
		printf("\n");

		if (i <= length / 2)
		{
			spaceCnt--;
			starCnt += 2 ;
			if (isEven && i == 1)
				starCnt--;
		}
		else
		{
			spaceCnt++;
			starCnt -= 2;
		}
	}

	return;
}


void main() {

	int length = 0;

	do
	{
		printf("별의 길이를 입력 : ");
		scanf_s("%d", &length);
	} while (length < 1);

	DrawStar(length);
	getchar();
	getchar(); getchar();
}

