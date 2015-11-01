#include <iostream>
#include "BufferPool.h"
using namespace std;

BufferPool buffer;

class typea
{
public:
	int a;
	double b;
	int arr[4];
	void set(int x,double y,int t1,int t2,int t3,int t4){
		a = x;
		b = y;
		arr[0] = t1;
		arr[1] = t2;
		arr[2] = t3;
		arr[3] = t4;
	}
	void show(){
		cout << a << " " << b << " " << arr[0] << " " << arr[1] << " " << arr[2] << " " << arr[3] << endl;
	}
};

class typeb
{
public:
	int a, b, c, d;
	bool flag;
	char arr[3];
	void set(int w, int x, int y, int z, bool f,char t1, char t2, char t3){
		a = w;
		b = x;
		c = y;
		d = z;
		flag = f;
		arr[0] = t1;
		arr[1] = t2;
		arr[2] = t3;
	}
	void show(){
		cout << a << " " << b << " " << c<<" "<<d<<" "<<flag<<" "<<arr[0] << " " << arr[1] << " " << arr[2]  << endl;
	}
};

class typec
{
public:
	int a, b,c;
	double fl;
	char arr[7];
	void set(int w, int x, int y, int z, double f, char t1, char t2, char t3){
		a = w;
		b = x;
		c = y;
		fl = f;
		arr[0] = t1;
		arr[1] = t2;
		arr[2] = t3;
	}
	void show(){
		cout << a << " " << b << " " << c << " " <<fl << " " << arr[0] << " " << arr[1] << " " << arr[2] << endl;
	}
};

int main()
{
	typea A1, A2,A3,A4,A5,A6;
	typeb B1, B2,B3,B4;
	typec C1,C2;
	map<FilePosition, int> blockMap;
	

	A1.set(1,10.5,-1,1000,500,-20);
	A2.set(5, -85.445, 12, 45, -1235, 34);
	A3.set(100,-2654.57,10,20,30,40);
	B1.set(1,2,3,4,0,'A','B','C');
	B2.set(-1,-3,-5,-7,1,'q','w','e');
	C1.set(11,22,33,44,55.5,'z','x','c');
	buffer.writeFileBlock(&A1,"a",baseIndex,sizeof(A1));
	buffer.readFileBlock(&A4, "a", baseIndex, sizeof(A4));

	buffer.writeFileBlock(&A2, "a", baseIndex, sizeof(A2));
	buffer.readFileBlock(&A5, "a", baseIndex, sizeof(A5));

	buffer.writeFileBlock(&A3, "a", baseIndex+2, sizeof(A3));


	buffer.writeFileBlock(&B1, "a", baseIndex, sizeof(B1));
	buffer.writeFileBlock(&B2, "b", baseIndex+1, sizeof(B2));

	buffer.writeFileBlock(&C1,"b",baseIndex+2,sizeof(C1));
	////buff.writeFileBlock();
	buffer.flushBufferPool();

	
	buffer.readFileBlock(&A6, "a", baseIndex+2, sizeof(A6));
	buffer.readFileBlock(&B3, "a", baseIndex, sizeof(B3));
	buffer.readFileBlock(&B4, "b", baseIndex + 1, sizeof(B4));
	buffer.readFileBlock(&C2,"b",baseIndex+2,sizeof(C2));

	A4.show();
	A5.show();
	A6.show();
	B3.show();
	B4.show();
	C2.show();
	//cout << sizeof(typec) << endl;
	system("pause");
	return 0;
}