#ifndef TINY_LUS_GLOBAL_H
#define TINY_LUS_GLOBAL_H

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <utility>
#include <vector>
#include <map>
#include <stack>
#include <sstream>
#include <set>
#include <string>

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
extern bool has_error;
extern bool is_optimized; //是否采取优化手段

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
    /**While循环语句块**/
            WhileK,
    /**赋值语句块**/
            AssignK,
    /**read语句块**/
            ReadK,
    /**write语句块**/
            WriteK,
    /**declarations语句块**/
            DeclarationsK,
    /**decl声明语句块**/
            DeclK,

} StmtKind;

typedef enum {
    /**含有运算符的表达式**/
            OpK,
    /**常量，在TINY++这里有NUM和STRING**/
            ConstK,
    /**标识符表达式**/
            IdK
} ExpKind;

/**用于类型检查**/
typedef enum {
    Void, Integer, Boolean
} ExpType;

/**
 * 用于 声明语句块 中的类型
 * **/
typedef enum {
    I_INT, I_BOOL, I_STRING
} IdType;

enum NodeType {
    PROGRAM = 0,
    STMT_SEQUENCE,
    IF_STMT,
    REPEAT_STMT,
    ASSIGN_STMT,
    READ_STMT,
    WRITE_STMT,
    WHILE_STMT,

    GTR_EXP, //大于
    GEQ_EXP, //大于等于
    LEQ_EXP, //小于等于
    LSS_EXP, //小于
    EQU_EXP, //等于
    LOG_OR_EXP,
    LOG_AND_EXP,
    LOG_NOT_EXP,
    ADD_EXP,
    SUB_EXP,
    MUL_EXP,
    DIV_EXP,
    FACTOR
};

typedef struct {
    TokenType kind; //符号的类型

    int i_val; //the value of TK_INT
    std::string s_val; //the value of TK_STRING or TK_ID
} Token;

typedef enum {
    /***函数***/
            OT_FUNC,
    /***变量***/
            OT_VAR,
    /***常量***/
            OT_CONST
} ObjType;

typedef enum {
    /***整型***/
            VT_INT,
    /***布尔***/
            VT_BOOL,
    /***字符串***/
            VT_STR
} ValType;

/**符号的各种属性**/
typedef struct {
    /**符号本身**/
    Token *tk;
    /**符号对象类型：函数/变量/常量**/
    ObjType objType;
    /**值的类型：int/bool/string**/
    ValType valType;
} Sym;

class TreeNode {
public:
    TreeNode *child[MAXCHILDREN];
    NodeType nodeType; //节点的类型
    ValType valType; //节点值的类型
    /**tk属性**/
    /**该属性随NodeType的不同而意义不同*/
    /**FACTOR——tk存储的是对应的token**/
    /**READ_STMT——tk存储的是待读的对象对应的token**/
    /**ASSIGN_STMT——tk存储的是待赋值的对象对应的token**/
    Token *tk;
    void setChild(TreeNode *c1 = nullptr, TreeNode *c2 = nullptr, TreeNode *c3 = nullptr) {
        /**父认子**/
        child[0] = c1;
        child[1] = c2;
        child[2] = c3;
    }
    TreeNode(NodeType nodeType, ValType valType = VT_INT, Token *tk = nullptr,
             TreeNode *c1 = nullptr,
             TreeNode *c2 = nullptr,
             TreeNode *c3 = nullptr)
            : nodeType(nodeType), valType(valType), tk(tk) {
        setChild(c1, c2, c3);
    }
    static TreeNode *
    create_Node(NodeType nodeType, ValType valType, TreeNode *c1 = nullptr, TreeNode *c2 = nullptr,
                TreeNode *c3 = nullptr) {
        return new TreeNode(nodeType, valType, nullptr, c1, c2, c3);
    }
    static TreeNode *
    create_Node(NodeType nodeType, TreeNode *c1 = nullptr, TreeNode *c2 = nullptr, TreeNode *c3 = nullptr) {
        return new TreeNode(nodeType, VT_INT, nullptr, c1, c2, c3);
    }
    static TreeNode *
    create_Node(NodeType nodeType, Token *tk) {
        return new TreeNode(nodeType, VT_INT, tk);
    }
};


/*class StmtNode : public TreeNode {
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
};*/

/**符号表的各种操作**/
class SymTable {
private:
    //std::vector<Sym *> sym_table;
    std::map<std::string, Sym *> sym_table;
public:
    explicit SymTable() = default;

    Sym *insertSym(const std::string &name) {
        Sym *sym = new Sym();
        sym_table.insert(std::make_pair(name, sym));
        return sym;
    }

    Sym *findSym(const std::string &name) {
        if (sym_table.find(name) != sym_table.end()) {
            return sym_table[name];
        } else {
            return nullptr;
        }
    }

    Sym *delSym(const std::string &name) {
        sym_table.erase(name);
    }
};

extern SymTable symTable;

/***中间代码生成***/
enum CodeType {
    C_READ,
    C_WRITE,
    C_LABEL,
    C_IF,
    C_GOTO,
    C_ASSIGN,
    C_ADD,
    C_SUB,
    C_MUL,
    C_DIV
};

struct MidCode {
    CodeType type;
    std::string s1;
    std::string s2;
    std::string t;

    MidCode(CodeType type, const std::string &_s1, const std::string &_s2, const std::string &_t) :
            type(type) {
        s1 = _s1;
        s2 = _s2;
        t = _t;
    }

