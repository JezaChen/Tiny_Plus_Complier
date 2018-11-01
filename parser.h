//
// Created by jeza on 18-11-1.
//

#ifndef TINY_LUS_PARSER_H
#define TINY_LUS_PARSER_H

#include "globals.h"

TreeNode *parse();


TreeNode *decl();
TreeNode *mul_exp();
TreeNode *add_exp();
TreeNode *stmt_sequence();
TreeNode *read_stmt();
TreeNode *if_stmt_helper();
TreeNode *varlist();
TreeNode *comparison_exp_helper();
TreeNode *logical_and_exp();
TreeNode *program();
TreeNode *assign_stmt();
TreeNode *statement();
TreeNode *factor();
TreeNode *logical_or_exp();
TreeNode *add_exp_helper();
TreeNode *if_stmt();
TreeNode *addop();
TreeNode *mul_exp_helper();
TreeNode *logical_and_exp_helper();
TreeNode *stmt_sequence_helper();
TreeNode *write_stmt();
TreeNode *type_specifier();
TreeNode *repeat_stmt();
TreeNode *declarations();
TreeNode *varlist_helper();
TreeNode *comparison_exp();
TreeNode *while_stmt();
TreeNode *mulop();
TreeNode *logical_or_exp_helper();
TreeNode *comparison_op();



#endif //TINY_LUS_PARSER_H
