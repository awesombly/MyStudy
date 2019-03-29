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
		std::cout << "\n0 - ���, 1 - ��ȣ �˻�, 2 - �̸� �˻�, 3 - ������ �߰�, 4 - ������ ����, 7 - ����, 8 - �ҷ�����, 9 - �ʱ�ȭ 99 - ����: ";
		std::cin >> index;

		try
		{
			switch (index)
			{
			case PRINT_DATA:
				if (manager->m_list.empty())
					throw exception("�Էµ� �����Ͱ� �����ϴ�.");

				manager->PrintAllData();
				break;
			case SEARCH_INDEX:
				if (manager->m_list.empty())
					throw exception("�Էµ� �����Ͱ� �����ϴ�.");

				do
				{
					std::cout << "�˻��� ������ ��ȣ�� �Է��ϼ��� : ";
					std::cin >> index;
				} while (index < 0 || index > (int)manager->m_list.size());
				manager->SearchIndex(index);
				
				break;
			case SEARCH_NAME:
				if (manager->m_list.empty())
					throw exception("�Էµ� �����Ͱ� �����ϴ�.");

				manager->SearchName();
				break;
			case DATA_INSERT:
					manager->m_list.emplace_back(manager->DataInput());
					std::cout << "�����Ͱ� �Է� �Ǿ����ϴ�.\n";
					break;
			case DATA_SAVE:
				manager->FileSave();
				std::cout << "�� " << manager->m_list.size() << "���� �����Ͱ� ����Ǿ����ϴ�.\n";
				break;
			case DATA_LOAD:
				manager->FileLoad();
				std::cout << "�� " << manager->m_list.size() << "���� �����͸� �ҷ��Խ��ϴ�.\n";
				break;
			case DATA_INIT:
				std::cout << "������ ������ ���� �Է��ϼ��� : ";
				std::cin >> index;
				manager->m_list.clear();
				for (int i = 0; i < index; i++)
					manager->m_list.emplace_back(*Data::Create());
				break;
			case DATA_DELETE:
				manager->DeleteData();
				break;
			case EXIT:
				std::cout << "����~";
				delete manager;
				getchar();
				return;
				break;
			default:
				std::cout << "�ٽ� ����\n";
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
