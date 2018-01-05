#pragma once
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

#define BLOCK 2048
#define MAXNAME 116
#define MAXDISCNAME 60
#define UNUSED 0
#define BEGIN 1
#define USED 2
#define ENDFILE 3
struct infoBlock{
	unsigned size{};
	char name[MAXNAME];
	unsigned next{};
	int flag{};
};
struct discBlock {
	size_t blocks{};
	size_t size{};
	char discName[MAXDISCNAME];
};
class Vfs {

private:
	fstream file;
	discBlock dBlock;
	infoBlock* iTab{};
	bool opened{};
public:

	int vfsCreate(char* vfsName, size_t size)
	{
		file.open(vfsName, std::fstream::out | std::fstream::in |  std::fstream::binary | std::fstream::trunc);
		if (!file.is_open()) return -10;
		char zerobuf[BLOCK]{};
		strcpy_s(dBlock.discName, vfsName);
		dBlock.blocks = sizeToBlocks(size);
		dBlock.size = sizeToBlocks(size) * BLOCK;
		file.write((char*)&dBlock, sizeof(discBlock));
		for (unsigned i = 0; i < dBlock.blocks; ++i)
			file.write(zerobuf, sizeof(infoBlock));
		for (unsigned i = 0; i < dBlock.blocks; ++i)
			file.write(zerobuf, BLOCK);

		return 0;
	}

	int vfsClose()
	{
		if (opened)
		{
			file.seekp(sizeof(discBlock), ios::beg);
			file.write((char*)iTab, sizeof(infoBlock)*dBlock.blocks);
			delete iTab;
		}
		file.close();
		opened = false;
		return 0;
	}

	int vfsOpen(char* fileName)
	{
		file.open(fileName, std::fstream::in | std::fstream::out | std::fstream::binary);
		if (!file.is_open()) return -1;
		file.read((char*)&dBlock, sizeof(dBlock));
		iTab = new infoBlock[dBlock.blocks];
		file.read((char*)iTab, sizeof(infoBlock)*dBlock.blocks);
		if (file.fail()) return -1;
		opened = true;
		return 0;
	}

	int copyToVfs(char* sourceName, char* destinationName)
	{
		fstream source(sourceName, std::fstream::in | std::fstream::binary | std::fstream::ate);
		if (!source.is_open()) return -1;
		char buffer[2048];
		size_t sourceSize = source.tellg();
		source.seekg(0, ios::beg);
		unsigned blocksNeeded = sizeToBlocks(sourceSize);

		vector<unsigned> blockList;
		for (unsigned i = 0; i < dBlock.blocks && blockList.size() < blocksNeeded; ++i)
		{
			if (i == dBlock.blocks - 1)
				return -10; //not enough space
			if (iTab[i].flag == UNUSED)
				blockList.push_back(i);
			if (i >= dBlock.blocks - 1) return -2;
		}

		for (unsigned i = 0; i < dBlock.blocks && i < blocksNeeded; ++i)
		{
			if (i == 0)
			{
				iTab[blockList[i]].flag = BEGIN;
				strcpy_s(iTab[blockList[i]].name, destinationName);
				if (sourceSize >= BLOCK)
				{
					iTab[blockList[i]].size = BLOCK;
					iTab[blockList[i]].next = blockList[i + 1];
				}
				else  iTab[blockList[i]].size = sourceSize;
			}
			else if (i != blocksNeeded-1)
			{
				iTab[blockList[i]].next = blockList[i + 1];
				iTab[blockList[i]].flag = USED;
				iTab[blockList[i]].size = BLOCK;
			}
			else
			{
				iTab[blockList[i]].flag = ENDFILE;
				iTab[blockList[i]].size = sourceSize - (blocksNeeded - 1) * BLOCK;
			}
			source.read(buffer, iTab[blockList[i]].size);
			file.seekp(blockAdress(blockList[i]), ios::beg); 
			file.write(buffer, iTab[blockList[i]].size); //to nie dziala!!!!! dlaczego ;//
		}
		file.flush();

		return 0;
	}

	int copyFromVfs(char* sourceName, char* destinationName)
	{
		return 1;
	}

	int listFiles()
	{
		return 1;
	}

	int deleteFromVfs(char* fileName)
	{
		return 1;
	}

	int deleteVfs(char* name)
	{
		file.open(name, std::fstream::in || std::fstream::out);
		file.read((char*)&dBlock, sizeof(dBlock));
		file.close();
		if (remove(dBlock.discName)) return -1;

		return 0;
	}

	int listInfoBlocks()
	{
		return 1;
	}


	unsigned sizeToBlocks(size_t size)
	{
		if (size % BLOCK)
			return size / BLOCK + 1;
		else
			return size / BLOCK;
	}

	size_t blockAdress(size_t nr)
	{
		return sizeof(discBlock) + sizeof(infoBlock)*dBlock.blocks+ BLOCK * nr;
	}
};