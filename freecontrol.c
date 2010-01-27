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

#define LOCK_FILE	"FreeControl.lock"
#define LOG_FILE	"FreeControl.log"
#define TEMP_DIR	"/tmp"
#define APP_DIR		"/home/pc/freecontrol/"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>

const char* program_name;

extern char** environ;

struct config_s {
	char app[100];
	int app_id;
	char app_ch;
	};

struct config_s config[100];

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

int open_prog(char s[100], int i) 
{
	int n;
	char s1[100];
	char s2[100];
	n=fork();
	if (n<0) {
		printf("...\n");
		exit(1);
		}
	if (n>0) {
		printf("+++\n");
		return(n);
		}
	while (s[strlen(s)-1]<' ' && strlen(s)>1)
		s[strlen(s)-1]=0;
	printf("000 %s\n", s);
	strcpy(s1,&s[2]);
	printf("000 %s\n", s1);
	strcpy(s2,APP_DIR);
	strcat(s2,s1);
	//strcat(s2,0);
	//strcat(s1,0);
	printf("000 %s\n", s2);
	//printf("--- %n\n", errno);
	n=execl(s2,s1,"-l", (char *)0);
	printf("111 \n");
	exit(0);
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
	printf("        %s -lkvh\n\n", program_name);
	printf("        -l   --load    Reads the config file, loads all the childs and stays\n");
	printf("                      resident.\n");
	printf("        -k   --kill    Sends kill to all the childs and finishes.\n");
	printf("        -v   --view    Shows the state of the program an sends view to all the\n");
	printf("                      childs.\n");
	printf("        -h   --help    Shows this help.\n\n");
}

void log_message(filename,message)
char *filename;
char *message;
{
FILE *logfile;
	logfile=fopen(filename,"a");
	if(!logfile) return;
	fprintf(logfile,"%s\n",message);
	fclose(logfile);
}

void signal_handler(sig)
int sig;
{
	switch(sig) {
	case SIGTERM:
		log_message(LOG_FILE,"terminate signal catched");
		exit(0);
		break;
	}
}

void daemonize()
{
	int i,lfp;
	char str[10];

	i=fork();
	if (i<0) exit(1); /* fork error */
	if (i>0) exit(0); /* parent exits */
	/* child (daemon) continues */
	setsid(); /* obtain a new process group */
	for (i=getdtablesize();i>=0;--i) close(i); /* close all descriptors */
	i=open("/dev/null",O_RDWR); dup(i); dup(i); /* handle standart I/O */
	umask(027); /* set newly created file permissions */
	//chdir(RUNNING_DIR); /* change running directory */
	//lfp=open(LOCK_FILE,O_RDWR|O_CREAT,0640);
	//if (lfp<0) exit(1); /* can not open */
	//if (lockf(lfp,F_TLOCK,0)<0) exit(0); /* can not lock */
	/* first instance continues */
	//sprintf(str,"%d\n",getpid());
	//write(lfp,str,strlen(str)); /* record pid to lockfile */
	signal(SIGCHLD,SIG_IGN); /* ignore child */
	signal(SIGTSTP,SIG_IGN); /* ignore tty signals */
	signal(SIGTTOU,SIG_IGN);
	signal(SIGTTIN,SIG_IGN);
	signal(SIGHUP,signal_handler); /* catch hangup signal */
	signal(SIGTERM,signal_handler); /* catch kill signal */
}

