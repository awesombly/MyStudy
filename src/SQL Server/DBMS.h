#pragma once
#include "header_SQL.h"




class DBMS
{
protected:
	enum {
		MAX_COLUMN = 100, MAX_BUFFER_SIZE = 255, MAX_NAME_SIZE = 50
	};
	SQLHANDLE	m_hEnv;					// ȯ�� �ڵ�
	SQLHANDLE	m_hDbc;					// ���� �ڵ�
	SQLRETURN	m_return = SQL_SUCCESS;	// ���� ��
	SQLHANDLE	m_hStmt;				// ��� �ڵ�, ���� ���?

	SQLLEN		m_affectCnt = -1;		// ���� ���� ���ڵ� ��
	SQLSMALLINT m_columnCnt;			// ��ȯ �÷�(�Ӽ�) ��
	SQLTCHAR	m_columnName[MAX_COLUMN][MAX_NAME_SIZE];			// Select�� ���� �÷�
	SQLLEN		m_columnLength[MAX_COLUMN];							// �÷� ����, ���� ����
	
	TCHAR		m_columnInfo[MAX_COLUMN][MAX_BUFFER_SIZE] { 0, };	// ���ε��� �÷� ����
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