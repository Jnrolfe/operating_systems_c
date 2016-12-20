#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <errno.h>
#include <sys/wait.h>

#define BSIZE 256

#define READ_END 0
#define WRITE_END 1

#define BASH_EXEC  "/bin/bash"
#define FIND_EXEC  "/usr/bin/find"
#define XARGS_EXEC "/usr/bin/xargs"
#define GREP_EXEC  "/bin/grep"
#define SORT_EXEC  "/usr/bin/sort"
#define HEAD_EXEC  "/usr/bin/head"

int main(int argc, char *argv[])
{
    int status;
    pid_t pid_1, pid_2, pid_3, pid_4;

    // declare pipe file descriptors
    int pipefd_1[2];
    int pipefd_2[2];
    int pipefd_3[2];

    if (argc != 4) {
        printf("usage: finder DIR STR NUM_FILES\n");
        exit(0);
    }

    // FIND
    pipe(pipefd_1); // make pipe 
    pid_1 = fork();
    if (pid_1 == 0) {
        /* First Child */
        // forming the command
        char cmdbuf[BSIZE];
        bzero(cmdbuf, BSIZE);
        sprintf(cmdbuf, "%s %s -name \'*\'.[ch]", FIND_EXEC, argv[1]);

        // close unused pipe ends 
        close(pipefd_1[READ_END]);

        // connect pipe
        dup2(pipefd_1[WRITE_END], STDOUT_FILENO);

        // close other pipe end(s)
        close(pipefd_1[WRITE_END]);

        // execute "find" passing DIR and additional parameters
        if((execl(BASH_EXEC, BASH_EXEC, "-c", cmdbuf, (char *) 0)) < 0)
        {
            fprintf(stderr, "\nError execing find. ERROR#%d\n", errno);
            return EXIT_FAILURE;
        }
        
        exit(0);
    }
    
    // XARGS and GREP
    pipe(pipefd_2); // make pipe 
    pid_2 = fork();
    if (pid_2 == 0) {
        /* Second Child */
        
        // close unused pipe ends
        close(pipefd_1[WRITE_END]);
        close(pipefd_2[READ_END]);
       
        // connect pipes
        dup2(pipefd_1[READ_END], STDIN_FILENO);
        dup2(pipefd_2[WRITE_END], STDOUT_FILENO);

        // close other pipe end(s)
        close(pipefd_1[READ_END]);
        close(pipefd_2[WRITE_END]);

        if((execl(XARGS_EXEC, XARGS_EXEC, GREP_EXEC, "-c", argv[2], (char *) 0)) < 0)
        {
            fprintf(stderr, "\nError execing xargs. ERROR#%d\n", errno);
            return EXIT_FAILURE;
        }
        
        exit(0);
    }

    // SORT
    pipe(pipefd_3); // make pipe
    pid_3 = fork();
    if (pid_3 == 0) {
        /* Third Child */
        // forming the command
        char cmdbuf[BSIZE];
        bzero(cmdbuf, BSIZE);

        // close unused pipe ends
        close(pipefd_1[READ_END]);
        close(pipefd_1[WRITE_END]);
        close(pipefd_2[WRITE_END]);
        close(pipefd_3[READ_END]);

        // connect pipes
        dup2(pipefd_2[READ_END], STDIN_FILENO);
        dup2(pipefd_3[WRITE_END], STDOUT_FILENO);

        // close other pipe end(s)
        close(pipefd_2[READ_END]);
        close(pipefd_3[WRITE_END]);
        
        if((execl(SORT_EXEC, SORT_EXEC, "-t", ":", "+1.0", "-2.0","--numeric", "--reverse",(char *) 0)) < 0)
        {
            fprintf(stderr, "\nError execing sort. ERROR#%d\n", errno);
            return EXIT_FAILURE;
        }
        
        exit(0);
    }
    
    // HEAD
    pid_4 = fork();
    if (pid_4 == 0) {
        /* Fourth Child */
        // forming the command
        char cmdbuf[BSIZE];
        bzero(cmdbuf, BSIZE);

        // close unused pipe ends
        close(pipefd_1[READ_END]);
        close(pipefd_1[WRITE_END]);
        close(pipefd_2[READ_END]);
        close(pipefd_2[WRITE_END]);
        close(pipefd_3[WRITE_END]);

        // connect pipe
        dup2(pipefd_3[READ_END], STDIN_FILENO);

        // close other pipe end
        close(pipefd_3[READ_END]);

        sprintf(cmdbuf, "--lines=%s", argv[3]);
        
        if((execl(HEAD_EXEC, HEAD_EXEC, cmdbuf, (char *) 0)) < 0)
        {
            fprintf(stderr, "\nError execing head. ERROR#%d\n", errno);
            return EXIT_FAILURE;
        }

        exit(0);
    }
    
    // close all pipe ends
    close(pipefd_1[READ_END]);
    close(pipefd_1[WRITE_END]);
    close(pipefd_2[READ_END]);
    close(pipefd_2[WRITE_END]);
    close(pipefd_3[READ_END]);
    close(pipefd_3[WRITE_END]);

    if ((waitpid(pid_1, &status, 0)) == -1) {
        fprintf(stderr, "Process 1 encountered an error. ERROR%d", errno);
        return EXIT_FAILURE;
    }
    if ((waitpid(pid_2, &status, 0)) == -1) {
        fprintf(stderr, "Process 2 encountered an error. ERROR%d", errno);
        return EXIT_FAILURE;
    }
    if ((waitpid(pid_3, &status, 0)) == -1) {
        fprintf(stderr, "Process 3 encountered an error. ERROR%d", errno);
        return EXIT_FAILURE;
    }
    if ((waitpid(pid_4, &status, 0)) == -1) {
        fprintf(stderr, "Process 4 encountered an error. ERROR%d", errno);
        return EXIT_FAILURE;
    }

    return 0;
}
