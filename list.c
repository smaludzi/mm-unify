/*
 * list.c
 *
 * Copyright (C) 2003 Slawomir Maludzinski
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
// $Id: list.c 103 2009-01-10 13:11:51Z slawek $
//
#include <stdlib.h>
#include <stdio.h>
#include "list.h"

List * list_new()
{
     List * list = NULL;

     list = (List *) malloc(sizeof(List));
     if (list == NULL) {
         fprintf(stderr, "cannot allocate list\n");
         exit(1);
     }

     list->beg = NULL;
     list->end = NULL;
     return list;
}

void list_delete(List * list)
{
     ListNode * tmp = NULL;
     ListNode * node = NULL;

     if (list == NULL) {
          return;
     }
     node = list->beg;
     while (node != NULL) {
          tmp = node->next;
          list_node_delete(node);
          node = tmp;
     }
     free(list);
}

void list_deallocator(void * data)
{
     if (data == NULL) {
          return;
     }
     list_delete((List *)data);
}

void list_null_deallocator(void * data)
{
}

unsigned int list_size(List * list)
{
     ListNode * tmp = NULL;
     register unsigned int ret = 0;

     if (list == NULL) {
          return 0;
     }
     tmp = list->beg;
     while (tmp != NULL) {
          tmp = tmp->next;
          ret++;
     }
     return ret;
}

void list_add_beg(List * list, void * data)
{
     list_add_beg_type_deallocator(list, 0, data, NULL);
}

void list_add_beg_type(List * list, int type, void * data)
{
     list_add_beg_type_deallocator(list, type, data, NULL);
}

void list_add_beg_deallocator(List * list, void * data,
                              ListNode_deallocator * deallocator)
{
     list_add_beg_type_deallocator(list, 0, data, deallocator);
}

void list_add_beg_type_deallocator(List * list, int type, void * data,
                              ListNode_deallocator * deallocator)
{
     ListNode * node = NULL;
     node = list_node_new_type_deallocator(type, data, deallocator);

     node->next = list->beg;
     if (list->beg != NULL) {
          list->beg->prev = node;
          list->beg = node;
     } else {
          list->beg = node;
          list->end = node;
     }
}

void list_add_end(List * list, void * data)
{
     list_add_end_type_deallocator(list, 0, data, NULL);
}

void list_add_end_type(List * list, int type, void * data)
{
     list_add_end_type_deallocator(list, type, data, NULL);
}

void list_add_end_deallocator(List * list, void * data,
                              ListNode_deallocator * deallocator)
{
     list_add_end_type_deallocator(list, 0, data, deallocator);
}

void list_add_end_type_deallocator(List * list, int type, void * data,
                                   ListNode_deallocator * deallocator)
{
     ListNode * node = NULL;
     node = list_node_new_type_deallocator(type, data, deallocator);

     node->prev = list->end;
     if (list->end != NULL) {
          list->end->next = node;
          list->end = node;
     } else {
          list->beg = node;
          list->end = node;
     }
}

void list_concatenate(List * first, List * second)
{
     if (first == NULL || second == NULL) {
          return;
     }
     if (second->beg == NULL && second->end == NULL) {
          free(second);
          return;
     }
     if (first->beg == NULL || first->end == NULL) {
          first->beg = second->beg;
          first->end = second->end;
          free(second);
          return;
     }
     first->end->next = second->beg;
     second->beg->prev = first->end;
     first->end = second->end;
     free(second);
}

void list_concatenate_rev(List * first, List * second)
{
     if (first == NULL || second == NULL) {
          return;
     }
     if (second->beg == NULL && second->end == NULL) {
          free(second);
          return;
     }
     if (first->beg == NULL || first->end == NULL) {
          first->beg = second->beg;
          first->end = second->end;
          free(second);
          return;
     }
     second->end->next = first->beg;
     first->beg->prev = second->end;
     first->beg = second->beg;
     free(second);
}

void list_reverse(List * list)
{
     ListNode * tmp = NULL;
     if (list == NULL) {
          return;
     }
     tmp = list->beg;
     list->beg = list->end;
     list->end = tmp;
}

void list_for_each(List * list, List_algorithm * alg)
{
     ListNode * node = NULL;
     if (list == NULL) {
          return;
     }
     node = list->beg;
     while (node != NULL) {
          alg(node->type, node->data);
          node = node->next;
     }
}

void * list_get_nth_node(List * list, int index)
{
     int i = 0;
     void * ret = NULL;
     ListNode * node = NULL;

     if (list == NULL) {
          return NULL;
     }

     node = list->beg;
     while (node != NULL && i < index) {
          i++;
          node = node->next;
     }
     if (node != NULL && i == index) {
          ret = node->data;
     }
     return ret;
}

// list iterator procedures

ListIterator list_iterator_first(List * list)
{
     ListIterator iter;

     if (list == NULL) {
          iter.curr = NULL;
          iter.list = NULL;
     } else {
          iter.curr = list->beg;
          iter.list = list;
     }
     return iter;
}

ListIterator list_iterator_last(List * list)
{
     ListIterator iter;

     if (list == NULL) {
          iter.curr = NULL;
          iter.list = NULL;
     } else {
          iter.curr = list->end;
          iter.list = list;
     }
     return iter;
}

char list_iterator_is_last(ListIterator iterator)
{
     if (iterator.curr == NULL) {
          return 1;
     } else {
          return 0;
     }
}

void * list_iterator_data(ListIterator iterator)
{
     if (iterator.curr != NULL) {
          return iterator.curr->data;
     } else {
          return NULL;
     }
}

void * list_iterator_take_data(ListIterator iterator)
{
     void * ret = NULL;

     if (iterator.curr != NULL) {
          ret = iterator.curr->data;
          iterator.curr->data = NULL;
     } else {
          ret = NULL;
     }
     return ret;
}

int list_iterator_type(ListIterator iterator)
{
     if (iterator.curr != NULL) {
          return iterator.curr->type;
     } else {
          return 0;
     }
}

ListNode_deallocator * list_iterator_deallocator(ListIterator iterator)
{
     if (iterator.curr != NULL) {
          return iterator.curr->deallocator;
     } else {
          return NULL;
     }
}

void list_iterator_next(ListIterator * iterator)
{
     if (iterator == NULL) {
          return;
     }
     iterator->curr = iterator->curr->next;
}

void list_iterator_previous(ListIterator * iterator)
{
     if (iterator == NULL) {
          return;
     }
     iterator->curr = iterator->curr->prev;
}

void list_iterator_set_data(ListIterator iterator, void * data)
{
     list_iterator_set_data_type_deallocator(iterator, 0, data, NULL);
}

void list_iterator_change_type(ListIterator iterator, int type)
{
     if (iterator.curr == NULL) {
          return;
     }
     iterator.curr->type = type;
}

void list_iterator_change_deallocator(ListIterator iterator,
                                      ListNode_deallocator deallocator)
{
   if (iterator.curr == NULL) {
      return;
   }
   iterator.curr->deallocator = deallocator;
}

void list_iterator_set_data_type(ListIterator iterator,
                                 int type, void * data)
{
     list_iterator_set_data_type_deallocator(iterator, type, data, NULL);
}

void list_iterator_set_data_type_deallocator(ListIterator iterator,
                                             int type, void * data,
                                             ListNode_deallocator deallocator)
{
     if (iterator.curr == NULL) {
          return;
     }
     if (iterator.curr->data != NULL) {
          if (iterator.curr->deallocator != NULL) {
               iterator.curr->deallocator(iterator.curr->data);
          } else {
               free(iterator.curr->data);
          }
     }
     iterator.curr->type = type;
     iterator.curr->data = data;
     iterator.curr->deallocator = deallocator;
}

void list_iterator_delete(ListIterator * iterator)
{
     ListNode * prev, * next;

     if (iterator->list == NULL) {
          return;
     }
     if (iterator->curr == NULL) {
          return;
     }
     prev = iterator->curr->prev;
     next = iterator->curr->next;
     if (prev != NULL) {
          prev->next = next;
     }
     if (next != NULL) {
          next->prev = prev;
     }
     if (iterator->list->beg == iterator->curr) {
          iterator->list->beg = next;
     }
     if (iterator->list->end == iterator->curr) {
          iterator->list->end = prev;
     }
     list_node_delete(iterator->curr);
     iterator->curr = next;
}

void
list_iterator_add_before_data(ListIterator iterator, void * data)
{
     list_iterator_add_before_data_type_deallocator(iterator,
                                                    0, data, NULL);
}

void
list_iterator_add_before_data_type(ListIterator iterator,
                                   int type, void * data)
{
     list_iterator_add_before_data_type_deallocator(iterator,
                                                    type, data, NULL);
}

void
list_iterator_add_before_data_type_deallocator(ListIterator iterator,
                                               int type, void * data,
                                          ListNode_deallocator deallocator)
{
     ListNode * node = NULL;
     ListNode * prev = NULL;

     if (iterator.list == NULL) {
          return;
     }
     if (iterator.curr == NULL) {
          printf("Cannot add node to list before NULL\n");
          return;
     }
     prev = iterator.curr->prev;

     node = list_node_new_type_deallocator(type, data, deallocator);

     node->prev = prev;
     node->next = iterator.curr;
     if (iterator.curr != NULL) {
          iterator.curr->prev = node;
     }

     if (iterator.list->beg == iterator.curr) {
          iterator.list->beg = node;
     }
     if (iterator.list->end == NULL) {
          iterator.list->end = node;
     }
}

void
list_iterator_add_after_data(ListIterator iterator, void * data)
{
     list_iterator_add_after_data_type_deallocator(iterator, 0, data, NULL);
}

void
list_iterator_add_after_data_type(ListIterator iterator,
                                  int type, void * data)
{
     list_iterator_add_after_data_type_deallocator(iterator,
                                                   type, data, NULL);
}

void
list_iterator_add_after_data_type_deallocator(ListIterator iterator,
                                              int type, void * data,
                                           ListNode_deallocator deallocator)
{
     ListNode * node;
     ListNode * next = NULL;

     if (iterator.list == NULL) {
          return;
     }
     if (iterator.curr == NULL) {
          printf("Cannot add node after NULL\n");
          return;
     }
     next = iterator.curr->next;

     node = list_node_new_type_deallocator(type, data, deallocator);

     node->prev = iterator.curr;
     node->next = next;
     iterator.curr->next = node;

     if (iterator.list->beg == NULL) {
          iterator.list->beg = node;
     }
     if (iterator.list->end == iterator.curr) {
          iterator.list->end = node;
     }
}

void list_iterator_insert_list_after(ListIterator iterator, List * inset)
{
     if (iterator.list == NULL || inset == NULL) {
          return;
     }
     if (iterator.curr == NULL) {
          return;
     }
     if (inset->beg == NULL || inset->end == NULL) {
          return;
     }

     inset->beg->prev = iterator.curr;
     inset->end->next = iterator.curr->next;

     if (iterator.curr->next != NULL) {
          iterator.curr->next->prev = inset->end;
     }
     iterator.curr->next = inset->beg;

     if (iterator.list->beg == iterator.list->end) {
          iterator.list->end = inset->end;
     }

     free(inset);
}

// list node procedures
ListNode * list_node_new(void * data)
{
     return list_node_new_type_deallocator(0, data, NULL);
}

ListNode * list_node_new_deallocator(void * data,
                             ListNode_deallocator deallocator)
{
     return list_node_new_type_deallocator(0, data, deallocator);
}

ListNode * list_node_new_type_deallocator(int type, void * data,
                             ListNode_deallocator deallocator)
{
     ListNode * ret = NULL;

     ret = (ListNode *) malloc(sizeof(ListNode));
     if (ret == NULL) {
          fprintf(stderr, "Out of memory, sorry!\n");
          exit(EXIT_FAILURE);
     }
     ret->type = type;
     ret->data = data;
     ret->prev = NULL;
     ret->next = NULL;
     ret->deallocator = deallocator;

     return ret;
}

void list_node_delete(ListNode * listNode)
{
     if (listNode == NULL) {
          return;
     }
     if (listNode->data != NULL) {
          if (listNode->deallocator != NULL) {
               listNode->deallocator(listNode->data);
          } else {
               free(listNode->data);
          }
     }
     free(listNode);
}

