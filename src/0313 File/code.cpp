#include "DataManager.h"

enum WORK {
	PRINT_DATA = 0, SEARCH_INDEX, SEARCH_NAME, DATA_INSERT, DATA_DELETE, DATA_SAVE = 7, DATA_LOAD, DATA_INIT, EXIT = 99
};




void main()
{

	DataManager* manager = new DataManager();
	srand((unsigned)time(0));

	int index = 0;
	Data* data = nullptr;
	
	while (1)
	{
		std::cout << "\n0 - ���, 1 - ��ȣ �˻�, 2 - �̸� �˻�, 3 - ������ �߰�, 4 - ������ ����, 7 - ����, 8 - �ҷ�����, 9 - �ʱ�ȭ 99 - ����: ";
		std::cin >> index;

		try
		{
			switch (index)
			{
			case PRINT_DATA:
				if (manager->list.isEmpty())
					throw exception("�Էµ� �����Ͱ� �����ϴ�.");

				manager->PrintAllData();
				break;
			case SEARCH_INDEX:
				if (manager->list.isEmpty())
					throw exception("�Էµ� �����Ͱ� �����ϴ�.");

				do
				{
					std::cout << "�˻��� ������ ��ȣ�� �Է��ϼ��� : ";
					std::cin >> index;
				} while (index < 0 || index >= manager->list.getCount());
				data = manager->list.FindIndex(index).m_pData;
				if (data != nullptr)
					data->Print(index);
				break;
			case SEARCH_NAME:
				if (manager->list.isEmpty())
					throw exception("�Էµ� �����Ͱ� �����ϴ�.");

				cout << "\n�� " << manager->SearchName() << "���� �����Ͱ� �ֽ��ϴ�.\n";
				break;
			case
				DATA_INSERT:
				manager->list.AddNode(manager->DataInput());
				std::cout << "�����Ͱ� �Է� �Ǿ����ϴ�.\n";
				break;
			case DATA_SAVE:
				manager->FileSave();
				std::cout << "�� " << manager->list.getCount() << "���� �����Ͱ� ����Ǿ����ϴ�.\n";
				break;
			case DATA_LOAD:
				manager->FileLoad();
				std::cout << "�� " << manager->list.getCount() << "���� �����͸� �ҷ��Խ��ϴ�.\n";
				break;
			case DATA_INIT:
				std::cout << "������ ������ ���� �Է��ϼ��� : ";
				std::cin >> index;
				manager->list.Clear();
				for (int i = 0; i < index; i++)
					manager->list.AddNode(Data::Create());
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
