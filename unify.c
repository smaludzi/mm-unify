/*
 * unify.c
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unify.h"

int unify(u_system * R)
{
	int ret;
    multi_equation * mult = NULL;
    temp_mult_eq_list * frontier = NULL;

    do
    {
        ret = select_multi_equation(&R->U, &mult);
        if (ret == 0)
        {
        	return 0;
        }
        
        if (mult->M != NULL)
        {
        	frontier = NULL;
        	reduce(mult->M, &frontier);
        	
        	ret = compact(&frontier, &(R->U));
        	if (ret == 0)
        	{
        		frontier_delete(&frontier);
        		return 0;
        	}
        	
        	frontier_delete(&frontier);
        }
        R->T = multi_equation_node_new(mult, R->T);
    }
    while (R->U.multi_eq_number > 0);
    
    return 1;
}

int select_multi_equation(u_part * U, multi_equation ** mult)
{
	multi_equation_list * node;

    if (U->zero_counter_multi_equation == NULL)
    {
    	return 0;
    }
    
	*mult = U->zero_counter_multi_equation->value;
	
	U->multi_eq_number--;

	node = U->zero_counter_multi_equation;
	U->zero_counter_multi_equation = U->zero_counter_multi_equation->next;
		
	multi_equation_node_delete(node);

	return 1;
}

int reduce(multi_term * M, temp_mult_eq_list ** frontier)
{
	temp_mult_eq_list * arg;
	
	arg = M->args;
	
	while (arg != NULL)
	{
		if (variable_queue_is_empty(arg->value->S))
		{
			reduce(arg->value->M, frontier);
		}
		else
		{
			*frontier = temp_mult_eq_node_new(arg->value, *frontier);
			arg->value = temp_mult_eq_new(variable_queue_new_var(variable_queue_head(arg->value->S)), NULL);
		}
	
		arg = arg->next;
	}

	return 1;
}

int compact(temp_mult_eq_list ** frontier, u_part * U)
{
	int ret;
    variable * V;
    variable_queue * vars;
    temp_mult_eq_list * node;
	multi_equation * mult, * mult1;

    while ((*frontier) != NULL)
    {
    	vars = (*frontier)->value->S;
    	
    	V = variable_queue_head(vars);
    	variable_queue_remove_head(vars);

    	mult = V->M;
    	mult->counter--;
    	
    	while (!variable_queue_is_empty(vars))
    	{
    		V = variable_queue_head(vars);
    		variable_queue_remove_head(vars);
    		
    		mult1 = V->M;
    		mult1->counter--;
    		
    		ret = merge_mult_eq(U, &mult, &mult1);
    		if (ret == 0)
    		{
    			return 0;
    		}
    	}
   
   		variable_queue_delete(vars);
   
       	ret = merge_multi_terms(&mult->M, &(*frontier)->value->M);
    	multi_term_delete((*frontier)->value->M);
    	if (ret == 0)
    	{
     		return 0;
    	}
    	
    	if (mult->counter == 0)
    	{
    		U->zero_counter_multi_equation =
    			multi_equation_node_new(mult, U->zero_counter_multi_equation);
    	}

		node = *frontier;    	
    	*frontier = (*frontier)->next;    	
    	temp_mult_eq_node_delete(node);
    }
    
	return 1;
}

int merge_mult_eq(u_part * U, multi_equation ** mult, multi_equation ** mult1)
{
	int ret;
	variable * V;
	variable_list * vars;
	multi_equation * multt;

    if (*mult == *mult1)
    {
    	return 0;
    }

	if ((*mult)->var_number < (*mult1)->var_number)
	{
		multt = *mult;
		*mult = *mult1;
		*mult1 = multt;
	}
		
	(*mult)->counter += (*mult1)->counter;
	(*mult)->var_number += (*mult1)->var_number;

	vars = (*mult1)->S;
	while (vars != NULL)
	{
		V = vars->value;
		
		vars = vars->next;
		
		V->M = (*mult);
		
		(*mult)->S = variable_node_new(V, (*mult)->S);
	}

	ret = merge_multi_terms(&(*mult)->M, &(*mult1)->M);
	if (ret == 0)
	{
		return 0;
	}
	
	if (U != NULL)
	{
		U->multi_eq_number--;
	}

	return 1;
}

int merge_multi_terms(multi_term ** M, multi_term ** M1)
{
    temp_mult_eq_list * arg, * arg1;
    
    if (*M == NULL)
    {
    	*M = *M1;
    	*M1 = NULL;
    }
    else if (*M1 != NULL)
    {
    	if (strcmp((*M)->f_symb, (*M1)->f_symb) != 0)
    	{
    		return 0;
    	}
    	else
    	{
    		arg = (*M)->args;
    		arg1 = (*M1)->args;
    		
    		while (arg != NULL)
    		{
				int ret;
    			variable_queue_append(&arg->value->S, &arg1->value->S);
    			
    			ret = merge_multi_terms(&arg->value->M, &arg1->value->M);
    			if (ret == 0)
    			{
    				return 0;
    			}
    			
    			arg = arg->next;
       			arg1 = arg1->next;
			}
    	}    	
    }
    
	return 1;
}

void frontier_delete(temp_mult_eq_list ** frontier)
{
	temp_mult_eq_list * node;
	
	if (*frontier == NULL)
	{
		return;
	}
	
	while (*frontier != NULL)
	{
		node = *frontier;    	

    	*frontier = (*frontier)->next;    	

    	temp_mult_eq_node_delete(node);
	}

	*frontier = NULL;
}

u_system * u_system_new()
{
	u_system * system;
	
	system = (u_system *)malloc(sizeof(u_system));
	if (system == NULL)
	{
		fprintf(stderr, "cannot allocat u_system\n");
	}

	system->T = NULL;
	system->U.multi_eq_number = 0;
	system->U.zero_counter_multi_equation = NULL;
	system->U.equations = NULL;

	return system;
}

void u_system_delete(u_system * system)
{
	if (system == NULL)
	{
		return;
	}
	
	/* decide who is releasing this memory */
	multi_equation_list_delete(system->U.equations);
	multi_equation_list_delete(system->U.zero_counter_multi_equation);
	multi_equation_list_delete(system->T);
		
	free(system);
}

