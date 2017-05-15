#include <iostream>
#include <fstream>
#include <arpa/inet.h>
#include <cstdlib>
#include <math.h>
#include <stdint.h>
using namespace std;
class Packer{
	int son;
	int brother;
	unsigned char simbol;
   public:
	void InitialVocabulary(Packer & arr[]);
	int Pack(const char * ifile,const char * ofile,uint8_t maxS,Packer & arr);
};
