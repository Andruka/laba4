#include <iostream>
#include <fstream>
#include <arpa/inet.h>
#include <cstdlib>
#include <math.h>
#include <stdint.h>
using namespace std;
class Packer{
	class Entry{
	   public:
		short son;
		short brother;
		unsigned char simbol;	
	};
	Entry * Vocabulary;
   public:
	void InitialVocabulary(uint8_t maxS);
	int Pack(const char * ifile,const char * ofile,uint8_t maxS);
};
