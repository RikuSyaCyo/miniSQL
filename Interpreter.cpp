#include"DataDefine.h"
#include"Interpreter.h"
#include"API.h"
#include<iostream>
#include <fstream>
#include<string>
#include "BufferPool.h"
#include "CatalogManager.h"
#include "TableManager.h"
#include "TupleManager.h"
#include "ConstantTable.h"
#include "FilePosition.h"
#include "TupleResults.h"
#define MAX_CMD_LENGTH 100
using namespace std;

BufferPool buffer;
CatalogManager catalog;
bool quit;
string error_info;					//���ڱ��������Ϣ
int cotype;                            
Table table;								//���ڴ�����ѡ���¼��ɾ����¼�������¼��
ConditionList conditions;	//���ڽ�����ʱ�򱣴�where����
Index index;								//���ڴ�������
string tname;					//����ɾ����
string iname;					//����ɾ������
string values[32];					//����insert����
int vnum;						//����insert����

int main(){
	string cmd;
	quit = false;
	//	��ϵͳ���г�ʼ��
    while(!quit){
    	//	�Ƚ������ַ�����ʼ��
		cmd = "";
		error_info = "";
		//	�ȴ��û���������
		cout << "Please input the command!"<<endl;
		if(!GetCommand(cmd))
			cout << error_info << endl;
		else
		{   //�����ʷֿ� 
		    cmd = SplitValue(cmd);
			//	��һ���û������������н���
			if(!AnalyzeCommand(cmd))
				cout << error_info << endl;
			//	�﷨��������������������⣬ͬʱ�����ڲ����ã�����ת����Ϣ���Ȩ
			else{
				API();
				cout << error_info << endl;
			}
				
		}
    }
    return 0;
		
}



bool GetCommand(string &cmd){
	string sql;
	string temp;
	bool finish = false;
	while(!finish)
	{
		cin>>sql;
		if(cmd.length() != 0)
			sql = " " + sql;
		cmd.append(sql);
		/*
		temp = sql;
		cmd = cmd + " ";
		cmd = cmd +temp;*/
		int pos = cmd.find(";");
		//	����Ѿ�������";"������Ƿ�
		if(pos != -1)
		{
			finish=true;
			if(pos != cmd.length()-1)
			{
				error_info = "Input wrong,it can't exist any words after ; !";
				return false;
			}
			//	����������һ��������򱨴�
			else if(pos == 0)
				{
					error_info = "Empty command!";
					return false;
				}
				else
					return true;
		}
		//	���û������";"������Ƿ����������������
	}
		
}

string GetValue(string cmd, int count, char stop)
{
	string temp;
	int begin = 0;
	int end = 0;
	int pos = 0;

	for(int i=0;i<cmd.length(); i++)
	{
		if(cmd.at(i) == stop)
		{
			if(cmd.at(i-1) != stop)
			{
				pos++;
				if(pos == count-1 )
                  begin = i+1;
  		    }
            else 
                 begin++;
		}
		if(pos == count)
		{
			end = i;
			break;
		}
	}

	temp.assign(cmd, begin, end - begin);
	return temp;
}

string SplitValue(string cmd)
{
	char temp[MAX_CMD_LENGTH];
	int count = 0;
	for(int i=0; i<cmd.length(); i++)
	{
		if(cmd.at(i) == '(' || cmd.at(i) == ')' || cmd.at(i) == ',' || cmd.at(i) == ';')
		{
			temp[count++] = ' ';
			temp[count++] = cmd.at(i);
			temp[count++] = ' ';
		}
		else
			temp[count++] = cmd.at(i);
	}
	temp[count++] = '\0';
	cmd = temp;
	
	string sql ="";
	count = 0;
	
	while(true)
	{
		string value = GetValue(cmd, ++count);
		if(value != ";")
			sql = sql+ value + " ";
		else
		{
			sql = sql + ";";
			break;
		}
	}
	cout<<"sql:"<<sql<<endl;
	return sql;
}

int GetCompareType(string &type)
{
	if(type == ">") return 0;
	else if(type == "<") return 1;
	else if(type == "=") return 2;	
	else if(type == ">=") return 3;
	else if(type == "<=") return 4;
	else if(type == "!=") return 5;
	else return 404;	
}

