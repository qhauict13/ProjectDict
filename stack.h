#ifndef __STACK__
#define __STACK__

typedef struct _node{
  char *key;
  struct _node *next;
} node;

extern node *makeNode(char *newKey);

extern void push(node **top, char *newKey);

extern void freeStack(node **top);

#endif 