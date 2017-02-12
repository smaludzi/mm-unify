/*
 * scanner.l
 *
 * Copyright (C) 2017 S�awomir Maludzi�ski
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
%{
#include <stdio.h>
#include <string.h>
#include "scanner.h"

unsigned int line_no = 1;

%}

DIGIT     [0-9]
ID	      [a-zA-Z_]
SPECIAL   [.,()=]

%option pointer
%option noyylineno
%option noyywrap

%%

<INITIAL>{
    [ \t\r]+	/* remove white space */

    \n {
        line_no++;
    }
    
    ":-" {
    	tokp->line_no = line_no;
    	tokp->val.string_val = (char *)yytext;
    	return TOK_IMPL;
    }
    
    "?-" {
    	tokp->line_no = line_no;
    	tokp->val.string_val = (char *)yytext;
    	return TOK_QUERY;
    }

	[A-Z]({ID}|{DIGIT})* {
		tokp->line_no = line_no;
		tokp->val.string_val = (char *)strdup(yytext);
		return TOK_VAR;
	}
	
    {ID}({ID}|{DIGIT})*	{
    	tokp->line_no = line_no;
    	tokp->val.string_val = (char *)strdup(yytext);
		return TOK_ATOM;
	}
    
    {SPECIAL} {
    	tokp->line_no = line_no;
     	tokp->val.char_val = yytext[0];
        return yytext[0];
    }

	<<EOF>>	{
    	 yyterminate();
    	 return yytext[0];
	}
	
	. {
		printf("%d unknown char %s\n", line_no, yytext);
	}
}

%%



