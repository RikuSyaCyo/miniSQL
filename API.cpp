#include "API.h"
#include "Interpreter.h"
#include <typeinfo>
#include <string>
#include <stdlib.h>
#include "BufferPool.h"
#include "CatalogManager.h"
#include "TableManager.h"
#include "TupleManager.h"
#include "ConstantTable.h"
#include "FilePosition.h"
#include "TupleResults.h"
#include <sstream>

extern BufferPool buffer;
extern CatalogManager catalog;

void API()
{
switch(cotype)
	{
		//创建表单
		case 0	:
			if(CallCreateTable())
				cout << "Creat table successful!"  << endl;
			else 
			    cout << error_info  << endl;
			break;
			
		//删除表单		
		case 1	:
			if(CallDropTable())
				cout << "Drop table successful!"  << endl;
			else 
			    cout << error_info << endl;
			break;
			
		//创建索引
		case 2	:
			if(CallCreateIndex())
				cout << "Creat index successful!"  << endl;	
			else 
			    cout << error_info  << endl;	
			break;
		

		//删除索引
		case 3	:
			if(CallDropIndex())
				cout << "Drop index successful!"  << endl;
			else 
			    cout << error_info  << endl;	
			break;
			
		//插入一条记录
		case 4	:
			if(CallInsert())
				cout << "Insert successful!"  << endl;
			else 
			    cout << error_info  << endl;	
			break;
			
			
		//没有条件的删除纪录
		case 5	:
			if(CallDelete())
				cout << "Delete successful!"  << endl;
			else 
			    cout << error_info  << endl;
			break;
			
			
			//没有条件的选择纪录
		case 6	:
			if(CallSelect())
				cout << "Select successful!"  << endl;
			else 
			    cout << error_info  << endl;
			break;
		
		//
		default	:
			break;
	}		
	
	
}




bool CallCreateTable()
{
	if(catalog.findTable(table.tname))
	{
		error_info="Table name repeat!";
		return false;
	}
	else{
		TableManager tab = catalog.createTable(table.tname);
		for (int i = 0; i < table.anum; i++)
		{
			const _Attribute& attr=table.a[i];
			if (attr.dtype != CHAR)
			{
				tab.InsertAttribute(attr.aname,attr.dtype,attr.atype);
			}
			else
			{
				tab.InsertAttribute(attr.aname,attr.dtype,attr.alen,attr.atype);
			}
			
		}
			
	}
	return true;
}

bool CallDropTable()
{
	
	if (!catalog.findTable(tname))
	{
		error_info = "Table not exsists!";
		return false;
	}
	else{
		return catalog.deleteTable(tname);
	}
	return true;
}

bool CallCreateIndex()
{   
	if (!catalog.findTable(index.tname))
	{
		error_info = "Table not exsists!";
		return false;
	}
	TableManager tab = catalog.getTable(index.tname);
	if (tab.hasIndexOn(index.aname) == true)
	{
		error_info = "Index exsists!";
		return false;
	}
	if (tab.isUnique(tab.getAttrIndex(index.aname)) == false)
	{
		error_info = "The attribute is not unique!";
		return false;
	}
	catalog.CreateIndexOn(index.tname,index.aname,index.iname);

	return true;
}

bool CallDropIndex()
{
	if (catalog.hasIndex(iname) == false)
	{
		error_info = "Index not exsists!";
		return false;
	}
	catalog.deleteIndex(iname);
	return true;
}