int GetCommandType(string &cmd){
	string op1 = GetValue(cmd, 1);
	
	if(op1 == "create"){
		string op2 = GetValue(cmd, 2);
		if(op2 == "table") return 0;
		else if(op2 == "index") return 2;
		else return 404;
	}					
	else if(op1 == "drop"){
	    string op2 = GetValue(cmd, 2);
		if(op2=="table") return 1;
		else if(op2=="index") return 3;
		else return 404;	
	}	
	else if(op1 == "insert") return 4;
	else if(op1 == "delete") return 5;
	else if(op1 == "select") return 6;
	else if(op1 == "execfile") return 7;
	else if(op1 == "quit") return 8;
	else return 404;	
}


bool AnalyzeCommand(string &cmd){
	cotype = GetCommandType(cmd);
	switch(cotype)
	{
		case 0	:
			if(!CheckCreateTable(cmd))
				return false;
			break;
			
		case 1 :
			if(!CheckDropTable(cmd))
				return false;
			break;
			
		case 2	:
			if(!CheckCreateIndex(cmd))
				return false;
			break;	
		
		case 3 :
			if(!CheckDropIndex(cmd))
				return false;
			break;
			
		case 4	:
			if(!CheckInsert(cmd))
				return false;
			break;
		
		case 5	:
			if(!CheckDelete(cmd))
				return false;
			break;
		
		case 6	:
			if(!CheckSelect(cmd))
				return false;
			break;
		
		case 7	:
			if(!ExecFile(cmd))
				return false;
			break;
		
		case 8	:
			Quit();
			break;
			
		case 404:
		    error_info = "Wrong type of operation!";
            return false;
            
		default	:
			break;
	}
	
	return true;
	
}

bool ExecFile(string &cmd){
	string sql;
	char line[MAX_CMD_LENGTH];
	string fileName = GetValue(cmd, 2);
	if(fileName == ";")
	{
		error_info ="Empty file name!";
		return false;	
	}
	
	ifstream fin;
	fin.open(fileName.c_str(), ios::in);
    	
	if(fin.is_open() == 0)
	{
		error_info = "Open error!";
		return false;
	}
	
	while(1)
	{
	bool finish = false;
	sql = "";
	string temp ;
	cout<<"succeed!"<<endl;
	while(!finish)
	{
		fin.getline(line, MAX_CMD_LENGTH);
		temp = line;
		cout<<"line:"<<temp<<endl;
		if(sql.length() != 0)
			temp = " " + temp;
		sql.append(temp);
		int pos = temp.find(";");
		//	����Ѿ�������";"������Ƿ�
		if(pos != -1)
		{
			finish=true;
			if(pos != temp.length()-1)
			{
				error_info = "Wrong command in the file ,it can't exist any words after ; !";
				return false;
			}
			//	����������һ��������򱨴�
			if(pos == 0)
			{
			     error_info = "Empty command!";
			     return false;
			}
		}
		cout<<"sql:"<<sql<<endl;
	}
		//	���û������";"������Ƿ����������������
    sql = SplitValue(sql);
	cout<<"sql:"<<sql<<endl ;	
    if(!AnalyzeCommand(sql))
		cout << error_info << endl;
	else API();
		
    if(fin.eof())
			break;	
	}
	cotype = 7;
	fin.close();
	return true;
	
}


void Quit(){
	quit=true;
}


