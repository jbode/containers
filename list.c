#include <stdlib.h>
#include <assert.h>
#include "list.h"

struct list {
  Node *head, *tail;
  size_t count;
  void *(*kcpy)(const void *);
  void *(*dcpy)(const void *);
  void (*kfree)(void *);
  void (*dfree)(void *);
  int  (*kcmp)(const void *, const void *);
};

static void *createInt( const void *i )
{
  (void) i;
  return malloc( sizeof (int) );
}

List *listCreate( void *(*kcpy)(const void *), void *(*dcpy)(const void *), void (*kfree)(void *), void (*dfree)(void *), int (*kcmp)(const void *, const void * ) )
{
  assert( kcpy && "listCreate: kcpy cannot be NULL" );
  assert( kfree && "listCreate: kfree cannot be NULL" );
  assert( kcmp && "listCreate: kcmp cannot be NULL" );

  List *l = malloc( sizeof *l );
  if ( l )
  {
    l->count = 0;
    l->kcpy = kcpy;
    l->dcpy = dcpy;
    l->kfree = kfree;
    l->dfree = dfree;
    l->kcmp = kcmp;

    int i;

    l->head = nodeCreate( &i, NULL, createInt, NULL );
    l->tail = nodeCreate( &i, NULL, createInt, NULL );
    nodeInsertBefore( l->head, l->tail );
  }
  return l;
}

void listClear( List *l )
{
  assert( l && "listClear: l cannot be NULL" );
  Node *toDelete;

  while ( nodeNext( l->head ) != l->tail )
  {
    toDelete = nodeRemoveAfter( l->head );
    nodeDestroy( toDelete, l->kfree, l->dfree );
  }
}

void listDestroy( List *l )
{
  assert( l && "listDestroy: l cannot be NULL" );
  listClear( l );
  nodeDestroy( l->head, free, NULL );
  nodeDestroy( l->tail, free, NULL );
  free( l );
}

size_t listSize( List *l )
{
  assert( l && "listSize: l cannot be NULL" );
  return l->count;
}

Node *listInsert( List *l, const void *key, const void *data )
{
  assert( l && "listInsert: l cannot be NULL" );
  assert( key && "listInsert: key cannot be NULL" );
  Node *n = nodeCreate( key, data, l->kcpy, l->dcpy );
  if ( n )
  {
    Node *cur;
    for ( cur = nodeNext( l->head ); cur != l->tail && l->kcmp( nodeKey( cur ), key ) < 0; cur = nodeNext( cur ) )
      ;
    if ( !nodeInsertBefore( n, cur ) )
    {
      nodeDestroy( n, l->kfree, l->dfree );
      n = NULL;
    }
    else
      l->count++;
  } 
  return n;
}

Node *listInsertAt( List *l, const void *key, const void *data, size_t pos )
{
  assert( l && "listInsertAt: l cannot be NULL" );
  assert( key && "listInsertAt: key cannot be NULL" );

  if ( pos > l->count )
    return NULL;

  Node *n = nodeCreate( key, data, l->kcpy, l->dcpy );
  if ( n )
  {
    Node *cur = l->head;
    for ( size_t i = 0; i < pos; i++ )
      cur = nodeNext( cur );

    if ( !nodeInsertBefore( n, cur ) )
    {
      nodeDestroy( n, l->kfree, l->dfree );
      n = NULL;
    }
    else
      l->count++;
  }
  return n;
}

Node *listPush( List *l, const void *key, const void *data )
{
  assert( l && "listPush: l cannot be NULL" );
  assert( key && "listPush: key cannot be NULL" );
  
  Node *n = nodeCreate( key, data, l->kcpy, l->dcpy );
  if ( n )
  {
    if ( !nodeInsertAfter( n, l->head ) )
    {
      nodeDestroy( n, l->kfree, l->dfree );
      n = NULL;
    }
    else
      l->count++;
  }
  return n;
}

Node *listAppend( List *l, const void *key, const void *data )
{
  assert( l && "listAppend: l cannot be NULL" );
  assert( key && "listAppend: key cannot be NULL" );

  Node *n = nodeCreate( key, data, l->kcpy, l->dcpy );
  if ( n )
  {
    if ( !nodeInsertBefore( n, l->tail ) )
    {
      nodeDestroy( n, l->kfree, l->dfree );
      n = NULL;
    }
    else
      l->count++;
  }
  return n;
}

bool listFindPos( List *l, const void *key, size_t *pos )
{
  assert( l && "listFindPos: l cannot be NULL" );
  assert( key && "listFindPos: key cannot be NULL" );
  assert( pos && "listFindPos: pos cannot be NULL" );

  *pos = 0;
  Node *n = nodeNext( l->head );
  while ( n && n != l->tail && l->kcmp( key, nodeKey( n ) ) )
  {
    (*pos)++;
    n = nodeNext( n );
  }

  if ( n == l->tail )
    return false;

  return true;
}

bool listContains( List *l, const void *key )
{
  assert( l && "listContains: l cannot be NULL" );
  assert( key && "listContains: key cannot be NULL" );

  return listFind( l, key ) != NULL;
}

Node *listFind( List *l, const void *key )
{
  Node *n = nodeNext( l->head );
  while ( n && n != l->tail && l->kcmp( key, nodeKey( n ) ) )
    n = nodeNext( n );

  if ( n != l->tail )
    return n;

  return NULL;
}
  
Node *listGet( List *l, size_t pos )
{
  assert( l && "listGet: l cannot be NULL" );

  Node *n = nodeNext( l->head );
  size_t i = 0;

  while ( n && n != l->tail  && i < pos )
  {
    n = nodeNext( n );
    i++;
  } 

  if ( n == l->tail )
    return NULL;

  return n;  
}

Node *listRemoveAt( List *l, size_t pos )
{
  assert( l && "listRemoveAt: l cannot be NULL" );
  
  if ( pos > l->count )
    return NULL;

  Node *n = nodeNext( l->head );
  size_t i = 0;
  
  while ( i < pos && n != l->tail )
  {
    i++;
    n = nodeNext( n );
  }

  if ( n == l->tail )
    return NULL;

  n = nodeRemoveBefore( nodeNext( n ) );
  if ( n )
    l->count--;

  return n;
}

Node *listRemove( List *l, const void *key )
{
  assert( l && "listRemove: l cannot be NULL" );
  assert( key && "listRemove: key cannot be NULL" );

  Node *n = nodeNext( l->head );
  while ( n && n != l->tail && l->kcmp( key, nodeKey( n ) ) != 0 )
    n = nodeNext( n );

  if ( !n || n == l->tail )
    return NULL;

  n = nodeRemoveBefore( nodeNext( n ) );
  if ( n )
    l->count--;

  return n;
}

Node *listPop( List *l )
{
  assert( l && "listPop: l cannot be NULL" );

  if ( nodeNext( l->head ) == l->tail )
    return NULL;

  Node *n = nodeRemoveAfter( l->head );
  if ( n )
    l->count--;

  return n;
}

Node *listPopTail( List *l )
{
  assert( l && "listPopTail: l cannot be NULL" );

  if ( nodePrev( l->tail ) == l->head )
    return false;

  Node *n = nodeRemoveBefore( l->tail );
  if ( n )
    l->count--;

  return n;
}

void listDump( List *l, FILE *stream )
{
  fprintf( stream, "%p: { head: %p, tail: %p }\n", (void *) l, (void *) l->head, (void *) l->tail );
}