bool CallInsert()
{
	if (!catalog.findTable(table.tname))
	{   error_info="The table is not existed!";
		return false;
	}
	TableManager tab = catalog.getTable(table.tname);
	if(tab.attributeCount() != vnum)
	{
		error_info="Value lack !";
		return false;
	}
	for (int k = 0; k < tab.attributeCount(); k++)
	{
		table.a[k].dtype = tab.getAttributeType(k);
	}
	table.anum = tab.attributeCount();
	//下面对values[]进行类型确认
	for(int i=0; i<table.anum; i++)
	{
	 if(table.a[i].dtype == 0){
			int flag;
			string inum ;
			 if(values[i].find('-') != -1){
            	if(values[i].find('-') != 0){
            		error_info = "Value is illegal !";
				    return false;	
	            }
                int a1 = values[i].find('-');
                string inum1 = values[i].substr(0, a1);
                string inum2 = values[i].substr(a1+1,values[i].length()-a1-1 );
                inum = inum1+inum2;
            }
            else inum = values[i];
			for(int j=0; j<inum.length(); j++){
             	flag=0;
			    for(int k=0;k<=9;k++){
			    	if((int)(inum.at(j)) == k + '0'){ 
					flag=1;
					break;
	    			}
			    }
			    if(flag==0){
				    error_info = "Value is illegal !";
				    return false;
    			    }
    			    
			}
		
		}
		string num ,fnum;
	   if(table.a[i].dtype == 1)
		{
            int flag;
            if(values[i].find('-') != -1){
            	if(values[i].find('-') != 0){
            		error_info = "Value is illegal !";
				    return false;	
	            }
                int t1 = values[i].find('-');
                string num1 = values[i].substr(0, t1);
                string num2 = values[i].substr(t1+1,values[i].length()-t1-1 );
                num = num1+num2;
               
            }
            else num = values[i] ;
            if(num.find('.') != -1){
                int t2 = num.find('.');
                string num3 = num.substr(0, t2);
                string num4 = num.substr(t2+1,values[i].length()-t2-1 );
                fnum = num3+num4;
               
            }
            else fnum = num ;
			
            for(int j=0; j<fnum.length(); j++){
             	flag=0;
			    for(int k=0;k<=9;k++){
			    	if((int)(fnum.at(j)) == k + '0'){ 
					flag=1;
					break;
	    			}
			    }
			    if(flag==0){
				    error_info = "Value is illegal !";
				    return false;
    			    }
			}
			
		}
		
		if(table.a[i].dtype == 2){
			if(values[i].find('\'') != 0){
		        error_info="Value's format is wrong, lack ' !";
		        return false;
			}
            if(values[i].rfind('\'') != (values[i].length()-1)){
		        error_info="Value's format is wrong, lack ' !";
		        return false;
			}
		    values[i]= values[i].substr(1,values[i].length()-2);
				
		}
		
	}
	

	int inf;
	TupleManager tuple = tab.CreateNewTuple();
	for (int k = 0; k < tab.attributeCount(); k++)
	{
		string str = values[k];
		int type = tab.getAttributeType(k);
		stringstream ss;
		ss << str;
		
		switch (type)
		{
		case INT:
			int number;
			ss >> number;
			inf=tuple.InsValue(k,number);
			break;
		case FLOAT:
			float number_float;
			ss >> number_float;
			inf=tuple.InsValue(k,number_float);
			break;
		case CHAR:
			inf=tuple.InsValue(k,str);
			break;
		default:
			break;
		}
		if (inf != INSERT_SUCCEED)
		{
			if (inf == INSERT_FAIL_NOTUNIQUE)
			{
				error_info = "The value is not unique.";
			}
			else
			{
				error_info = "CHAR value is out of length.";
			}
			return false;
		}
	}
	return true;
}

