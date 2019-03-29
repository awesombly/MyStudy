#include "DBMS.h"



DBMS::DBMS()
{
	// ȯ�� �ڵ� �Ҵ�(  �ڵ� ����,      �θ� �ڵ�,  �Ҵ��ҳ�)
	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv);
	// ���� �Ӽ� ����
	SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
}


DBMS::~DBMS()
{
	// ����
	if (m_hStmt) SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
	if (m_hDbc)	 SQLDisconnect(m_hDbc);
	if (m_hDbc)	 SQLFreeHandle(SQL_HANDLE_STMT, m_hDbc);
	if (m_hEnv)	 SQLFreeHandle(SQL_HANDLE_STMT, m_hEnv);
}


// ���� �ڵ� �Ҵ�, ���� -> ��� �ڵ� �Ҵ�
bool DBMS::Connect(const TCHAR* connectStr)
{
	// DB Connect �ڵ� �Ҵ�
	SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDbc);
	
	SQLTCHAR	 inCon[255] = { 0, };
	SQLTCHAR	 outCon[255] = { 0, };
	SQLSMALLINT  outConResult = 0;
	wsprintf((TCHAR *)inCon, connectStr);

	m_return = SQLDriverConnect(m_hDbc, nullptr,
		inCon,  (SQLSMALLINT)sizeof(inCon),
		outCon, (SQLSMALLINT)sizeof(outCon), 
		&outConResult, SQL_DRIVER_NOPROMPT);

	// MDB ������ ��η� ����. ��� ������ ���� ���丮���� MDB ã��.
	//ret = SQLDriverConnect(hDbc, NULL, (SQLTCHAR *)InCon, sizeof(InCon), OutCon, sizeof(OutCon), &cbOutCon, SQL_DRIVER_NOPROMPT);
	// FileDSN ����, SQL ������ ���� ������ ������ DSN ������ ��η� ����. �ݵ�� ���� ��η�.
	//wsprintf((TCHAR *)InCon, _T("FileDsn=%s"), ConStr);
	//ret = SQLDriverConnect(hDbc, NULL, (SQLTCHAR *)InCon, sizeof(InCon), OutCon, sizeof(OutCon), &cbOutCon, SQL_DRIVER_NOPROMPT);
	//SQLBrowseConnect(,);
	// DB ����, DSN�� ���� ����.
	// DSN ����ε�,, �׿�
	//m_return = SQLConnect(m_hDbc, 
		//(SQLTCHAR*)"Driver={SQL Server}; Server=kgca.kr; Address=192.168.0.8, 1433; Network=dbmssocn; Database=TestDB;", SQL_NTS,
		//(SQLTCHAR*)userId, SQL_NTS, (SQLTCHAR*)password, SQL_NTS);


	// ���� üũ
	if (m_return != SQL_SUCCESS && m_return != SQL_SUCCESS_WITH_INFO)
	{
		ErrorMessageSQL();
		return false;
	}
	// ��� �ڵ� �Ҵ� (ȯ��->����->���)
	m_return = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt);
	if (m_return != SQL_SUCCESS && m_return != SQL_SUCCESS_WITH_INFO)
		ErrorMessageSQL();

	cout << "~ DB�� ����Ǿ����ϴ� ~" << endl << endl;
	return true;
}

