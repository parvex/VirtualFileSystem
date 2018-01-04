#pragma once
#include <fstream>
using namespace std;

class infoBlock{

public:

private:

};
class discBlock {
	size_t size;
	char* discName;
};
class Vfs {

public:

	int vfsCreate(std::string vfsName, size_t size)
	{
		cout << sizeof(infoBlock);
		return 0;
	}

	int vfsClose()
	{
		return 0;
	}

private:
	fstream file;
	discBlock dBlock;
	unsigned blockAmount;
	infoBlock* infoTab;
};