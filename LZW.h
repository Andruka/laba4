#include <iostream>
#include <fstream>
#include <arpa/inet.h>
#include <cstdlib>
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
	void InitialVocabulary(int maxS);
	int Pack(const char * ifile,const char * ofile,int maxS);
	int Unpack(const char * ifile,const char * ofile);
};