bool CallSelect()
{
	if (!catalog.findTable(table.tname))
	{
		error_info = "Table is not existed!";
		return false;
	}

	TableManager tab = catalog.getTable(table.tname);
	for (int k = 0; k < tab.attributeCount(); k++)
	{
		table.a[k].aname = tab.getAttriName(k);
		table.a[k].dtype = tab.getAttributeType(k);
	}
	table.anum = tab.attributeCount();
	ConditionList temp = new Condition;
	temp = conditions;
	while(temp!=NULL) {
	    int flag;
	    flag = 0;
		for(int i=0; i<table.anum; i++){
			if(table.a[i].aname == temp->aname){
					flag = 1;
					if(table.a[i].dtype == 0){
			            int flag1;
			            string inum ;
			            if(temp->value.find('-') != -1){
            	             if(temp->value.find('-') != 0){
            		         error_info = "Value is illegal !";
				             return false;	
	                         }
                             int a1 = temp->value.find('-');
                             string inum1 = temp->value.substr(0, a1);
                             string inum2 = temp->value.substr(a1+1,temp->value.length()-a1-1 );
                             inum = inum1+inum2;
                             }
                        else inum = temp->value;
		 	            for(int j=0; j<inum.length(); j++){
             	            flag1=0;
                            for(int k=0;k<=9;k++){
			    	              if((int)(inum.at(j)) == k + '0'){ 
					                  flag1=1;
					                  break;
    			                  }
			                 }
			                if(flag1==0){
				                 error_info = "Value is illegal !";
				                 return false;
  			                 }
    			    
			            }
		              temp->dtype = 0;
		            }
		            string num,fnum;
	                if(table.a[i].dtype == 1){
                         int flag1;
                         if(temp->value.find('-') != -1){
							 if (temp->value.find('-') != 0){
            		               error_info = "Value is illegal !";
				                   return false;	
	                          }
							 int t1 = temp->value.find('-');
							 string num1 = temp->value.substr(0, t1);
							 string num2 = temp->value.substr(t1 + 1, temp->value.length() - t1 - 1);
                         num = num1+num2;
                         }
                         else num = temp->dtype ;
                         if(num.find('.') != -1){
                         int t2 = num.find('.');
                         string num3 = num.substr(0, t2);
                         string num4 = num.substr(t2+1,num.length()-t2-1 );
                         fnum = num3+num4;
                        }
                         else fnum = num ;
                         for(int j=0; j<fnum.length(); j++){
             	              flag1 = 0;
			                  for(int k=0;k<=9;k++){
			    	               if((int)(fnum.at(j)) == k + '0'){ 
	                                    flag1=1;
					                    break;
    			                   }
			                  }
			                  if(flag1==0){
                                   error_info = "Value is illegal !";
				                   return false;
    			              }
			             }
			             temp->dtype = 1;
		           }
		
		           if(table.a[i].dtype == 2){
 	                    if(temp->value.find('\'') != 0){
		                      error_info="Value's format is  wrong, lack ' !";
	                          return false;
                        }
                        if(temp->value.rfind('\'') != (temp->value.length()-1)){
		                      error_info="Value's format is wrong , lack ' !";
		                      return false;
			            }
                        temp->value= temp->value.substr(1,temp->value.length()-2);
			            temp->dtype = 2;
	               }
		     }		
				
		}
		if(flag == 0){
			error_info = "Attribute not exists!";
			return false;
		}
		temp = temp->next;			
	}
	
	int cnt = 0;
	TupleResults results(tab.strName());
	if (conditions == NULL)
		results = tab.getAllTuples();
	else
		while (conditions != NULL){
			TupleResults nowResults(tab.strName());
			string str = conditions->value;
			int type = conditions->dtype;
			stringstream ss;
			ss << str;
			switch (type)
			{
			case INT:
				int number;
				ss >> number;
				nowResults = tab.selectTuples(conditions->aname, conditions->cmtype, number);
				break;
			case FLOAT:
				float number_float;
				ss >> number_float;
				nowResults = tab.selectTuples(conditions->aname, conditions->cmtype, number_float);
				break;
			case CHAR:
				nowResults = tab.selectTuples(conditions->aname, conditions->cmtype, str);
				break;
			default:
				break;
			}
			if (cnt == 0)
				results = nowResults;
			else
				results.And(nowResults);
			cnt++;
			conditions = conditions->next;
		}
	
	if (results.size() <= 100)
	{
		tab.disTableInf();
		for (int i = 0; i < results.size(); i++)
		{
			TupleManager tuple(results.tableName(), results.getTupleIndexAt(i));
			tuple.display();
		}
	}
	else
	{
		tab.disTableInf();
		for (int i = results.size()-100; i < results.size(); i++)
		{
			TupleManager tuple(results.tableName(), results.getTupleIndexAt(i));
			tuple.display();
		}
	}
	cout << results.size() << " tuples in total!" << endl;
	return true;
}

