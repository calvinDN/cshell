/********************************************
* File Name : ShellImplementation.c
* Purpose : Contains functions related to shell command execution.
* Creation Date : 27-02-2012
* Last Modified : Tue 06 Mar 2012 09:43:18 PM EST
* Created By :  Calvin Nichols  
********************************************/

#include "ListType.h"
#include "ShellInterface.h"
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

struct command *LALARM;

void alarm_handler(int sig){
    int err;
    double intUsage;
    FILE *fp;
    char *str = calloc(100,sizeof(char)), *usage = calloc(1024,sizeof(char)), *trash = calloc(1024,sizeof(char));
    char *check = calloc(100,sizeof(char));

    strcpy(check, "ps -p ");
    sprintf(str,"%d",LALARM->pid);
    strcat(check, str);
    strcat(check, " -o pcpu");
    
    fp = popen(check, "r");
    if (fp == NULL) {
        perror("popen");
        exit(126);
    }
     
    fgets(trash, 1000, fp);
    fgets(usage, 1000, fp);
    usage = strtok(usage, " ");
    usage[strlen(usage)-1] = '\0'; /*drop newline*/
    
    intUsage = atof(usage);
    //printf("usageint: '%f'\n", intUsage);
    
    if (intUsage < LALARM->pcpu){
        kill(LALARM->pid, SIGINT);
        LALARM = NULL;
        printf("\nprocess killed because it did not meet efficiency requirements\n");
    }
    else
        alarm(2);
    
    pclose(fp);
    if (err == -1) {
        perror("pclose");
        exit(126);
    }
}

void reset_cmdline(char *cmdLine){
    int size;
    size = strlen(cmdLine);
    bzero(cmdLine, size);
}

void piping(struct command *L){
    int status, i, length;
    int pipefd[10][2];
    
    length = list_size(L);
    
    for (i=0; i<length-1; i++){
        pipe(pipefd[i]);
    }
    
    //first arguement
    //fork, stdin
    pid_t pid = fork();
    if (pid == 0) {
        if (L->infile && !L->outfile)
            io_redirect(L);
        dup2(pipefd[0][1], 1); //connect the standard output of cat to the write end of the first pipe
        execvp(L->cmd, L->argv); 
        exit(0);
    } 
    else {
        L->pid = pid;
        close(pipefd[0][1]);
    }
    
    L=L->next;
    //everything in between
    i = 0;
    while (L->next != NULL){
        pid = fork();
        if (pid == 0) {
            dup2(pipefd[i][0], 0);     //connect the standard input of uniq to the read end of the first pipe
            dup2(pipefd[i+1][1], 1);   //connect the standard output of uniq to the write end of the second pipe
            execvp(L->cmd, L->argv);       
        }
        else {
            L->pid = pid;
            close(pipefd[i+1][1]);
        }
        i++;
        L=L->next;
    }
    
    //last arguement
    //fork stdout
    pid = fork();
    if (pid == 0) {
        if (!L->infile && L->outfile)
            io_redirect(L);
        dup2(pipefd[length-2][0], 0); //connect the standard input of sort to the read end of the second pipe
        execvp(L->cmd, L->argv);
    }
    else {
        L->pid = pid;
        if (L->background == 0)
            pid = wait(&status);
    }
}

void exec_cmdline(struct command *L){
    int i, in, out, status;
    pid_t pid;
    
    if (L->pipe == 1)
        piping(L);
    else {
        pid = fork();
        if (pid == 0){
        
            if (L->infile || L->outfile)
                io_redirect(L);
            
            i = execvp(L->cmd, L->argv);
            printf("errno is %d\n", errno);
            if (i < 0){
                printf("%s: command not found\n", L->cmd);
                exit(1);
            }
        }
        else if (pid < 0){
            printf("fork failed\n");
            exit(1);
        }
        else {
            //pid of child
            if (L->pid == 0)
                L->pid = pid;
            //printf("pid: %d\n", L->pid);
            if (L->pcpu > 0){
                LALARM = L;
                alarm(5);
                //efficiency_enforcement(L, pid);
            }
            if (L->background == 0)
                wait(NULL);
        }
    }
}

void directive(struct command *L, int direc){
    if (direc == 7){ //kill
        if (L->pcpu > 0)
            alarm(0);
        while (L->next != NULL){
            kill(L->pid, SIGKILL);
            L = L->next;
        }
        kill(L->pid, SIGKILL);
    }
    else if (direc == 8){ //stop
        if (L->pcpu > 0)
            alarm(0);
        while (L->next != NULL){
            kill(L->pid, SIGSTOP);
            L = L->next;
        }
        kill(L->pid, SIGSTOP);
    }
    else if (direc == 9){ //cont
        if (L->pcpu > 0){
            LALARM = L;
            alarm(5);
        }
        while (L->next != NULL){
            kill(L->pid, SIGCONT);
            L = L->next;
        }
        kill(L->pid, SIGCONT);
    }
    else
        perror("directive");
}

void io_redirect(struct command *L){

    int in, out; 

    if (L->infile && L->outfile){
        in = open(L->infile, O_RDONLY);

        out = open(L->outfile, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);

        dup2(in, 0);
        dup2(out, 1);
        
        close(in);
        close(out);
    }
    else if (L->infile){
        in = open(L->infile, O_RDONLY);
        dup2(in, 0);
        close(in);
    }
    else if (L->outfile){
        out = open(L->outfile, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
        dup2(out, 1);
        close(out);
    }
    else {
        printf("calling I/O redirection with no file specified\n");
        exit(126);
    }
    
}

