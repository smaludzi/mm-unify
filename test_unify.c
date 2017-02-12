/*
 * test_unify.c
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
#include "unify.h"

void test_one()
{
	u_system * R;
	
	R = u_system_new();
	
	u_system_delete(R);
}

void test_two()
{
	u_system * R;
	
	R = u_system_new();
	
	
	
	u_system_delete(R);
}

void test_three()
{
	multi_term * multi_term1;
	multi_term * multi_term2;
	temp_mult_eq * mult1;
	temp_mult_eq * mult2;
	variable * var1;
	variable * var2;
	variable * var3;
	variable_queue * var_q1;
	variable_queue * var_q2;
	
	var1 = variable_new("var1");
	var2 = variable_new("var2");
	var3 = variable_new("var3");
	
	var_q1 = variable_queue_new();
	var_q2 = variable_queue_new();
	
	variable_queue_add_var(var_q1, var1);
	variable_queue_add_var(var_q1, var2);
	variable_queue_add_var(var_q2, var3);
	
	mult1 = temp_mult_eq_new(var_q1, NULL);
	mult2 = temp_mult_eq_new(var_q2, NULL);
	
	multi_term1 = multi_term_new("func_1", NULL);
	multi_term2 = multi_term_new("func_1", NULL);
	
	multi_term1->args = temp_mult_eq_node_new(mult1, multi_term1->args);
	multi_term2->args = temp_mult_eq_node_new(mult2, multi_term2->args);
	
	multi_term_print(multi_term1);
	printf("\n");
	multi_term_print(multi_term2);
	printf("\n");
	
	merge_multi_terms(&multi_term1, &multi_term2);
	
	multi_term_print(multi_term1);
	printf("\n");
	multi_term_print(multi_term2);
	printf("\n");
	
	multi_term_delete(multi_term1);
	multi_term_delete(multi_term2);
	
	variable_delete(var1);
	variable_delete(var2);
	variable_delete(var3);
}

#if 0
void test()
{
	multi_term * multi1;
	multi_term * multi2;

	multi1 = create_multi_term_single(term1, symbol_table);
	multi2 = create_multi_term_single(term2, symbol_table);

	//merge_multi_terms(&multi->M, &multi2->M);
	//merge_mult_eq(NULL, &multi, &multi2);
	multi_term_print(multi1);
	multi_term_print(multi2);

	merge_multi_terms(&multi1, &multi2);

	multi_term_print(multi1);
	multi_term_print(multi2);
	
	multi_term_delete(multi1);
	multi_term_delete(multi2);
}
#endif

int main(int argc, char * argv[])
{
	test_one();
	test_two();
	test_three();

	return 0;
}

