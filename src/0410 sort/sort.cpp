#include "header.h"

using namespace std;
// 머지
const int kMaxCount = 1000;
const int kMaxInt = 2147483647;

string caesar(string s, const int& n)
{
	for (auto& iter : s)
	{
		//if (iter == ' ')
		//continue;
		if (iter >= 65 && iter <= 65 + 25)
		{
			iter += n;
			while (iter > 65 + 25)	iter -= 26;
			while (iter < 65)	iter += 26;
		}
		else if (iter >= 97 && iter <= 97 + 25)
		{
			iter += n;
			while (iter > 97 + 25)	iter -= 26;
			while (iter < 97)	iter += 26;
		}
	}
	return s;
}

long long gcd(const long long& high, const long long& lower)
{
	if (lower != 0)
	{
		return gcd(lower, high % lower);
	}
	return high;
}

long long nlcm(vector<int> num)
{
	long long answer = 0;

	answer = *num.begin();
	for (auto iter = ++num.begin(); iter != num.end(); iter++)
	{
		long long high = answer;
		long long lower = *iter;

		if (answer < *iter)
		{
			lower = answer;
			high = *iter;
		}
		answer = (high * lower / gcd(high, lower));
	}

	return answer;
}

int getMinSum(vector<int> A, vector<int> B)
{
	int answer = 0;

	sort(A.begin(), A.end());
	sort(B.rbegin(), B.rend());
	
	auto& iterB = B.begin();
	for (auto& iterA = A.begin(); iterA != A.end(); iterA++, iterB++)
	{
		answer += (*iterA) * (*iterB);
	}

	return answer;
}

/*vector<vector<int> >productMatrix(vector<vector<int> >A, vector<vector<int> >B)
{
	vector<vector<int> >answer(A.size(), vector<int>(B[0].size()) ) ;

	for (auto row = 0; row < answer.size(); row++)
	{
		for (auto col = 0; col < answer[0].size(); col++)
		{
			for (auto in = 0; in < A[0].size(); in++)
			{
				answer[row][col] +=	A[row][in] * B[in][col];
			}
		}
	}

	return answer;
}*/

int expressions(int testCase)
{
	int answer = 0;

	int sum = 0;

	for (int out = 1; out <= testCase; out++)
	{
		for (int in = out; in <= testCase; in++)
		{
			sum += in;
			if (sum == testCase)
			{
				answer++;
				break;
			}
			if (sum > testCase)	break;
		}
	}

	return answer;
}

vector<int> bestSet(int no, int sum)
{
	if (no > sum)	
		return vector<int>{-1};
		
	vector<int> answer(no, sum / no);


	int total = 0;
	for (auto iter : answer)
		total += iter;
	while (total != sum)
	{
		if (total < sum)
		{
			auto& ele = min_element(answer.begin(), answer.end());
			*ele += 1 ;
		}
		else
		{
			auto& ele = max_element(answer.begin(), answer.end());
			*ele -= 1;
		}

		total = 0;
		for (auto iter : answer)
			total += iter;
	}

	sort(answer.begin(), answer.end());
	return answer;
}

int getMax(int count, ...) noexcept
{
	int max = 0;
	va_list pArgs;
	va_start(pArgs, count);

	while (count-- > 0)
	{
		const int& temp = va_arg(pArgs, int);
		if(max < temp)
			max = temp;
	}

	va_end(pArgs);
	return max;
}

int getMin(int count, ...) noexcept
{
	va_list pArgs;
	va_start(pArgs, count);

	int min = va_arg(pArgs, int);
	while (--count > 0)
	{
		const int& temp = va_arg(pArgs, int);
		if (min > temp)
			min = temp;
	}

	va_end(pArgs);
	return min;
}

int hopscotch(vector<vector<int> > board)
{
	const int& rowCount = board.size() - 1;
	for (int row = 0; row < rowCount; row++)
	{
		board[row + 1][0] += getMax(3, board[row][1], board[row][2], board[row][3]);
		board[row + 1][1] += getMax(3, board[row][0], board[row][2], board[row][3]);
		board[row + 1][2] += getMax(3, board[row][1], board[row][0], board[row][3]);
		board[row + 1][3] += getMax(3, board[row][1], board[row][2], board[row][0]);
	}
	
	return getMax(4, board[rowCount][0], board[rowCount][1], board[rowCount][2], board[rowCount][3]);
}

int chooseCity(const int& n, vector<pair<int, int>> city) noexcept
{
	int answer = 0, totalValue = 0;

	sort(city.begin(), city.end());
	for (const auto& iter : city)
		totalValue += iter.second;

	int sumValue = 0;
	for (const auto& iter : city)
	{
		sumValue += iter.second;
		if (sumValue >= totalValue / 2)
		{ 
			answer = iter.first;
			break;
		}
			
	}

	return answer;
}