void u_system_print(u_system * system)
{
	if (system == NULL)
	{
		return;
	}
	
	printf("multi_eq_number: %d\n", system->U.multi_eq_number); 
	
	printf("U:\n");
	multi_equation_list_print(system->U.equations);
	printf("\n");
	
	printf("T:\n");
	multi_equation_list_print(system->T);
	printf("\n");
}

multi_equation * multi_equation_new(multi_term * mult)
{
	multi_equation * multi;
	
	multi = (multi_equation *)malloc(sizeof(multi_equation));
	if (multi == NULL)
	{
		fprintf(stderr, "cannot allocate multi_equation\n");
	}
	
	multi->ref_cnt = 0;
	multi->counter = 0;
	multi->var_number = 0;
	multi->S = NULL;
	multi->M = mult;
	
	return multi;
}

void multi_equation_delete(multi_equation * multi)
{
	if (multi == NULL)
	{
		return;
	}
	
	multi->ref_cnt--;

	if (multi->ref_cnt == 0)
	{
		variable_list_delete(multi->S);
		multi_term_delete(multi->M);
		free(multi);	
	}
}

void multi_equation_print(multi_equation * multi)
{
	printf("[%d,%d]", multi->counter, multi->var_number);
	
	variable_list_print(multi->S);
	
	printf("=");
	
	multi_term_print(multi->M);
}

multi_equation_list * multi_equation_node_new(multi_equation * value, multi_equation_list * next)
{
	multi_equation_list * node;
	
	node = (multi_equation_list *)malloc(sizeof(multi_equation_list));
	if (node == NULL)
	{
		fprintf(stderr, "cannot allocate multi_equation_node\n");
		exit(-1);
	}

	node->value = value;
	node->value->ref_cnt++;
	node->next = next;
	
	return node;
}

void multi_equation_node_delete(multi_equation_list * multi)
{
	if (multi == NULL)
	{
		return;
	}
	
	multi_equation_delete(multi->value);
	free(multi);
}

void multi_equation_list_delete(multi_equation_list * multi)
{
	if (multi == NULL)
	{
		return;
	}
	
	while (multi != NULL)
	{
		multi_equation_list * node;
	
		node = multi->next;		
		multi_equation_delete(multi->value);
		free(multi);
		
		multi = node;
	}
}

