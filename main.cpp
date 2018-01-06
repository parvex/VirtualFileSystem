#include <iostream>

using namespace std;
#include "vfs.h"

int main(int argc, char ** argv)
{
	char * vfs_name;
	char * command;
	Vfs disc;
	////disc.vfsCreate("dysk", 100000);
	////disc.vfsClose();
	//disc.vfsOpen("dysk");
	//disc.deleteFromVfs("test.txt");
	//disc.listFiles();
	//disc.listInfoBlocks();
	//getchar();
	////disc.copyToVfs("plan.png", "plan.png");
	////disc.copyFromVfs("plan.png", "copiedplan.png");
	//disc.vfsClose();




	if (argc < 3)
	{
		cout << argv[0] << " <vfs name> command (...)\n";
		cout << "Availible commands: \n";
		cout << "- create <size in bytes>\n";
		cout << "- map\n";
		cout << "- list\n";
		cout << "- push <source file name> <destination file name>\n";
		cout << "- pull <source file name> <destination file name>\n";
		cout << "- remove <file name>\n";
		cout << "- delete\n";
		return 1;
	}

	vfs_name = argv[1];
	command = argv[2];

	if (strcmp("create", command) == 0)
	{
		if (argc == 4)
		{
			size_t size = atoi(argv[3]);
			if(disc.vfsCreate(vfs_name, size))
			{
				cout << "Nie udalo sie otworzyc dysku wirtualnego!\n";
				return 2;
			}
			disc.vfsClose();
		}
		else
			cout << argv[0]<< " <vfs name> create <size in bytes>" << endl;
	}
	else if (strcmp("map", command) == 0)
	{
		if (argc == 3)
		{
			if (disc.vfsOpen(argv[1]))
			{
				cout << "Nie udalo sie otworzyc dysku wirtualnego!\n";
					return 2;
			}
			disc.listInfoBlocks();
			disc.vfsClose();
		}
		else
			cout << argv[0] << " <vfs name> dump\n";
	}
	else if (strcmp("list", command) == 0)
	{
		if (argc == 3)
		{
			if (disc.vfsOpen(argv[1]))
			{
				cout << "Nie udalo sie otworzyc dysku wirtualnego!\n";
				return 2;
			}
			disc.listFiles();
			disc.vfsClose();
		}
		else
			cout << argv[0] << " <vfs name> list\n";
	}
	else if (strcmp("push", command) == 0)
	{
		if (argc == 5)
		{
			if (disc.vfsOpen(argv[1]))
			{
				cout << "Nie udalo sie otworzyc dysku wirtualnego!\n";
					return 2;
			}
			cout << "Wysylanie pliku, wynik: " << disc.copyToVfs(argv[3], argv[4]) << endl;
			disc.vfsClose();
		}
		else
			cout << argv[0] << "<vfs name> push <source file name> <destination file name>\n";
	}
	else if (strcmp("pull", command) == 0)
	{
		if (argc == 5)
		{
			if (disc.vfsOpen(argv[1]))
			{
				cout << "Nie udalo sie otworzyc dysku wirtualnego!\n";
				return 2;
			}
			cout << "Pobieranie pliku, wynik: " << disc.copyFromVfs(argv[3], argv[4]) << endl;
			disc.vfsClose();
		}
		else
			cout << argv[0] << "<vfs name> pull <source file name> <destination file name>\n";
	}
	else if (strcmp("remove", command) == 0)
	{
		if (argc == 4)
		{
			if (disc.vfsOpen(argv[1]))
			{
				cout << "Nie udalo sie otworzyc dysku wirtualnego!\n";
				return 2;
			}

			disc.deleteFromVfs(argv[3]);

			disc.vfsClose();
		}
		else
			cout << argv[0] << " <vfs name> pull <source file name> <destination file name>\n";
	}
	else if (strcmp("delete", command) == 0)
	{
		if (argc == 3)
			if (disc.deleteVfs(argv[1]))
				return -12;
			else
				cout << argv[0] << " <vfs name> delete\n";
	}
	else
	{
		cout << argv[0] << " invalid command: " << command << endl;
		return 1;
	}

	return 0;
}