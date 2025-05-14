#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "front.h"
#include "parser.h"

char* getTokenName(int token);

/* Global Variable */

int nextToken;
char lexeme [100];


/* Local Variables */
static int charClass;
static char nextChar;
static FILE *in_fp;
static int lexLen;


/* Local Function declarations */
static void addChar();
static void getChar();
static void getNonBlank();

/******************************************************/
/* main driver */
int main(int argc, char* argv[])
{
    // print my identification
    printf("%s", "DCooke Analyzer :: R11800534\n\n");


    // we need to check if the file was provided
    if (argc != 2) {
        // if no file was provided as an input print an error
        printf("Error: No input file provided.\n");
        return 2; // Exit code 2 for missing file
    }

    /* Open the input data file and process its contents */
    if ((in_fp = fopen(argv[1], "r")) == NULL) {
        printf("Error: The provided file does not exist");
        return 3; // Exit code 3 for non-existent file
    } else {
        getChar();
        do {
            P();
        } while (nextToken != EOF);
    }



    if(errorFlag == 0){
        printf("Syntax Validated\n");
    }else{
        // message for syntax error encounter
        printf("Invalid Syntax");
        return 1; // Exit code 1 for syntax error
    }
    return 0;
}



/*****************************************************/
/* lookup - a function to lookup operators and parentheses and return the
 * token */
static int lookup(char ch) {
    switch (ch) {
        case '(':
            addChar();
            nextToken = LEFT_PAREN;
            break;
        case ')':
            addChar();
            nextToken = RIGHT_PAREN;
            break;

        case '=':
            addChar();
            getChar();          // get the next character to check
                                // if the next character is an assigment op as well

            if(nextChar == '='){
                addChar();
                nextToken = EQUAL_OP;
            }
            else{
                ungetc(nextChar,in_fp);  // We must go one character back if
                                                  // the nextChar was not the same as
                                                  // compared character. If we do not
                                                  // do this step, the lexer will skip
                                                  // a character.
                nextToken = ASSIGN_OP;
            }
            break;

        case '+':

            addChar();
            getChar();
            // printf("%c",nextChar);
            if(nextChar == '+') {
                addChar();
                nextToken = INC_OP;
            }else{
                ungetc(nextChar,in_fp);
                nextToken = ADD_OP;
            }
            break;
        case '-':
            addChar();
            getChar();
            if(nextChar == '-'){
                addChar();
                nextToken = DEC_OP;
            }
            else{
                ungetc(nextChar,in_fp);
                nextToken = SUB_OP;
            }
            break;
        case '*':
            addChar();
            nextToken = MULT_OP;
            break;
        case '/':
            addChar();
            nextToken = DIV_OP;
            break;

        case '{':
            addChar();
            nextToken = LEFT_CBRACE;
            break;

        case '}':
            addChar();
            nextToken = RIGHT_CBRACE;
            break;

        case '<':
            addChar();
            getChar();
            if(nextChar == '='){
                addChar();
                nextToken = LEQUAL_OP;
            }else{
                ungetc(nextChar,in_fp);
                nextToken = LESSER_OP;
            }
            break;

        case '>':
            addChar();
            getChar();
            if(nextChar == '='){
                addChar();
                nextToken = GEQUAL_OP;
            }else{
                ungetc(nextChar,in_fp);
                nextToken = GREATER_OP;
            }
            break;

        case '!':
            addChar();
            getChar();
            if(nextChar == '=') {
                addChar();
                nextToken = NEQUAL_OP;
            }
            else{
                ungetc(nextChar,in_fp);
                nextToken = UNKNOWN;
                break;

                case ';':
                    addChar();
                nextToken = SEMICOLON;
                break;
                default:
                    addChar();
                nextToken = UNKNOWN;       // in case the character is unknown
                                           // assign nextToken an UNKNOWN value
                break;
            }
            return nextToken;
    }
}

/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
static void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    } else {
        printf("Error - lexeme is too long \n");
    }
}

/*****************************************************/
/* getChar - a function to get the next character of input and determine its
 * character class */
static void getChar() {
    if ((nextChar = getc(in_fp)) != EOF) {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else charClass = NOTKNOWN;
    } else {
        charClass = EOF;
    }
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it returns a non-whitespace
 * character */
static void getNonBlank() {
    while (isspace(nextChar)) getChar();
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic expressions */
int lex() {

    lexLen = 0;
    getNonBlank();

    switch (charClass) {
        /* Parse identifiers */
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            // after getting the lexeme, compare it for key-words
            // to see if the lexeme is a keyword
            if (strcmp(lexeme, "while") == 0) {
                nextToken = KEY_WHILE;
            } else if (strcmp(lexeme, "do") == 0) {
                nextToken = KEY_DO;
            } else if (strcmp(lexeme, "write") == 0) {
                nextToken = KEY_WRITE;
            } else if (strcmp(lexeme, "read") == 0) {
                nextToken = KEY_READ;
            } else {
                // if not keyword, then it is an identifier
                nextToken = IDENT;
            }
            break;

            /* Parse integer literals */
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;

            /* Parentheses, curly braces and operators */
        case NOTKNOWN:
            lookup(nextChar);
            getChar();
            break;

            /* EOF */
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
            break;
    } /* End of switch */
//    depending on token integer value, print according token name for a lexeme
    if(nextToken != EOF){
        printf("%s\t%s\n",lexeme, getTokenName(nextToken));
    }

    return nextToken;
} /* End of function lex */

char* getTokenName(int token) {
    switch (token) {
        case INT_LIT:
            return "INT_LIT";
        case IDENT:
            return "IDENT";
        case ASSIGN_OP:
            return "ASSIGN_OP";
        case ADD_OP:
            return "ADD_OP";
        case SUB_OP:
            return "SUB_OP";
        case MULT_OP:
            return "MULT_OP";
        case DIV_OP:
            return "DIV_OP";
        case LEFT_PAREN:
            return "LEFT_PAREN";
        case RIGHT_PAREN:
            return "RIGHT_PAREN";
        case KEY_READ:
            return "KEY_READ";
        case KEY_WRITE:
            return "KEY_WRITE";
        case KEY_WHILE:
            return "KEY_WHILE";
        case KEY_DO:
            return "KEY_DO";
        case LEFT_CBRACE:
            return "LEFT_CBRACE";
        case RIGHT_CBRACE:
            return "RIGHT_CBRACE";
        case DEC_OP:
            return "DEC_OP";
        case INC_OP:
            return "INC_OP";
        case SEMICOLON:
            return "SEMICOLON";
        case LEQUAL_OP:
            return "LEQUAL_OP";
        case GEQUAL_OP:
            return "GEQUAL_OP";
        case EQUAL_OP:
            return "EQUAL_OP";
        case NEQUAL_OP:
            return "NEQUAL_OP";
        case LESSER_OP:
            return "LESSER_OP";
        case GREATER_OP:
            return "GREATER_OP";
        default:
            return "UNKNOWN";
    }
}