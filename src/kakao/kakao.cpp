// kakao.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <cmath>
#include <algorithm>


using namespace std;

//vector<string> solution(vector<string> record)
//{
//	map<string, string> idMap;
//	vector<string> answer;
//	size_t first, second;		// 공백 위치
//	string command, userId, nickName;
//
//	for (int i = 0; i < record.size(); ++i)
//	{
//		first = record[i].find(' ');
//		command  = record[i].substr(0, first);
//		
//		second = record[i].find(' ', first + 1);
//		userId   = record[i].substr(first + 1, second - first - 1);
//
//		if (second == string::npos)
//			continue;
//		nickName = record[i].substr(second + 1);
//		idMap[userId] = nickName;
//	}
//
//	// Convert
//	for (auto&& iter = record.begin(); iter != record.end(); ++iter)
//	{
//		first = iter->find(' ');
//		command = iter->substr(0, first);
//
//		second = iter->find(' ', first + 1);
//		userId = iter->substr(first + 1, second - first - 1);
//
//		if (command == "Enter")
//		{
//			answer.emplace_back(idMap[userId] + "님이 들어왔습니다.");
//		}
//		else if (command == "Leave")
//		{
//			answer.emplace_back(idMap[userId] + "님이 나갔습니다.");
//		}
//	}
//
//	return answer;
//}








int solution(vector<int> food_times, long long k) noexcept
{
	int answer = 0;

	size_t size = food_times.size();
	size_t i = -1; 

	int cnt = k / size;
	k = k % size;
	for (i = 0; i < size; ++i)
	{
		food_times[i] -= std::min(cnt, food_times[i]);
		k -= food_times[i];
	}

	for( i = 0; i < size; ++i)
	{
		if (food_times[i] > 0)
		{
			if (--k <= 0)
				return i + 1;
		}
	}
	
	return -1;
}






int main()
{
	//vector<string> str = {"Enter uid1234 Muzi", "Enter uid4567 Prodo", "Leave uid1234", "Enter uid1234 Prodo", "Change uid4567 Ryan"};

	//auto answer = solution(str);
	//for (auto& iter : answer)
	//{
	//	std::cout << iter << endl;
	//}

	solution({ 3, 1, 2 }, 5);

    return 0;
}

