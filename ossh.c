/********************************************
* File Name : ossh.c
* Purpose : Reproduce a shell with some basic features.
* Creation Date : 27-02-2012
* Last Modified : Tue 06 Mar 2012 09:43:18 PM EST
* Created By :  Calvin Nichols  
********************************************/

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
#include "ListInterface.h"
#include "ShellInterface.h"

#define MAX_ARG_LEN 10

static void bg_handler(int sig){
    int status;
	while(waitpid(-1,&status,WNOHANG) > 0);
	alarm(0);
}

int main() {

    /*DECLARATIONS*/
    struct command *L[100];
    char *cmdLine = calloc(100,sizeof(char)); //command line
    int *groupD;                              //keep track of directives
    char c;
    int numLists = 0;                         //number of lists created
    int listNum=0;                            //specific list to deal with
    int i;

    signal(SIGCHLD, bg_handler);    //allow children to die
    signal(SIGALRM, alarm_handler); //check efficiency

    L[numLists] = (struct command *)calloc(1, sizeof(struct command));
    initialize_list(L[numLists]);
    
    printf("My Shell > ");
    fflush(stdout);
    
    while(1) {
        c = getchar();
        
        switch(c) {
            case'\n':
                if (cmdLine[0] == '\0') {
                    printf("My Shell > ");
                }
                else { /*execute command line*/
                    
                    groupD = parse_cmdline(L[numLists], cmdLine);
                    if (groupD[0] == 0){
                        exec_cmdline(L[numLists]);
                    }
                    else {
                        /*directives*/
                        for (i=0; i<numLists+1; i++){
                            if (L[i]->group == groupD[1]){
                                directive(L[i], groupD[0]);
                            }
                        }
                    }
                    printf("My Shell > ");
                    
                    numLists++;
                    L[numLists] = (struct command *)calloc(1, sizeof(struct command));
                    initialize_list(L[numLists]);
                    reset_cmdline(cmdLine);
                }
            break;
            
            default:
                strncat(cmdLine, &c, 1);
                break;
        }
    }
    for (i=0; i<numLists+1; i++)
        free_proclist(L[i]);
    free_listofproclist(L);
    free(cmdLine);
return -1;
}
