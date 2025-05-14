#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "front.h"
#include "ctype.h"
#include <stdlib.h>

void error();
void errorEOF();
void whileCode();

// non literals
void S();
void V();
void E();
void C();
void T();
void F();
void O();
void N();
void TPrime();
void EPrime();

int errorFlag = 0;
int lexFlag = 0;

// Here is the BNF rules for this parser

/* P:: = S
 * S:: = V=E | read(V) | write(V) | do{S} while(C) | S;S
 * C:: = E<E| E>E| E==E| E!=E| E<=E| E>=E
 * E:: = T| E+T| E-T  --------> E::= TE'  and E' = +TE' | -TE' | epsilon
 * T:: = F| T*F| T/F  --------> T::= FT'  and T'::= *FT' | /FT' | epsilon
 * F:: = (E)|O|N|V
 * O:: = V++| V--
 * V:: = a | b | ... | y | z | aV| bV| ... | yV| zV
 * N:: = 0 | 1 | ... | 8 | 9 | 0N| 1N| ... | 8N| 9N
 * */


// syntax error displayer
void error()
{
    if(nextToken != EOF){
        printf("Error encounter: The next lexeme was %s and the next token was %s\n", lexeme, getTokenName(nextToken));
        errorFlag = 1; // set the flag to 1 if there is any syntax error
        lexFlag = 1;   // this flag is to recover from a syntax error
                      // the parser will not call lex(); in side P(); if there was an error
    }
}

// syntax error displayer for EOF when there is a semicolon at the end of last statement
void errorEOF()
{
    printf("Error encounter: The next lexeme was %s and the next token was %s\n", lexeme, getTokenName(nextToken));
    errorFlag = 1;

}


// P:: = S */
void P(){
    if(lexFlag != 0){
        S();
    }else{
        lex();
        S();
    }
}
//

//* S:: V=E| read(V)  | write(V) | do{S} while(C)| S;S */

void S() {
    if(nextToken == IDENT){
        lex();
        if(nextToken == ASSIGN_OP){
            lex();
            E();
            if(nextToken != SEMICOLON) {
                // check the scenarios where the last assignment statement ends without semicolon
                if (nextToken == RIGHT_CBRACE) {
                    lex();
                    whileCode();
                }else if(nextToken == EQUAL_OP){
                    error();
                    lex();
                    E();
                    if(nextToken == SEMICOLON){
                        lex();
                        S();
                    }else{
                        error();
                    }
                }else if(nextToken == RIGHT_PAREN){
                    error();
                    lex();
                    TPrime();
                    if(nextToken == SEMICOLON){
                        lex();
                        S();
                    }else{
                        error();
                    }
                }
                else{
                    error();
                    S();
                }
            }
            else{
                lex();
                if(nextToken == RIGHT_CBRACE){
                    error();
                    whileCode();
                }else if(nextToken == EOF){
                    errorEOF();
                } else {
                    S();
                }
            }
        }
        else{
            error();
        }
    }
    else if(nextToken == KEY_READ){
        lex();
        if(nextToken == LEFT_PAREN){
            lex();
            V();
            if(nextToken == RIGHT_PAREN){
                lex();
                if(nextToken != SEMICOLON){
                    if (nextToken == RIGHT_CBRACE) {
                        lex();
                        whileCode();
                    }
                    else {
                        S();
                    }
                }
                else{
                    lex();
                    if(nextToken == EOF){
                        errorEOF();
                    } else {
                        S();
                    }
                }
            }
            else{
                error();
            }
        }
        else{
            error();
        }
    }
    else if(nextToken == KEY_WRITE){
        lex();
        if(nextToken == LEFT_PAREN){
            lex();
            V();
            if(nextToken == RIGHT_PAREN){
                lex();
                if(nextToken != SEMICOLON){
                    if (nextToken == RIGHT_CBRACE) {
                        lex();
                        whileCode();
                    }
                    else {
                        S();
                    }
                }
                else{
                    lex();
                    if(nextToken == EOF){
                        errorEOF();
                    } else {
                        S();
                    }
                }
            }
            else{
                error();
            }
        }
        else{
            error();
        }
    }
    else if(nextToken == KEY_DO){
        lex();
        if(nextToken == LEFT_CBRACE){
            lex();
            S();
            if(nextToken == RIGHT_CBRACE){
                lex();
                whileCode();
            }
            else{
                error();
            }
        }
        else{
            error();
        }
    }
    else if(nextToken == SEMICOLON){ // condition to handle empty statements
        lex();
        if (nextToken == EOF) {
            errorEOF();
        }
        else if(nextToken == RIGHT_CBRACE){
            lex();
            whileCode();
        }
        else {
            S();
        }
    }
    else{

        error();
        lex();
    }
}

