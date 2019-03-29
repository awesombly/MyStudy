#pragma once
#include "header_SQL.h"




class DBMS
{
protected:
	enum {
		MAX_COLUMN = 100, MAX_BUFFER_SIZE = 255, MAX_NAME_SIZE = 50
	};
	SQLHANDLE	m_hEnv;					// 환경 핸들
	SQLHANDLE	m_hDbc;					// 연결 핸들
	SQLRETURN	m_return = SQL_SUCCESS;	// 리턴 값
	SQLHANDLE	m_hStmt;				// 명령 핸들, 직접 사용?

	SQLLEN		m_affectCnt = -1;		// 영향 받은 레코드 수
	SQLSMALLINT m_columnCnt;			// 반환 컬럼(속성) 수
	SQLTCHAR	m_columnName[MAX_COLUMN][MAX_NAME_SIZE];			// Select시 뽑을 컬럼
	SQLLEN		m_columnLength[MAX_COLUMN];							// 컬럼 길이, 상태 정보
	
	TCHAR		m_columnInfo[MAX_COLUMN][MAX_BUFFER_SIZE] { 0, };	// 바인딩될 컬럼 정보
protected:
	void ErrorMessageSQL();
	void PrintRecord();
public:
	bool Connect(const TCHAR* connectStr);
	bool Execute(const tstring_view& SQL);
public:
	DBMS();
	virtual ~DBMS();
};