void load_config() {

	if(getppid()==1) {
		printf("FreeControl already loaded");
		return; /* already a daemon */
		}

	char lock_file[200];
	strcpy(lock_file, TEMP_DIR);
	strcat(lock_file, "/");
	strcat(lock_file, LOCK_FILE);

	int i,lfp;
	char str[10];

	i=fork();  // fork this process
	if (i<0) exit(1); // fork error
	if (i>0) return; // parent exits

	// child (daemon) continues

	setsid(); /* obtain a new process group */

//	for (i=getdtablesize();i>=0;--i) close(i); /* close all descriptors */
	//i=open("/dev/null",O_RDWR); dup(i); dup(i); /* handle standart I/O */

	umask(027); /* set newly created file permissions */

	lfp=open(lock_file,O_RDWR|O_CREAT,0640);
	if (lfp<0) {
		log_message(LOG_FILE,"FreeControl already loaded");
		return; /* already a daemon */		
		} /* can not open */

	if (lockf(lfp,F_TLOCK,0)<0) { /* can not lock */
		log_message(LOG_FILE,"FreeControl already loaded");
		return; /* already a daemon */		
		} /* can not open */

	/* first instance continues */
	sprintf(str,"%d\n",getpid());
	write(lfp,str,strlen(str)); /* record pid to lockfile */

	signal(SIGCHLD,SIG_IGN); /* ignore child */
	signal(SIGTSTP,SIG_IGN); /* ignore tty signals */
	signal(SIGTTOU,SIG_IGN);
	signal(SIGTTIN,SIG_IGN);
	signal(SIGHUP,SIG_IGN); /* ignore hangup signal */
	signal(SIGTERM,signal_handler); /* catch kill signal */

	key_t key;

	//   Get the main key 'A'

	key = ftok(TEMP_DIR,'A');

	if (key == -1)  {
		log_message(LOG_FILE,"Couldn't open the main key.");
		return;
		}

	//   Open or create the main message queue
	int msgqueue_id;
	if ((msgqueue_id = msgget(key, IPC_CREAT|0660)) ==-1)
	{
		log_message(LOG_FILE,"Couldn not open the main message queue");
		return;
	}

	//   Open all the programs in /Data/Config

	// Open config file

	FILE * Archivo;
	char s[200];
	Archivo=fopen("./Data/Config","r");
	
	if (Archivo==NULL) {
		log_message(LOG_FILE,"Cannot open the Config file.");
		return;
	}
	i=0;
	int error;

	// Reads al config file and store it in config struct

	while (fgets(s,200,Archivo)!=NULL)
  	{
		//printf("Read: %s \n",s);
		strcpy(config[i].app, s);
		config[i].app_ch=i+'B';
		i++;
	}

	strcpy(config[i].app, "");

	for (i=0; config[i].app!= ""; i++) {
		config[i].app_id= open_prog(s,i);
		if (config[i].app_id<0)
		{
			char msg[200];
			sprintf(msg, "Cannot run: %s \n",s);
			log_message(LOG_FILE, msg);
			return;
		}
		//printf("Waiting response -- \n");
		espera_ok(msgqueue_id, i);
  	}

	fclose (Archivo);
	
	//daemonize();

	for (;;) {
		sleep(10);
		}

}

// ----------------------------------------------
// terminate()
//       Sends SIGTERM to the resident part
// ----------------------------------------------

void terminate() {

	int lfp,pid;
	char lock_file[200];
	strcpy(lock_file, TEMP_DIR);
	strcat(lock_file, "/");
	strcat(lock_file, LOCK_FILE);

	// Search for the lock_file to know if FreeControl is loaded
	FILE * pFile;
	pFile = fopen (lock_file,"r");
  	if (pFile<0) {
		// If not loaded send error message
		printf("FreeControl not loaded");
		return;
		}
	// Get the PID of the resident part
	fscanf (pFile, "%d", &pid);
        fclose (pFile);
	// Send SIGTERM to end the resident part
	kill(pid,SIGTERM);
	printf("Teminating resident part");
}

// ---------------------------------------------------
// 		Starting point
// ---------------------------------------------------

int main (int argc, char* argv[])
{
	print_copyright();

	program_name=argv[0];

	// If no argument, print usage
	if (argc<=1)
	{
		print_usage();
		return(0);
	}

	int next_option;

	// Set available options

	const char* const short_options = "lkvh";

	const struct option long_options[] = {
		{ "load", 0, NULL, 'l' },
		{ "kill", 0, NULL, 'k' },
		{ "view", 0, NULL, 'v' },
		{ "help", 0, NULL, 'h' },
		{ NULL, 0, NULL, 0 }
		};

	// Switch on aguments past

	do {
		next_option = getopt_long (argc, argv, short_options, long_options, NULL);
		switch (next_option)
		{
		case 'l':
			// Option -l
			// Load configuration
			load_config();
			break;
		case 'k':
			// Option -k
			// Terminate resident part
			terminate();
			break;
		case 'v':
			// Option -v
			// View states
			printf("Option -v\n");
			break;
		case 'h':
			// Option -h
			// Help, print usage
			print_usage();
			return(0);
		case '?':
			// Unknown option
			// Print usage
			print_usage();
			return(1);
		}
	}
	while (next_option != -1);

}

