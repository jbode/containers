#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stdio.h>
#include "node.h"

typedef struct list List;

List *listCreate( void *(*kcopy)(const void *), void *(*dcopy)(const void *), void (*kfree)(void *), void (*dfree)(void *), int (*kcmp)(const void *, const void *) );
void listClear( List *l );
void listDestroy( List *l );

size_t listSize( List *l );

/**
 * Insert data in key order
 */
Node *listInsert( List *l, const void *key, const void *data );

/**
 * Insert data at a given position; returns false if pos is out of range
 */
Node *listInsertAt( List *l, const void *key, const void *data, size_t pos );

/**
 * Add data to the head of the list
 */
Node *listPush( List *l, const void *key, const void *data );

/**
 * Append data to the tail of the list
 */
Node *listAppend( List *l, const void *key, const void *data );

/**
 * Get the position of a data item in the list; returns
 * false if key not found
 */
bool listFindPos( List *l, const void *key, size_t *pos );

/**
 * Return true if list contains key
 */
bool listContains( List *l, const void *key );

/**
 * Return node for a given key; return NULL if not found
 */
Node *listFind( List *l, const void *key );

/**
 * Get node at a given position; returns NULL if out of range
 */
Node *listGet( List *l, size_t pos );

/**
 * Remove item at given index, returns NULL if pos out of range
 */
Node *listRemoveAt( List *l, size_t pos );

/**
 * Remove item by key
 */
Node *listRemove( List *l, const void *key );

/**
 * Remove list head
 */
Node *listPop( List *l );

/**
 * Remove list tail
 */
Node *listPopTail( List *l );

/**
 * Dump the list for debugging purposes.
 */
void listDump( List *l, FILE *stream );

#endif
