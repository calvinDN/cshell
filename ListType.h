/********************************************
* File Name : ListType.h
* Purpose : Includes list struct.
* Creation Date : 27-02-2012
* Last Modified : Tue 06 Mar 2012 09:43:18 PM EST
* Created By :  Calvin Nichols  
********************************************/

struct command {
    char **argv;                /*command line arguements*/
    char *cmd;                  /*argv[0]*/
    int argc;                   /*argument count*/
    int pcpu;                   /*percentage of cpu, efficiency*/
    int background;             /*background process, 0 off, 1 on*/
    int pipe;                   /*pipe, 0 off, 1 on*/
    char *infile;               /*input redirection*/
    char *outfile;              /*output redirection*/
    int group;                  /*group id*/
    int pid;                    /*save pid of process*/

    int position;               
    struct command *next;       
};
