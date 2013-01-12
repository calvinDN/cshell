CALVIN NICHOLS  

EXECUTION NOTES:
Because of my parsing method I have one alteration to my input.
input and output redirection are always specified directly after the program name even while specifying multiple pipes, for example:
program1 ^ infile $ outfile
pargram1 ^ infile -> program2 -> program3 $ outfile

Job Control must always be executed with background ("!") on.
Only job groups from 0 to 9 are supported.

COMPILATION:
make
make all
make ossh

REMOVE .o FILES:
make clean

SUBMITTED FILES:
ossh.c                      -main c file
ShellImplementation.c       -shell functions
ShellInterface.h            -shell function header file
ListImplementation.c        -list functions
ListInterface.h             -list function header file
ListType.h                  -list struct type
README.txt                  -readme
a2_docs.pdf                 -documentation descibing assignment 2
makefile                    -makefile

REFERENCES:
background process
http://stackoverflow.com/questions/1618756/how-do-i-exec-a-process-in-the-background-in-c
reapChildren.c seminar 3 scott dougan

shell input
http://linuxgazette.net/111/ramankutty.html

parsing
CIS2500, lab4 by Calvin Nichols (me)

piping
parentPipe.c & childPipe.c seminar 3 scott dougan
http://www.dgp.toronto.edu/~ajr/209/notes/procfiles/pipe-example.c

efficiency waiting
http://cboard.cprogramming.com/c-programming/138057-waitpid-non-blocking-fork.html

multiple pipes
http://stackoverflow.com/questions/8300301/chained-pipes-in-linux-using-pipe

linked list
http://www.c.happycodings.com/Data_Structures/code5.html
