/*
 * uni.c
 *
 * Copyright (C) 2017 Slawomir Maludzinski
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
#include <stdio.h>
#include "scanner.h"
#include "parser.h"

extern int yyparse();

void scan()
{
	token tok;
    int tok_type;

	while ((tok_type = lex_scan(&tok)))
	{
		switch (tok_type)
		{
			case '.':
		    case ')':
		    case '(':
		    case ',':
		    case '=':
				printf("%d spec: %c\n", tok.line_no, tok.val.char_val);
				break;
			case TOK_IMPL:
				printf("%d :-\n", tok.line_no);
				break;
			case TOK_QUERY:
				printf("%d ?-\n", tok.line_no);
				break;
			case TOK_VAR:
			    printf("%d var: %s\n", tok.line_no, tok.val.string_val);
			    break;
			case TOK_ATOM:
				printf("%d atom: %s\n", tok.line_no, tok.val.string_val);
				break;
		}
	}
}

int main(int argc, char * argv[])
{
	if (argc > 1)
	{
		yyin = fopen(argv[1], "r");
	}
	else
	{
		yyin = stdin;
	}
	
	/* scan(); */
	yyparse();

	fclose(yyin);

	yylex_destroy();

	return 0;
}

