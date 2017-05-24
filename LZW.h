#include <iostream>
#include <fstream>
#include <arpa/inet.h>
#include <cstdlib>
#include <stdint.h>
using namespace std;
class Packer{
	class Entry{
	   public:
		unsigned short parent;
		unsigned short son;
		unsigned short brother;
		unsigned char simbol;	
	};
	Entry * Vocabulary;
   public:
	void InitialVocabulary(unsigned int maxS);
	int Pack(const char * ifile,const char * ofile,unsigned int maxS);
	int Unpack(const char * ifile,const char * ofile);
};

