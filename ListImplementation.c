/********************************************
* File Name : ListImplementation.c
* Purpose : Contains functions related to the list.
* Creation Date : 27-02-2012
* Last Modified : Tue 06 Mar 2012 09:43:18 PM EST
* Created By :  Calvin Nichols  
* Reference: http://www.c.happycodings.com/Data_Structures/code5.html
********************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ListInterface.h"

#define MAX_ARG_LEN 15

void initialize_list(struct command *L){
    L->argv = calloc(MAX_ARG_LEN,sizeof(char *));
    L->cmd = NULL;
    L->argc = 0;
    L->pcpu = -1;
    L->background = 0;
    L->pipe = 0;
    L->infile = NULL;
    L->outfile = NULL;
    L->group = -1;
    L->position = 1;
    L->pid = 0;
    L->next = NULL;
}

int list_size(struct command *L){
    while(L->next != NULL){
        L=L->next;
    }
    return L->position;
    
return -1;
}

void add_command(struct command *L){
    int size=1;
    while(L->next != NULL){
        L = L->next;
        size++;
    }
    size++;
    L->next = (struct command *) calloc(1, sizeof(struct command));

    L->next->argv = calloc(MAX_ARG_LEN,sizeof(char *));
    L->next->cmd = NULL;
    L->next->argc = 0;
    L->next->pcpu = -1;
    L->next->background = 0;
    L->next->pipe = 0;
    L->next->infile = NULL;
    L->next->outfile = NULL;
    L->group = -1;
    L->next->position = size;
    L->next->pid = 0;
    L->next->next = NULL;
}

void insert_arg(struct command *L, char *token){
    int size, check;
    
    //most current command
    while(L->next != NULL){
        L = L->next;
    }
    
    size = strlen(token);
    L->argv[L->argc] = calloc(size+1, sizeof(char));
    strcpy(L->argv[L->argc], token);
    strncat(L->argv[L->argc], "\0", 1);

    if (L->argc == 0){
        size = strlen(L->argv[L->argc]);
        L->cmd = calloc(size, sizeof(char));
        strcpy(L->cmd, L->argv[L->argc]);
    }

    L->argc++;
}

void free_proclist(struct command *L) {
    int i;
    struct command *temp;
    temp = (struct command *)malloc(sizeof(struct command));
    
    while(L->next != NULL){
        temp = L->next;
        for (i=0; i<L->argc+1; i++)
            free(L->argv[i]);
        free(L->argv);
        free(L->cmd);
        free(L->infile);
        free(L->outfile);
        free(L);
        L = temp;
    }
    for (i=0; i<L->argc+1; i++)
        free(L->argv[i]);
    free(L->argv);
    free(L->cmd);
    free(L->infile);
    free(L->outfile);
    free(L);
    
    L = NULL;
}

extern void free_listofproclist(struct command **L){
    free(L);
}

int identify_token(char *token){

    if (strcmp(token,"->") == 0)
        return 1;
    else if (strcmp(token,"^") == 0)
        return 2;
    else if (strcmp(token,"$") == 0)    
        return 3;
    else if (strcmp(token,"!") == 0)
        return 4;
    else if (strcmp(token,">") == 0)
        return 5;
    else if (token[0] == '[')
        return 6;
    else if (strcmp(token,"jkill") == 0)
        return 7;
    else if (strcmp(token,"jstop") == 0)
        return 8;
    else if (strcmp(token,"jcont") == 0)
        return 9;
    else
        return 0;
}

int *parse_cmdline(struct command *L, char *cmdLine){
    char *token = NULL;
    static int groupD[2] = {0,0};
    int currentCmd = 0, intSelect = 0, i=0, size, jobGroup=-1;
    struct command *previousCmd = (struct command *)malloc(sizeof(struct command)); //or here...
    struct command *lCopy = (struct command *)malloc(sizeof(struct command)); //dont think i need to malloc here
    char identifier[4];
    lCopy = L;
    token = strtok(cmdLine, " ");
    
    while (token != NULL){

        intSelect = identify_token(token);
        
        //if intSelect = 1,2,3,4 or 5 discard identifier token
        if (intSelect == 1 || intSelect == 2 || intSelect == 3 || intSelect == 4 || intSelect == 5){
            strcpy(identifier, token);
            token = strtok(NULL, " ");
        }
        else if (intSelect == 7 || intSelect == 8 || intSelect == 9){
            token = strtok(NULL, " ");
            jobGroup = token[0] - 48;
        }
        
        switch(intSelect){    
            case 1: /*pipe*/
                
                if (identify_token(token) != 0){
                    printf("error: expected argument after '%s'\n", identifier);
                    exit(126);
                }
                
                add_command(L);

                while(L->next != NULL){
                    previousCmd = L;
                    L = L->next;
                }
                
                previousCmd->pipe = 1;

                currentCmd++;
            break;
            
            case 2: /*infile*/
                i=0;
                
                if (identify_token(token) != 0){
                    printf("error: expected argument after '%s'\n", identifier);
                    exit(126);
                }
                
                while (L->next != NULL){
                    L = L->next;
                    i++; 
                }
                if (i != currentCmd){
                    perror("infile");
                    exit(126);
                }
                size = strlen(token);
                L->infile = calloc(size+1, sizeof(char));
                strcpy(L->infile, token);
                strncat(L->infile, "\0", 1);
                token = strtok(NULL, " ");
            break;
            
            case 3: /*outfile*/
                i=0;
                
                if (identify_token(token) != 0){
                    printf("error: expected argument after '%s'\n", identifier);
                    exit(126);
                }
                
                while (L->next != NULL){
                    L = L->next;
                    i++; 
                }
                if (i != currentCmd){
                    perror("outfile");
                    exit(126);
                }              
                size = strlen(token);
                L->outfile = calloc(size+1, sizeof(char));
                strcpy(L->outfile, token);
                strncat(L->outfile, "\0", 1);
                token = strtok(NULL, " ");
            break;
            
            case 4: /*background process*/
                i=0;

                if (token != NULL){
                    printf("error: arguement after '!'\n");
                    exit(126);
                }

                while (L->next != NULL){
                    L = L->next;
                    i++; 
                }
                if (i != currentCmd){
                    perror("background");
                    exit(126);
                }
                L->background = 1;
            break;
            
            case 5: /*efficiency*/
                i=0;
                
                if (identify_token(token) != 0){
                    printf("error: expected argument after '%s'\n", identifier);
                    exit(126);
                }
                
                while (L->next != NULL){
                    L = L->next;
                    i++; 
                }
                if (i != currentCmd){
                    perror("efficiency");
                    exit(126);
                }
                L->pcpu = atoi(token);
                token = strtok(NULL, " ");
            break;
            
            case 6: /*set job control directive*/
                jobGroup = token[1] - 48;
                //printf("group: %d\n\n\n", jobGroup);
                token = strtok(NULL, " ");
                //all tokens from now on need to be job group 7
            break;
            
            case 7: /*enforce directive command*/ //jkill
                groupD[0] = 7;
                groupD[1] = jobGroup;
                return groupD;
            
            case 8: //jstop
                groupD[0] = 8;
                groupD[1] = jobGroup;
                return groupD;
                
            case 9: //jcont
                groupD[0] = 9;
                groupD[1] = jobGroup;
                return groupD;
            
            default: /*add arguement to current command*/ //not getting here after pipe
            
                if (identify_token(token) != 0){
                    printf("error: identifier '%s' caught but not handled\n", identifier);
                    exit(126);
                }
                
                insert_arg(L, token);
                token = strtok(NULL, " ");
            break;
            
        }
        L = lCopy;
    }
    if (jobGroup > -1){
        while (L->next != NULL){
            L->group = jobGroup;
            L = L->next;
        }
        L->group = jobGroup;
    }
