/*
 * scanner.h
 *
 * Copyright (C) 2007 Slawomir Maludzinski
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
#ifndef __SCANNER_H__
#define __SCANNER_H__ 1

#include "parser.h"
#include "list.h"
#include "term.h"

typedef union token_value {
    char char_val;
    char * string_val;
    term * term_val;
    List * terms_val;
} token_value;

typedef struct token {
    int type;
    token_value val; /* union */
	unsigned int line_no;
} token;

extern unsigned int line_no;

extern FILE * yyin;

extern int lex_scan(token * tokp);
extern int yylex_destroy();

#define YYSTYPE token
#define YY_DECL int lex_scan(token * tokp)

#endif /* __SCANNER_H__ */

