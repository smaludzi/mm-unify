/*
 * list.h
 *
 * Copyright (C) 2008 Slawomir Maludzinski
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
//
// $Id: list.h 129 2010-01-09 21:05:22Z smaludzi $
//
#ifndef __LIST_H__
#define __LIST_H__ 1

typedef void List_algorithm(int type, void * data);

typedef void ListNode_deallocator(void * data);

typedef struct ListNode {
     int type;
     void * data;
     struct ListNode * prev;
     struct ListNode * next;
     ListNode_deallocator * deallocator;
} ListNode;

typedef struct List {
     ListNode * beg;
     ListNode * end;
} List;

typedef struct ListIterator {
     List * list;
     ListNode * curr;
} ListIterator;

List * list_new();

void list_delete(List * list);

void list_deallocator(void * data);

void list_null_deallocator(void * data);

unsigned int list_size(List * list);

void list_add_beg(List * list, void * data);
void list_add_beg_type(List * list, int type, void * data);
void list_add_beg_deallocator(List * list, void * data,
                              ListNode_deallocator * deallocator);
void list_add_beg_type_deallocator(List * list, int type, void * data,
                                   ListNode_deallocator * deallocator);

void list_add_end(List * list, void * data);
void list_add_end_type(List * list, int type, void * data);
void list_add_end_deallocator(List * list, void * data,
                              ListNode_deallocator * deallocator);
void list_add_end_type_deallocator(List * list, int type, void * data,
                                   ListNode_deallocator * deallocator);

void list_concatenate(List * first, List * second);

void list_concatenate_rev(List * first, List * second);

void list_reverse(List * list);

void list_for_each(List * list, List_algorithm * alg);

void * list_get_nth_node(List * list, int index);

ListNode_deallocator * list_get_deallocator(List * list);

// list iterator procedures
ListIterator list_iterator_first(List * list);

ListIterator list_iterator_last(List * list);

char list_iterator_is_last(ListIterator iterator);

void * list_iterator_data(ListIterator iterator);

void * list_iterator_take_data(ListIterator iterator);

int list_iterator_type(ListIterator iterator);

ListNode_deallocator * list_iterator_deallocator(ListIterator iterator);

void list_iterator_next(ListIterator * iterator);

void list_iterator_previous(ListIterator * iterator);

void list_iterator_set_data(ListIterator iterator, void * data);

void list_iterator_set_data_type(ListIterator iterator, int type, void * data);

void list_iterator_set_data_type_deallocator(ListIterator iterator,
                                             int type, void * data,
                                             ListNode_deallocator deallocator);

void list_iterator_change_type(ListIterator iterator, int type);

void list_iterator_change_deallocator(ListIterator iterator,
                                      ListNode_deallocator deallocator);

void list_iterator_delete(ListIterator * iterator);

void list_iterator_add_before_data(ListIterator iterator, void * data);

void list_iterator_add_before_data_type(ListIterator iterator,
                                        int type, void * data);

void list_iterator_add_before_data_type_deallocator(ListIterator iterator,
                                                    int type, void * data,
                                        ListNode_deallocator deallocator);

void list_iterator_add_after_data(ListIterator iterator, void * data);

void list_iterator_add_after_data_type(ListIterator iterator,
                                       int type, void * data);

void list_iterator_add_after_data_type_deallocator(ListIterator iterator,
                                                   int type, void * data,
                                        ListNode_deallocator deallocator);

void list_iterator_insert_list_after(ListIterator iterator, List * inset);

// list node procedures
ListNode * list_node_new(void * data);

ListNode * list_node_new_deallocator(void * data,
                                     ListNode_deallocator deallocator);
ListNode * list_node_new_type_deallocator(int type, void * data,
                                          ListNode_deallocator deallocator);
void list_node_delete(ListNode * list_node);

#endif /* __LIST_H__ */

