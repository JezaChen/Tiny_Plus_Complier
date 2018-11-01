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
} TokenType;

extern FILE *source; /* source code text file */
extern FILE *listing; /* listing output text file */
extern FILE *code; /* code text file for TM simulator */
extern int lineno; /* source line number for listing */

extern int TraceScan;

#define MAXCHILDREN 3 //定义每个节点最大孩子数为3
typedef enum {
    /**语句类型 **/
            StmtK,
    /**表达式类型**/
            ExpK
} NodeKind;

typedef enum {
    /**IF语句块**/
            IfK,
    /**Repeat循环语句块**/
            RepeatK,
    /**赋值语句块**/
            AssignK,
    /**read语句块**/
            ReadK,
    /**write语句块**/
            WriteK
} StmtKind;

typedef enum {
    OpK,
    ConstK,
    IdK
} ExpKind;

/**用于类型检查**/
typedef enum {
    Void, Integer, Boolean
} ExpType;

class TreeNode {
public:
    TreeNode *child[MAXCHILDREN];
    TreeNode *sibling;
    TreeNode *parent;
    int lineno;
    NodeKind nodeKind;
    int childrenNum;
    /* union {
         StmtKind stmt;
         ExpKind exp;
     } kind;*/
    union {
        TokenType op;
        int val;
        char *name;
    } attr;

    TreeNode(const int &lineno) : sibling(nullptr), lineno(lineno), childrenNum(0) {
        for (auto &i : child)
            i = nullptr;
    }

    void appendChild(TreeNode *node) {
        if (childrenNum == MAXCHILDREN - 1) {
            //todo
            std::cerr << "APPEND CHILDREN ERROR: REACH MAXIMUM" << std::endl;
        }
        child[childrenNum++] = node;
    }
};

class StmtNode : public TreeNode {
public:
    StmtKind kind;

    StmtNode(const StmtKind &kind, const int &lineno) : TreeNode(lineno), kind(kind), nodeKind(StmtK) {}

    static StmtNode *create_Stmt_Node(const StmtKind &kind, const int &lineno) {
        auto *node = new StmtNode(kind, lineno);
        return node;
    }
};

class ExpNode : public TreeNode {
public:
    ExpKind kind;
    ExpType type;

    ExpNode(const ExpKind &kind, const int &lineno) : TreeNode(lineno), kind(kind), type(Void), nodeKind(ExpK) {}

    static ExpNode *create_Exp_Node(const ExpKind &kind, const int &lineno) {
        auto *node = new ExpNode(kind, lineno);
        return node;
    }
};

#endif //TINY_LUS_GLOBAL_H
