/*
    MemoryManager is part of FreeControl Package
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

#define LOCK_FILE	"MemoryManager.lock"
#define LOG_FILE	"MemoryManager.log"

#include "Common.c"


struct mymsgbuf {
	long mtype;
	char mtext[10];
};

main(int argc, char* argv[]) {

	program_name=argv[0];
	print_copyright();

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
			return(0);
			break;
		case 's':
			printf("Option -s\n");
			return(0);
			break;
		case 'k':
			printf("Option -k\n");
			return(0);
			break;
		case 'v':
			printf("Option -v\n");
			return(0);
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
	
	daemonize();

	int i;
	struct mymsgbuf qbuf;
	long type;
	int cant;

	//FILE * Archivo;
	//Archivo=fopen("MM.dbg","w");
	//fprintf(Archivo, "Starting MemoryManager\n");

	key_t key;

	//   Get the main key 'A'

	key = ftok(".",'A');

	if (key == -1)  {
		//fprintf(Archivo, "Couldn't open the main key.\n");
		return (1);
		}
	//   Open or create the main message queue
	int msgqueue_id;
	if ((msgqueue_id = msgget(key, IPC_CREAT|0660)) ==-1)
	{
		//fprintf(Archivo, "Could not open the main message queue\n");
		return(1);
	}

	type = 1;
	qbuf.mtype = type;
	char* text;
	text="OK";
	//strcpy(qbuf.mtext, &text);
	qbuf.mtext[0]='O';
	qbuf.mtext[1]='K';
	qbuf.mtext[2]=0;
	sleep(5);
	printf("Sending...\n");	
	
	int error;
	error =	msgsnd(msgqueue_id, &qbuf, 3, 0);

	printf("Response: %d \n", error);
	//cant = msgrcv(n+1,  &qbuf, strlen(qbuf.mtext), type, 0);
	//fclose(Archivo);

	struct timespec interval_t;

	interval_t.tv_sec=0;
	interval_t.tv_nsec=1000;

	for (;;) {
		nanosleep(interval_t);
		}
	}
