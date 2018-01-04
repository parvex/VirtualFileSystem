#include <iostream>
#include <string.h>
using namespace std;
#include "Vfs.h"

int main(int argc, char ** argv)
{
	char * vfs_name;
	char * command;
	Vfs disc;

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
	/*else if (strcmp("dump", command) == 0)
	{
		if (argc == 3)
		{
			v = vfs_open(vfs_name);
			if (!v)
			{
				printf("Nie udalo sie otworzyc dysku wirtualnego!\n");
				return 2;
			}

			vfs_dump(v);

			vfs_close(v);
		}
		else
			printf("%s <vfs name> dump\n", argv[0]);
	}
	else if (strcmp("list", command) == 0)
	{
		if (argc == 3)
		{
			v = vfs_open(vfs_name);
			if (!v)
			{
				printf("Nie udalo sie otworzyc dysku wirtualnego!\n");
				return 2;
			}

			vfs_list(v);

			vfs_close(v);
		}
		else
			printf("%s <vfs name> list\n", argv[0]);
	}
	else if (strcmp("push", command) == 0)
	{
		if (argc == 5)
		{
			v = vfs_open(vfs_name);
			if (!v)
			{
				printf("Nie udalo sie otworzyc dysku wirtualnego!\n");
				return 2;
			}

			printf("Wysylanie pliku, wynik: %d\n", vfs_copy_to(v, argv[3], argv[4]));

			vfs_close(v);
		}
		else
			printf("%s <vfs name> push <source file name> <destination file name>\n", argv[0]);
	}
	else if (strcmp("pull", command) == 0)
	{
		if (argc == 5)
		{
			v = vfs_open(vfs_name);
			if (!v)
			{
				printf("Nie udalo sie otworzyc dysku wirtualnego!\n");
				return 2;
			}

			printf("Pobieranie pliku, wynik: %d\n", vfs_copy_from(v, argv[3], argv[4]));

			vfs_close(v);
		}
		else
			printf("%s <vfs name> pull <source file name> <destination file name>\n", argv[0]);
	}
	else if (strcmp("remove", command) == 0)
	{
		if (argc == 4)
		{
			v = vfs_open(vfs_name);
			if (!v)
			{
				printf("Nie udalo sie otworzyc dysku wirtualnego!\n");
				return 2;
			}

			vfs_delete_file(v, argv[3]);

			vfs_close(v);
		}
		else
			printf("%s <vfs name> pull <source file name> <destination file name>\n", argv[0]);
	}
	else if (strcmp("delete", command) == 0)
	{
		if (argc == 3)
			vfs_delete(vfs_name);
		else
			printf("%s <vfs name> delete\n", argv[0]);
	}
	else
	{
		printf("%s invalid command `%s`\n", argv[0], command);
		return 1;
	}
*/


	system("pause");
	return 0;
}