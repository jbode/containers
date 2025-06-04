#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "node.h"

enum linkType { LINK_PREV, LINK_NEXT, LINK_LEFT, LINK_RIGHT, LINK_PARENT, NUM_LINKS };
struct node {
  void *key;
  void *data;
  struct node *link[NUM_LINKS];
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

    memset( n->link, 0, sizeof *n->link * NUM_LINKS );
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
  return n->link[LINK_NEXT];
}

Node *nodePrev( const Node *n )
{
  assert( n && "nodePrev: n cannot be NULL" );
  return n->link[LINK_PREV];
}

Node *nodeLeft( const Node *n )
{
  assert( n && "nodeLeft: n cannot be NULL" );
  return n->link[LINK_LEFT];
}

Node *nodeRight( const Node *n )
{
  assert( n && "nodeRight: n cannot be NULL" );
  return n->link[LINK_RIGHT];
}

Node *nodeParent( const Node *n )
{
  assert( n && "nodeParent: n cannot be NULL" );
  return n->link[LINK_PARENT];
}

Node *nodeInsertBefore( Node *toInsert, Node *current )
{
  assert( toInsert && "insertBefore: toInsert cannot be NULL" );
  toInsert->link[LINK_NEXT] = current;
  if ( current )
  {
    toInsert->link[LINK_PREV] = current->link[LINK_PREV];
    if ( current->link[LINK_PREV] )
      current->link[LINK_PREV]->link[LINK_NEXT] = toInsert;

    current->link[LINK_PREV] = toInsert;
  }
  return toInsert;
}

Node *nodeInsertAfter( Node *toInsert, Node *current )
{
  assert( toInsert && "insertAfter: toInsert cannot be NULL" );
  toInsert->link[LINK_PREV] = current;
  if ( current )
  {
    toInsert->link[LINK_NEXT] = current->link[LINK_NEXT];
    if ( current->link[LINK_NEXT] )
      current->link[LINK_NEXT]->link[LINK_PREV] = toInsert;
    current->link[LINK_NEXT] = toInsert;
  }
  return toInsert;
}

Node *nodeInsertLeft( Node *toInsert, Node *parent )
{
  assert( toInsert && "insertLeft: toInsert cannot be NULL" );
  assert( toInsert && "insertLeft: parent cannot be NULL" );

  toInsert->link[LINK_LEFT] = parent->link[LINK_LEFT];
  toInsert->link[LINK_PARENT] = parent;
  parent->link[LINK_LEFT] = toInsert;
  return toInsert;
}

Node *nodeInsertRight( Node *toInsert, Node *parent )
{
  
  assert( toInsert && "insertRight: toInsert cannot be NULL" );
  assert( toInsert && "insertRight: parent cannot be NULL" );

  toInsert->link[LINK_RIGHT] = parent->link[LINK_RIGHT];
  toInsert->link[LINK_PARENT] = parent;
  parent->link[LINK_RIGHT] = toInsert;
  return toInsert;
}

Node *nodeRemoveBefore( Node *current )
{
  assert( current && "removeBefore: current cannot be NULL" );
  Node *pre = current->link[LINK_PREV];

  if ( pre )
    current->link[LINK_PREV] = pre->link[LINK_PREV];

  if ( pre->link[LINK_PREV] )
    pre->link[LINK_PREV]->link[LINK_NEXT] = current;

  return pre;
}

Node *nodeRemoveAfter( Node *current )
{
  assert( current && "removeAfter: current cannot be NULL" );
  Node *next = nodeNext( current );

  if ( next )
    current->link[LINK_NEXT] = next->link[LINK_NEXT];
  
  if ( next->link[LINK_NEXT] )
    next->link[LINK_NEXT]->link[LINK_PREV] = current;

  return next;
}

Node *nodeRemoveLeft( Node *current )
{
  Node *l = nodeLeft( current );
  if ( l )
  {
    current->link[LINK_LEFT] = l->link[LINK_LEFT];
    if ( l->link[LINK_LEFT] )
    {
      l->link[LINK_LEFT]->link[LINK_PARENT] = current;
    }
  }
  return l;
}

Node *nodeRemoveRight( Node *current )
{
  Node *r = nodeRight( current );
  if ( r )
  {
    current->link[LINK_RIGHT] = r->link[LINK_RIGHT];
    if ( r->link[LINK_RIGHT] )
      r->link[LINK_RIGHT]->link[LINK_PARENT] = current;
  }
  return r;
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
  fprintf( stream, "%p: {key: %p; data: %p; link[LINK_NEXT]: %p; link[LINK_PREV]: %p}\n", (void *) n, (void *) n->key, (void *) n->data, (void *) n->link[LINK_NEXT], (void *) n->link[LINK_PREV] );
}

