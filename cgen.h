//
// Created by jeza1 on 2018/11/21.
//

#ifndef TINY_LUS_CGEN_H
#define TINY_LUS_CGEN_H

#include "globals.h"



void codeGen(TreeNode* syntaxTree);
void traversal_back(TreeNode *root);

int emit(CodeType type, const std::string &t = "", const std::string &s1 = "", const std::string &s2 = "");
void cgen();

#endif //TINY_LUS_CGEN_H