bool CallDelete()
{
	if (!catalog.findTable(table.tname))
	{
		error_info = "Table is not existed!";
		return false;
	}
	TableManager tab = catalog.getTable(table.tname);
	for (int k = 0; k < tab.attributeCount(); k++)
	{
		table.a[k].aname = tab.getAttriName(k);
		table.a[k].dtype = tab.getAttributeType(k);
	}
	table.anum = tab.attributeCount();
	ConditionList temp = new Condition;
	temp = conditions;
		while(temp!=NULL) {
	    int flag;
	    flag = 0;
		for(int i=0; i<table.anum; i++){
			if(table.a[i].aname == temp->aname){
					flag = 1;
					if(table.a[i].dtype == 0){
			            int flag1;
			            string inum ;
			            if(temp->value.find('-') != -1){
            	             if(temp->value.find('-') != 0){
            		         error_info = "Value is illegal !";
				             return false;	
	                         }
                             int a1 = temp->value.find('-');
                             string inum1 = temp->value.substr(0, a1);
                             string inum2 = temp->value.substr(a1+1,temp->value.length()-a1-1 );
                             inum = inum1+inum2;
                             }
                        else inum = temp->value;
		 	            for(int j=0; j<inum.length(); j++){
             	            flag1=0;
                            for(int k=0;k<=9;k++){
			    	              if((int)(inum.at(j)) == k + '0'){ 
					                  flag1=1;
					                  break;
    			                  }
			                 }
			                if(flag1==0){
				                 error_info = "Value is illegal !";
				                 return false;
  			                 }
    			    
			            }
		              temp->dtype = 0;
		            }
		            string num,fnum;
	                if(table.a[i].dtype == 1){
                         int flag1;
						 if (temp->value.find('-') != -1){
							 if (temp->value.find('-') != 0){
            		               error_info = "Value is illegal !";
				                   return false;	
	                          }
							 int t1 = temp->value.find('-');
							 string num1 = temp->value.substr(0, t1);
							 string num2 = temp->value.substr(t1 + 1, temp->value.length() - t1 - 1);
                         num = num1+num2;
                         }
                         else num = temp->dtype ;
                         if(num.find('.') != -1){
                         int t2 = num.find('.');
                         string num3 = num.substr(0, t2);
                         string num4 = num.substr(t2+1,num.length()-t2-1 );
                         fnum = num3 + num4;
                        }
                         else fnum = num ;
                         for(int j=0; j<fnum.length(); j++){
             	              flag1 = 0;
			                  for(int k=0;k<=9;k++){
			    	               if((int)(fnum.at(j)) == k + '0'){ 
	                                    flag1=1;
					                    break;
    			                   }
			                  }
			                  if(flag1==0){
                                   error_info = "Value is illegal !";
				                   return false;
    			              }
			             }
			             temp->dtype = 1;
		           }
		
		           if(table.a[i].dtype == 2){
 	                    if(temp->value.find('\'') != 0){
		                      error_info="Value's format is  wrong, lack ' !";
	                          return false;
                        }
                        if(temp->value.rfind('\'') != (temp->value.length()-1)){
		                      error_info="Value's format is wrong , lack ' !";
		                      return false;
			            }
                        temp->value= temp->value.substr(1,temp->value.length()-2);
			            temp->dtype = 2;
	               }
		     }		
				
		}
		if(flag == 0){
			error_info = "Attribute not exists!";
			return false;
		}
		temp = temp->next;			
	}
	
	int cnt = 0;
	TupleResults results(tab.strName());
	if (conditions==NULL)
		results=tab.getAllTuples();
	else
	while (conditions != NULL){
		TupleResults nowResults(tab.strName());
		string str = conditions->value;
		int type = conditions->dtype;
		stringstream ss;
		ss << str;
		switch (type)
		{
		case INT:
			int number;
			ss >> number;
			nowResults = tab.selectTuples(conditions->aname, conditions->cmtype, number);
			break;
		case FLOAT:
			float number_float;
			ss >> number_float;
			nowResults = tab.selectTuples(conditions->aname, conditions->cmtype, number_float);
			break;
		case CHAR:
			nowResults = tab.selectTuples(conditions->aname, conditions->cmtype, str);
			break;
		default:
			break;
		}
		if (cnt == 0)
			results = nowResults;
		else
			results.And(nowResults);
		cnt++;
		conditions = conditions->next;
	}

	for (int i = 0; i < results.size(); i++)
	{
		TupleManager tuple(results.tableName(), results.getTupleIndexAt(i));
		tuple.Delete();
	}
	cout << "Completely delete " << results.size() << " tuples!" << endl;
	return true;
}




	
