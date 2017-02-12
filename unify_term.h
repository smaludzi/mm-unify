/*
 * unify_term.h
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
#ifndef __UNIFY_TERM_H__
#define __UNIFY_TERM_H__ 1

#include "term.h"
#include "unify.h"
#include "hash.h"

char terms_consistent(term * term1, term * term2);
int unify_terms(term * term1, term * term2);

u_system * create_system(term * term1, term * term2, hash * symbol_table);

multi_equation * create_multi_equation(term * term1, term * term2, hash * symbol_table);
multi_equation * create_multi_equation_variable(variable * var);

multi_term * create_multi_term(term * term1, term * term2, hash * symbol_table);
multi_term * create_multi_term_single(term * term1, hash * symbol_table);

temp_mult_eq * create_temp_mult_eq(term * term1, term * term2, hash * symbol_table);
temp_mult_eq * create_temp_mult_eq_single(term * term1, hash * symbol_table);

hash * create_symbol_table(term * term1, term * term2);
void symbol_table_add_variable(hash * symbol_table, term * term1);

#endif /* __UNIFY_TERM_H__ */

