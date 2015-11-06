#ifndef	API_H
#define	API_H


#include "DataDefine.h"


//	API主程序
void API();

void ReadTable();

void ReadIndex(string iname);

//	创建表时的内部调用
bool CallCreateTable();

//	删除表时的内部调用
bool CallDropTable();

//	创建索引时的内部调用
bool CallCreateIndex();

//	删除索引时的内部调用
bool CallDropIndex();

//	插入纪录时的内部调用
bool CallInsert();

//	删除纪录时的内部调用
bool CallDelete();

//	选择纪录时的内部调用
bool CallSelect();




/*******************************************************************************
//	下面是需要Record模块实现的功能

//	实现选择纪录的功能
bool Select(Table table, Condition_list conditions);

//	实现插入纪录的功能
bool Insert(Table table, string attr_value[]);

//	实现删除纪录的功能
bool Delete(Table table, Condition_list conditions);
********************************************************************************/

/*******************************************************************************/

#endif
