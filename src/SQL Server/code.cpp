#include "DBMS.h"





int main()
{
	DBMS database;
	
	database.Connect(_T("Driver={SQL Server}; Server=kgca.kr; Address=192.168.0.8, 1433; Network=dbmssocn; Database=TestDB; Uid=sa; Pwd=kgca!@34"));

	database.Execute(_T("Select * From tblCigar"));
	
	//database.Execute(_T("Insert Into tblCigar(name, price) Values('���ϻ�', 100)"));
	//database.Execute(_T("Insert Into tblCigar(name, price) Values('��������', 5500)"));
	
	//database.Execute(_T("Delete From tblCigar Where price <= 2000"));

	//database.Execute(_T("Update tblCigar Set price = 1111 Where name = 'ī���׶�'"));


	// ����� ���ν��� ����
	database.Execute(_T("{ CALL usp_Test }"));
	// ��ȯ �� ���� ������ ���ε� ����
	database.Execute(_T("{ ? = CALL usp_Select(5000) }"));

	while (true);
	return 0;
}