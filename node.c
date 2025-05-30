#include <stdlib.h>
#include <assert.h>
#include "node.h"

struct node {
  void *key;
  void *data;
  struct node *next;
  struct node *prev;
};

Node *nodeCreate( const void *key, const void *data, void *(*kcopy)(const void *), void *(*dcopy)(const void *) )
{
  assert( key && "nodeCreate: key parameter cannot be NULL" );
  assert( kcopy && "nodeCreate: kcopy paramete cannot be NULL" );

  Node *n = malloc( sizeof *n );
  if ( n )
  {
    n->key = kcopy( key );
    if ( data && dcopy )
      n->data = dcopy( data );

    n->next = n->prev = NULL;
  }
  return n;
}

void nodeDestroy( Node *n, void (*kfree)( void * ), void (*dfree)( void *) )
{
  assert( n && "nodeDestroy: n cannot be NULL" );
  assert( kfree && "nodeDestroy: kfree cannot be NULL" );

  if ( n ) 
  {
    kfree( n->key );
    if ( dfree )
      dfree( n->data );
    else
      free( n->data );
    free( n );
  }
}

  
Node *nodeNext( const Node *n )
{
  assert( n && "nodeNext: n cannot be NULL" );
  return n->next;
}

Node *nodePrev( const Node *n )
{
  assert( n && "nodePrev: n cannot be NULL" );
  return n->prev;
}

Node *insertBefore( Node *toInsert, Node *current )
{
  assert( toInsert && "insertBefore: toInsert cannot be NULL" );
  toInsert->next = current;
  if ( current )
  {
    toInsert->prev = current->prev;
    if ( current->prev )
      current->prev->next = toInsert;

    current->prev = toInsert;
  }
  return toInsert;
}

Node *insertAfter( Node *toInsert, Node *current )
{
  assert( toInsert && "insertAfter: toInsert cannot be NULL" );
  toInsert->prev = current;
  if ( current )
  {
    toInsert->next = current->next;
    if ( current->next )
      current->next->prev = toInsert;
    current->next = toInsert;
  }
  return toInsert;
}

Node *removeBefore( Node *current )
{
  assert( current && "removeBefore: current cannot be NULL" );
  Node *pre = current->prev;

  if ( pre )
    current->prev = pre->prev;

  if ( pre->prev )
    pre->prev->next = current;

  return pre;
}

Node *removeAfter( Node *current )
{
  assert( current && "removeAfter: current cannot be NULL" );
  Node *next = current->next;

  if ( next )
    current->next = next->next;
  
  if ( next->next )
    next->next->prev = current;

  return next;
}

void *nodeKey( const Node *n )
{
  return n->key;
}

void *nodeData( const Node *n )
{
  return n->data;
}

void nodeDump( const Node *n, FILE *stream )
{
  fprintf( stream, "%p: {key: %p; data: %p; next: %p; prev: %p}\n", (void *) n, (void *) n->key, (void *) n->data, (void *) n->next, (void *) n->prev );
}

