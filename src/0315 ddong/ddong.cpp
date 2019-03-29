#include "header.h"
using namespace std;


Manager* g_manager = nullptr;

int main()
{
	srand((unsigned)time(NULL));
	g_manager = Manager::getInstance();
	
	thread thP(&Manager::Process, g_manager);
	g_manager->Draw();
	g_manager->Draw();
	g_manager->KeyCheck();
	
	while (1);
	delete g_manager;
	return 0;
}
