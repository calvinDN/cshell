/********************************************
* File Name : ShellInterface.c
* Purpose : Contains function declarations.
* Creation Date : 27-02-2012
* Last Modified : Tue 06 Mar 2012 09:43:18 PM EST
* Created By :  Calvin Nichols  
********************************************/

/************************************
* Subroutine: reset_cmdline
* Purpse: clear command line
* Preconditions: command line
* Postconditions: none
*************************************/
extern void reset_cmdline(char *cmdLine);

/************************************
* Subroutine: io_redirect
* Purpse: redirect input and output
* Preconditions: input or output redirect
* Postconditions: none
*************************************/
extern void io_redirect(struct command *L);

/************************************
* Subroutine: exec_cmdline
* Purpse: execute command line arguements
* Preconditions: list
* Postconditions: none
* Reference: http://stackoverflow.com/questions/1618756/how-do-i-exec-a-process-in-the-background-in-c
*            reapChildren.c seminar 3 Scott Dougan
*************************************/
extern void exec_cmdline(struct command *L);

/************************************
* Subroutine: piping
* Purpse: pipe programs
* Preconditions: pipe
* Postconditions: none
* Reference: parentPipe.c & childPipe.c seminar 3 scott dougan
*            http://www.dgp.toronto.edu/~ajr/209/notes/procfiles/pipe-example.c
*            http://stackoverflow.com/questions/8300301/chained-pipes-in-linux-using-pipe
*************************************/
extern void piping(struct command *L);

/************************************
* Subroutine: directive
* Purpse: kill, stop or continue currently running processes
* Preconditions: directive
* Postconditions: none
*************************************/
extern void directive(struct command *L, int direc);

/************************************
* Subroutine: alarm_handler
* Purpse: check efficiency of a process
* Preconditions: efficiency
* Postconditions: none
* Reference: http://cboard.cprogramming.com/c-programming/138057-waitpid-non-blocking-fork.html
*************************************/
extern void alarm_handler(int sig);
