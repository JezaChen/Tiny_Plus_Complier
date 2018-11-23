#include <string>
#include <iostream>
#include "globals.h"
#include "parser.h"
#include "util.h"
#include "scan.h"
#include "errors.h"
//
// Created by jeza on 18-11-1.
//

/**当前读到的token**/
//static TokenType token;
//static std::string tokenString;
static Token token;
SymTable symTable;

void throw_syntax_error(const ERROR_CODE &error_code, const int &lineno, char *error_details = nullptr) {
    printError(error_code, lineno);
}

static bool match(TokenType expectedToken) {
    if (token.kind == expectedToken) {
        auto tmp = getToken(lineno);
        token.kind = tmp.first;
        token.i_val = 0;
        token.s_val = "";
        if (token.kind == NUM)
            token.i_val = atoi(tmp.second.c_str());
        else if (token.kind == STRING || token.kind == ID) {
            token.s_val = tmp.second;
            return true;
        }
    } else {
        throw_syntax_error(SYNTAX_ERROR, lineno); //TODO
        return false;
    }

}

static bool match_one(const std::vector<TokenType> &list) {
    for (auto &item : list) {
        if (item == token.kind) {
            match(item);
            return true;
        }
    }
    return false;
}

static bool token_is_in(const std::vector<TokenType> &list) {
    for (auto &item : list) {
        if (item == token.kind) {
            return true;
        }
    }
    return false;
}

void nextToken() {
    auto tmp = getToken(lineno);
    token.kind = tmp.first;
    if (token.kind == TK_INT)
        token.i_val = atoi(tmp.second.c_str());
    else if (token.kind == TK_STRING || token.kind == ID) {
        token.s_val = tmp.second;
    }
}

Token *cp_token() {
    Token *ret = new Token;
    ret->s_val = token.s_val;
    ret->kind = token.kind;
    ret->i_val = token.i_val;
    return ret;
}

Token *cp_token(const Token &will_be_copied_token) {
    Token *ret = new Token;
    ret->s_val = will_be_copied_token.s_val;
    ret->kind = will_be_copied_token.kind;
    ret->i_val = will_be_copied_token.i_val;
    return ret;
}

TreeNode *program() {
    declarations();
    return stmt_sequence();
}

TreeNode *declarations() {
    while (token.kind == TK_INT || token.kind == TK_BOOL || token.kind == TK_STRING) {
        Token type = token; //保存类型的符号
        // nextToken(); //跳过类型声明
        do {
            nextToken(); //跳过类型声明
            Token id = token; //保存标识符的符号
            if (match(ID)) {
                if (symTable.findSym(id.s_val)) //如果重复声明，报错
                    throw_syntax_error(SEMANTIC_MULTIPLE_DECLARATIONS, lineno);

                Sym *sym = symTable.insertSym(id.s_val);
                sym->tk = cp_token(id); //fixed: 不加id的话会把分号、逗号的token赋值过去，原因是已经读取了下一个token
                sym->objType = OT_VAR; //变量
                switch (type.kind) { //根据type.kind类型判断valType
                    case TK_INT:
                        sym->valType = VT_INT;
                        break;
                    case TK_BOOL:
                        sym->valType = VT_BOOL;
                        break;
                    case TK_STRING:
                        sym->valType = VT_STR;
                        break;
                }
            }
        } while (token.kind == TK_COMMA);
        match(TK_SEMICOLON);
    }
}


TreeNode *stmt_sequence() {
    TreeNode *t1 = nullptr, *t2 = nullptr;
    std::vector<TokenType> stmt_first{TK_IF, TK_WHILE, TK_REPEAT, ID, TK_READ, TK_WRITE};
    Token first = token;
    bool is_semicolon_met = true;

    while (match_one(stmt_first)) {
        /***语义分析：判断是否缺失双引号***/
        if (!is_semicolon_met)
            throw_syntax_error(SEMANTIC_MISSING_SEMICOLON, lineno - 1); //由于已经读取到下一行了，需要将当前行数-1

        switch (first.kind) {
            case TK_IF:
                t2 = if_stmt(); //
                break;
            case TK_WHILE:
                t2 = while_stmt(); //
                break;
            case TK_REPEAT:
                t2 = repeat_stmt(); //
                break;
            case ID:
                t2 = assign_stmt(first); //
                break;
            case TK_READ:
                t2 = read_stmt(); //
                break;
            case TK_WRITE:
                t2 = write_stmt();
                break;
        }
        if (t1 == nullptr) {
            t1 = t2;
        } else {
            t1 = TreeNode::create_Node(STMT_SEQUENCE, t1, t2);
        }
        if (token.kind == TK_SEMICOLON) {
            match(TK_SEMICOLON);
            is_semicolon_met = true;
        } else {
            is_semicolon_met = false;
        }
        first = token;
    }
    return t1;
}

