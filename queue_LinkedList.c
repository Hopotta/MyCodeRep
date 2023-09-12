#include <stdio.h>
#include <stdlib.h>
typedef struct _data{
    int id;
}data;

typedef struct _node{
    data *d;
    struct _node *next,*prev;
}node;

typedef struct _queue{
    node *head,*tail;
    int count;
    int maxsize;
}queue;

queue *queue_create(int maxsize){
    queue *q=(queue *)malloc(sizeof(queue));
    q->count=0;
    q->maxsize=maxsize;
    q->head=NULL;
    q->tail=NULL;
    
    return (q);
}

int queue_enter(queue *q,data *d){
    node *n=NULL;
    if(q->count>=q->maxsize){
        return (0);
    }
    
    n=(node *)malloc(sizeof(node));
    if(n==NULL){
        return (0);
    }
    n->d=d;
    n->next=NULL;
    n->prev=NULL;
    
    
    if(q->head==NULL){
        q->head=n;
        q->tail=n;
    }else{
        n->prev=q->tail;
        q->tail->next=n;
        q->tail=n;
    }
    
    q->count++;
    
    return (-1);
}

node *queue_leave(queue *q){
    node *n=q->head;
    
    if(q->count<=0){
        return (NULL);
    }
    
    if(q->count<=1){
        q->head=NULL;
        q->tail=NULL;
    }else{
        q->head->next->prev=NULL;
        q->head=q->head->next;
    }
    
    q->count--;
        
    return (n);
}


