#include "LZW.h"

using namespace std;

int main(){
int type;
char * ifile;
char * ofile;
unsigned int maxS;
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
if(type==1){
    cout<<"Введите максимальный размер таблицы:"<<endl;
    cout<<"2^"<<endl;
    cin>>maxS;
    if(maxS<8 || maxS>16){
        cout<<"Ошибка!Некорректный размер словаря!"<<endl;
	return 1;
	}
    Packer ob;
    if(ob.Pack(ifile,ofile,maxS)){
	cout<<"Ошибка!"<<endl;
	return 1;
	}
    cout<<"Упаковка завершена"<<endl;
    }
if(type==2){
    Unpacker ob;
    if(ob.Unpack(ifile,ofile)){
	cout<<"Ошибка!"<<endl;
	return 1;	
	}
    cout<<"Распаковка завершена"<<endl;	
    }
}