TreeNode *if_stmt() {
    TreeNode *cond_exp = nullptr, *then_stmt = nullptr, *else_stmt = nullptr;
    /***语义检查***/
    cond_exp = logical_or_exp();

    if (cond_exp->valType != VT_BOOL)
        throw_syntax_error(SEMANTIC_COND_BOOL_ERROR, lineno);
    match(TK_THEN);
    then_stmt = stmt_sequence();
    if (token.kind == TK_ELSE) { //如果还有else部分...
        match(TK_ELSE);
        else_stmt = stmt_sequence();
    }
    match(TK_END);

    return TreeNode::create_Node(IF_STMT, cond_exp, then_stmt, else_stmt);
}

TreeNode *write_stmt() {
    //TK_WRITE不再需要匹配
    TreeNode *cond_exp;
    cond_exp = logical_or_exp();
    return TreeNode::create_Node(WRITE_STMT, cond_exp);
}

TreeNode *repeat_stmt() {
    //TK_REPEAT不再需要匹配
    TreeNode *rep_stmt = nullptr, *cond_exp = nullptr;
    rep_stmt = stmt_sequence();
    match(TK_UNTIL);
    cond_exp = logical_or_exp();
    return TreeNode::create_Node(REPEAT_STMT, rep_stmt, cond_exp);
}

TreeNode *assign_stmt(Token id_token) {
    //ASSIGN不再需要匹配
    std::string id_name = id_token.s_val;
    Sym *id_sym = symTable.findSym(id_name);

    /***语义分析模块***/
    if (id_sym == nullptr) {
        throw_syntax_error(SEMANTIC_UNDEFINED_IDENTIFIER, lineno);
    }

    TreeNode *cond_exp = nullptr;
    match(TK_ASSIGN);
    cond_exp = logical_or_exp();

    /***语义分析模块***/
    if (id_sym->valType != cond_exp->valType) {
        throw_syntax_error(SEMANTIC_CANNOT_ASSIGN_DIFFERENT_TYPE, lineno);
    }

    TreeNode *ret_node = TreeNode::create_Node(ASSIGN_STMT, cond_exp);
    ret_node->tk = id_sym->tk; //fixed 给赋值节点增加一个综合属性，存储要赋值的token

    return ret_node;
}

TreeNode *read_stmt() {
    //TK_READ不再需要匹配
    Token id_token = token;
    match(ID);
    /***语义分析模块***/
    /***将read对象作为READ_STMT的tk属性，用于中间代码生成器的识别***/
    TreeNode *ret_node = TreeNode::create_Node(READ_STMT);
    ret_node->tk = cp_token(id_token);
    return ret_node;
}

TreeNode *while_stmt() {
    //TK_WHILE不再需要匹配
    TreeNode *cond_exp = nullptr, *rep_stmt = nullptr;
    cond_exp = logical_or_exp();
    /***语义分析模块***/
    if (cond_exp->valType != VT_BOOL)
        throw_syntax_error(SEMANTIC_COND_BOOL_ERROR, lineno);

    match(TK_DO);
    rep_stmt = stmt_sequence();
    match(TK_END);


    //return TreeNode::create_Node(WHILE_STMT, cond_exp, rep_stmt);
    //todo
    return TreeNode::create_Node(WHILE_STMT, cond_exp, rep_stmt);
}

TreeNode *logical_or_exp() {
    TreeNode *log_and_exp = nullptr, *log_or_exp = nullptr;
    log_and_exp = logical_and_exp();
    if (token.kind == TK_OR) {
        match(TK_OR);
        log_or_exp = logical_or_exp();
    } else {
        return log_and_exp;
    }
    TreeNode *ret_node = TreeNode::create_Node(LOG_OR_EXP, log_and_exp, log_or_exp);
    /***语义分析模块***/
    if (log_and_exp->valType != VT_BOOL || log_or_exp->valType != VT_BOOL)
        throw_syntax_error(SEMANTIC_OPERATION_BETWEEN_DIFFERENT_TYPES, lineno);
    ret_node->valType = VT_BOOL;
    return ret_node;
}

TreeNode *logical_and_exp() {
    TreeNode *log_and_exp = nullptr, *log_or_exp = nullptr;
    log_and_exp = comparison_exp();
    if (token.kind == TK_AND) {
        match(TK_AND);
        log_or_exp = logical_and_exp();
    } else {
        return log_and_exp; //减少树的深度
    }
    TreeNode *ret_node = TreeNode::create_Node(LOG_AND_EXP, log_and_exp, log_or_exp);
    /***语义分析模块***/

    //and之间两个表达式有一个不是bool类型，错误
    if (log_and_exp->valType != VT_BOOL || log_or_exp->valType != VT_BOOL) {
        throw_syntax_error(SEMANTIC_TYPE_CANNOT_BE_OPERATED, lineno);
    }
    ret_node->valType = VT_BOOL;
    return ret_node;

}

