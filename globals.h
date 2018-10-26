#ifndef TINY_LUS_GLOBAL_H
#define TINY_LUS_GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define MAXRESERVED 18 // 关键字最长长度

typedef enum {
    ENDFILE, ERROR,
    /**Reversed Tokens**/
    TK_TRUE, TK_FALSE, TK_OR, TK_AND,
    TK_NOT, TK_INT, TK_BOOL, TK_STRING,
    TK_WHILE, TK_DO, TK_IF, TK_THEN,
    TK_ELSE, TK_END, TK_REPEAT, TK_UNTIL,
    TK_READ, TK_WRITE,
    /**Multi-character tokens**/
    ID, NUM, STRING,
    /**Special Symbols**/
    TK_GTR, TK_LEQ, TK_GEQ, TK_COMMA, TK_SEMICOLON,
    TK_ASSIGN, TK_ADD, TK_SUB, TK_MUL, TK_DIV, TK_LP,
    TK_RP, TK_LSS, TK_EQU
}TokenType;

extern FILE* source; /* source code text file */
extern FILE* listing; /* listing output text file */
extern FILE* code; /* code text file for TM simulator */
extern int lineno; /* source line number for listing */

extern int TraceScan;
#endif //TINY_LUS_GLOBAL_H