void multi_equation_list_print(multi_equation_list * multi)
{
	int first = 1;

	if (multi == NULL)
	{
		printf("{}");
		return;
	}
	
	printf("{");
	
	while (multi != NULL)
	{
		if (first == 0)
		{
			printf(",\n");
		}
	
		multi_equation_print(multi->value);
		
		first = 0;
		
		multi = multi->next;
	}
	
	printf("}");
}

multi_term * multi_term_new(const char * f_symb, temp_mult_eq_list * args)
{
	multi_term * multi;
	
	multi = (multi_term *)malloc(sizeof(multi_term));
	if (multi == NULL)
	{
		fprintf(stderr, "cannot allocate multi_term\n");
	}
	
	multi->f_symb = f_symb;
	multi->args = args;
	
	return multi;
}

void multi_term_delete(multi_term * multi)
{
	if (multi == NULL)
	{
		return;
	}
	
	temp_mult_eq_list_delete(multi->args);
	free(multi);
}

void multi_term_print(multi_term * multi)
{
	if (multi == NULL)
	{
		printf("()");
		return;
	}
	
	if (multi->f_symb != NULL)
	{
		printf("%s", multi->f_symb);
	}

	if (multi->args != NULL)
	{	
		printf("(");
	
		temp_mult_eq_list_print(multi->args);

		printf(")");
	}
}

temp_mult_eq_list * temp_mult_eq_node_new(temp_mult_eq * value, temp_mult_eq_list * next)
{
	temp_mult_eq_list * node;
	
	node = (temp_mult_eq_list *)malloc(sizeof(temp_mult_eq_list));
	if (node == NULL)
	{
		fprintf(stderr, "cannot allocate temp_mult_eq_node\n");
		exit(-1);
	}
	
	node->value = value;
	node->next = next;
	
	return node;
}

void temp_mult_eq_node_delete(temp_mult_eq_list * multi)
{
	if (multi == NULL)
	{
		return;
	}
	
	if (multi->value != NULL)
	{
		free(multi->value);
	}
	free(multi);
}

void temp_mult_eq_list_delete(temp_mult_eq_list * multi)
{
	temp_mult_eq_list * node;

	if (multi == NULL)
	{
		return;
	}
	
	while (multi != NULL)
	{
		node = multi->next;
		
		temp_mult_eq_delete(multi->value);
		free(multi);
		
		multi = node;
	}
}

void temp_mult_eq_list_print(temp_mult_eq_list * multi)
{
	char first = 1;
	
	if (multi == NULL)
	{
		return;
	}
	
	while (multi != NULL)
	{
		if (first == 0)
		{
			printf(",");
		}
	
		temp_mult_eq_print(multi->value);
		
		first = 0;
		
		multi = multi->next;
	}
}

temp_mult_eq * temp_mult_eq_new(variable_queue * S, multi_term * M)
{
	temp_mult_eq * mult;
	
	mult = (temp_mult_eq *)malloc(sizeof(temp_mult_eq));
	if (mult == NULL)
	{
		fprintf(stderr, "cannot allocate temp_mult_eq\n");
		exit(-1);
	}
	
	mult->S = S;
	mult->M = M;
	
	return mult;
}

void temp_mult_eq_delete(temp_mult_eq * mult)
{
	if (mult == NULL)
	{
		return;
	}
	
	variable_queue_delete(mult->S);
	multi_term_delete(mult->M);
	free(mult);
}

void temp_mult_eq_print(temp_mult_eq * mult)
{
	if (mult == NULL)
	{
		return;
	}
	
	printf("<");
	
	variable_queue_print(mult->S);
	
	printf(",");
	
	multi_term_print(mult->M);
	
	printf(">");
}

variable_queue * variable_queue_new()
{
	return variable_queue_new_var(NULL);
};

variable_queue * variable_queue_new_var(variable * var)
{
	variable_queue * queue;
	
	queue = (variable_queue *)malloc(sizeof(variable_queue));
	if (queue == NULL)
	{
		fprintf(stderr, "cannot allocate variable queue\n");
		exit(-1);
	}
	
	if (var == NULL)
	{
		queue->head = queue->tail = NULL;
	}
	else
	{
		queue->head = queue->tail = variable_node_new(var, NULL);
	}
		
	return queue;
}

void variable_queue_delete(variable_queue * q)
{
	if (q == NULL)
	{
		return;
	}
	
	variable_list_delete(q->head);
	free(q);
}

