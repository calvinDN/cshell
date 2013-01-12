/********************************************
* File Name : ListInterface.h
* Purpose : Contains function declarations.
* Creation Date : 27-02-2012
* Last Modified : Tue 06 Mar 2012 09:43:18 PM EST
* Created By :  Calvin Nichols  
********************************************/

#include "ListType.h"

/************************************
* Subroutine: add_command
* Purpse: add and initialize a node to the linked list
* Preconditions: list
* Postconditions: old list + 1
*************************************/
extern void add_command(struct command *L);

/************************************
* Subroutine: initialize_list
* Purpse: initialize first node in linked list
* Preconditions: list
* Postconditions: initialized list
*************************************/
extern void initialize_list(struct command *L);

/************************************
* Subroutine: insert_arg
* Purpse: add a command line arguement to the list
* Preconditions: list
* Postconditions: L->argv != NULL, L->argc != 0, L->cmd != NULL
*************************************/
extern void insert_arg(struct command *L, char *token);

/************************************
* Subroutine: free_proclist
* Purpse: free a list of commands
* Preconditions: list
* Postconditions: empty list
*************************************/
extern void free_proclist(struct command *L);

/************************************
* Subroutine: list_size
* Purpse: return size of list
* Preconditions: list
* Postconditions: none
*************************************/
extern int list_size(struct command *L);

/************************************
* Subroutine: identify_token
* Purpse: identify command line tokens
* Preconditions: list, token
* Postconditions: none
*************************************/
extern int identify_token(char *token);

/************************************
* Subroutine: parse_cmdline
* Purpse: parse command line
* Preconditions: list, command line
* Postconditions: none
* Reference: CIS2500, lab4 by Calvin Nichols (me)
*************************************/
extern int *parse_cmdline(struct command *L, char *cmdLine);

/************************************
* Subroutine: print_list
* Purpse: print all struct elements in a list
* Preconditions: list
* Postconditions: none
*************************************/
extern void print_list(struct command *L);

/************************************
* Subroutine: free_listofproclist
* Purpse: free the 
* Preconditions: 
* Postconditions: 
*************************************/
extern void free_listofproclist(struct command **L);
