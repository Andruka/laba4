#include "LZW.h" 
void Packer::InitialVocabulary(uint8_t maxS){
 int MS=1<<(maxS-1);
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
int Packer::Pack(const char * ifile,const char * ofile,uint8_t maxS){
 unsigned char ch;
 int MS=1<<(maxS-1);
 short size=256;
 short temp;
 short tempSB;
 char byte=0;
 int length=0;cout<<1<<endl;
 InitialVocabulary(maxS);cout<<2<<endl;
 ifstream fin(ifile);cout<<3<<endl;
 if(!fin)return 1;
  fin.unsetf (std::ios::skipws);cout<<4<<endl;
  ofstream fout (ofile);
  fout<<"L";
  fout<<"Z";  
  fout<<"W";
  fout<<"5";
  fout<<maxS;
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
					break;
					}
				}
			continue;
			}
		}
	else
		{
		temp=htons(temp);
		for(int i=16-maxS;i<16;++i)
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
 temp=htons(temp);
 for(int i=16-maxS;i<16;++i)
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
 fout.write((char*)&byte, sizeof(char));
 fin.close();
 fout.close();
}
