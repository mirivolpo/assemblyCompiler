CC = gcc
CFLAGS = -Wall -ansi -pedantic
OBJFILES = firstPass.o secondPass.o assembler.o label.o mcr.o machineCode.o helpFunctions.o
HDRFILES = firstPass.h secondPass.h assembler.h label.h mcr.h machineCode.h helpFunctions.h

assembler: $(OBJFILES)
	$(CC) $(CFLAGS) -o assembler $(OBJFILES)
helpFunctions.o: helpFunctions.c helpFunctions.h
	$(CC) $(CFLAGS) -c helpFunctions.c
mcr.o: mcr.c mcr.h helpFunctions.h
	$(CC) $(CFLAGS) -c mcr.c

firstPass.o: firstPass.c firstPass.h mcr.h secondPass.h
	$(CC) $(CFLAGS) -c firstPass.c

secondPass.o: secondPass.c secondPass.h machineCode.h
	$(CC) $(CFLAGS) -c secondPass.c

assembler.o: assembler.c assembler.h firstPass.h
	$(CC) $(CFLAGS) -c assembler.c

label.o: label.c label.h mcr.h
	$(CC) $(CFLAGS) -c label.c

machineCode.o: machineCode.c machineCode.h label.h
	$(CC) $(CFLAGS) -c machineCode.c


clean:
	rm -f *.o assembler
