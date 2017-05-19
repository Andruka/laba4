#include "LZW.h"
Packer::Entry::Entry(){
 son=0;
 brother=0;
 simbol=0;
}
void Packer::InitialVocabulary(uint8_t maxS){
 uint8_t MS=pow(2,maxS);
 Vocabulary = new Entry [MS];
 for(int i=0;i<256;++i)
	{
	Vocabulary[i].simbol=i;
	}
}
int Packer::Pack(const char * ifile,const char * ofile,uint8_t maxS){
 unsigned char ch;
 short MS=pow(2,maxS);
 short size=256;
 short temp;
 short tempSB;
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
  fout<<maxS;
  fin>>ch;
  while(!fin.eof())
	{
	if (Vocabulary[temp].son)
		{
		tempSB=Vocabulary[temp].son;
		if(Vocabulary[tempSB].simbol==ch)
			{
			temp=Vocabulary[temp].son;
			continue;
			}
		if(Vocabulary[temp].simbol!=ch && Vocabulary[tempSB].brother!=0)
			{
			while(Vocabulary[tempSB].brother!=0)
				{
				tempSB=Vocabulary[tempSB].brother;
				if(Vocabulary[tempSB].simbol==ch)
					{
					temp=tempSB;
					break;
					}
				}
			}
		if(Vocabulary[tempSB].simbol==ch)continue;
		else
			{
			temp=htons(temp);
			for(int i=16-maxS;i<16;++i)
				{
				if(temp && (0x80 >> i))
					{
					byte|=(0x80 >> length);
					++length;
					}
				if(length==8)
					{
					fout.write((char*)&byte, sizeof(char));
					length=byte=0;
					}
				}
			if(size<MS)
				{
				temp=ntohs(temp);
				if(Vocabulary[temp].son==0)
					{
					Vocabulary[temp].son=size;
					Vocabulary[size].simbol=ch;
					temp=(short)ch;
					++size;
					}
				else
					{
					tempSB=Vocabulary[temp].brother;
					while(Vocabulary[tempSB].brother!=0)
						{
						tempSB=Vocabulary[tempSB].brother;
						}
					Vocabulary[tempSB].brother=size;
					Vocabulary[size].simbol=ch;
					temp=(short)ch;
					++size;
					}
				}
			}
		}
	}
 temp=htons(temp);
 for(int i=16-maxS;i<16;++i)
 	{
 	if(temp && (0x80 >> i))
		{
 		byte|=(0x80 >> length);
 		++length;
		}
       	if(length==8)
		{
		fout.write((char*)&byte, sizeof(char));
		length=byte=0;
		}
	}
 fout.write((char*)&byte, sizeof(char));
 fin.close();
 fout.close();
}
