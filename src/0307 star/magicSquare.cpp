#include <cstdio>
#include <cstdlib>
/*
int mbjin[20][20];
// ũ�Ⱑ iSize�� ���������� �����.
void MagicSquare(int iSize)
{
	// Ȧ�� ������
	if (iSize % 2 == 1)
	{
		int iWidth, iHeight;
		int suja;
		// 0���� �߰��� 1�� ä���.
		int iRow = 0;
		int	iCol = (iSize - 1) / 2;
		mbjin[0][iCol] = 1;

		for (suja = 2; suja <= iSize*iSize; suja++)
		{
			// ���� ���� ���� �����̸� 
			if (iRow == 0)		iHeight = iSize - 1;		// ���� �Ʒ�������
			else				iHeight = iRow - 1;		// �ϳ� �������� �̵�

														// ��� 1
														// ���� ���� ���� ������ ���̸�
			if (iCol == iSize - 1)	iWidth = 0;				// ���� ���� ����
			else				iWidth = iCol + 1;		// �ϳ� ������ ���� �̵�

														// ��� 2
														//	// ���� ���� ���� ���� ���̸�
														//	if( iCol ==0 )		iWidth = iSize-1;	// ���� ������ ����
														//	else				iWidth = iCol-1;	// �ϳ� ���� ���� �̵�

														// �̵����� ��ġ�� ��ĭ�̸�
			if (mbjin[iHeight][iWidth] == 0)
			{
				iRow = iHeight, iCol = iWidth;			// �� ��ġ�� ���ڸ� ä�� ��ġ�� �Ѵ�.
			}
			else
			{
				iRow++;									// ��ĭ�� �ƴϸ� �Ʒ������� �̵�
			}
			mbjin[iRow][iCol] = suja;					// ���ڸ� ä���.
		}
	}
	// 4��� ������
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
					// ���ǿ� ���� �ʴ� �κ��� ������� ����
					mbjin[i][j] = numCnt;
				}
				numCnt++;
			}
		}
	}
}

// �������� ũ�⸦ �Է¹ް� �������� ����Ѵ�.
void main()
{
	int iSize;

	// ������ �Է�
	for (;;)
	{
		printf("������ ũ��( 3 ~ 19���� Ȧ��)....?");
		scanf_s("%d", &iSize);
		if (iSize >= 3 && iSize < 20)
		{
			break;
		}

	}

	// ������ ���
	MagicSquare(iSize);

	// ������ ���
	int sum = 0;

	for (int iRow = 0; iRow < iSize; iRow++)
	{
		for (int jCol = 0; jCol < iSize; jCol++)
		{
			sum += mbjin[iRow][jCol];
			printf("%4d", mbjin[iRow][jCol]);
		}

		printf("\t\t�� : %d \n", sum);
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