bool CheckCreateTable(string &cmd){
	string temp;
	
	int count = 0;
	
	if(cmd.find(",") == -1)
	{
		error_info = "Format wrong!";
		return false;
	}
	
	while(true)
	{
		count ++;
		temp = GetValue(cmd, count, ',');
		if(temp.at(temp.length()-1) == ';')
			break;
	}
	
	table.anum = count-1;
	cout<<"aunm:"<<table.anum<<endl;
	
	//��ȡ����
	temp = GetValue(cmd, 3);
	if(temp == "," || temp == "(" || temp == ")" || temp == ";")
	{
		error_info = "Table name's format is wrong!";
		return false;
	}
	table.tname = temp;
	cout<<"tname:"<<table.tname<<endl;
	
	if(GetValue(cmd, 4) != "(")
	{
		error_info = "Wrong format,lack ( !";
		return false;
	}
	
	//����Ϊ��ȡ������Ϣ
	count = 4;
	for(int i=0; i<table.anum; i++)
	{
		temp = GetValue(cmd, ++count);
		if(temp == "," || temp == "(" || temp == ")" || temp == ";")
		{
			error_info = "Attribute's name format is wrong!";
			return false;
		}
		table.a[i].aname = temp;
		cout<<"aname:"<<table.a[i].aname<<endl;
		temp = GetValue(cmd, ++count);
		table.a[i].aid = i+1;
		cout<<"aid:"<<table.a[i].aid<<endl;
		//���������������Ϊchar(n)
		if(temp == "char")
		{
			table.a[i].dtype = 2;
			temp = GetValue(cmd, ++count);
			if(temp!= "(")
			{
				error_info = "Data type format is wrong,lack ( !";
				return false;
			}
			temp = GetValue(cmd, ++count);
			int len,l,flag;
		    len = 0;
            for(int k=0; k<temp.length(); k++){
             	flag=0;
			    for(int j=0;j<=9;j++){
			    	cout<<temp.at(k)<<endl;
			    	if((int)temp.at(k) == (j+'0')){ 
					l = j;
					flag = 1;
					break;
	    			}
			    }
    			if(flag == 0){
			    	error_info = "Range wrong!";
			    	return false;
			    }
			    len = len*10+l;
			    cout<<"len:"<<len<<endl;
			}
			if(len>255){
				error_info = "Range beyond!";
			    	return false;
			}
			temp = GetValue(cmd, ++count);
			if(temp!= ")")
			{
				error_info = "Data type's format is wrong,lack )!";
				return false;
			}
			
			table.a[i].alen = len;
			cout<<"a.len:"<<table.a[i].alen<<endl;
			
			/* 
			if(temp.find('(') != 4)
			{
				error_info = "Data type format is wrong5!";
				return false;
			}
			if(temp.find(')') != (temp.length()-1))
			{
				error_info = "Data type format is wrong6!";
				return false;
			}
             int t1 = temp.find('(');
             int t2 = temp.find(')');
             string num = temp.substr(t1 + 1, t2 - t1 - 1);
			 int len,l,flag;
			 len = 0;
             for(int k=0; k<num.length(); k++){
             	flag=0;
			    for(int j=0;j<=9;j++){
			    	cout<<num.at(k)<<endl;
			    	if((int)num.at(k) == (j+'0')){ 
					l = j;
					flag = 1;
					break;
	    			}
			    }
    			if(flag == 0){
			    	error_info = "Range wrong7!";
			    	return false;
			    }
			    len = len*10+l;
			    cout<<"len:"<<len<<endl;
			}
   
			if(len>32){
				error_info = "Range beyond8!";
			    	return false;
			}
			
			table.a[i].alen = len;
			cout<<"a.len:"<<table.a[i].alen<<endl;
			if(temp.find(")") == -1)
			{
				error_info = "Data type format is wrong9!";
				return false;
			}*/
		}
		
		//���������������Ϊint
		else if(temp == "int")
		{
			table.a[i].dtype = 0;
			table.a[i].alen = 1;
		}
		//���������������Ϊfloat
		else if(temp == "float")
		{
			table.a[i].dtype = 1;
			table.a[i].alen = 1;
		}
		//��������������ͳ���
		else
			{
				error_info = "Wrong data type!";
				return false;
			}
		
		temp = GetValue(cmd, ++count);
		
		//�����������Ϊunique
		if(temp == "unique")
		{
			if(GetValue(cmd, ++count) != ",")
			{
				error_info = "Format wrong,lack , !";
				return false;
			}
			table.a[i].atype = 0;
		}
		else if(temp == ",")
			table.a[i].atype = 1;
			
		//����������ͳ���
		else
			{
				error_info = "The type of attribute is wrong!" ;
				return false;
			}
	
	}
	
	//���潫Ҫ�õ�primary_key
	if(GetValue(cmd, ++count) != "primary")
	{
		error_info = "Lacking primary key!";
		return false;
	}
	if(GetValue(cmd, ++count) != "key")
	{
		error_info = "The format of primary key is wrong, lack key !";
		return false;
	}
	if(GetValue(cmd, ++count) != "(")
	{
		error_info = "The format of primary key is wrong , lack ( !";
		return false;
	}
	
	/////////////////////////////////////////
	//�����ȥprimary_key����������
	temp = GetValue(cmd, ++count);
	bool ae = false;		//����ȷ�������Ƿ���ȷ
	for(int i=0; i<table.anum; i++)
	{
		if(table.a[i].aname == temp)
		{
			table.a[i].atype = 2;
			ae = true;
			cout<<"primary:"<<table.a[i].aname<<endl;
			break;
		}
		
	}
	
	if(!ae)
	{
		error_info = "Attribute is not existed!";
		return false;
	}
	
	
	
	if(GetValue(cmd, ++count) != ")")
	{
		error_info = "The format of primary key is wrong , lack )!";
		return false;
	}
	
	//��ȡ�����б����
	if(GetValue(cmd, ++count) != ")")
	{
		error_info = "Table's format is wrong , lack )!";
		return false;
	}
	if(GetValue(cmd, ++count) != ";")
	{
		error_info = "Table's format is wrong , lack ; !";
		return false;
	}
	
	return true;
	
}


