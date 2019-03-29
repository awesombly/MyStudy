#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>


// ����
int StringLength(char* string) {
	if (string == NULL) return 0;

	int length = 0;
	while (string[length++] != NULL);
	return length - 1;
}
// �빮��
char* StringUpper(char* string) {
	int length = StringLength(string);
	for(int i = 0; i < length; i++)
	{
		if (string[i] >= 97 && string[i] <= (97 + 26))
		{
			string[i] -= 32;
		}
	}
	return string;
}
// �ҹ���
char* StringLower(char* string) {
	int length = StringLength(string);
	for (int i = 0; i < length; i++)
	{
		if (string[i] >= 65 && string[i] <= (65 + 26))
		{
			string[i] += 32;
		}
	}
	return string;
}
// ����
char* StringCopy(char* string1, char* string2) {
	int length = StringLength(string2);
	for (int i = 0; i < length; i++)
	{
		if (string2[i] != NULL)
			string1[i] = string2[i];
	}
	string1[length] = '\0';

	return string1;
}
// ����
char* StringConcate(char* string1, char* string2) {
	char* buffer = (char*)malloc(sizeof(char) * (StringLength(string1) + StringLength(string2)));
	StringCopy(buffer, string1);
	buffer += StringLength(string1);
	StringCopy(buffer, string2);
	buffer -= StringLength(string1);

	return buffer;
}
// ���� ����
char* StringSet(char* string, char _char) {
	int length = StringLength(string);

	for (int i = 0; i < length; i++)
	{
		string[i] = _char;
	}
	return string;
}
// ��
int StringCompare(char* string1, char* string2) {
	int length = StringLength(string1);
	for (int i = 0; i < length; i++)
	{
		if (string1[i] < string2[i])
			return -1;
		if (string1[i] > string2[i])
			return 1;
	}
	return 0;
}
// n��
int StringNCompare(char* string1, char* string2, int nCount) {
	for (int i = 0; i < nCount; i++)
	{
		if (string1[i] < string2[i])
			return -1;
		if (string1[i] > string2[i])
			return 1;
	}
	return 0;
}
// ���� ã��
char* StringChar(char* string, char _char) {
	int length = StringLength(string);
	for (int i = 0; i < length; i++)
	{
		if (string[i] == _char)
		{
			return string += i;
		}
	}
	return NULL;
}
// ���ڿ� ã��
char* StringString(char* string, char* string2) {
	int length = StringLength(string);
	int str2Length = StringLength(string2) - 1;
	for (int i = 0; i < length; i++, string++)
	{
		if (!StringNCompare(string, string2, str2Length))
		{
			return string;
		}
	}
	return NULL;
}
// ��ūȭ
char* StringToken(char* string, char* oper) {
	static char* staticStr = NULL;
	if (string == NULL)
	{
		if (staticStr == NULL)
			return NULL;
		string = staticStr;
	}

	int length = StringLength(string);
	int operLength = StringLength(oper);

	for (int i = 0; i < length; i++)
	{
		for (int k = 0; k < operLength; k++)
		{
			if (string[i] == oper[k])
			{
				string[i] = '\0';
				staticStr = string + i + 1;
				return string;
			}
		}
	}
	if (string[length] == '\0')
	{
		staticStr = NULL;
		return string;
	}

	return NULL;
}



void main() {
	//char* string = (char*)malloc(sizeof(char) * 20);
	
	char string1[20] = "I am String~";
	char string2[20] = "is Second String~";
	char string3[20] = "Th-is Third Str,ing";
	char* token = NULL;

	printf(" string1 : %s,\n string2 : %s, \n string3 : %s\n\n", string1, string2, string3);
	printf("���� : %d\n", StringLength(string1));

	printf("�빮�� : %s\n", StringUpper(string1));

	printf("����(1, 2) : %s\n", StringCopy(string1, string2));

	printf("��(1, 2) : %d\n", StringCompare(string1, string2));
	printf("��(1, 3) : %d\n", StringCompare(string1, string3));

	printf("����(2, 3) : %s\n", StringConcate(string2, string3));

	printf("���ڿ� ����(n) : %s\n", StringSet(string1, 'n'));

	printf("���� ã��(3, 'r') : %s\n", StringChar(string3, 'r'));

	printf("���ڿ� ã��(3, \"is Th\") : %s\n", StringString(string3, "is Th"));

	printf("���ڿ� �и�(3, \" ,-\") : ");

	token = StringToken(string3, " ,-");
	while (token != NULL)
	{
		printf("%s   ", token);
		token = StringToken(NULL, " ,-");
	}
	printf("\n\n-end-");

	getchar();
}