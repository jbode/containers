#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

void *copyInt( const void *data )
{
  const int *ldata = data;
  int *p = malloc( sizeof *p );
  if ( p )
    *p = *ldata;
  return p;
}

int cmpInt( const void *l, const void *r )
{
  const int *ll = l;
  const int *lr = r;

  if ( *ll < *lr )
    return -1;
  else if ( *ll > *lr )
    return 1;
  return 0;
}

char *randomStr( char *buf, size_t size )
{
  static const char *alpha = " abcdefghijklmnopqrstuvwxyz_,-";
  for ( size_t i = 0; i < size - 1; i++ )
    buf[i] = alpha[rand() % (sizeof alpha)];
  buf[size-1] = 0;
  return buf;
}

void *copyStr( const void *s )
{
  char *ls = malloc( strlen( (const char *) s ) + 1 );
  if ( ls )
    strcpy( ls, (const char *) s );
  return ls;
}

int cmpStr( const void *l, const void *r )
{
  return strcmp( (const char *) l, (const char *) r );
}

int main( void )
{
  srand(time(NULL));

  List *li = listCreate( copyInt, copyStr, free, free, cmpInt );
  List *ls = listCreate( copyStr, copyInt, free, free, cmpStr );
  if ( li && ls )
  {
    for ( size_t i = 0; i < 10; i++ )
    {
      int val = rand() % 1000;
      char buf[16];
 
      printf( "    inserting %d, \"%s\"\n", val, randomStr( buf, sizeof buf ) );
      Node *ni = listInsert( li, &val, buf );
      printf( "    inserting \"%s\", %d\n", buf, val );
      Node *ns = listInsert( ls, buf, &val );
      if ( !ni || !ns )
      {
        fputs( "    Error inserting value\n", stderr );
        break;
      }
    }

    printf( "    li list size: %zu\n", listSize( li ) );
    printf( "    ls list size: %zu\n", listSize( ls ) );

    puts( "    li list contents: " );
    for ( Node *n = listPop( li ); n; n = listPop( li ) )
    {
      printf( "\t    key: %5d; value \"%s\"\n", *(int *) nodeKey( n ), (char *) nodeData( n ) );
      nodeDestroy( n, free, free );
    };

    puts( "    ls list contents: " );
    for ( Node *n = listPop( ls ); n; n = listPop( ls ) )
    {
      printf( "\t    key: \"%s\"; value: %d\n", (char *) nodeKey( n ), *(int *) nodeData( n ) );
      nodeDestroy( n, free, free );
    }

    listDestroy( li );
    listDestroy( ls );
  }
  return 0;
}

