#include "globals.h"
#include "util.h"
#include "errors.h"

//bool has_error;
void printToken(TokenType token, const char *tokenString, const int &lineno) {

    switch (token) {
        case TK_TRUE:
        case TK_FALSE:
        case TK_OR:
        case TK_AND:
        case TK_NOT:
        case TK_INT:
        case TK_BOOL:
        case TK_STRING:
        case TK_WHILE:
        case TK_DO:
        case TK_IF:
        case TK_THEN:
        case TK_ELSE:
        case TK_END:
        case TK_REPEAT:
        case TK_UNTIL:
        case TK_READ:
        case TK_WRITE:
            fprintf(listing, "(KEY, %s)\n", tokenString);
            break;
        case TK_GTR:
            fprintf(listing, "(TK_GTR, >)\n");
            break;
        case TK_LEQ:
            fprintf(listing, "(TK_LEQ, <=)\n");
            break;
        case TK_GEQ:
            fprintf(listing, "(TK_GEQ, >=)\n");
            break;
        case TK_COMMA:
            fprintf(listing, "(TK_COMMA, ,)\n");
            break;
        case TK_SEMICOLON:
            fprintf(listing, "(TK_SEMICOLON, ;)\n");
            break;
        case TK_ASSIGN:
            fprintf(listing, "(TK_ASSIGN, :=)\n");
            break;
        case TK_ADD:
            fprintf(listing, "(TK_ADD, +)\n");
            break;
        case TK_SUB:
            fprintf(listing, "(TK_SUB, -)\n");
            break;
        case TK_MUL:
            fprintf(listing, "(TK_MUL, *)\n");
            break;
        case TK_DIV:
            fprintf(listing, "(TK_DIV, /)\n");
            break;
        case TK_LP:
            fprintf(listing, "(TK_LP, ()\n");
            break;
        case TK_RP:
            fprintf(listing, "(TK_RP, ))\n");
            break;
        case TK_LSS:
            fprintf(listing, "(TK_LSS, <)\n");
            break;
        case TK_EQU:
            fprintf(listing, "(TK_EQU, =)\n");
            break;
        case ID:
            fprintf(listing, "(ID, %s)\n", tokenString);
            break;
        case NUM:
            fprintf(listing, "(NUM, %s)\n", tokenString);
            break;
        case ERROR:
            fprintf(listing, "\033[1;;31mAn Error is detected at line %d: %s \033[0m\n", lineno, tokenString);
            break;
        case STRING:
            fprintf(listing, "(STR, %s)\n", tokenString);
            break;
        default:
            fprintf(listing, "Unknown token: %d\n", token);
    }
}

void printError(const int &error_code, const int &lineno, char *error_details) {
    if (listing == stdout) //输出到控制台可以带有颜色
        fprintf(listing, "\033[1;;31mAn Error is detected at line %d: %s \033[0m\n", lineno,
                error_items[error_code].error_description.c_str());
    else
        fprintf(listing, "An Error is detected at line %d: %s\n", lineno,
                error_items[error_code].error_description.c_str());

    if (error_code != SEMANTIC_MISSING_SEMICOLON) //如果报的错是严重错误，就不生成中间代码文件了
        has_error = true;
}


