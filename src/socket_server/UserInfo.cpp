#include "stdafx.h"
#include "UserInfo.h"


bool operator == (const UserInfo* pUser, const UserInfo& pUser2)
{
	return pUser == &pUser2;
}