variable * variable_queue_head(variable_queue * q1)
{
	if (q1 == NULL)
	{
		return NULL;
	}
	
	if (q1->head == NULL)
	{
		return NULL;
	}
	
	return q1->head->value;
}

void variable_queue_add_var(variable_queue * q1, variable * var)
{
	 if (q1->head == NULL && q1->tail == NULL)
	 {
	 	q1->head = q1->tail = variable_node_new(var, NULL);
	 	return;
	 }
	 
	 variable_list * vars = variable_node_new(var, NULL);
	 q1->tail->next = vars;
	 q1->tail = vars;
}

void variable_queue_remove_head(variable_queue * q1)
{
	if (q1 == NULL)
	{
		return;
	}
	
	if (q1->head != NULL)
	{
		variable_list * node;
		
		node = q1->head;		
		q1->head = q1->head->next;

		free(node);
	}
	
	if (q1->head == NULL)
	{
		q1->tail = NULL;
	}
}

char variable_queue_is_empty(variable_queue * q1)
{
	if (q1 == NULL)
	{
		return 1;
	}
	if (q1->head == NULL)
	{
		return 1;
	}
	return 0;
}

void variable_queue_append(variable_queue ** q1, variable_queue ** q2)
{
	if (*q2 == NULL)
	{
		return;
	}
	
	if ((*q1) == NULL)
	{
	    *q1 = *q2;
		return;
	}
	
	(*q1)->tail->next = (*q2)->head;
	if ((*q2)->tail != NULL)
	{
		(*q1)->tail = (*q2)->tail;
	}
	free(*q2);
	(*q2) = NULL;
}

void variable_queue_print(variable_queue * q)
{
	int first = 1;
	variable_list * vars;

	if (q == NULL)
	{
		printf("{}");
		return;
	}

	printf("{");
	
	vars = q->head;
	while (vars != NULL)
	{
		if (first == 0)
		{
			printf(",");
		}
	
		variable_print(vars->value);
		
		first = 0;
		
		vars = vars->next;
	}
	
	printf("}");
}

variable_list * variable_node_new(variable * value, variable_list * next)
{
	variable_list * node;
	
	node = (variable_list *)malloc(sizeof(variable_list));
	if (node == NULL)
	{
		fprintf(stderr, "cannot allocate variable_node\n");
		exit(-1);
	}
	
	node->value = value;
	node->next = next;
	
	return node;
}

void variable_node_delete(variable_list * node)
{
	if (node == NULL)
	{
		return;
	}
	
	free(node);
}

void variable_list_delete(variable_list * vars)
{
	variable_list * v;
	
	while (vars != NULL)
	{
		v = vars->next;
		
		/* variables are managed by symbol_table
		variable_delete(vars->value); */
		free(vars);
		
		vars = v;
	}
}

int variable_list_size(variable_list * vars)
{
	int size = 0;

	if (vars == NULL)
	{
		return size;
	}
	
	while (vars != NULL)
	{
		size++;
		vars = vars->next;
	}
	
	return size;
}

char variable_list_is_empty(variable_list * vars)
{
	if (vars == NULL)
	{
		return 1;
	}
	
	return 0;
}

void variable_list_print(variable_list * vars)
{
	char first = 1;
	variable * var;

	printf("{");

	while (vars != NULL)
	{
		var = vars->value;
		
		if (first == 0)
		{
			printf(",");
		}
		
		variable_print(var);		

		first = 0;
	
		vars = vars->next;
	}
	
	printf("}");
}

variable * variable_new(const char * name)
{
	variable * var;
	
	var = (variable *)malloc(sizeof(variable));
	if (var == NULL)
	{
		fprintf(stderr, "cannot allocate variable\n");
	}
	
	var->name = name;
	
	return var;
}

void variable_delete(variable * var)
{
	free(var);
}

void variable_hash_delete(const char * key, void * value)
{
	variable_delete(value);
}

void variable_inc_mult_eq_counter(variable * var, int inc)
{
	if (var == NULL)
	{
		return;
	}
	if (var->M == NULL)
	{
		return;
	}
	var->M->counter += inc;
}

void variable_print(variable * var)
{
	if (var == NULL)
	{
		return;
	}
	
	if (var->name != NULL)
	{
		printf("%s", var->name);
	}
}

void variable_hash_print(void * data)
{
	variable_print((variable *)data);
}

