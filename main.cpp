#include <iostream>
#include <string.h>
using namespace std;
#include "vfs.h"
#include <stdlib.h>
int main(int argc, char ** argv)
{
	char * vfs_name;
	char * command;
	Vfs disc;
	disc.vfsCreate("dysk", 100000);
	disc.vfsClose();
	disc.vfsOpen("dysk");
	disc.copyToVfs("test.txt", "test.txt");
	disc.vfsClose();


	return 0;
	if (argc < 3)
	{
		cout << argv[0] << " <vfs name> command (...)\n";
		cout << "@@@@@@@@@@@@@@@@@@";
		cout << "Availible commands: \n";
		cout << "- create <size in bytes>\n";
		cout << "- dump\n";
		cout << "- list\n";
		cout << "- push <source file name> <destination file name>\n";
		cout << "- pull <source file name> <destination file name>\n";
		cout << "- remove <file name>\n";
		cout << "- delete\n";
		cout << "@@@@@@@@@@@@@@@@@@";
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
				printf("Nie udalo sie utworzyc dysku wirtualnego!\n");
				return 2;
			}
			disc.vfsClose();
		}
		else
			printf("%s <vfs name> create <size in bytes>\n", argv[0]);
	}
	else if (strcmp("dump", command) == 0)
	{
		if (argc == 3)
		{
			
			if (disc.vfsOpen(argv[1]))
			{
				printf("Nie udalo sie otworzyc dysku wirtualnego!\n");
				return 2;
			}

			disc.listInfoBlocks();

			disc.vfsClose();
		}
		else
			printf("%s <vfs name> dump\n", argv[0]);
	}
	else if (strcmp("list", command) == 0)
	{
		if (argc == 3)
		{
			
			if (disc.vfsOpen(argv[1]))
			{
				printf("Nie udalo sie otworzyc dysku wirtualnego!\n");
				return 2;
			}

			disc.listFiles();

			disc.vfsClose();
		}
		else
			printf("%s <vfs name> list\n", argv[0]);
	}
	else if (strcmp("push", command) == 0)
	{
		if (argc == 5)
		{
			
			if (disc.vfsOpen(argv[1]))
			{
				printf("Nie udalo sie otworzyc dysku wirtualnego!\n");
				return 2;
			}

			printf("Wysylanie pliku, wynik: %d\n", disc.copyToVfs(argv[3], argv[4]));

			disc.vfsClose();
		}
		else
			printf("%s <vfs name> push <source file name> <destination file name>\n", argv[0]);
	}
	else if (strcmp("pull", command) == 0)
	{
		if (argc == 5)
		{
			if (disc.vfsOpen(argv[1]))
			{
				printf("Nie udalo sie otworzyc dysku wirtualnego!\n");
				return 2;
			}

			printf("Pobieranie pliku, wynik: %d\n", disc.copyFromVfs(argv[3], argv[4]));

			disc.vfsClose();
		}
		else
			printf("%s <vfs name> pull <source file name> <destination file name>\n", argv[0]);
	}
	else if (strcmp("remove", command) == 0)
	{
		if (argc == 4)
		{
			if (disc.vfsOpen(argv[1]))
			{
				printf("Nie udalo sie otworzyc dysku wirtualnego!\n");
				return 2;
			}

		disc.deleteFromVfs(argv[3]);

			disc.vfsClose();
		}
		else
			printf("%s <vfs name> pull <source file name> <destination file name>\n", argv[0]);
	}
	else if (strcmp("delete", command) == 0)
	{
		if (argc == 3)
			if (disc.deleteVfs(argv[1]))
				return -12;
		else
			printf("%s <vfs name> delete\n", argv[0]);
	}
	else
	{
		printf("%s invalid command `%s`\n", argv[0], command);
		return 1;
	}


	return 0;
}