int findLargestSquare(vector<vector<char> > board) noexcept
{
	int answer = 0;
	const int& rowCount = board.size();
	const int& colCount = board[0].size();

	for (int row = 0; row < rowCount; row++)
	{
		for (int col = 0; col < colCount; col++)
		{
			board[row][col] = board[row][col] == 'O' ? 1 : 0;

			if(board[row][col] != 0 && row != 0 && col != 0)
			{
				board[row][col] += getMin(3, board[row][col - 1], board[row - 1][col], board[row - 1][col - 1]);
				if (answer < board[row][col])
					answer = board[row][col];
			}
		}
	}

	return answer * answer;
}

string change124(int no)
{
	string answer = "";

	no--;
	do
	{
		int value = no % 3 - 1 + '1';
		answer.insert(answer.begin(), value);
	} while ((no = no / 3 - 1) >= 0);

	for (auto& iter : answer)
	{
		switch (iter)
		{
		case '0':
			iter = '1';
			break;
		case '1':
			iter = '2';
			break;
		case '2':
			iter = '4';
			break;
		}
	}
	return answer;
}
//
//int tiling(int n)
//{
//	if (n <= 1)
//		return 1;
//	return tiling(n - 1) + tiling(n - 2);
//}

vector<int> setAlign(int n, long long cnt)
{
	vector<int> answer;
	vector<int> tempArray;

	tempArray.resize(n);
	for (int i = 0; i < n; i++)
		tempArray[i] = i + 1;
	cnt--;

	for (int out = 1; out <= n; out++)
	{
		int facto = 1;
		for (int in = n - out; in > 1; in--)
			facto *= in;

		int input = cnt / facto;
		answer.push_back(tempArray[input]);
		tempArray.erase(tempArray.begin() + input);

		cnt = cnt % facto;
	}
	return answer;
}

//
//int change(int total, vector<int> coin)
//{
//	int answer = 0;
//	sort(coin.begin(), coin.end());
//
//	for (int i = total; total > 0; total--)
//	{
//
//	}
//
//	return answer;
//}
//
//vector<int> numberBlock(long long begin, long long end)
//{
//	vector<int> answer;
//
//	auto BlockCnt = end - begin;
//
//	for (int out = 0; out <= BlockCnt; out++)
//	{
//		/*for (int in = 10000000; in > 0; in--)
//		{
//			if (end % in == 0)
//			{
//				answer.insert(answer.begin(), in);
//				end--;
//				break;
//			}
//		}*/
//		answer.insert(answer.begin(), gcd(end--, 10000000));
//	}
//
//	return answer;
//}



void bubblesort(vector<int>& _vector) noexcept
{
	int count = _vector.size();
	bool issorting = false;

	time_t start = clock();

	for (int out = 1; out < count; ++out)
	{
		issorting = true;
		for (int in = 0; in < count - out; ++in)
		{
			if (_vector[in] > _vector[in + 1])
			{
				swap(_vector[in], _vector[in + 1]);
				issorting = false;
			}
		}
		//if (issorting)
		//{
		//	std::cout << out << "번째 반복중 정렬 완료" << endl;
		//	break;
		//}
	}

	std::cout << __func__ << " : " << (clock() - start) / 1000.0f << endl;
}

void selectsort(vector<int>& _vector) noexcept
{
	int count = _vector.size();

	time_t start = clock();

	int maxvalueindex = 0;
	for (int out = 0; out < count - 1; ++out)
	{
		maxvalueindex = 0;
		for (int in = 1; in < count - out; ++in)
		{
			if (_vector[in] > _vector[maxvalueindex])
			{
				maxvalueindex = in;
			}
		}
		swap(_vector[count - out - 1], _vector[maxvalueindex]);
	}

	std::cout << __func__ << " : " << (clock() - start) / 1000.0f << endl;
}

void insertsort(vector<int>& _vector) noexcept
{
	int count = _vector.size();
	time_t start = clock();

	int in = 0;
	for (int out = 1; out < count; ++out)
	{
		int tempvalue = _vector[out];

		for (in = out - 1; in >= 0; --in)
		{
			if (tempvalue < _vector[in])
			{
				_vector[in + 1] = std::move(_vector[in]);
			}
			else
			{
				break;
			}
		}
		_vector[in + 1] = std::move(tempvalue);
	}

	std::cout << __func__ << " : " << (clock() - start) / 1000.0f << endl;
}

