/*************************************************************************************************************
 * Lexical Analyzer for TINY++
 * Initial Version Created by J. Chen, Z. Mu in Nov 26, 2018
 * TINY++ Lexical Grammar
 * 1. Keywords must be begin with letters.
 * 2. Characters cannot be contained in numbers.
 * 3. The comment is enclosed by brackets, and cannot be nested, but it can include more than one line.
 * 4. The string is enclosed by single quote.
 ************************************************************************************************************/

#include "globals.h"
#include "util.h"
#include "scan.h"
#include "errors.h"
#include <string>
#include <iostream>
#include <utility>
#include <stack>

/***
 * @enum STATE
 * @brief 有限状态机的状态集
 * */
typedef enum {
    START, INID, INNUM, INCOMMENT, INASSIGN, INLEQ, INGEQ, INSTR, SUCCESS, FAILED
} STATE;


#define BUFLEN 256 //缓冲区最大大小

static char lineBuf[BUFLEN]; //缓冲区
static int bufSize = 0; //缓冲区的大小
static int linepos = 0; //每一行中读到了第几个字符
static bool EOF_flag = FALSE;

static char getNextChar() {
    if (linepos >= bufSize) {
        if (fgets(lineBuf, BUFLEN - 1, source)) {
            lineno++; //FIXME 漏洞代码-SB001-解决了最后一行报错飘了的问题
            bufSize = strlen(lineBuf);
            linepos = 0;
            return lineBuf[linepos++];
        } else {
            EOF_flag = true;
            return EOF;
        }
    }
    return lineBuf[linepos++];
}

/**
 * 回退指针
 * */
static void ungetNextChar() {
    if (!EOF_flag)
        linepos--;
}

/**
 * @struct 关键字表
 * @brief 其中，str关键字的字符串表示，tok为关键字对应的TOKEN类型
 * **/
static struct {
    std::string str;
    TokenType tok;
} reversedWords[MAXRESERVED] = {
        {"if",     TK_IF},
        {"true",   TK_TRUE},
        {"false",  TK_FALSE},
        {"or",     TK_OR},
        {"and",    TK_AND},
        {"not",    TK_NOT},
        {"int",    TK_INT},
        {"bool",   TK_BOOL},
        {"string", TK_STRING},
        {"while",  TK_WHILE},
        {"do",     TK_DO},
        {"then",   TK_THEN},
        {"else",   TK_ELSE},
        {"end",    TK_END},
        {"repeat", TK_REPEAT},
        {"until",  TK_UNTIL},
        {"read",   TK_READ},
        {"write",  TK_WRITE}
};

/**
 * @brief 查询关键字表
 * @details 当获得了一个ID类型的token之后，还需要调用此函数进行判断其是不是关键字，如果是，就返回其对应的关键字TOKEN类型
 * @param 该ID类型token的字符串表示
 * @return 如果是关键字，返回对应的TOKEN类型；如果不是，直接返回ID即可。
 * **/
static TokenType reversedLookUp(const std::string &s) {
    for (auto &i : reversedWords) {
        if (i.str == s)
            return i.tok;
    }
    return ID;
}


static int brace_num_nested = 0;

/***
 * @brief 获取TOKEN函数，词法分析的核心算法
 * @param ret_lineno out 用于返回值，token对应的行号
 * @return std::pair类型，其中第一个元素为读取到的TOKEN类型，第二个元素为token对应的字符串表示
 * */
