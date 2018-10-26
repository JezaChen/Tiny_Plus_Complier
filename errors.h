//
// Created by jeza on 18-10-26.
//

#ifndef TINY_LUS_ERRORS_H
#define TINY_LUS_ERRORS_H

#include <string>

enum LEXICAL_ERROR_CODE {
    ALPHA_AFTER_NUMBRR_ERROR = 0,
    ASSIGN_LEXICAL_ERROR,
    SINGLE_QUOTES_MISSING_FOR_STRING_ERROR,
};

struct {
    LEXICAL_ERROR_CODE error_code;
    std::string error_description;
} error_items[3] = {
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
        }
};

#endif //TINY_LUS_ERRORS_H