/*
 * term.c
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
#include <stdlib.h>
#include <stdio.h>
#include "term.h"

term * term_new(int type, char * name)
{
	return term_new_list(type, name, NULL);
}

term * term_new_list(int type, char * name, List * terms)
{
	term * t;
	
	t = malloc(sizeof(term));
	if (t == NULL)
	{
		fprintf(stderr, "cannot allocate term\n");
	}
	
	t->type = type;
	t->name = name;
	t->terms = terms;
	
	return t;
}

void term_delete(term * t)
{
	if (t == NULL)
	{
		return;
	}
	
	list_delete(t->terms);
	free(t->name);
	free(t);
}

void term_deallocator(void * data)
{
	term_delete((term *)data);
}

void term_print(term * t)
{
	if (t == NULL)
	{
		return;
	}

	term_print_rec(t);
}

void term_print_rec(term * t)
{
    printf("%s", t->name);

	if (t->terms != NULL)
	{
		printf("(");
		
		term_list_print_rec(t->terms);
	
		printf(")");
	}
}

void term_list_print(List * l)
{
	if (l == NULL)
	{
		return;
	}

	term_list_print_rec(l);	
}

void term_list_print_rec(List * l)
{
	char first = 1;

	ListIterator iter = list_iterator_last(l);
	while (!list_iterator_is_last(iter))
	{
		if (first == 0)
		{
			printf(", ");
		}

		term_print_rec((term *)list_iterator_data(iter));

		first = 0;
		
		list_iterator_previous(&iter);
	}
}

