ossh: ossh.o ShellImplementation.o ListImplementation.o
	gcc ossh.o ShellImplementation.o ListImplementation.o -o ossh

all: ossh.o ShellImplementation.o ListImplementation.o
	gcc ossh.o ShellImplementation.o ListImplementation.o -o ossh

ossh.o: ossh.c ListInterface.h ShellInterface.h ListType.h 
	gcc -c ossh.c

ShellImplementation.o: ShellImplementation.c ShellInterface.h ListType.h 
	gcc -c ShellImplementation.c

ListImplementation.o: ListImplementation.c ListInterface.h ListType.h
	gcc -c ListImplementation.c
	
clean:
	@ rm *.o
