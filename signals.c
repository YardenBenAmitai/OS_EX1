#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

int pid_list[5];
int indx;

void sigCatcher();  //declaration of function


int main() {
	pid_t pid, finished;
	int status;
	
	signal(SIGINT, sigCatcher);
	
	for (int i=0; i<5; i++){
		pid=fork();
		if (pid<0){
			perror("fork malfuctioned");
		} else if(pid>0){
			pid_list[i] = pid;
		} else{
			printf("PID %d ready\n", getpid());
			indx=i-1;
			pause();
			exit(0);
		}
	}
	
	sleep(3);
	
	kill(pid_list[4], SIGINT);
	for (int i=0; i<5; i++){
		finished=wait(&status);
		printf("Process %d is dead\n", finished);
	}
	exit(0);
}

void sigCatcher(){
	//printf("inside sigCatcher\n");
	printf("PID %d caught one\n", getpid());
	signal(SIGINT, sigCatcher);
	if (indx!=-1){
		kill(pid_list[indx], SIGINT);
	}
}