void insertsort(vector<int>& _vector, const int& shellcount) noexcept
{
	int count = kMaxCount;

	int in = 0;
	for (int out = 0 + shellcount; out < count; out += shellcount)
	{
		int tempvalue = _vector[out];

		for (in = out - shellcount; in >= 0; in -= shellcount)
		{
			if (tempvalue < _vector[in])
			{
				_vector[in + shellcount] = std::move(_vector[in]);
			}
			else
			{
				break;
			}
		}
		_vector[in + shellcount] = std::move(tempvalue);
	}

}

void shellsort(vector<int>& _vector) noexcept
{
	int count = _vector.size();

	time_t start = clock();

	while (count > 1)
	{
		count = count / 3 + 1;

		insertsort(_vector, count);
	}

	std::cout << __func__ << " : " << (clock() - start) / 1000.0f << endl;
}

void quicksort(vector<int>& _vector, const int& beginstart, const int& beginend) noexcept
{
	//if (beginstart >= beginend)	return;

	time_t t_start = clock();
	int start = beginstart;
	int end = beginend;

	//int pivot = _vector[beginstart];
	int pivot = _vector[(beginstart + beginend) / 2];

	while (start <= end)
	{
		while (_vector[start] < pivot)
			++start;
		while (pivot < _vector[end])
			--end;
		if (start <= end)
			swap(_vector[start++], _vector[end--]);
	}
	//swap(_vector[beginstart], _vector[--start]);

	if (beginstart < end)
		quicksort(_vector, beginstart, end);
	if (start < beginend)
		quicksort(_vector, start, beginend);

	if (beginstart == 0 && beginend == _vector.size() - 1)
		std::cout << __func__ << " : " << (clock() - t_start) / 1000.0f << endl;
}


int fibonacciNM(int T, int n)
{
	for (int i = 0; i < n; i++)
	{

	}

}

void main()
{
	//long long begin = 99999999999990, end = 100000000000000;
	//vector<int> ret = numberBlock(begin, end);

	//int total = 5;
	//vector<int > test{ 1,2,5 };

	//// 아래는 테스트로 출력해 보기 위한 코드입니다.
	//cout << change(total, test);


	//int testn = 6;
	//long long testcnt = 5;
	//vector<int> testAnswer = setAlign(testn, testcnt);

	//for (int i = 0; i< testAnswer.size(); i++)
	//{
	//	cout << testAnswer[i] << " ";
	//}

	//int testn = 4;
	//int testAnswer = tiling(testn);
	//cout << testAnswer;


	/*int testNo = 10;
	string testAnswer = change124(testNo);
	cout << testAnswer;*/


	/*vector<vector<char>> board{
		{ 'X','O','O','O','X' },
		{ 'X','O','O','O','O' },
		{ 'X','X','O','O','O' },
		{ 'X','X','O','O','O' },
		{ 'X','X','X','X','X' } };
	cout << findLargestSquare(board);*/



	//vector<vector<int> > test{ { 1,2,3,5 },{ 5,6,7,8 },{ 4,3,2,1 } };
	//아래는 테스트로 출력해 보기 위한 코드입니다.
	//cout << hopscotch(test);

/*
	int tn = 3;
	pair<int, int> a, b, c;
	a.first = 1, a.second = 5;
	b.first = 2, b.second = 2;
	c.first = 3, c.second = 3;
	vector<pair<int, int>> tcity{ a,b,c };
	cout << chooseCity(tn, tcity);
*/

	//int n = 3, s = 13;
	//vector<int> test = bestSet(n, s);


	//vector<vector<int> >A{  { 1,4 },
	//						{ 2,5 },
	//						{ 3,6 } };
	//vector<vector<int> >B{  { 7,8,9 },
	//						{ 10,11,12 } };
	//vector<vector<int> > testAnswer = productMatrix(A, B);

	//for (int i = 0; i<testAnswer.size(); i++)
	//{
	//	for (int j = 0; j<testAnswer[i].size(); j++)
	//		cout << testAnswer[i][j] << " ";
	//	cout << "\n";
	//}


	//vector<int> test{ 2,6,8,14 };
	//cout << nlcm(test);

	//vector<int> tA{ 1,2 }, tB{ 3,4 };
	//getMinSum(tA, tB);

	//string a = caesar("a b zz", 26);



	/* 정렬~
	//srand(unsigned(time(null)));
	vector<int> array, arraycopy;
	array.reserve(kMaxCount);

	for (int i = 0; i < kMaxCount; ++i)
	{
		array.push_back(rand() % (kMaxCount * 2));
	}

	arraycopy = array;
	bubblesort(arraycopy);

	arraycopy = array;
	selectsort(arraycopy);

	arraycopy = array;
	insertsort(arraycopy);

	arraycopy = array;
	shellsort(arraycopy);


	arraycopy = array;
	quicksort(arraycopy, 0, arraycopy.size() - 1);

*/
	while (1);
}