bool CheckDropTable(string &cmd){
	
	
	//��ȡҪɾ���ı���
	string temp ;
	temp = GetValue(cmd, 3);
	if(temp == ";")
	{
		error_info = "Lack table name !";
		return false;
	}
	
	if(GetValue(cmd, 4) != ";")
	{
		error_info = "Format wrong , lack ; !";
		return false;
	}
	tname = temp ;
	return true;
	
}


bool CheckCreateIndex(string &cmd){
	string temp;
	temp = GetValue(cmd, 3);
	if(temp == ";")
	{
		error_info = "Lack index name!" ;
		return false;
	}
	else index.iname = temp;
	
	if(GetValue(cmd, 4) != "on")
	{
		error_info = "Format wrong , lack on !";
		return false;
	}
	
	//�õ���Ҫ��������������Ӧ�ı���
	temp = GetValue(cmd, 5);
	if(index.iname == ";")
	{
		error_info = "Lack table name!";
		return false;
	}
	else index.tname = temp;
	
	//����õ���Ҫ��������������Ӧ��������
	if(GetValue(cmd, 6) != "(")
	{
		error_info = "Format wrong , lack ( !";
		return false;
	}
	
	temp = GetValue(cmd, 7);
	if(temp == ")")
	{
		error_info = "Lack attribute name!";
		return false;
	}
	else index.aname = temp ;
	
	if(GetValue(cmd, 8) != ")")
	{
		error_info = "Format wrong , lack ) !";
		return false;
	}
	
	if(GetValue(cmd, 9) != ";")
	{
		error_info = "Format wrong , lack ; !";
		return false;
	}
		
	return true;
	
}


bool CheckDropIndex	(string &cmd){
	string temp ;
	temp = GetValue(cmd, 3);
	if(temp == ";")
	{
		error_info = "Lack index name!";
		return false;
	}
	
	if(GetValue(cmd, 4) != ";")
	{
		error_info = "Format wrong , lack ; !";
		return false;
	}
	iname = temp;
	return true;	
}

bool CheckInsert(string &cmd){
	string temp;
	int count = 0;
	if(cmd.find(",") == -1)
	{
		error_info = "Format wrong!";
		return false;
	}
	while(1)
	{
		count ++;
		temp = GetValue(cmd, count, ',');
		if(temp.at(temp.length()-1) == ';')
			break;
	}
	vnum = count;
	cout<<"vnum:"<<vnum<<endl;
	
	if(GetValue(cmd, 2) != "into")
	{   
		error_info = "Format wrong , lack into !";
		return false;
	}
	
	//�õ���Ҫ�����¼�ı���
	temp = GetValue(cmd, 3);
	if(temp == "," || temp == "(" || temp == ")" || temp == ";")
	{
		error_info = "Table name's format is wrong!";
		return false;
	}
	table.tname = temp;
	
	if(GetValue(cmd, 4) != "values")
	{
		error_info = "Format wrong , lack values !";
		return false;
	}
	
	if(GetValue(cmd, 5) != "(")
	{
		error_info = "Format wrong , lack ( !";
		return false;
	}
	
	//����õ�value_count������
	//ȡ�õ�һ������
	temp = GetValue(cmd, 6);
	if(temp == "," || temp == "(" || temp == ")" || temp == ";")
	{
		error_info = "Value's format wrong!";
		return false;
	}
	values[0] = temp;
	
	for(int i=1; i<vnum; i++)
	{
		if(GetValue(cmd, 5+2*i) != ",")
		{
			error_info = "Wrong format , lack , !";
			return false;
		}
		temp = GetValue(cmd, 6+2*i);
		if(temp == "," || temp == "(" || temp == ")" || temp == ";")
		{
			error_info = "Value's format wrong!";
			return false;
		}
		values[i] = temp;
	}
	
	if(GetValue(cmd, 5+2*vnum) != ")")
	{
		error_info = "Wrong format , lack ) !";
		return false;
	}
	if(GetValue(cmd, 6+2*vnum) != ";")
	{
		error_info = "Wrong format , lack ; !";
		return false;
	}
	
	return true;
	
}

