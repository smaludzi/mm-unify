/*
 * parser.y
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
#include "scanner.h"
#include "unify_term.h"

int yyerror(char * str)
{
	fprintf(stderr, "%s\n", str);
	
	return 0;
}

int yylex(token * tokp)
{
	return lex_scan(tokp);
}

%}
%pure_parser

%token <val.string_val> TOK_ATOM
%token <val.string_val> TOK_VAR
%token <val.string_val> TOK_IMPL
%token <val.string_val> TOK_QUERY

%type <val.term_val> term
%type <val.terms_val> terms

%start program

%%

term: TOK_ATOM
      {
          $$ = term_new(TERM_ATOM, $1);
      }
    | TOK_VAR
      {
          $$ = term_new(TERM_VAR, $1);
      }
    | TOK_ATOM '(' ')'
      {
      	  $$ = term_new(TERM_TERM, $1);
      }
    | TOK_ATOM '(' terms ')'
      {
      	  $$ = term_new_list(TERM_TERM, $1, $3); 
      }
;

terms: term
      {
      	  $$ = list_new();
      	  list_add_beg_deallocator($$, $1, term_deallocator);
      }
    | terms ',' term
      {
      	  list_add_beg_deallocator($$, $3, term_deallocator);
      }
;

fact: term '.'
    {
        printf("FACT BEG\n");
        
        term_print($1);
        
        printf("FACT END\n");
    
    	term_delete($1);
    }
;

rule: term TOK_IMPL terms '.'
    {
    	printf("RULE BEG\n");
    
        term_print($1);
        term_list_print($3);
        
        printf("RULE END\n");
    
    	term_delete($1);
    	list_delete($3);
    }
;

query: TOK_QUERY terms '.'
    {
        printf("QUERY BEG\n");
        
        term_list_print($2);
        
        printf("\nQUERY END\n");
    
        list_delete($2);
    }
    | term '=' term '.'
    {
    	term_print($1); printf(" = "); term_print($3);
    	printf(".\n");
    
    	if (terms_consistent($1, $3))
    	{
    		printf("terms are consistent\n");

	    	if (unify_terms($1, $3) == 1)
	    	{
	    		printf("terms can be unified\n");
	    	}
	    	else
	    	{
	    		printf("terms cannot be unified\n");
	    	}
    	}
    	else
    	{
    		printf("terms are not consistent\n");
    	}
    
        term_delete($1);
        term_delete($3);
    }
;

claus: fact
    | rule
    | query

clauses: claus
    | clauses claus
;

program: clauses
;

%%

