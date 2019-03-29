#include "DBMS.h"



DBMS::DBMS()
{
	// 환경 핸들 할당(  핸들 종류,      부모 핸들,  할당할놈)
	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv);
	// 버전 속성 설정
	SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
}


DBMS::~DBMS()
{
	// 해제
	if (m_hStmt) SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
	if (m_hDbc)	 SQLDisconnect(m_hDbc);
	if (m_hDbc)	 SQLFreeHandle(SQL_HANDLE_STMT, m_hDbc);
	if (m_hEnv)	 SQLFreeHandle(SQL_HANDLE_STMT, m_hEnv);
}


// 연결 핸들 할당, 연결 -> 명령 핸들 할당
bool DBMS::Connect(const TCHAR* connectStr)
{
	// DB Connect 핸들 할당
	SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDbc);
	
	SQLTCHAR	 inCon[255] = { 0, };
	SQLTCHAR	 outCon[255] = { 0, };
	SQLSMALLINT  outConResult = 0;
	wsprintf((TCHAR *)inCon, connectStr);

	m_return = SQLDriverConnect(m_hDbc, nullptr,
		inCon,  (SQLSMALLINT)sizeof(inCon),
		outCon, (SQLSMALLINT)sizeof(outCon), 
		&outConResult, SQL_DRIVER_NOPROMPT);

	// MDB 파일의 경로로 접속. 경로 생략시 현재 디렉토리에서 MDB 찾음.
	//ret = SQLDriverConnect(hDbc, NULL, (SQLTCHAR *)InCon, sizeof(InCon), OutCon, sizeof(OutCon), &cbOutCon, SQL_DRIVER_NOPROMPT);
	// FileDSN 접속, SQL 서버의 연결 정보를 가지는 DSN 파일의 경로로 접속. 반드시 완전 경로로.
	//wsprintf((TCHAR *)InCon, _T("FileDsn=%s"), ConStr);
	//ret = SQLDriverConnect(hDbc, NULL, (SQLTCHAR *)InCon, sizeof(InCon), OutCon, sizeof(OutCon), &cbOutCon, SQL_DRIVER_NOPROMPT);
	//SQLBrowseConnect(,);
	// DB 접속, DSN에 직접 연결.
	// DSN 방법인듯,, 잉여
	//m_return = SQLConnect(m_hDbc, 
		//(SQLTCHAR*)"Driver={SQL Server}; Server=kgca.kr; Address=192.168.0.8, 1433; Network=dbmssocn; Database=TestDB;", SQL_NTS,
		//(SQLTCHAR*)userId, SQL_NTS, (SQLTCHAR*)password, SQL_NTS);


	// 에러 체크
	if (m_return != SQL_SUCCESS && m_return != SQL_SUCCESS_WITH_INFO)
	{
		ErrorMessageSQL();
		return false;
	}
	// 명령 핸들 할당 (환경->접속->명령)
	m_return = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt);
	if (m_return != SQL_SUCCESS && m_return != SQL_SUCCESS_WITH_INFO)
		ErrorMessageSQL();

	cout << "~ DB에 연결되었습니다 ~" << endl << endl;
	return true;
}

bool DBMS::Execute(const tstring_view& SQL)
{
	// 인자 바인딩
	SWORD RetParam;
	SQLINTEGER pPersonIDs = 3;
	SQLLEN cbRetParam;
	m_return = SQLBindParameter(m_hStmt, 1, SQL_PARAM_OUTPUT, SQL_C_SSHORT,
							   SQL_INTEGER, 0, 0, &RetParam, 0, &cbRetParam);
	if (m_return != SQL_SUCCESS && m_return != SQL_SUCCESS_WITH_INFO && m_return != SQL_NO_DATA)
	{
		ErrorMessageSQL();
		cout << "잘못된 인자 입니다. -> " << SQL << endl;
		return false;
	}

	m_return = SQLBindParameter(m_hStmt, 2, SQL_PARAM_INPUT, SQL_C_LONG,
							   SQL_INTEGER, 0, 0, &pPersonIDs, 0, NULL);
	if (m_return != SQL_SUCCESS && m_return != SQL_SUCCESS_WITH_INFO && m_return != SQL_NO_DATA)
	{
		ErrorMessageSQL();
		cout << "잘못된 인자 입니다. -> " << SQL << endl;
		return false;
	}

	// Prepare - sql문 실행시 검증 및 중간과정상 비용이 많이 들어, 준비 시키고, 실행 시키도록 함
	m_return = SQLPrepare(m_hStmt, (SQLTCHAR*)SQL.data(), SQL_NTS);
	if (m_return != SQL_SUCCESS && m_return != SQL_SUCCESS_WITH_INFO && m_return != SQL_NO_DATA)
	{
		ErrorMessageSQL();
		cout << "잘못된 명령 입니다. -> " << SQL << endl;
		return false;
	}
	// 실제 실행부
	m_return = SQLExecute(m_hStmt);
	if (m_return != SQL_SUCCESS && m_return != SQL_SUCCESS_WITH_INFO && m_return != SQL_NO_DATA)
	{
		ErrorMessageSQL();
		cout << "실행에 실패했습니다. -> " << SQL << endl;
		return false;
	}
	//m_return = SQLExecDirect(m_hStmt, (SQLTCHAR*)SQL.data(), SQL_NTS);
	//if (m_return != SQL_SUCCESS && m_return != SQL_SUCCESS_WITH_INFO && m_return != SQL_NO_DATA)
	//{
//		ErrorMessageSQL();
		//cout << "잘못된 명령 입니다. -> " << SQL << endl;
		//return false;
	//}
	
	// Insert, Update, Delete시 영향 받은 레코드 개수 반환
	SQLRowCount(m_hStmt, &m_affectCnt);
	// Select시 결과 컬럼 수 반환
	SQLNumResultCols(m_hStmt, &m_columnCnt);
	// column 갯수가 0일땐 반환 값이 없으므로(Not Select), 바인딩 할 필요 X
	if (m_columnCnt == 0)
	{
		SQLCloseCursor(m_hStmt);
		SQLFreeStmt(m_hStmt, SQL_UNBIND);
		cout << m_affectCnt << "개 행이 영향을 받았습니다." << endl << endl;
		m_affectCnt = 0;
		SQLCloseCursor(m_hStmt);
		SQLFreeStmt(m_hStmt, SQL_UNBIND);
		return true;
	}
	if (m_columnCnt > MAX_COLUMN)
	{
		MessageBox(nullptr, _T("최대 컬럼 수를 초과했습니다."), __FUNCTION__, MB_OK);
		return false;
	}
	
	// ~Select문 문자 바인딩, 컬럼 번호는 1부터 시작
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
	cout << endl << "->" << count << " 개의 레코드가 검색되었습니다." << endl << endl;
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
	
	//SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, ii, SqlState, &NativeError, Msg, sizeof(Msg), &MsgLen)		// 최근 에러 출력
	// 에러 정보 반환
	while (SQLGetDiagRec(SQL_HANDLE_DBC, m_hDbc, ++index, sqlState, &nativeError,
						 msg, sizeof(msg), &msgLength) != SQL_NO_DATA)
	{
		MessageBox(nullptr, (LPCSTR)msg, (LPCSTR)sqlState, MB_OK);
	}
	//exit(0);
	//while (true);
}
