#ifndef NODE_H
#define NODE_H

#include <stdio.h>

typedef struct node Node;

Node *nodeCreate( const void *key, const void *data, void *(*kcopy)(const void *), void *(*dcopy)(const void *) );
void nodeDestroy( Node *n, void (*kfree)(void *), void (*dfree)(void *) );

Node *nodeNext( const Node *n );
Node *nodePrev( const Node *n );
Node *nodeLeft( const Node *n );
Node *nodeRight( const Node *n );
Node *nodeParent( const Node *n );
Node *nodeInsertBefore( Node *toInsert, Node *current );
Node *nodeInsertAfter( Node *toInsert, Node *current );
Node *nodeInsertLeft( Node *toInsert, Node *parent );
Node *nodeInsertRight( Node *toInsert, Node *parent );

Node *nodeRemoveBefore( Node *current );
Node *nodeRemoveAfter( Node *current );
Node *nodeRemoveLeft( Node *current );
Node *nodeRemoveRight( Node *current );

void *nodeKey( const Node *n );
void *nodeData( const Node *n );

void nodeDump( const Node *n, FILE *stream );

#endif