    /**
     * @brief 输出实验要求的中间代码字符串
     * todo
     * */
    std::string to_str() {
        std::string ret_str;
        switch (type) {
            case C_ADD:
                ret_str = t + ":=" + s1 + "+" + s2;
                break;
            case C_SUB:
                ret_str = t + ":=" + s1 + "-" + s2;
                break;
            case C_MUL:
                ret_str = t + ":=" + s1 + "*" + s2;
                break;
            case C_DIV:
                ret_str = t + ":=" + s1 + "/" + s2;
                break;
            case C_ASSIGN:
                ret_str = t + ":=" + s1;
                break;
            case C_GOTO:
                ret_str = "goto L" + t;
                break;
            case C_LABEL:
                ret_str = "Label L" + t;
                break;
            case C_IF:
                ret_str = "if " + s1 + " goto L" + t;
                break;
            case C_READ:
                ret_str = "read " + t;
                break;
            case C_WRITE:
                ret_str = "write " + t;
                break;
        }
        return ret_str;
    }
};

class MidCodeVec {
public:
    MidCodeVec() {
        codeVec = std::vector<MidCode>();
    }

    std::vector<MidCode> codeVec;

    int push(const MidCode &code) {
        codeVec.push_back(code);
        return codeVec.size() - 1;
    }

    int push(CodeType type, const std::string &s1, const std::string &s2, const std::string &t) {
        codeVec.emplace_back(type, s1, s2, t);
        return codeVec.size() - 1;
    }

    /***
     * @brief 擦除中间代码集合中的某一行中间代码
     * @details 仅用于擦除多余的goto语句！
     * */
    void remove(const int &index) {
        codeVec.erase(codeVec.begin() + index);
    }

    /***
     * @brief 获取某一行的中间代码字符串表现形式，前面有一个行号
     * */
    std::string get_item_str(int index) {
        std::string tmp = codeVec[index].to_str();
        std::stringstream ss;
        ss << (index + 1) << ") " << tmp;
        return ss.str();
    }

    std::string num_to_str(int num) {
        std::stringstream ss;
        ss << num;
        return ss.str();
    }

    /***
     * @brief 用于拉链回填，填goto的终点
     * @param index 要填哪个语句
     * @dest 终点处
     * */
    void set_goto_destination(int index, int dest) {
        codeVec[index].t = num_to_str(dest);
    }

    /**
     * @brief 简单的中间代码简化
     *
     * */
    void shrink_codes() {
        std::map<std::string, int> label_goto;
        //删除没有被goto到的label
        for (int i = 0; i < codeVec.size(); i++) {
            if (codeVec[i].type == C_GOTO || codeVec[i].type == C_IF) {
                label_goto[codeVec[i].t]++;
            }
        }

        //要倒过来删！
        for (int i = codeVec.size() - 1; i >= 0; i--) {
            if (codeVec[i].type == C_LABEL) {
                if (label_goto.count(codeVec[i].t) == 0) {
                    remove(i);
                }
            }
        }

        //删除goto和同一个label紧接的语句块
        for (int i = codeVec.size() - 1; i >= 1; i--) {
            if (codeVec[i].type == C_LABEL && codeVec[i - 1].type == C_GOTO &&
                codeVec[i].t == codeVec[i - 1].t && label_goto[codeVec[i].t] == 1) {
                remove(i);
                remove(i - 1);
                i--;
            }
        }
    }
};

//extern MidCodeVec midCodeVec;

struct StackNode {
    TreeNode *treeNode;
    std::string val;

    StackNode(TreeNode *treeNode, std::string val) : treeNode(treeNode), val(val) {}
};

class AnalyzingStack {
public:
    std::stack<StackNode> analyzingStack;

    AnalyzingStack() = default;

    void push(const StackNode &node) {
        analyzingStack.push(node);
    }

    void push(TreeNode *treeNode, const std::string &val) {
        analyzingStack.push(StackNode(treeNode, val));
    }

    StackNode pop() {
        StackNode ret_node = analyzingStack.top();
        analyzingStack.pop();
        return ret_node;
    }

};

extern MidCodeVec midCodeVec;
extern AnalyzingStack analyzingStack;

enum ListType {
    TRUELIST = 0,
    FALSELIST,
    NEXTLIST //NEXT链用于处理具有else语句的if语句有奇效
};

class TFList {
public:
    //std::map<std::string, std::vector<int> > truelist_map;
    //std::map<std::string, std::vector<int> > falselist_map;
    //std::map<std::string, std::vector<int> > nextlist_map;
    std::vector<std::map<TreeNode *, std::vector<int> > > list_map;

    TFList() {
        for (int i = 0; i < 3; i++)
            list_map.emplace_back();
    }

    void makelist(ListType listType, TreeNode *node, int i) {
        list_map[listType][node] = std::vector<int>();
        list_map[listType][node].push_back(i);
    }

    std::vector<int> getlist(ListType listType, TreeNode *node) {
        return list_map[listType][node];
    }

    void setlist(ListType listType, TreeNode *node, std::vector<int> lst) {
        list_map[listType][node] = lst;
    }

    std::vector<int> mergelist(ListType listType, TreeNode *node1, TreeNode *node2) {
        auto lst1 = list_map[listType][node1];
        auto lst2 = list_map[listType][node2];
        lst1.insert(lst1.end(), lst2.begin(), lst2.end());
        return lst1;
    }

    void backpatch(ListType listType, TreeNode *node, int targetIndex) {
        auto lst = list_map[listType][node];
        for (auto &index : lst) {
            midCodeVec.set_goto_destination(index, targetIndex);
        }
    }
};

extern TFList tfList;

//extern AnalyzingStack analyzingStack;

#endif //TINY_LUS_GLOBAL_H
