Packer::Packer(){
 son=0;
 brother=0;
 simbol=0;
}
void Packer::InitialVocabulary(Packer & arr[]){
 for(short i=0;i<256;++i)
	{
	cout<<i<<endl;
	arr[i].simbol=i;
	}
}
int Packer::Pack(const char * ifile,const char * ofile,uint8_t maxS,Packer & arr){
 unsigned char ch;
 short size=256;
 short temp;
 short tempSB;
 char byte=0;
 int length=0;
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
	if (arr[temp].son)
		{
		tempSB=arr[temp].son;
		if(arr[tempSB].simbol==ch)
			{
			temp=arr[tempSB];
			continue;
			}
		if(arr[temp].simbol!=ch && arr[tempSB].brother!=0)
			{
			while(arr[tempSB].brother!=0)
				{
				tempSB=arr[tempSB].brother;
				if(arr[tempSB].simbol==ch)
					{
					temp=tempSB;
					break;
					}
				}
			}
		if(arr[tempSB].simbol==ch)continue;
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
			temp=ntohs(temp);
			if(arr[temp].son==0)
				{
				arr[temp].son=size;
				arr[size].simbol=ch;
				temp=(short)ch;
				++size;
				}
			else
				{
				tempSB=arr[temp].brother;
				while(arr[tempSB].brother!=0)
					{
					tempSB=arr[tempSB].brother;
					}
				arr[tempSB].brother=size;
				arr[size].simbol=ch;
				temp=(short)ch;
				++size;
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
