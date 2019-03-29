#include <cstdio>
#include <cstdlib>
/*
int mbjin[20][20];
// 크기가 iSize인 마방진으로 만든다.
void MagicSquare(int iSize)
{
	// 홀수 마방진
	if (iSize % 2 == 1)
	{
		int iWidth, iHeight;
		int suja;
		// 0행의 중간에 1을 채운다.
		int iRow = 0;
		int	iCol = (iSize - 1) / 2;
		mbjin[0][iCol] = 1;

		for (suja = 2; suja <= iSize*iSize; suja++)
		{
			// 현재 행이 제일 위행이면 
			if (iRow == 0)		iHeight = iSize - 1;		// 제일 아래행으로
			else				iHeight = iRow - 1;		// 하나 위행으로 이동

														// 방법 1
														// 현재 행이 제일 오는쪽 열이면
			if (iCol == iSize - 1)	iWidth = 0;				// 제일 왼쪽 열로
			else				iWidth = iCol + 1;		// 하나 오른쪽 열로 이동

														// 방법 2
														//	// 현재 행이 제일 왼쪽 열이면
														//	if( iCol ==0 )		iWidth = iSize-1;	// 제일 오른쪽 열로
														//	else				iWidth = iCol-1;	// 하나 왼쪽 열로 이동

														// 이동후의 위치가 빈칸이면
			if (mbjin[iHeight][iWidth] == 0)
			{
				iRow = iHeight, iCol = iWidth;			// 이 위치를 숫자를 채울 위치로 한다.
			}
			else
			{
				iRow++;									// 빈칸이 아니면 아래행으로 이동
			}
			mbjin[iRow][iCol] = suja;					// 숫자를 채운다.
		}
	}
	// 4배수 마방진
	else if (iSize % 4 == 0)
	{
		int numCnt = 1;

		for(int i = 0; i < iSize; i++) 
		{
			for (int j = 0; j < iSize; j++)
			{
				if ( ((i % 4 == 1) || (i % 4 == 2)) ^ ((j % 4 == 1) || (j % 4 == 2)) )
				{
					
					mbjin[i][j] = iSize * iSize - numCnt + 1;
				}
				else
				{
					// 조건에 맞지 않는 부분은 순서대로 기입
					mbjin[i][j] = numCnt;
				}
				numCnt++;
			}
		}
	}
}

// 마방진의 크기를 입력받고 마방진을 계산한다.
void main()
{
	int iSize;

	// 마방진 입력
	for (;;)
	{
		printf("마방진 크기( 3 ~ 19사이 홀수)....?");
		scanf_s("%d", &iSize);
		if (iSize >= 3 && iSize < 20)
		{
			break;
		}

	}

	// 마방진 계산
	MagicSquare(iSize);

	// 마방진 출력
	int sum = 0;

	for (int iRow = 0; iRow < iSize; iRow++)
	{
		for (int jCol = 0; jCol < iSize; jCol++)
		{
			sum += mbjin[iRow][jCol];
			printf("%4d", mbjin[iRow][jCol]);
		}

		printf("\t\t계 : %d \n", sum);
		sum = 0;
	}

	printf("\n");
	for (int jCol = 0; jCol < iSize; jCol++)
	{
		for (int iRow = 0; iRow < iSize; iRow++)
		{
			sum += mbjin[iRow][jCol];
		}

		printf("%d \t", sum);
		sum = 0;
	}

}*/