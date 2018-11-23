//
// Created by jeza on 18-10-26.
//

#ifndef TINY_LUS_ERRORS_H
#define TINY_LUS_ERRORS_H

#include <string>

/**
 * =====================↓下面是词法错误=========================
 * ALPHA_AFTER_NUMBER_ERROR 字母紧接数字错误
 * ASSIGN_LEXICAL_ERROR 赋值符号没有打全错误
 * SINGLE_QUOTES_MISSING_FOR_STRING_ERROR 字符串缺失单引号错误
 * LEFT_BRACE_MISSING_FOR_COMMENTS_ERROR 注释左大括号缺失错误
 * RIGHT_BRACE_MISSING_FOR_COMMENTS_ERROR 注释右大括号缺失错误
 * ILLEGAL_CHARACTER非法字符
 * COMMENTS_NESTED_ERROR 注释中出现嵌套错误
 * ======================↑上面是词法错误========================
 * ======================↓下面是语法错误========================
 * SYNTAX_ERROR 语法错误
 * ======================↑上面是语法错误========================
 * ======================↓下面是语义错误========================
 * SEMANTIC_COND_BOOL_ERROR 条件判断语句必须是BOOL类型
 * SEMANTIC_UNDEFINED_IDENTIFIER 未定义的变量
 * SEMANTIC_OPERATION_BETWEEN_DIFFERENT_TYPES 符号用于不同的数据类型之间
 * SEMANTIC_TYPE_CANNOT_BE_OPERATED 该类型不能用于该运算符
 * SEMANTIC_CANNOT_ASSIGN_DIFFERENT_TYPE 不能赋予不同类型的值
 * SEMANTIC_ILLEGAL_CHARACTER 语义分析中遇到的非法字符
 * SEMANTIC_MISSING_SEMICOLON 缺少末尾的双引号
 * SEMANTIC_MULTIPLE_DECLARATIONS 多次声明
 * **/
enum ERROR_CODE {
    /**词法错误**/
            ALPHA_AFTER_NUMBRR_ERROR = 0,
    ASSIGN_LEXICAL_ERROR,
    SINGLE_QUOTES_MISSING_FOR_STRING_ERROR,
    LEFT_BRACE_MISSING_FOR_COMMENTS_ERROR,
    RIGHT_BRACE_MISSING_FOR_COMMENTS_ERROR,
    ILLEGAL_CHARCTER,
    COMMENTS_NESTED_ERROR,

    /**语法错误**/
    SYNTAX_ERROR,

    /**语义检查错误**/
    SEMANTIC_COND_BOOL_ERROR,
    SEMANTIC_UNDEFINED_IDENTIFIER,
    SEMANTIC_OPERATION_BETWEEN_DIFFERENT_TYPES,
    SEMANTIC_TYPE_CANNOT_BE_OPERATED,
    SEMANTIC_CANNOT_ASSIGN_DIFFERENT_TYPE,
    SEMANTIC_ILLEGAL_CHARACTER,
    SEMANTIC_MISSING_SEMICOLON,
    SEMANTIC_MULTIPLE_DECLARATIONS
};

struct {
    ERROR_CODE error_code;
    std::string error_description;
} error_items[17] = {
        {
                ALPHA_AFTER_NUMBRR_ERROR,
                "Numbers cannot be followed by letters."
        },
        {
                ASSIGN_LEXICAL_ERROR,
                "The assignment symbols are not complete."
        },
        {
                SINGLE_QUOTES_MISSING_FOR_STRING_ERROR,
                "String Constant Single Quotation Mark Not Completed."
        },
        {
                LEFT_BRACE_MISSING_FOR_COMMENTS_ERROR,
                "The left brace is missing"
        },
        {
                RIGHT_BRACE_MISSING_FOR_COMMENTS_ERROR,
                "The right brace is missing"
        },
        {
                ILLEGAL_CHARCTER,
                "Meet An Illegal Character"
        },
        {
                COMMENTS_NESTED_ERROR,
                "An nested comment is found."
        },
        {
                SYNTAX_ERROR,
                "An Syntax Error is found: "
        },
        {
                SEMANTIC_COND_BOOL_ERROR,
                "The value of expression must be boolean"
        },
        {
                SEMANTIC_UNDEFINED_IDENTIFIER,
                "Undefined Identifier"
        },
        {
                SEMANTIC_OPERATION_BETWEEN_DIFFERENT_TYPES,
                "This operator can not be used between two different types."
        },
        {
                SEMANTIC_TYPE_CANNOT_BE_OPERATED,
                "This type cannot be operated by this operator."
        },
        {
                SEMANTIC_CANNOT_ASSIGN_DIFFERENT_TYPE,
                "Cannot Assign different type"
        },
        {
                SEMANTIC_ILLEGAL_CHARACTER,
                "Meet an illegal character when doing semantic analyzing."
        },
        {
                SEMANTIC_MISSING_SEMICOLON,
                "A single quotation mark is missing at the end of sentence."
        },
        {
                SEMANTIC_MULTIPLE_DECLARATIONS,
                "Cannot declare the same identifier again!"
        }
};

#endif //TINY_LUS_ERRORS_H