#ifndef FRONT_H
#define FRONT_H

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define NOTKNOWN 99

/* Token codes */

#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define KEY_READ 98
#define KEY_WRITE 97
#define KEY_WHILE 96
#define KEY_DO 95
#define LEFT_CBRACE 94
#define RIGHT_CBRACE 93
#define DEC_OP 92
#define INC_OP 91
#define SEMICOLON 90
#define LEQUAL_OP 89
#define GEQUAL_OP 88
#define EQUAL_OP 87
#define NEQUAL_OP 86
#define LESSER_OP 85
#define GREATER_OP 84
#define UNKNOWN 83


int lex();
char* getTokenName(int token);



#endif
