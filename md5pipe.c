#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include "md5.h"
#include <stdbool.h> 
bool pass_test=false;
char encrypted[128];
pid_t p; 

void handler(){
	//signal(SIGUSR1, handler);
	if(strlen(encrypted)!=32){
		exit(EXIT_FAILURE);
	}
	pass_test=true;
	exit(0);
}

int main(){
	int fd1[2];
    int fd2[2];
    char input_str[20];
    
	
    if (pipe(fd1)==-1) {
        fprintf(stderr, "pipe failed" ); 
        return 1; 
    }
    if (pipe(fd2)==-1){ 
        fprintf(stderr, "pipe failed" ); 
        return 1; 
    }
	printf("plain text: ");
    scanf(" %s", input_str); 
    p=fork();
	
    if (p<0){
        fprintf(stderr, "fork malfunctioned" ); 
        return 1;
		
    } else if (p > 0){
		int stat;
        char concat_str[128];
        close(fd1[0]);  // close reading end of first pipe 
  
        // write input string and close writing end of first pipe. 
        write(fd1[1], input_str, strlen(input_str)+1); 
        close(fd1[1]); 
		
        // wait for child
        wait(&stat);
		
        close(fd2[1]); // close writing end of second pipe 
  
        // read string from child 
        read(fd2[0], concat_str, 128);
		
		if(WEXITSTATUS(stat)!=1){
			killpg(p, SIGKILL);
			printf("encrypted by process %d : %s\n", getpid(), concat_str);
			close(fd2[0]);
			exit(0);
		}
		exit(EXIT_FAILURE);
        
		
    } else { 
	
        close(fd1[1]);  // close writing end of first pipe 
        // read a string using first pipe 
        char concat_str[20];
		
        read(fd1[0], concat_str, 20); 
  
        concat_str[strlen(concat_str)] = '\0';
		size_t len = strlen(concat_str);
		
		// close both reading ends 
        close(fd1[0]); 
        close(fd2[0]); 
		signal(SIGUSR1, handler);
		
		//encrypt
		md5(concat_str, len);
		
		//convert to buffer
		if(altered){ //bool variable in md5.h
			uint8_t *conv;
			conv=(uint8_t *)&h0;
			sprintf(encrypted+strlen(encrypted), "%2.2x%2.2x%2.2x%2.2x", conv[0], conv[1], conv[2], conv[3], h0);
			conv=(uint8_t *)&h1;
			sprintf(encrypted+strlen(encrypted), "%2.2x%2.2x%2.2x%2.2x", conv[0], conv[1], conv[2], conv[3], h1);
			conv=(uint8_t *)&h2;
			sprintf(encrypted+strlen(encrypted), "%2.2x%2.2x%2.2x%2.2x", conv[0], conv[1], conv[2], conv[3], h2);
			conv=(uint8_t *)&h3;
			sprintf(encrypted+strlen(encrypted), "%2.2x%2.2x%2.2x%2.2x", conv[0], conv[1], conv[2], conv[3], h3);
		}
		
		encrypted[strlen(encrypted)] = '\0';
		
        write(fd2[1], encrypted, strlen(encrypted)+1); 
        close(fd2[1]);
		
		if(raise(SIGUSR1)==1){
			exit(EXIT_FAILURE);
		}
		while(true){
			sleep(1);
		}
    }
}