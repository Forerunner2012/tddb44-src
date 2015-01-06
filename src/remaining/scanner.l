%{

#include <stdlib.h>
#include <string.h>


//#include "scanner.hh"
// This is where you put #include directives as needed for later labs.
// include "ast.hh", parser.hh" in that order
#include "symtab.hh"
#include "ast.hh"
#include "parser.hh"

int column = 0;

extern YYLTYPE yylloc; // Used for position information, see below.

%}

%option yylineno
%option 8bit
%option noyywrap
%option caseless
%option outfile = "scanner.cc"

/* If you want to include any flex declarations, this is where to do it. */

%x c_comment
%x d_comment
%x c_string
DIGIT [0-9]
/* {DIGIT}+"."{DIGIT}*
 ID [a-z][a-z0-9]* */flex

/* Position information is entered into the yylloc variable, so we will be
   able to generate good error messages later in later phases. */
/* Your code should be entered below the %%. Expressions to handle the
   following: Diesel comments, Diesel string constants, Diesel
   identifiers, integers, reals, and whitespace. */
%%
\.                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_DOT;
                         }
;                        {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_SEMICOLON;
                         }
=                        {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_EQ;
                         }
\:                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_COLON;
                         }
\(                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_LEFTPAR;
                         }
\)                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_RIGHTPAR;
                         }
\[                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_LEFTBRACKET;
                         }
\]                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_RIGHTBRACKET;
                         }
,                        {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_COMMA;
                         }
\<                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_LESSTHAN;
                         }
\>                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_GREATERTHAN;
                         }
\+                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_ADD;
                         }
\-                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_SUB;
                         }
\*                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_MUL;
                         }
\/                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_RDIV;
                         }
":="                     {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_ASSIGN;
                         }
"<>"                     {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_NOTEQ;
                         }


of                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_OF;                               
                         }
if                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_IF;
                         }
do                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_DO;                            
                         }
or                       {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_OR;                            
                         }
var                      {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_VAR;                            
                         }
end                      {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_END;                               
                         }
and                      {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_AND;                               
                         }
div                      {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_IDIV;
						 }
mod                      {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_MOD;                               
                         }
not                      {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_NOT;                               
                         }
then                     {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_THEN;
                         }
else                     {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_ELSE;
                         }
const                    {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_CONST;
                         }
array                    {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_ARRAY;
                         }
begin                    {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_BEGIN;
                         }
while                    {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_WHILE;
                         }
elsif                    {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_ELSIF;
                         }
return                   {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_RETURN;
                         }
program                  {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_PROGRAM;
                         }
function                 {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_FUNCTION;
                         }
procedure                {
                            yylloc.first_line = yylineno;
                            yylloc.first_column = column;
                            column += yyleng;
                            return T_PROCEDURE;                            
                         }

{DIGIT}+		{
			  yylval.ival = atoi(yytext);
			  yylloc.first_line = yylineno;
			  yylloc.first_column = column;
			  column += yyleng;
			  return T_INTNUM;
			}

{DIGIT}+"."		{
			  yylval.rval = atof(yytext);
			  yylloc.first_line = yylineno;
			  yylloc.first_column = column;
			  column += yyleng;
			  return T_REALNUM;
			}
			
{DIGIT}+[eE][+-]?{DIGIT}+	{
			  yylval.rval = atof(yytext);
			  yylloc.first_line = yylineno;
			  yylloc.first_column = column;
			  column += yyleng;
			  return T_REALNUM;
			}

{DIGIT}*"."{DIGIT}+([eE][+-]?{DIGIT}+)?	{
			  yylval.rval = atof(yytext);
			  yylloc.first_line = yylineno;
			  yylloc.first_column = column;
			  column += yyleng;
			  return T_REALNUM;
			}
			
[a-zA-Z"_"][a-zA-Z0-9"_"]* {
			yylloc.first_line = yylineno;
			yylloc.first_column = column;
			column += yyleng;

			yylval.pool_p = sym_tab->pool_install(sym_tab->capitalize(yytext));
			sym_index tmp = sym_tab->lookup_symbol(yylval.pool_p);
			if(tmp != NULL_SYM) {
				sym_tab->pool_forget(yylval.pool_p);
				yylval.pool_p = (sym_tab->get_symbol(tmp))->id;
			}
			return T_IDENT;
			}
			
[ \t]+			{
			  yylloc.first_column = column;
			  column += yyleng;
			}

[\n]+			{
			  yylloc.first_column = column;
			  column = 0;
			}

"\'"                    {
                            column += yyleng;
			    yymore();
                            BEGIN(c_string);
                         }

<c_string>
{
    "\'"	        {
                          printf("\n%s\n", yytext);
                          yylval.pool_p = sym_tab->pool_install(sym_tab->fix_string(yytext)); /* or also \'.**\' , need a fun-ction to handle the error*/
			  yylloc.first_line = yylineno;
			  yylloc.first_column = column;
			  column += yyleng;
			  return T_STRINGCONST;
                        }
    "\'\'"		{
			  yymore();
			  column += yyleng;
			}
    [^\n]               {
			  yymore();
			  column += yyleng;
			}
    \n                  yyerror("Newline in string");
    <<EOF>>             {
                            yyerror("Unterminated string");
                            yyterminate();
                        }
}
			
\/\/.*$                  column = 0; /* Skip single-line comment */
"/\*"                    {
                            column += yyleng;
                            BEGIN(c_comment);
                         }
                         
<c_comment>
{
    "\*/"                {
                            column += 2;
                            BEGIN(INITIAL);
                         }
    "/\*"                {
                            column += 2;
                            yyerror("Suspicious comment");
                         }
    [^\n]                column++; /* Skip stuff in comments */
    \n                   column = 0;
    <<EOF>>              {
                            yyerror("Unterminated comment");
                            yyterminate();
                         }
}

"\{"                    {
                            column += yyleng;
                            BEGIN(d_comment);
                         }

<d_comment>
{
    "\}"                {
                            column += 2;
                            BEGIN(INITIAL);
                        }
    "\{"                {
                            column += 2;
                            yyerror("Suspicious d comment");
                        }
    [^\n]               column++; /* Skip stuff in comments */
    \n                  column = 0;
    <<EOF>>             {
                            yyerror("Unterminated d comment");
                            yyterminate();
                        }
}

<<EOF>>                  yyterminate();
.                        yyerror("Illegal character");	






