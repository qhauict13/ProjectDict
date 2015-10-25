#include "stack.h"
#include <string.h>
#include <stdlib.h>

node *makeNode(char *newKey){
  node *newNode;
  newNode = (node*)malloc(sizeof(node));
  if(newNode == NULL) return NULL;

  newNode->key = (char*)malloc(sizeof(char)*100);

  strcpy(newNode->key, newKey);
  newNode->next = NULL;
  return newNode;
}

void push(node **top, char *newKey){
  if(*top == NULL){
    *top = makeNode(newKey);
    return;
  }
  node *newNode = makeNode(newKey);
  newNode->next = *top;
  *top = newNode;
}

void freeStack(node **top){
  if(*top == NULL)
    return;
  while(*top != NULL){
    node* temp = (node*)malloc(sizeof(node));
    temp = *top;
    *top = (*top)->next;
    free(temp);
  }
}