///* C:: = E<E| E>E| E==E| E!=E| E<=E| E>=E */
void C(){
    E();
    if(nextToken == LESSER_OP || nextToken == GREATER_OP || nextToken == EQUAL_OP || nextToken == NEQUAL_OP || nextToken == LEQUAL_OP || nextToken == GEQUAL_OP){
        lex();
        E();
    }
    else{
        error();
    }
}

/* E:: = T| E+T| E-T */
void E(){
    T();
    EPrime();
}

/* E' = +TE' | -TE' | epsilon */
void EPrime(){
    if(nextToken == ADD_OP || nextToken == SUB_OP){
        lex();
        T();
        EPrime();
    }
    // where what supposed to be an operator is UNKNOWN, recover from error
    else if(nextToken == UNKNOWN){
        error();
        lex();
        T();
        EPrime();
    }

    // to recover from a specific syntax error
    // where parser was expecting "+" or " or "," or ","/", it finds "="
    else if(nextToken == ASSIGN_OP){
        error();
        lex();
        T();
        EPrime();
    }
}

/* T:: = F| T*F| T/F */
void T(){
    F();
    TPrime();
}

/* T'::= *FT' | /FT' | epsilon */
void TPrime(){
    if(nextToken == MULT_OP || nextToken == DIV_OP){
        lex();
        F();
        TPrime();
    }
}

/* F:: = (E)|O|N|V */
void F(){
    if(nextToken == LEFT_PAREN){
        lex();
        E();
        if(nextToken == RIGHT_PAREN){
            lex();
        }
        else{
            if(nextToken != SEMICOLON) {
            error();
            }
        }
    }
    else if(nextToken == IDENT){
        lex();
        O();
    }
    else if(nextToken == INT_LIT){
        N();
    }
    else{
        if(nextToken != LESSER_OP){
        error();
    }}

}

//* O:: = V++| V-- */
void O(){
    if(nextToken == INC_OP || nextToken == DEC_OP){
        lex();
    }
}

// V:: = a | b | ... | y | z | aV| bV| ... | yV| zV
void V() {
    int flag = 0;
    int i;
    for (i = 0; i < strlen(lexeme); ++i) {
        if(isupper(lexeme[i]) || (!(isalpha(lexeme[i])))){
            flag = 1;
            break;
        }
    }
    if((flag == 0) && (nextToken != KEY_DO && nextToken != KEY_WRITE && nextToken != KEY_READ && nextToken != KEY_WHILE)){
        lex();
    }
    else{
        error();
    }
}

// N:: = 0 | 1 | ... | 8 | 9 | 0N| 1N| ... | 8N| 9N
void N(){
    int flag = 0;
    int i;
    for (i = 0; i < strlen(lexeme); ++i) {
        if(!isdigit(lexeme[i])){
            flag = 1;
        }
    }
    if(flag == 0){
        lex();
    }
    else{
        error();
    }
}

// since while keyword was used a lot, I made a function for it for efficiency
void whileCode(){
    if(nextToken == KEY_WHILE){
        lex();
        if(nextToken == LEFT_PAREN){
            lex();
            C();
            if(nextToken == RIGHT_PAREN){
                lex();
                if(nextToken != SEMICOLON){
                    if(nextToken == RIGHT_CBRACE){
                        lex();
                        whileCode();
                    }
                    else {
                        S();
                    }
                }
                else{

                    lex();
                    whileCode();
                    if(nextToken == EOF){
                        errorEOF();
                    } else {
                        S();
                    }

                }
            }
            else{   // if error occurs inside the condition of while loop, then
                    // get the next token until the right parenthesis.
                //if(nextToken != (LESSER_OP)){
                    error();
                //}
                while(nextToken != RIGHT_PAREN){
                    lex();
                }
                if(nextToken == RIGHT_PAREN){
                    lex();
                    if(nextToken != SEMICOLON){
                        if(nextToken == RIGHT_CBRACE){
                            lex();
                            whileCode();
                        }
                        else {
                            S();
                        }
                    }
                    else{

                        lex();
                        if(nextToken == EOF){
                            errorEOF();
                        } else {
                            S();
                        }
                    }
                }

            }
        }
        else{
            error();
        }
    }
}
