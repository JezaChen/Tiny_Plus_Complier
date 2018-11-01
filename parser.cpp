#include "globals.h"
#include "parser.h"
#include "util.h"
#include "scan.h"
#include "errors.h"
//
// Created by jeza on 18-11-1.
//

/**当前读到的token**/
static TokenType token;
static int lineno;

void throw_syntax_error(const ERROR_CODE &error_code, const int &lineno, char *error_details = nullptr) {
    printError(error_code, lineno, error_details);
}

static void match(TokenType expectedToken) {
    if (token == expectedToken)
        token = getToken(lineno).first;
    else {
        throw_syntax_error(SYNTAX_ERROR, lineno); //TODO
    }
}

TreeNode *program() {

    if (token == TK_INT || token == TK_BOOL || token == TK_STRING ||
        token == TK_WHILE || token == TK_IF || token == TK_REPEAT ||
        token == TK_READ || token == TK_WRITE || token == ID) {
        declarations();
        stmt_sequence();
    } else {
        throw_syntax_error(SYNTAX_ERROR, lineno);
    }
}

TreeNode *declarations() {
    if (token == TK_INT || token == TK_BOOL || token == TK_STRING) {
        decl();
        match(TK_SEMICOLON);
        declarations();
    } else if (token == TK_WHILE || token == TK_IF || token == TK_REPEAT ||
            token == TK_READ || token == TK_WRITE || token == ID) {
        return nullptr;
    } else {
        throw_syntax_error(SYNTAX_ERROR, lineno);
    }

}

TreeNode *parse() {
    return nullptr;
}




