//
// Created by jeza on 18-11-1.
//

#ifndef TINY_LUS_PARSER_H
#define TINY_LUS_PARSER_H

#include "globals.h"

TreeNode *parse();



TreeNode *mul_exp();
TreeNode *add_exp();
TreeNode *stmt_sequence();
TreeNode *read_stmt();
TreeNode *logical_and_exp();
TreeNode *program();
TreeNode *assign_stmt(Token id_token);
TreeNode *factor();
TreeNode *logical_or_exp();
TreeNode *if_stmt();
TreeNode *write_stmt();
TreeNode *repeat_stmt();
TreeNode *declarations();
TreeNode *comparison_exp();
TreeNode *while_stmt();




#endif //TINY_LUS_PARSER_H
