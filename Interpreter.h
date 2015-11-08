#ifndef _ITERPRETER_H
#define _ITERPRETER_H

#include "DataDefine.h"
#include <string>

int main();

//	从标准输入设备读取用户输入的一条命令
bool GetCommand(string &cmd);

//分离命令中的单词
string SplitValue(string cmd);

//获取命令中的第n个词语
string GetValue(string cmd, int count, char stop=' ');

int GetCompareType(string &ctype);
//	得到操作类型，并返回
int GetCommandType(string &cmd);

//	对用户输入的命令进行解析
bool AnalyzeCommand(string &cmd);

//	解析一个脚本文件里的所有命令
bool ExecFile(string &cmd);

//	退出 
void Quit();

//	对create table操作进行语法检测
bool CheckCreateTable(string &cmd);

//	对drop类型操作进行语法检测
bool CheckDropTable(string &cmd);

//	对create index操作进行语法检测
bool CheckCreateIndex(string &cmd);

//	对drop table操作进行语法检测
bool CheckDropIndex(string &cmd);

//	对insert类型操作进行语法检测
bool CheckInsert(string &cmd);

//	对delete类型操作进行语法检测
bool CheckDelete(string &cmd);

//	对select类型操作进行语法检测
bool CheckSelect(string &cmd);

	
#endif