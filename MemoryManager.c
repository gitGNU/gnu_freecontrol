/*
    MemoryManager is part of FreeControl Package
    Copyright (C) 2009  Quique Rodiguez <quesoruso74@yahoo.com>

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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>

struct mymsgbuf {
	long mtype;
	char mtext[10];
};

main() {

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
	for (;;) {}
	}