TreeNode *comparison_exp() {
    TreeNode *arith_exp = nullptr, *comp_exp = nullptr;
    arith_exp = add_exp();
    std::vector<TokenType> comp_op_list{TK_GTR, TK_EQU, TK_GEQ, TK_LEQ, TK_LSS};
    NodeType type;
    if (token_is_in(comp_op_list)) {
        switch (token.kind) {
            case TK_GTR:
                type = GTR_EXP;
                break;
            case TK_GEQ:
                type = GEQ_EXP;
                break;
            case TK_LEQ:
                type = LEQ_EXP;
                break;
            case TK_LSS:
                type = LSS_EXP;
                break;
            case TK_EQU:
                type = EQU_EXP;
                break;
        }
        match_one(comp_op_list);
        comp_exp = comparison_exp();
    }

    if (!comp_exp) //减少语法树的高度，直接返回
        return arith_exp;
    TreeNode *ret_node = TreeNode::create_Node(type, arith_exp, comp_exp);
    /***语义分析模块***/
    if (comp_exp) {
        if (!arith_exp) {
            //todo 或许可以什么都不用干
        }

            //类型不一致，没办法比较
        else if (comp_exp->valType != arith_exp->valType) {
            throw_syntax_error(SEMANTIC_OPERATION_BETWEEN_DIFFERENT_TYPES, lineno);
        }
    }
    ret_node->valType = VT_BOOL; //该节点的类型为BOOL类型了
    return ret_node;
}

TreeNode *add_exp() {
    TreeNode *mu_exp = nullptr, *ad_exp = nullptr;
    mu_exp = mul_exp();
    std::vector<TokenType> add_op_list{TK_ADD, TK_SUB};
    NodeType type;
    if (token_is_in(add_op_list)) {
        switch (token.kind) {
            case TK_ADD:
                type = ADD_EXP;
                break;
            case TK_SUB:
                type = SUB_EXP;
                break;
        }
        match_one(add_op_list);
        ad_exp = add_exp();
    }
    if (!ad_exp)
        return mu_exp;

    TreeNode *ret_node = TreeNode::create_Node(type, mu_exp, ad_exp);
    /***语义分析模块***/
    if (ad_exp) {
        //乘号用在非整型时，报错
        if (mu_exp->valType != VT_INT || ad_exp->valType != VT_INT) {
            throw_syntax_error(SEMANTIC_TYPE_CANNOT_BE_OPERATED, lineno);
        }
    }

    ret_node->valType = VT_INT;
    return ret_node;
}

TreeNode *mul_exp() {
    TreeNode *fac = nullptr, *mu_exp = nullptr;
    fac = factor();
    std::vector<TokenType> add_op_list{TK_MUL, TK_DIV};
    NodeType type;
    if (token_is_in(add_op_list)) {
        switch (token.kind) {
            case TK_MUL:
                type = MUL_EXP;
                break;
            case TK_DIV:
                type = DIV_EXP;
                break;
        }
        match_one(add_op_list);
        mu_exp = mul_exp();
    }
    if (!mu_exp)
        return fac;

    TreeNode *ret_node = TreeNode::create_Node(type, fac, mu_exp);

    /***语义分析模块***/
    if (mu_exp) {
        //乘号用在非整型时，报错
        if (fac->valType != VT_INT || mu_exp->valType != VT_INT) {
            throw_syntax_error(SEMANTIC_TYPE_CANNOT_BE_OPERATED, lineno);
        }

    }
    ret_node->valType = VT_INT; //todo 不管怎样，先返回个int
    return ret_node;
}

TreeNode *factor() {
    std::vector<TokenType> factor_first{TK_TRUE, TK_FALSE, ID, NUM, STRING, TK_LP};
    if (!token_is_in(factor_first)) {
        throw_syntax_error(SEMANTIC_ILLEGAL_CHARACTER, lineno); //
        return nullptr;
    }
    TreeNode *t1 = nullptr;
    if (token.kind == TK_LP) {
        match(TK_LP);
        TreeNode *exp = logical_and_exp();
        t1 = TreeNode::create_Node(FACTOR, exp);
        t1->valType = exp->valType;
        match(TK_RP);
    } else {
        t1 = TreeNode::create_Node(FACTOR, cp_token());
        std::string id_name;

        switch (token.kind) {
            case TK_TRUE:
            case TK_FALSE:
                t1->valType = VT_BOOL;
                break;
            case STRING:
                t1->valType = VT_STR;
                break;
            case NUM:
                t1->valType = VT_INT;
                break;
            case ID:
                /***语义检查***/
                id_name = token.s_val;
                Sym *id_sym;
                if ((id_sym = symTable.findSym(id_name)) == nullptr) {
                    throw_syntax_error(SEMANTIC_UNDEFINED_IDENTIFIER, lineno);
                    t1->valType = VT_INT;
                } else
                    t1->valType = id_sym->valType;
                break;
            default:
                throw_syntax_error(SEMANTIC_ILLEGAL_CHARACTER, lineno);
                t1 = nullptr;
        }
        match_one(factor_first);
    }
    return t1;
}


TreeNode *parse() {
    lineno = 0;
    TreeNode *root;
    nextToken();
    root = program();
    if (token.kind != ENDFILE) {
        std::cerr << "Code ends before file\n" << std::endl;
    }
    return root;
}
