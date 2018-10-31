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

void throw_syntax_error(const ERROR_CODE &error_code, const int& lineno, char *error_details = nullptr) {
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
    
}

TreeNode parse() {
    return TreeNode(<#initializer#>);
}




