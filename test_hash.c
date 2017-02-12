/*
 * test_hash.h
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
#include <string.h>
#include <assert.h>
#include "hash.h"

void test_one()
{
	hash * h = hash_new();
	
	hash_delete(h);
}

void test_two()
{
	hash * h = hash_new();
	
	hash_insert(h, "raz", "value_one");
	hash_insert(h, "dwa", "value_two");
	hash_insert(h, "trzy", "value_three");
	hash_insert(h, "cztery", "value_four");
	hash_insert(h, "piec", "value_five");
	hash_insert(h, "szesc", "value_six");
	hash_insert(h, "siedem", "value_seven");
	hash_insert(h, "osiem", "value_eight");
	
	hash_print(h);
	
	hash_delete(h);
}

void test_three()
{
	hash * h = hash_new();
	
	hash_insert(h, "raz", "value_one");
	hash_print(h);

	hash_insert(h, "dwa", "value_two");
	hash_print(h);

	hash_insert(h, "trzy", "value_three");
	hash_print(h);

	hash_insert(h, "cztery", "value_four");
	hash_print(h);

	hash_insert(h, "piec", "value_five");
	hash_print(h);
	
	char * value;
	
	value = (char *)hash_search(h, "raz");
	assert(strcmp(value, "value_one") == 0);
	
	value = (char *)hash_search(h, "piec");
	assert(strcmp(value, "value_five") == 0);
	
	value = (char *)hash_search(h, "trzy");
	assert(strcmp(value, "value_three") == 0);
	
	value = (char *) hash_search(h, "none");
	assert(value == NULL);
	
	hash_delete(h);
}

int main(int argc, char * argv[])
{
	test_one();
	test_two();
	test_three();

	return 0;
}