return groupD;
}

void print_list(struct command *L){
    int i;
//print all commands and their variables
    printf("\n");
    while (L->next != NULL){
        printf("\n");
        for (i=0; i<L->argc+1; i++)
            printf("argv: %s \n",L->argv[i]);
    
        printf("cmd: %s\n", L->cmd);
        printf("argc: %d\n", L->argc);
        printf("pcpu: %d\n", L->pcpu);
        printf("bg: %d\n", L->background);
        printf("pipe: %d\n", L->pipe);
        printf("in: %s\n", L->infile);
        printf("out: %s\n", L->outfile);
        printf("group: %d\n", L->group);
        printf("pid: %d\n", L->pid);
        printf("pos: %d\n", L->position);
        
        L = L->next;
    }
    printf("\n");
    for (i=0; i<L->argc+1; i++)
            printf("argv: %s \n",L->argv[i]);
    
    printf("cmd: %s\n", L->cmd);
    printf("argc: %d\n", L->argc);
    printf("pcpu: %d\n", L->pcpu);
    printf("bg: %d\n", L->background);
    printf("pipe: %d\n", L->pipe);
    printf("in: %s\n", L->infile);
    printf("out: %s\n", L->outfile);
    printf("group: %d\n", L->group);
    printf("pid: %d\n", L->pid);
    printf("pos: %d\n", L->position);
}