bool DBMS::Execute(const tstring_view& SQL)
{
	// ���� ���ε�
	SWORD RetParam;
	SQLINTEGER pPersonIDs = 3;
	SQLLEN cbRetParam;
	m_return = SQLBindParameter(m_hStmt, 1, SQL_PARAM_OUTPUT, SQL_C_SSHORT,
							   SQL_INTEGER, 0, 0, &RetParam, 0, &cbRetParam);
	if (m_return != SQL_SUCCESS && m_return != SQL_SUCCESS_WITH_INFO && m_return != SQL_NO_DATA)
	{
		ErrorMessageSQL();
		cout << "�߸��� ���� �Դϴ�. -> " << SQL << endl;
		return false;
	}

	m_return = SQLBindParameter(m_hStmt, 2, SQL_PARAM_INPUT, SQL_C_LONG,
							   SQL_INTEGER, 0, 0, &pPersonIDs, 0, NULL);
	if (m_return != SQL_SUCCESS && m_return != SQL_SUCCESS_WITH_INFO && m_return != SQL_NO_DATA)
	{
		ErrorMessageSQL();
		cout << "�߸��� ���� �Դϴ�. -> " << SQL << endl;
		return false;
	}

	// Prepare - sql�� ����� ���� �� �߰������� ����� ���� ���, �غ� ��Ű��, ���� ��Ű���� ��
	m_return = SQLPrepare(m_hStmt, (SQLTCHAR*)SQL.data(), SQL_NTS);
	if (m_return != SQL_SUCCESS && m_return != SQL_SUCCESS_WITH_INFO && m_return != SQL_NO_DATA)
	{
		ErrorMessageSQL();
		cout << "�߸��� ��� �Դϴ�. -> " << SQL << endl;
		return false;
	}
	// ���� �����
	m_return = SQLExecute(m_hStmt);
	if (m_return != SQL_SUCCESS && m_return != SQL_SUCCESS_WITH_INFO && m_return != SQL_NO_DATA)
	{
		ErrorMessageSQL();
		cout << "���࿡ �����߽��ϴ�. -> " << SQL << endl;
		return false;
	}
	//m_return = SQLExecDirect(m_hStmt, (SQLTCHAR*)SQL.data(), SQL_NTS);
	//if (m_return != SQL_SUCCESS && m_return != SQL_SUCCESS_WITH_INFO && m_return != SQL_NO_DATA)
	//{
//		ErrorMessageSQL();
		//cout << "�߸��� ��� �Դϴ�. -> " << SQL << endl;
		//return false;
	//}
	
	// Insert, Update, Delete�� ���� ���� ���ڵ� ���� ��ȯ
	SQLRowCount(m_hStmt, &m_affectCnt);
	// Select�� ��� �÷� �� ��ȯ
	SQLNumResultCols(m_hStmt, &m_columnCnt);
	// column ������ 0�϶� ��ȯ ���� �����Ƿ�(Not Select), ���ε� �� �ʿ� X
	if (m_columnCnt == 0)
	{
		SQLCloseCursor(m_hStmt);
		SQLFreeStmt(m_hStmt, SQL_UNBIND);
		cout << m_affectCnt << "�� ���� ������ �޾ҽ��ϴ�." << endl << endl;
		m_affectCnt = 0;
		SQLCloseCursor(m_hStmt);
		SQLFreeStmt(m_hStmt, SQL_UNBIND);
		return true;
	}
	if (m_columnCnt > MAX_COLUMN)
	{
		MessageBox(nullptr, _T("�ִ� �÷� ���� �ʰ��߽��ϴ�."), __FUNCTION__, MB_OK);
		return false;
	}
	
	// ~Select�� ���� ���ε�, �÷� ��ȣ�� 1���� ����
	for (SQLSMALLINT i = 0; i < m_columnCnt; i++)
	{
		SQLBindCol(	   m_hStmt, i + 1, SQL_C_TCHAR, m_columnInfo[i], MAX_BUFFER_SIZE, &m_columnLength[i]);
		SQLDescribeCol(m_hStmt, i + 1,				m_columnName[i], MAX_NAME_SIZE, nullptr, nullptr, nullptr, nullptr, nullptr);
	}
	
	PrintRecord();
	return true;
}

void DBMS::PrintRecord()
{
	int count = 0;
	if (m_columnCnt > 5)
		m_columnCnt = 5;
	for (int i = 0; i < m_columnCnt; i++)
	{
		cout << "[";
		cout.width(12);
		cout << m_columnName[i] << "]\t";
	}
	while (SQLFetch(m_hStmt) != SQL_NO_DATA)
	{
		cout << endl;
		for (int i = 0; i < m_columnCnt; i++)
		{
			cout.width(14);
			cout << m_columnInfo[i] << "\t";
		}
		++count;
	}
	cout << endl << "->" << count << " ���� ���ڵ尡 �˻��Ǿ����ϴ�." << endl << endl;
	m_columnCnt = 0;
	SQLCloseCursor(m_hStmt);
	SQLFreeStmt(m_hStmt, SQL_UNBIND);
}


void DBMS::ErrorMessageSQL()
{
	SQLSMALLINT index = 0;
	SQLINTEGER	nativeError = 0;
	SQLTCHAR	sqlState[6] = { 0, };
	SQLTCHAR	msg[255] = { 0, };
	SQLSMALLINT msgLength = 0;
	
	//SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, ii, SqlState, &NativeError, Msg, sizeof(Msg), &MsgLen)		// �ֱ� ���� ���
	// ���� ���� ��ȯ
	while (SQLGetDiagRec(SQL_HANDLE_DBC, m_hDbc, ++index, sqlState, &nativeError,
						 msg, sizeof(msg), &msgLength) != SQL_NO_DATA)
	{
		MessageBox(nullptr, (LPCSTR)msg, (LPCSTR)sqlState, MB_OK);
	}
	//exit(0);
	//while (true);
}