bool CheckDelete(string &cmd){
	
	if(GetValue(cmd, 2) != "from")
	{
		error_info = "Wrong format, lack from !";
		return false;
	}
	
	string temp = GetValue(cmd, 3);
	if(temp == ";")
	{
		error_info = "Lack table name!";
		return false;
	}
	table.tname = temp;
	
	if(GetValue(cmd, 4) == ";")
	{
		conditions = NULL;
		return true;
	}
	//�������where�Ӿ��select�����н���
	if(GetValue(cmd, 4) != "where")
	{
		error_info = "Wrong format , lack  where!";
		return false;
	}
	
	if((GetValue(cmd, 5) == ";")||(GetValue(cmd, 5) == "and"))
	{
		error_info = "Wrong format , lack and!";
		return false;
	}
	int count = 1;
	for(int i=5;;i++)
	{
		if(GetValue(cmd, i) == ";")
			break;
		if(GetValue(cmd, i) == "and")
			count++;
	}
	cout<<"count:"<<count<<endl;
	conditions = new Condition;

	conditions->next = NULL;

	temp = GetValue(cmd, 5);
	if(temp == ";" || temp == "," || temp == "(" || temp == ")"|| temp == ">"|| temp == "<"|| temp == "="|| temp == ">="|| temp == "<="|| temp == "!=" )
	{
		error_info = "Condition's format is wrong!";
		return false;
	}
    
	conditions->aname = temp;
	temp = GetValue(cmd, 6);
	if(GetCompareType(temp) == 404)
	{
		error_info = "The type of comparation is wrong!";
		return false;
	}
	conditions->cmtype = GetCompareType(temp);

	temp = GetValue(cmd, 7);
	if(temp == ";" || temp == "," || temp == "(" || temp == ")"|| temp == ">"|| temp == "<"|| temp == "="|| temp == ">="|| temp == "<="|| temp == "!=" )
	{
		error_info = "Condition's format is wrong!";
		return false;
	}

	conditions->value = temp;

	//���ֻ��һ���Ƚ�����,�����Ϸ���
	if(count == 1)
	{
		return true;
	}
	ConditionList t = new Condition;
	t = conditions;
	//��ȡʣ�༸���Ƚ�����
	for(int i=1; i<count; i++)
	{
		ConditionList c = new Condition;
		
		
		if(GetValue(cmd, 4+4*i) != "and")
		{
			error_info = "Wrong format , lack and !";
			return false;
		}
		
		temp = GetValue(cmd, 5+4*i);
		if(temp == ";" || temp == "," || temp == "(" || temp == ")"|| temp == ">"|| temp == "<"|| temp == "="|| temp == ">="|| temp == "<="|| temp == "!=" )
	   {
            error_info = "Condition's format is wrong!";
			return false;
       }

		c->aname = temp;
		temp = GetValue(cmd, 6+4*i);
		if(GetCompareType(temp) == 404)
		{
			error_info = "The type of comparation is wrong!";
		    return false;
		}
		c->cmtype = GetCompareType(temp);
		temp = GetValue(cmd, 7+4*i);
		if(temp == ";" || temp == "," || temp == "(" || temp == ")"|| temp == ">"|| temp == "<"|| temp == "="|| temp == ">="|| temp == "<="|| temp == "!=" )
	   {
            error_info = "Condition's format is wrong!";
			return false;
       }


		c->value = temp;
		c->next = NULL;
		t->next = c;
		t = c;
	}
	while(conditions!=NULL)
	{
		cout<<"aname:"<<conditions->aname<<endl;
			cout<<"value:"<<conditions->value<<endl;
				cout<<"cmtype:"<<conditions->cmtype<<endl;
				conditions = conditions->next;
		
	}
	return true;
}


