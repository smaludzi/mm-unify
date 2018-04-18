/*
 * hash.h
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
#ifndef __HASH_H__
#define __HASH_H__ 1

typedef struct hash_item {
    const char * key;
    void * value;
} hash_item;

typedef void hash_item_delete(const char * key, void * value);
typedef void hash_item_printer(void * value);

typedef struct hash {
    hash_item_delete * item_delete_func;
    hash_item * items;
    int size;
    int elems;
    int watermark;
} hash;

hash * hash_new();
hash * hash_new_size(int size);
hash * hash_new_delete(hash_item_delete * item_delete_func);
hash * hash_new_delete_size(hash_item_delete * item_delete_func, int size);

void hash_item_null_deallocator(hash_item * item);
void hash_delete(hash * h);

void hash_insert(hash * h, const char * key, void * value);
void * hash_search(hash * h, const char * key);
char hash_contains(hash * h, const char * key);
void hash_remove(hash * h, const char * key);

/** private **/
unsigned long hash_str(const char * str);
int hash_inc_size(hash * h);
void hash_resize(hash * h);
void hash_print(hash * h);
void hash_printer(hash * h, hash_item_printer * printer);

#endif /* __HASH_H__ */

