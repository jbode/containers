#ifndef NODE_H
#define NODE_H

#include <stdio.h>

typedef struct node Node;

Node *nodeCreate( const void *key, const void *data, void *(*kcopy)(const void *), void *(*dcopy)(const void *) );
void nodeDestroy( Node *n, void (*kfree)(void *), void (*dfree)(void *) );

Node *nodeNext( const Node *n );
Node *nodePrev( const Node *n );

Node *insertBefore( Node *toInsert, Node *current );
Node *insertAfter( Node *toInsert, Node *current );

Node *removeBefore( Node *current );
Node *removeAfter( Node *current );

void *nodeKey( const Node *n );
void *nodeData( const Node *n );

void nodeDump( const Node *n, FILE *stream );

#endif
