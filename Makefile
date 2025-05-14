all: dcooke_parser

dcooke_parser: front.o parser.o
	$(CC) -Wall front.o parser.o -o dcooke_parser

.c.o:
	$(CC) -Wall -c $<


clean:
	$(RM) parser.o front.o dcooke_parser
