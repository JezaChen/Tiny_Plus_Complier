/****************************************************/
/* File: tiny.l                                     */
/* Lex specification for TINY                       */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

%{
#include "globals.h"
#include "util.h"
#include "scan.h"
/* lexeme of identifier or reserved word */
char tokenString[MAXTOKENLEN+1];
%}

digit       [0-9]
number      {digit}+
letter      [a-zA-Z]
identifier  {letter}+
newline     \n
whitespace  [ \t]+

%%

"true"            {return TK_TRUE;}
"false"          {return TK_FALSE;}
"or"          {return TK_OR;}
"and"           {return TK_AND;}
"not"        {return TK_NOT;}
"int"         {return TK_INT;}
"bool"          {return TK_BOOL;}
"..."         {return TK_STRING;}
"while"            {return TK_WHILE;}
"do"             {return TK_DO;}
"if"            {return TK_IF;}
"then"          {return TK_THEN;}
"else"          {return TK_ELSE;}
"end"           {return TK_END;}
"repeat"        {return TK_REPEAT;}
"until"         {return TK_UNTIL;}
"read"          {return TK_READ;}
"write"         {return TK_WRITE;}
">"             {return TK_GTR;}
"<="             {return TK_LEQ;}
">="             {return TK_GEQ;}
","             {return TK_COMMA;}
":="            {return TK_ASSIGN;}
"="             {return TK_EQU;}
"<"             {return TK_LSS;}
"+"             {return TK_ADD;}
"-"             {return TK_SUB;}
"*"             {return TK_MUL;}
"/"             {return TK_DIV;}
"("             {return TK_LP;}
")"             {return TK_RP;}
";"             {return TK_SEMICOLON;}
{number}        {return NUM;}
{identifier}    {return ID;}
{newline}       {lineno++;}
{whitespace}    {/* skip whitespace */}
"{"             { char c;
                  do
                  { c = input();
                    if (c == EOF) break;
                    if (c == '\n') lineno++;
                  } while (c != '}');
                }
.               {return ERROR;}

%%

TokenType getToken(void)
{ static int firstTime = TRUE;
  TokenType currentToken;
  if (firstTime)
  { firstTime = FALSE;
    lineno++;
    yyin = source;
    yyout = listing;
  }
  currentToken = yylex();
  strncpy(tokenString,yytext,MAXTOKENLEN);
  if (TraceScan) {
    fprintf(listing,"\t%d: ",lineno);
    printToken(currentToken,tokenString);
  }
  return currentToken;
}

