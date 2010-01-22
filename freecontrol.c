/*
    FreeControl
    Copyright (C) 2009, 2010  Quique Rodiguez <quesoruso74@yahoo.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>

const char* program_name;

extern char** environ;

struct mymsgbuf {
	long mtype;
	char mtext[10];
};

void espera_ok(int qid, int n) {
	struct mymsgbuf qbuf;
	long type;
	int cant;
	
	printf("+\n"); 

	do {
		type = 1;
		qbuf.mtype = type;
		printf("-\n");
		cant = msgrcv(qid,  &qbuf, 10, type, 0);
		printf(".\n");
	} while (cant == 0);
	
}	

int abrir_prog(char s[200], int i)
{
	char *t1;
	char s1[200];
	strcpy(s1,s);

	for ( t1 = strtok(s1," ");
	      t1 != NULL;
	      t1 = strtok(NULL, " ") )
		printf("%s\n",t1);

	return (system(s));
}

int print_copyright()
{
	printf("FreeControl\n");
	printf("Copyright (C) 2009  Quique Rodiguez <quesoruso74@yahoo.com>\n\n");
	printf("This program is free software: you can redistribute it and/or modify\n");
	printf("it under the terms of the GNU General Public License as published by\n");
	printf("the Free Software Foundation, either version 3 of the License, or\n");
	printf("any later version.\n\n");
	printf("This program is distributed in the hope that it will be useful,\n");
	printf("but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
	printf("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
	printf("GNU General Public License for more details.\n\n");
	printf("You should have received a copy of the GNU General Public License\n");
	printf("along with this program.  If not, see <http://www.gnu.org/licenses/>.\n\n");
}

int print_usage()
{
	printf("Usage:\n\n");
	printf("        %s -lrskvh\n\n", program_name);
	printf("        -l   --load    Reads the config file, loads all the childs and stays\n");
	printf("                      resident.\n");
	printf("        -r   --run     Sends run to all the childs.\n");
	printf("        -s   --stop    Sends stop to all the childs.\n");
	printf("        -k   --kill    Sends kill to all the childs and finishes.\n");
	printf("        -v   --view    Shows the state of the program an sends view to all the\n");
	printf("                      childs.\n");
	printf("        -h   --help    Shows this help.\n\n");
}

int main (int argc, char* argv[])
{
	print_copyright();

	program_name=argv[0];

	if (argc<=1)
	{
		print_usage();
		return(0);
	}

	int next_option;

	const char* const short_options = "lrskvh";

	const struct option long_options[] = {
		{ "load", 0, NULL, 'l' },
		{ "run", 0, NULL, 'r' },
		{ "stop", 0, NULL, 's' },
		{ "kill", 0, NULL, 'k' },
		{ "view", 0, NULL, 'v' },
		{ "help", 0, NULL, 'h' },
		{ NULL, 0, NULL, 0 }
		};
	
	do {
		next_option = getopt_long (argc, argv, short_options, long_options, NULL);
		switch (next_option)
		{
		case 'l':
			printf("Option -l\n");
			break;
		case 'r':
			printf("Option -r\n");
			break;
		case 's':
			printf("Option -s\n");
			break;
		case 'k':
			printf("Option -k\n");
			break;
		case 'v':
			printf("Option -v\n");
			break;
		case 'h':
			print_usage();
			return(0);
		case '?':
			print_usage();
			return(1);
		}
	}
	while (next_option != -1);

	key_t key;

	//   Get the main key 'A'

	key = ftok("/tmp",'A');

	if (key == -1)  {
		printf("Couldn't open the main key.\n");
		return (1);
		}

	//   Open or create the main message queue
	int msgqueue_id;
	if ((msgqueue_id = msgget(key, IPC_CREAT|0660)) ==-1)
	{
		printf("Couldn not open the main message queue\n");
		return(1);
	}

	//   Open all the programs in /Data/Config

	FILE * Archivo;
	char s[200];
	Archivo=fopen("./Data/Config","r");
	
	if (Archivo==NULL) {
		printf("Cannot open the Config file.\n");
	}
	int i=0;
	int error;
	while (fgets(s,200,Archivo)!=NULL)
  	{
		printf("Read: %s \n",s);
		//error =	system(s);
		error = abrir_prog(s,i);
		if (error!= 0)
		{
			printf("Cannot run: %s \n",s);
			return(1);
		}
		i+=100;
		printf("Waiting response -- \n");
		espera_ok(msgqueue_id, i);
  	}

	fclose (Archivo);
	
	pid_t father, son;

	father = getpid();

	if ((son=fork())==0)
		{
		for (i=0; i<100; i++) {
			sleep(1);
			//printf("-\n");
			}
		}
	
	return 0;
}