bool CheckSelect(string &cmd){
	
	if(GetValue(cmd, 2) != "*")
	{
		error_info = "Wrong format , lack * !";
		return false;
	}
	if(GetValue(cmd, 3) != "from")
	{
		error_info = "Wrong format, lack from!";
		return false;
	}
	
	//ȡ��Ҫ����ѡ������ı�ı���
	string temp = GetValue(cmd, 4);
	if(temp == ";")
	{
		error_info = "Lack table name!";
		return false;
	}
	table.tname = temp;
	
	// �����û��������ѡ����ֱ������
	if(GetValue(cmd, 5) == ";")
	{
		conditions = NULL;
		return true;
	}
	
	//�������where�Ӿ��select�����н���
	if(GetValue(cmd, 5) != "where")
	{
		error_info = "Wrong format , lack where!";
		return false;
	}
	
	int count = 1;
	for(int i=6;;i++)
	{
		if(GetValue(cmd, i) == ";")
			break;
		if(GetValue(cmd, i) == "and")
			count++;
	}
	
	cout<<"count:"<<count<<endl;
	conditions = new Condition;

	conditions->next = NULL;

	temp = GetValue(cmd, 6);
	if(temp == ";" || temp == "," || temp == "(" || temp == ")"|| temp == ">"|| temp == "<"|| temp == "="|| temp == ">="|| temp == "<="|| temp == "!=" )
	{
		error_info = "Condition's format is wrong!";
		return false;
	}
    
	conditions->aname = temp;
	
	temp = GetValue(cmd, 7);
	if(GetCompareType(temp) == 404)
	{
		error_info = "The type of comparation is wrong!";
		return false;
	}
	conditions->cmtype = GetCompareType(temp);
	
	temp = GetValue(cmd, 8);
	if(temp == ";" || temp == "," || temp == "(" || temp == ")"|| temp == ">"|| temp == "<"|| temp == "="|| temp == ">="|| temp == "<="|| temp == "!=" )
	{
		error_info = "Condition's format is wrong!";
		return false;
	}
    
	conditions->value = temp;
	
	//���ֻ��һ���Ƚ�����,�����Ϸ���
	if(count == 1)
	{
		return true;
	}
	
	ConditionList t = new Condition;
	t = conditions;
	//��ȡʣ�༸���Ƚ�����
	for(int i=1; i<count; i++)
	{
		ConditionList c = new Condition;
		
		
		if(GetValue(cmd, 5+4*i) != "and")
		{
			error_info = "Wrong format!";
			return false;
		}
		
		temp = GetValue(cmd, 6+4*i);
		if(temp == ";" || temp == "," || temp == "(" || temp == ")"|| temp == ">"|| temp == "<"|| temp == "="|| temp == ">="|| temp == "<="|| temp == "!=" )
	   {
            error_info = "Condition's format is wrong!";
			return false;
       }
		c->aname = temp;
		
		temp = GetValue(cmd, 7+4*i);
		if(GetCompareType(temp) == 404)
		{
			error_info = "The type of comparation is wrong!";
		    return false;
		}
		c->cmtype = GetCompareType(temp);
		
		temp = GetValue(cmd, 8+4*i);
		if(temp == ";" || temp == "," || temp == "(" || temp == ")"|| temp == ">"|| temp == "<"|| temp == "="|| temp == ">="|| temp == "<="|| temp == "!=" )
	   {
            error_info = "Condition's format is wrong!";
			return false;
       }

		c->value = temp;
		
		c->next = NULL;
		t->next = c;
		t = c;
	}
	while(conditions!=NULL)
	{
		cout<<"aname:"<<conditions->aname<<endl;
			cout<<"value:"<<conditions->value<<endl;
				cout<<"cmtype:"<<conditions->cmtype<<endl;
				conditions = conditions->next;
		
	}
	return true;
	
}

