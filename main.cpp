#include "LZW.h"

using namespace std;

int main()
{
int type;
char * ifile;
char * ofile;
uint8_t maxS;
ifile = new char[30];
ofile = new char[30];
cout<<"Выберите действие:"<<endl;
cout<<"1-упаковать файл"<<endl;
cout<<"2-распаковать файл"<<endl;
cin>>type;
cout<<"Введите имя файла на вход"<<endl;
cin>>ifile;
cout<<"Введите имя файла на выход"<<endl;
cin>>ofile;
if(type==1)
	{
	cout<<"Введите максимальный размер таблицы:"<<endl;
	cout<<"2^"<<endl;
	cin>>maxS;
	Packer ob;
	ob.Pack(ifile,ofile,maxS);
	cout<<"Упаковка завершена"<<endl;
	}
if(type==2)
	{
	return 1;
	}
}
