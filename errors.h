//
// Created by jeza on 18-10-26.
//

#ifndef TINY_LUS_ERRORS_H
#define TINY_LUS_ERRORS_H

#include <string>

/**
 * ALPHA_AFTER_NUMBER_ERROR 字母紧接数字错误
 * ASSIGN_LEXICAL_ERROR 赋值符号没有打全错误
 * SINGLE_QUOTES_MISSING_FOR_STRING_ERROR 字符串缺失单引号错误
 * LEFT_BRACE_MISSING_FOR_COMMENTS_ERROR 注释左大括号缺失错误
 * RIGHT_BRACE_MISSING_FOR_COMMENTS_ERROR 注释右大括号缺失错误
 * **/
enum LEXICAL_ERROR_CODE {
    ALPHA_AFTER_NUMBRR_ERROR = 0,
    ASSIGN_LEXICAL_ERROR,
    SINGLE_QUOTES_MISSING_FOR_STRING_ERROR,
    LEFT_BRACE_MISSING_FOR_COMMENTS_ERROR,
    RIGHT_BRACE_MISSING_FOR_COMMENTS_ERROR
};

struct {
    LEXICAL_ERROR_CODE error_code;
    std::string error_description;
} error_items[6] = {
        {
                ALPHA_AFTER_NUMBRR_ERROR,
                "数字后面不能加字母! 听懂了没有!"
        },
        {
                ASSIGN_LEXICAL_ERROR,
                "这世界上总有人赋值符号不打全, 后来他们都看到自己的脑浆了!"
        },
        {
                SINGLE_QUOTES_MISSING_FOR_STRING_ERROR,
                "这世界上好人不会死坏人不会死, 只有写字符串不加单引号的人会死!"
        },
        {
                LEFT_BRACE_MISSING_FOR_COMMENTS_ERROR,
                "注释中左大括号缺失"
        },
        {
                RIGHT_BRACE_MISSING_FOR_COMMENTS_ERROR,
                "注释中右大括号丢失"
        }
};

#endif //TINY_LUS_ERRORS_H