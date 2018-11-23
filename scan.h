/****************************************************/
/* File: scan.h                                     */
/* The scanner interface for the TINY compiler      */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#ifndef _SCAN_H_
#define _SCAN_H_

#include "globals.h"
#include <utility>
#include <string>

/* MAXTOKENLEN is the maximum size of a token */
#define MAXTOKENLEN 40

/* tokenString array stores the lexeme of each token */
//extern char tokenString[MAXTOKENLEN+1];

/* function getToken returns the
 * next token in source file
 */
/**
 * @param lineno 该Token的行数
 * */
std::pair<TokenType, std::string> getToken(int& lineno);

#endif