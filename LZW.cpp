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
void Unpacker::InitialVocabulary(unsigned int MS){
 Vocabulary = new Entry [MS];
 for(int i=0;i<256;++i)
	{
	Vocabulary[i].simbol=i;	
	Vocabulary[i].root=i;
	Vocabulary[i].parent=0;
	
	}
 for(int i=256;i<MS;++i)
	{
	Vocabulary[i].simbol=0;	
	Vocabulary[i].root=0;
	Vocabulary[i].parent=0;
	}

}
void Unpacker::Write(unsigned short temp , ofstream & fout){
 stack <unsigned char> str;
	while(temp>255)
		{
		str.push(Vocabulary[temp].simbol);
		temp=Vocabulary[temp].parent;
		}
	str.push(Vocabulary[temp].simbol);
	while(str.size()!=0)
		{
		fout.write((char*)&str.top(), sizeof(char));
		str.pop();
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
			{
			fout.write((char*)&byte, sizeof(char));
			length=0;
			byte=0;
			}
		}
	cout<<temp<<"----"<<size<<"----"<<sizeItem<<endl;
	if(size<MS)
		{
		if(Vocabulary[temp].son==0)
			{
			Vocabulary[temp].son=size;
			Vocabulary[size].simbol=ch;
			if(size==(1<<sizeItem) && sizeItem!=maxS)++sizeItem;
			++size;
			}
		else
			{
			tempSB=Vocabulary[temp].son;
			while(Vocabulary[tempSB].brother!=0)
				{
				tempSB=Vocabulary[tempSB].brother;
				}
			Vocabulary[tempSB].brother=size;
			Vocabulary[size].simbol=ch;
			if(size==(1<<sizeItem) && sizeItem!=maxS)++sizeItem;
			++size;
			}
		}
	temp=(short)ch;
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
			{
			fout.write((char*)&byte, sizeof(char));
			length=0;
			byte=0;
			}
		}
	cout<<temp<<"----"<<size<<"----"<<sizeItem<<endl;
 if(length!=0)
 	{
 	fout.write((char*)&byte, sizeof(char));
	}
 delete [] Vocabulary;
 fin.close();
 fout.close();
 return 0;
 }

int Unpacker::Unpack(const char * ifile,const char * ofile){
 stack <unsigned char> str;
 unsigned char ch;
 unsigned int maxS;
 unsigned int MS;
 unsigned short sizeItem=8;
 unsigned int size=256;
 unsigned short temp;
 unsigned short buf=0; 
 int length=0;
 int a=0;
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
 temp=(short)ch;
 if(maxS > 8)++sizeItem;
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
	cout<<buf<<"----"<<size<<"----"<<sizeItem<<endl;
	if(size<MS)
		{
		Vocabulary[size].parent=temp;
		Vocabulary[size].simbol=Vocabulary[buf].root;
		++size;
		if(size==(1<<sizeItem) && sizeItem!=maxS) ++sizeItem;		
		}
		Write(temp , fout);
		temp=buf;
		if(fin.eof())
			{
			Write(buf , fout);
			return 0;
			}
	buf=0;
	}
 delete [] Vocabulary;
 fin.close();
 fout.close();
 }