std::pair<TokenType, std::string> getToken(int &ret_lineno) {
    bool isNeedToSave;
    TokenType currToken;
    std::string tokenString;
    bool is_unget; //是否已经出现了回滚操作

    /**
     * 一开始处于START状态
     * @update 如果大括号栈中还有东西，应让它处于注释状态（错误处理）
     * **/
    STATE state = (brace_num_nested == 0 ? START : INCOMMENT);

    while (state != SUCCESS && state != FAILED) { //如果仍不处于终态
        char c = getNextChar();
        isNeedToSave = true;
        is_unget = false;
        switch (state) {
            /**初始状态**/
            case START:
                if (isdigit(c)) {
                    state = INNUM; //转移状态到IN数字
                } else if (isalpha(c)) {
                    state = INID; //转移状态到IN标识符
                } else if (c == ':') {
                    state = INASSIGN;
                } else if (c == ' ' || c == '\t' || c == '\n') {
                    isNeedToSave = false;
                } else if (c == '{') {
                    isNeedToSave = false;
                    state = INCOMMENT;
                } else if (c == '}') {
                    //FIXME-1:ERROR
                    isNeedToSave = false;
                    state = FAILED;
                    currToken = ERROR;
                    tokenString = error_items[LEFT_BRACE_MISSING_FOR_COMMENTS_ERROR].error_description;
                    /**不回滚，吃掉右大括号，继续分析**/
                } else if (c == '<') {
                    state = INLEQ;
                } else if (c == '>') {
                    state = INGEQ;
                } else if (c == '\'') {
                    state = INSTR;
                } else {
                    char ch;
                    state = SUCCESS;
                    switch (c) {
                        case EOF: //读到文本结束符
                            isNeedToSave = false;
                            currToken = ENDFILE;
                            break;
                        case '=':
                            currToken = TK_EQU;
                            break;
                        case '+':
                            currToken = TK_ADD;
                            break;
                        case '-':
                            currToken = TK_SUB;
                            break;
                        case '*':
                            currToken = TK_MUL;
                            break;
                        case '/':
                            currToken = TK_DIV;
                            break;
                        case '(':
                            currToken = TK_LP;
                            break;
                        case ')':
                            currToken = TK_RP;
                            break;
                        case ',':
                            currToken = TK_COMMA;
                            break;
                        case ';':
                            currToken = TK_SEMICOLON;
                            break;
                        default:
                            /**FIXME 遇到非法字符**/
                            state = FAILED;
                            currToken = ERROR;
                            tokenString = error_items[ILLEGAL_CHARCTER].error_description + c;
                            /**设置不保存，避免tokenString被覆盖**/
                            isNeedToSave = false;
                    }
                }
                break;
                /**注释状态**/
            case INCOMMENT:
                isNeedToSave = false;
                if (c == EOF) {
                    /*state = SUCCESS; //TODO 不知道是什么恶心的语法，先打上去再输，给钱就优化
                    currToken = ENDFILE;*/
                    //FIXME 如果读到文本终止符，意味着这个文本并没有存在右大括号符，此时应该及时返回并把终止符吐回去
                    state = FAILED;
                    currToken = ERROR;
                    tokenString = error_items[RIGHT_BRACE_MISSING_FOR_COMMENTS_ERROR].error_description;
                    ungetNextChar();
                } else if (c == '}') {
                    if (!brace_num_nested) {

                    }
                    state = START;
                } else if (c == '{') {
                    /**FIXME 如果在注释状态下再读到左大括号，意味着出现了嵌套，需要进一步处理**/
                    brace_num_nested++;
                    state = FAILED;
                    currToken = ERROR;
                    tokenString = error_items[COMMENTS_NESTED_ERROR].error_description;
                    /**不用回滚**/
                }
                break;
                /**数字状态**/
            case INNUM:
                if (isalpha(c)) { //当数字紧接字母的时候，是错误的
                    ungetNextChar();
                    is_unget = true;
                    isNeedToSave = false;

                    state = FAILED;
                    currToken = ERROR;
                    tokenString = error_items[0].error_description;
                } else if (!isdigit(c)) {
                    state = SUCCESS;
                    currToken = NUM;
                    ungetNextChar();
                    is_unget = true;
                }
                break;
                /**标识符状态**/
            case INID:
                if (!isdigit(c) && !isalpha(c)) {
                    state = SUCCESS;
                    currToken = ID; //FIXME
                    ungetNextChar();
                    is_unget = true;
                }
                break;
                /**赋值符号状态**/
            case INASSIGN:
                if (c == '=') {
                    state = SUCCESS;
                    currToken = TK_ASSIGN;
                } else {
                    ungetNextChar();
                    is_unget = true;
                    isNeedToSave = false;

                    state = FAILED;
                    currToken = ERROR;
                    tokenString = error_items[1].error_description;
                }
                break;
                /**小于等于符号状态**/
            case INLEQ:
                state = SUCCESS;
                if (c == '=') {
                    currToken = TK_LEQ;
                } else {
                    currToken = TK_LSS;
                }
                break;
                /**大于等于符号状态**/
            case INGEQ:
                state = SUCCESS;
                if (c == '=') {
                    currToken = TK_GEQ;
                } else {
                    currToken = TK_GTR;
                }
                break;
                /**字符串状态**/
            case INSTR:
                if (c == '\'') {
                    state = SUCCESS;
                    currToken = STRING;
                } else if (c == '\n' || c == EOF) {
                    state = FAILED;
                    ungetNextChar();
                    currToken = ERROR;
                    tokenString = error_items[2].error_description;
                }
            case SUCCESS:
            case FAILED:
                break;
        }
        if (isNeedToSave && !is_unget) { //回滚后的状态不需要将读取到的字符拼接到结果字符串之后
            tokenString += c;
        }
    }

    ret_lineno = lineno; //FIXME
    if (state == SUCCESS) {
        if (currToken == ID) { //把id送进去检查是不是关键字来的
            currToken = reversedLookUp(tokenString);
        }
        return std::make_pair(currToken, tokenString);
    } else {
        //std::cerr << "Error!" << std::endl;
        return std::make_pair(ERROR, tokenString);
    }
}
