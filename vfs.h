#pragma once
#include <fstream>
#include <iostream>
#include <iomanip> 
#include <vector>
#include <string.h>
#include <stdlib.h>
using namespace std;

#define BLOCK 2048
#define MAXNAME 112
#define MAXDISCNAME 60
#define UNUSED 0
#define BEGIN 1
#define USED 2
#define ENDFILE 3
struct InfoBlock{
	unsigned size{};
	unsigned fileSize{};
	char name[MAXNAME]{};
	int next{};
	int flag{};
};
struct DiscBlock {
	size_t blocks{};
	size_t size{};
	char discName[MAXDISCNAME];
};
class Vfs {

private:
	fstream file;
	DiscBlock dBlock;
	InfoBlock* iTab{};
	bool opened{};
public:

	int vfsCreate(char* vfsName, size_t size)
	{
		file.open(vfsName, std::fstream::out | std::fstream::in |  std::fstream::binary | std::fstream::trunc);
		if (!file.is_open()) return -10;
		char zerobuf[BLOCK]{};
		strcpy(dBlock.discName, vfsName);
		dBlock.blocks = sizeToBlocks(size);
		dBlock.size = sizeToBlocks(size) * BLOCK;
		file.write((char*)&dBlock, sizeof(DiscBlock));
		for (unsigned i = 0; i < dBlock.blocks; ++i)
			file.write(zerobuf, sizeof(InfoBlock));
		for (unsigned i = 0; i < dBlock.blocks; ++i)
			file.write(zerobuf, BLOCK);

		return 0;
	}

	int vfsClose()
	{
		if (opened)
		{
			file.seekp(sizeof(DiscBlock), ios::beg);
			file.write((char*)iTab, sizeof(InfoBlock)*dBlock.blocks);
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
		iTab = new InfoBlock[dBlock.blocks];
		file.read((char*)iTab, sizeof(InfoBlock)*dBlock.blocks);
		if (file.fail()) return -1;
		opened = true;
		return 0;
	}

	int copyToVfs(char* sourceName, char* destinationName)
	{
		fstream source(sourceName, std::fstream::in | std::fstream::binary | std::fstream::ate);
		if (!source.is_open()) return -1;
		char buffer[2048];
		size_t sourceSize = (size_t)source.tellg();
		source.seekg(0, ios::beg);
		unsigned blocksNeeded = sizeToBlocks(sourceSize);

		vector<unsigned> blockList;
		for (unsigned i = 0; i < dBlock.blocks && blockList.size() < blocksNeeded; ++i)
		{
			if (iTab[i].flag == UNUSED)
				blockList.push_back(i);

		}
		if(blocksNeeded > blockList.size()) return -14;
		for (unsigned i = 0; i < dBlock.blocks && i < blocksNeeded; ++i)
		{
			strcpy(iTab[blockList[i]].name, destinationName);
			if (i == 0)
			{
				iTab[blockList[i]].flag = BEGIN;
				iTab[blockList[i]].fileSize = sourceSize;

				if (sourceSize >= BLOCK)
				{
					iTab[blockList[i]].size = BLOCK;
					iTab[blockList[i]].next = blockList[i + 1];
				}
				else
				{
					iTab[blockList[i]].size = sourceSize;
					iTab[blockList[i]].next = -1;
				}
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
				iTab[blockList[i]].next = -1;
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
		fstream destFile(destinationName, std::fstream::out | std::fstream::binary | std::fstream::trunc);
		int currentBlock = -1;
		size_t currentSize;
		size_t fileSize;
		char buffer[BLOCK];
		for (size_t i = 0; i < dBlock.blocks; ++i)
			if (iTab[i].flag == BEGIN && strcmp(iTab[i].name, sourceName) == 0)
			{
				currentBlock = i;
				currentSize = iTab[i].size;
				fileSize = iTab[i].fileSize;
			}
		if(currentBlock == -1) return -1;

		while (currentBlock != -1)
		{
			file.seekp(blockAdress(currentBlock));
			file.read(buffer, iTab[currentBlock].size);
			destFile.write(buffer, iTab[currentBlock].size);



			currentBlock = iTab[currentBlock].next;
		}
		destFile.close();

		return 0;
	}

	int listFiles()
	{
		cout << "File list:\n\n";
		for (size_t i = 0; i < dBlock.blocks; ++i)
			if (iTab[i].flag == BEGIN)
			{
				cout << iTab[i].name << "  " << iTab[i].fileSize << " bytes\n";
			}
		return 0;
	}

	int deleteFromVfs(char* fileName)
	{
		int currentBlock = -1;
		size_t i = 0;
		for (; i < dBlock.blocks; ++i)
			if (iTab[i].flag == BEGIN && strcmp(iTab[i].name, fileName) == 0)
				currentBlock = i;
		int temp;
		while (currentBlock >= 0)
		{
			iTab[currentBlock].flag = UNUSED;
			temp = iTab[currentBlock].next;
			iTab[currentBlock].next = -1;
			currentBlock = temp;
		}
		return 0;
	}

	int deleteVfs(char* name)
	{
		file.open(name, std::fstream::in | std::fstream::out);
		file.read((char*)&dBlock, sizeof(dBlock));
		file.close();
		if (remove(dBlock.discName)) return -1;

		return 0;
	}

	int listInfoBlocks()
	{
		cout << "Disc map:\n\n";
		cout << "###############\n";
		size_t taken = 0;
		for (size_t i = 0; i < dBlock.blocks; ++i)
		{
			if(iTab[i].flag == USED || iTab[i].flag == BEGIN)
				taken += BLOCK;
			cout << "filename: " << iTab[i].name << endl;
			cout << "block: " << i << endl;
			cout << "adress: " << blockAdress(i) << endl;
			cout << "flag: " << iTab[i].flag << endl;
			cout << "next: " << iTab[i].next << endl;
			int flag{};
			if (iTab[i].flag == BEGIN)
			{
				cout << "filesize: " << iTab[i].fileSize << " bytes\n";
				cout << "size:" << iTab[i].size << endl;
			}
			if(iTab[i].flag == ENDFILE)
				cout << "size:" << iTab[i].size << endl;
			cout << "--------------\n";
			
		}
		cout << "###############\n";
		cout << "Disc size: " << dBlock.size << endl;
		cout << "Memory taken: " << taken << endl;
		cout << std::fixed << std::setprecision(2) << (double)taken / dBlock.size *100 << "% space is taken\n";
		return 0;
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
		return sizeof(DiscBlock) + sizeof(InfoBlock)*dBlock.blocks+ BLOCK * nr;
	}
};