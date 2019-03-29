#include "Manager.h"

enum WORK {
	PRINT_DATA = 0, SEARCH_INDEX, SEARCH_NAME, DATA_INSERT, DATA_DELETE, DATA_SAVE = 7, DATA_LOAD, DATA_INIT, EXIT = 99
};




void main()
{
	DataManager* manager = new DataManager();
	srand((unsigned)time(0));

	int index = 0;

	while (1)
	{
		std::cout << "\n0 - 출력, 1 - 번호 검색, 2 - 이름 검색, 3 - 데이터 추가, 4 - 데이터 삭제, 7 - 저장, 8 - 불러오기, 9 - 초기화 99 - 종료: ";
		std::cin >> index;

		try
		{
			switch (index)
			{
			case PRINT_DATA:
				if (manager->m_list.empty())
					throw exception("입력된 데이터가 없습니다.");

				manager->PrintAllData();
				break;
			case SEARCH_INDEX:
				if (manager->m_list.empty())
					throw exception("입력된 데이터가 없습니다.");

				do
				{
					std::cout << "검색할 데이터 번호를 입력하세요 : ";
					std::cin >> index;
				} while (index < 0 || index > (int)manager->m_list.size());
				manager->SearchIndex(index);
				
				break;
			case SEARCH_NAME:
				if (manager->m_list.empty())
					throw exception("입력된 데이터가 없습니다.");

				manager->SearchName();
				break;
			case DATA_INSERT:
					manager->m_list.emplace_back(manager->DataInput());
					std::cout << "데이터가 입력 되었습니다.\n";
					break;
			case DATA_SAVE:
				manager->FileSave();
				std::cout << "총 " << manager->m_list.size() << "건의 데이터가 저장되었습니다.\n";
				break;
			case DATA_LOAD:
				manager->FileLoad();
				std::cout << "총 " << manager->m_list.size() << "건의 데이터를 불러왔습니다.\n";
				break;
			case DATA_INIT:
				std::cout << "생성할 데이터 수를 입력하세요 : ";
				std::cin >> index;
				manager->m_list.clear();
				for (int i = 0; i < index; i++)
					manager->m_list.emplace_back(*Data::Create());
				break;
			case DATA_DELETE:
				manager->DeleteData();
				break;
			case EXIT:
				std::cout << "ㅂㅂ~";
				delete manager;
				getchar();
				return;
				break;
			default:
				std::cout << "다시 쓰셈\n";
				break;
			}
		}
		catch (exception ex)
		{
			cout << ex.what() << endl;
		}
	}
	delete manager;
	getchar();
}
