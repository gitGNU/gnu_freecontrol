/*
    Common.c is part of FreeControl Package
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
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>

#define RUNNING_DIR	"/tmp"

const char* program_name;

extern char** environ;

int print_copyright()
{
	printf("%s\n\n", program_name);
	printf("Copyright (C) 2009, 2010  Quique Rodiguez <quesoruso74@yahoo.com>\n\n");
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
	printf("        -l   --load    Load the program and stay resident.\n");
	printf("        -r   --run     Run the program.\n");
	printf("        -s   --stop    Stop the program.\n");
	printf("        -k   --kill    Kill the program.\n");
	printf("        -v   --view    Shows the state of the program\n");
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
	case SIGUSR1:
		view_stat();
		break;
	case SIGTERM:
		log_message(LOG_FILE,"terminate signal catched");
		ending();
		exit(0);
		break;
	}
}

void daemonize()
{
	int i,lfp;
	char str[10];
	//if(getppid()==1) return; /* already a daemon */
	//i=fork();
	//if (i<0) exit(1); /* fork error */
	//if (i>0) exit(0); /* parent exits */
	/* child (daemon) continues */
//	setsid(); /* obtain a new process group */
//	for (i=getdtablesize();i>=0;--i) close(i); /* close all descriptors */
	//i=open("/dev/null",O_RDWR); dup(i); dup(i); /* handle standart I/O */
//	umask(027); /* set newly created file permissions */
//	chdir(RUNNING_DIR); /* change running directory */
//	lfp=open(LOCK_FILE,O_RDWR|O_CREAT,0640);
//	if (lfp<0) exit(1); /* can not open */
//	if (lockf(lfp,F_TLOCK,0)<0) exit(0); /* can not lock */
	/* first instance continues */
//	sprintf(str,"%d\n",getpid());
//	write(lfp,str,strlen(str)); /* record pid to lockfile */
	signal(SIGCHLD,SIG_IGN); /* ignore child */
	signal(SIGTSTP,SIG_IGN); /* ignore tty signals */
	signal(SIGTTOU,SIG_IGN);
	signal(SIGTTIN,SIG_IGN);
	signal(SIGUSR1,signal_handler); /* catch user1 signal */
	signal(SIGTERM,signal_handler); /* catch kill signal */
}

