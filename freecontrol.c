/*
    FreeControl
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
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>

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

int main (int argc, char* argv[])
{
	printf("'%s' started.\n", argv[0]);
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

	printf("\nIniciando.............\n");
	
	key_t key;

	//   Get the main key 'A'

	key = ftok(".",'A');

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
	
	
	return 0;
}



