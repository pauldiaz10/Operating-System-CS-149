//
//  main.c
//  Homework6
//
//  Created by Kevin Trinh on 7/30/16.
//
//

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <sys/ioctl.h>


#define BUFFER_SIZE 32
#define READ 0
#define WRITE 1
#define MAX_PROCESSES 5
#define SECONDS_END 30

int fds[MAX_PROCESSES][2];


int main(int argc, const char * argv[]) {
    //Create 2d array holding process and the read and write pipes.
    //These are the file descriptors for the processes to write to, as well as the parent to read from.
    pid_t pids[MAX_PROCESSES];
    int n = MAX_PROCESSES;
    int alive = MAX_PROCESSES;
    
    fd_set inputs, inputfds;
    struct timeval timeout;
    
    FD_ZERO(&inputs); //Zero out the file descriptor set
    
    //Creating a pipe.
    for(int i=0; i< MAX_PROCESSES; i++){
        if(pipe(fds[i]) < 0){
            perror("pipe");
            return 1;
        }
    }
    
    //Add the read line of the file descriptors to the set of FD to select from.
    for(int i=0; i< MAX_PROCESSES; i++){
        if(i < MAX_PROCESSES){
            FD_SET(fds[i][READ], &inputs);
        }
    }
    
    //Start time

       	time_t start;
	time(&start);
    //grow my children
    for (int i = 0; i < n; i++) {
        if ((pids[i] = fork()) < 0) {
            perror("fork");
            return 1;
        } else if (pids[i]== 0) {
                //timing purposes
            if(i < (MAX_PROCESSES - 1)){
                
                srand(i); //If we do not give a special seed to each child, it will use the same random seed.
                int msg_count = 0;
                //need to put time here

                time_t current;
		time(&current);
                while(difftime(current, start) < SECONDS_END){
                    time(&current);

                    char child_message[BUFFER_SIZE];
			//Peter: Add timestamp here
                    sprintf(child_message, "Child %i message %i", i + 1, ++msg_count);
                    child_message[strlen(child_message)] = '\0';
                    write(fds[i][WRITE], child_message, BUFFER_SIZE);
                    sleep(rand() % 3); // 1 - 2.
                }
        
            }else{
                //Special child
                time_t current = time(NULL);

                while(difftime(current, start) < SECONDS_END){
                    current = time(NULL);
                    char input_message[BUFFER_SIZE];
                    scanf("%s", input_message);
                    printf("Sending %s\n", input_message);
			//Peter: Add timestamp here.
                    write(fds[i][WRITE], input_message, BUFFER_SIZE);
                }
                
            }
            exit(0);
        }
    }
    
    printf("Starting parent process\n");
    

    int status;

	int log;
	log = open("output.txt", O_APPEND | O_CREAT | O_WRONLY);

    while(alive > 0){
        inputfds = inputs;
	timeout.tv_sec = 2;
	timeout.tv_usec = 500;
        int result = select(FD_SETSIZE, &inputfds, (fd_set*)0, (fd_set*)0, &timeout);

        if(result < 0)
            perror("result");
        
        for(int j=0; j<MAX_PROCESSES; j++){
           
            //We have to figure out which one is it
                if(FD_ISSET(fds[j][READ],&inputfds)){
                    //which one can we read from
                    //IF SO, read here and print out
                    char buffer[BUFFER_SIZE];
                    read(fds[j][READ], buffer, BUFFER_SIZE);
                    //Should clear it from select again.
                    printf("Read from child process %i: %s\n", (j + 1), buffer);
			//Append timestamp at child sending to pipe, then append timestamp here as well. 2 timestamps.
			write(log, buffer, strlen(buffer));
			write(log, "\n", 1);
                }
   
        }
        //check for exit 0 is success.
        for(int i=0; i<MAX_PROCESSES; i++){
            if(waitpid(pids[i], &status, WNOHANG) > 0){
                printf("Child process %i exited with status %i\n", i + 1, status);
                alive--;
            }
            
        }

        
    }
    
    close(log);
    
    return 0;
}