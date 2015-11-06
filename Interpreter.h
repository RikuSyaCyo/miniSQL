#ifndef _ITERPRETER_H
#define _ITERPRETER_H

#include "DataDefine.h"
#include <string>

int main();

//	�ӱ�׼�����豸��ȡ�û������һ������
bool GetCommand(string &cmd);

string GetValue(string cmd, int count, char stop=' ');

string SplitValue(string cmd);

int GetCompareType(string &ctype);
//	�õ��������ͣ�������
int GetCommandType(string &cmd);

//	���û������������н���
bool AnalyzeCommand(string &cmd);

//	����һ���ű��ļ������������
bool ExecFile(string &cmd);

//	�˳� 
void Quit();

//	��create table���������﷨���
bool CheckCreateTable(string &cmd);

//	��drop���Ͳ��������﷨���
bool CheckDropTable(string &cmd);

//	��create index���������﷨���
bool CheckCreateIndex(string &cmd);

//	��drop table���������﷨���
bool CheckDropIndex(string &cmd);

//	��insert���Ͳ��������﷨���
bool CheckInsert(string &cmd);

//	��delete���Ͳ��������﷨���
bool CheckDelete(string &cmd);

//	��select���Ͳ��������﷨���
bool CheckSelect(string &cmd);

	
#endif