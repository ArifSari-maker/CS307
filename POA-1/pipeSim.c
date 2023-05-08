#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

// Arif Kemal Sari 28999
int main() {

    
    printf("I'm SHELL process, with PID: %d - Main command is: man df | grep -e -h -A 1 -m 1 \n", getpid()); //"SHELL" process
    
    
    int fd[2];  // Creates file descriptor and pipes each other
    pipe(fd);  //pipes each other

    // Creates a child process for MAN process
    int id1 = fork();

    if (id1 < 0) {
    // fork failed; exit
    fprintf(stderr, "fork 1 failed\n");
    exit(1);
    } 
    
    
    else if (id1 == 0) { //  Child process for "MAN"

        printf("I’m MAN process, with PID: %d - My command is: man df \n", getpid());

        // Writes man process result to write-end of file descriptor
        dup2(fd[1], STDOUT_FILENO);
        execlp("man", "man", "df", NULL);
    } 
    
    // Returns to main "SHELL" process
    else{

        // Creates a child process for GREP process
        int id2 = fork();

        if (id2 < 0) {
        // fork failed; exit
        fprintf(stderr, "fork 2 failed\n");
        exit(1);
        } 
        
        else if (id2 == 0) { // Child process for "GREP" 

            printf("I’m GREP process, with PID: %d - My command is: grep -e -h -A 1 -m 1 \n", getpid());

            
            int file = open("output.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);  // creates output.txt file

            
            dup2(fd[0], STDIN_FILENO); // Gets man process output from  file descriptor

           
            dup2(file, STDOUT_FILENO); 
            execlp("grep", "grep", "-e", "-h", "-A", "1", "-m", "1", NULL);
        } 

       
        else{ // Returns to  "SHELL" process

            // Waits for child processes 
            waitpid(id1, NULL, 0); 
            waitpid(id2, NULL, 0);            
            printf("I’m SHELL process, with PID: %d - execution is completed, you can find the results in output.txt \n", getpid());
            } 
    }
    return 0;
}

