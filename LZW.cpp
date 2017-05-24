#include "LZW.h" 
void Packer::InitialVocabulary(unsigned int maxS){
 unsigned int MS=1<<maxS;
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
 unsigned short sizeItem=8;
 unsigned int size=256;
 unsigned short temp;
 unsigned short tempSB;
 char byte=0;
 int length=0;
 InitialVocabulary(maxS);
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
		if(temp && (0x8000 >> i))
		{
			byte|=(0x80 >> length);
			++length;
			}
		if(length==8)
			{
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
			++size;
			if(size==(1<<sizeItem))++sizeItem;
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
			++size;
			if(size==(1<<sizeItem))++sizeItem;
			}
		}
	fin>>ch;
	}
 if(length!=0)
	{
 	for(int i=16-sizeItem;i<16;++i)
 		{
 		if(temp && (0x8000 >> i))
			{
 			byte|=(0x80 >> length);
 			++length;
			}
       		if(length==8)
			{
			fout.write((char*)&byte, sizeof(char));
			length=0;
			byte=0;
			}
		}
	}
 fout.write((char*)&byte, sizeof(char));
 fin.close();
 fout.close();
 return 0;
 }
//int Packer::Unpack(const char * ifile,const char * ofile){
// 
// }
