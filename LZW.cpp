#include "LZW.h" 
void Packer::InitialVocabulary(unsigned int MS){
 Vocabulary = new Entry [MS];
 for(int i=0;i<256;++i)
	{
	Vocabulary[i].simbol=i;
	Vocabulary[i].son=0;
	Vocabulary[i].brother=0;	
	}
 for(int i=256;i<MS;++i)
	{
	Vocabulary[i].son=0;
	Vocabulary[i].brother=0;
	Vocabulary[i].simbol=0;
	}
}
int Packer::Pack(const char * ifile,const char * ofile,unsigned int maxS){
 unsigned char ch;
 unsigned int MS=1<<maxS;
 unsigned int sizeItem=8;
 unsigned int size=256;
 unsigned short temp;
 unsigned short tempSB;
 char byte=0;
 int length=0;
 InitialVocabulary(MS);
 ifstream fin(ifile);
 if(!fin)return 1;
  fin.unsetf (std::ios::skipws);
  ofstream fout (ofile);
  fout<<"L";
  fout<<"Z";  
  fout<<"W";
  fout<<"5";
  fout<<(uint8_t)maxS;
  fin>>ch;
  temp=(short)ch;
  fin>>ch;
  while(!fin.eof())
	{
	if (Vocabulary[temp].son!=0)
		{
		tempSB=Vocabulary[temp].son;
		if(Vocabulary[tempSB].simbol==ch)
			{
			temp=Vocabulary[temp].son;
			fin>>ch;
			continue;
			}
		if(Vocabulary[tempSB].simbol!=ch && Vocabulary[tempSB].brother!=0)
			{
			while(Vocabulary[tempSB].brother!=0)
				{
				tempSB=Vocabulary[tempSB].brother;
				if(Vocabulary[tempSB].simbol==ch)
					{
					temp=tempSB;
					fin>>ch;
					break;
					}
				}
			if(temp==tempSB)continue;
			}
		}
	for(int i=16-sizeItem;i<16;++i)
		{
		if((temp & (0x8000 >> i))!=0)
			{
			byte|=(0x80 >> length);
			}
		++length;
		if(length==8)
			{cout<<temp<<endl;
			fout.write((char*)&byte, sizeof(char));
			length=0;
			byte=0;
			}
		}
	if(size<MS)
		{
		if(Vocabulary[temp].son==0)
			{
			Vocabulary[temp].parent=temp;
			Vocabulary[temp].son=size;
			Vocabulary[size].simbol=ch;
			temp=(short)ch;
			if(size==(1<<sizeItem))++sizeItem;
			++size;
			}
		else
			{
			tempSB=Vocabulary[temp].son;
			while(Vocabulary[tempSB].brother!=0)
				{
				tempSB=Vocabulary[tempSB].brother;
				}
			Vocabulary[tempSB].parent=temp;
			Vocabulary[tempSB].brother=size;
			Vocabulary[size].simbol=ch;
			temp=(short)ch;
			if(size==(1<<sizeItem))++sizeItem;
			++size;
			}
		}
	fin>>ch;
	}
 	for(int i=16-sizeItem;i<16;++i)
 		{
 		if((temp & (0x8000 >> i))!=0)
			{
 			byte|=(0x80 >> length);
			}
		++length;
       		if(length==8)
			{cout<<temp<<endl;
			fout.write((char*)&byte, sizeof(char));
			length=0;
			byte=0;
			}
		}
 if(length!=0)
 	{
 	fout.write((char*)&byte, sizeof(char));
	}
 delete [] Vocabulary;
 fin.close();
 fout.close();
 return 0;
 }

int Packer::Unpack(const char * ifile,const char * ofile){
 vector <unsigned char> str;
 unsigned char ch;
 unsigned char simbol;
 unsigned int maxS;
 unsigned int MS;
 unsigned short sizeItem=8;
 unsigned int size=256;
 unsigned short temp1;
 unsigned short temp2;
 unsigned short buf=0; 
 int length=0;
 ifstream fin(ifile);
 if(!fin)return 1;
 fin.unsetf (std::ios::skipws);
 ofstream fout (ofile);
 fin>>ch;
 if(ch!='L')return 1;
 fin>>ch;
 if(ch!='Z')return 1;
 fin>>ch;
 if(ch!='W')return 1;
 fin>>ch;
 if(ch!='5')return 1;
 fin>>ch;
 maxS=(unsigned int)ch;
 MS=1<<maxS;
 InitialVocabulary(MS);
 fin>>ch;
 temp1=(short)ch;
 fin.read((char*)&ch, sizeof(char));
 while(!fin.eof())
	{
	for(int i=16-sizeItem;i<16;++i)
		{
		if((ch & (0x80 >> length))!=0)
			{
			buf|=(0x8000 >> i);
			}
		++length;
       		if(length==8)
			{
			length=0;
			fin.read((char*)&ch, sizeof(char));
			}	
		}
	temp2=buf;cout<<buf<<endl;
	if(size<MS)
		{
		while(Vocabulary[temp2].parent!=0)
			{
			temp2=Vocabulary[temp2].parent;
			}
		simbol=Vocabulary[temp2].simbol;
		if(Vocabulary[temp1].son!=0)
			{
			temp2=Vocabulary[temp1].son;
			while(Vocabulary[temp2].brother!=0)
				{
				temp2=Vocabulary[temp2].brother;
				}
			Vocabulary[temp2].brother=size;
			Vocabulary[size].parent=temp1;
			Vocabulary[size].simbol=simbol;
			if(size==(1<<sizeItem)) ++sizeItem;
			++size;		
			}
		else
			{
			Vocabulary[temp1].son=size;
			Vocabulary[size].parent=temp1;
			Vocabulary[size].simbol=simbol;
			if(size==(1<<sizeItem)) ++sizeItem;
			++size;
			}
		}
	str.push_back(Vocabulary[temp1].simbol);
	while(Vocabulary[temp1].parent!=0)
		{
		temp1=Vocabulary[temp1].parent;
		str.push_back(Vocabulary[temp1].simbol);
		}
	while(str.size()!=0)
		{
		fout<<str.back();
		str.pop_back();
		}
	temp1=buf;
	buf=0;
	}
 str.push_back(Vocabulary[temp1].simbol);
 while(Vocabulary[temp1].parent!=0)
	{
	temp1=Vocabulary[temp1].parent;
	str.push_back(Vocabulary[temp1].simbol);
	}
 while(str.size()!=0)
	{
	fout<<str.back();
	str.pop_back();
	}
 delete [] Vocabulary;
 fin.close();
 fout.close();
 }
