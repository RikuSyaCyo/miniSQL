#ifndef	API_H
#define	API_H


#include "DataDefine.h"


//	API主程序
void API();

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


#endif
