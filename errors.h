//
// Created by jeza on 18-10-26.
//

#ifndef TINY_LUS_ERRORS_H
#define TINY_LUS_ERRORS_H

#include <string>

/**
 * =====================�������Ǵʷ�����=========================
 * ALPHA_AFTER_NUMBER_ERROR ��ĸ�������ִ���
 * ASSIGN_LEXICAL_ERROR ��ֵ����û�д�ȫ����
 * SINGLE_QUOTES_MISSING_FOR_STRING_ERROR �ַ���ȱʧ�����Ŵ���
 * LEFT_BRACE_MISSING_FOR_COMMENTS_ERROR ע���������ȱʧ����
 * RIGHT_BRACE_MISSING_FOR_COMMENTS_ERROR ע���Ҵ�����ȱʧ����
 * ILLEGAL_CHARACTER�Ƿ��ַ�
 * COMMENTS_NESTED_ERROR ע���г���Ƕ�״���
 * ======================�������Ǵʷ�����========================
 * ======================���������﷨����========================
 * SYNTAX_ERROR �﷨����
 * ======================���������﷨����========================
 * ======================���������������========================
 * SEMANTIC_COND_BOOL_ERROR �����ж���������BOOL����
 * SEMANTIC_UNDEFINED_IDENTIFIER δ����ı���
 * SEMANTIC_OPERATION_BETWEEN_DIFFERENT_TYPES �������ڲ�ͬ����������֮��
 * SEMANTIC_TYPE_CANNOT_BE_OPERATED �����Ͳ������ڸ������
 * SEMANTIC_CANNOT_ASSIGN_DIFFERENT_TYPE ���ܸ��費ͬ���͵�ֵ
 * SEMANTIC_ILLEGAL_CHARACTER ��������������ķǷ��ַ�
 * SEMANTIC_MISSING_SEMICOLON ȱ��ĩβ��˫����
 * SEMANTIC_MULTIPLE_DECLARATIONS �������
 * **/
enum ERROR_CODE {
    /**�ʷ�����**/
            ALPHA_AFTER_NUMBRR_ERROR = 0,
    ASSIGN_LEXICAL_ERROR,
    SINGLE_QUOTES_MISSING_FOR_STRING_ERROR,
    LEFT_BRACE_MISSING_FOR_COMMENTS_ERROR,
    RIGHT_BRACE_MISSING_FOR_COMMENTS_ERROR,
    ILLEGAL_CHARCTER,
    COMMENTS_NESTED_ERROR,

    /**�﷨����**/
    SYNTAX_ERROR,

    /**���������**/
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