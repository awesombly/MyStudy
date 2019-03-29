#include "DBMS.h"





int main()
{
	DBMS database;
	
	database.Connect(_T("Driver={SQL Server}; Server=kgca.kr; Address=192.168.0.8, 1433; Network=dbmssocn; Database=TestDB; Uid=sa; Pwd=kgca!@34"));

	database.Execute(_T("Select * From tblCigar"));
	
	//database.Execute(_T("Insert Into tblCigar(name, price) Values('단팥빵', 100)"));
	//database.Execute(_T("Insert Into tblCigar(name, price) Values('옥수수빵', 5500)"));
	
	//database.Execute(_T("Delete From tblCigar Where price <= 2000"));

	//database.Execute(_T("Update tblCigar Set price = 1111 Where name = '카스테라'"));


	// 스토어 프로시져 실행
	database.Execute(_T("{ CALL usp_Test }"));
	// 반환 및 인자 있을시 바인딩 가능
	database.Execute(_T("{ ? = CALL usp_Select(5000) }"));

	while (true);
	return 0;
}