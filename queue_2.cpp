#include <stdio.h>
#include <stdlib.h>

typedef struct _data{
	int id;
}data;

typedef struct _node{
	data *d;
	struct _node *next;
}node;

typedef struct _queue{
	node *head,*tail;
	int count;
    int maxsize;
}queue;

queue *queue_create(int maxsize){
	queue *q=(queue *)malloc(sizeof(queue));
	q->head=NULL;
	q->tail=NULL; 
	q->count=0;
	q->maxsize=maxsize;
	return q;
} 

queue *queue_destroy(queue *q){
	node *n=NULL;
	while(q->head!=NULL){
		n=q->head;
		q->head=q->head->next;
		
		free(n->d);
		free(n);
	}
	free(q);
	return(NULL);
}

int enqueue(queue *q,data *d){
	if(q->count>=q->maxsize){
		return(0);
	}
	node *n=(node *)malloc(sizeof(node));
	if(n==NULL){
		return 0;
	}
	n->d=d;
	n->next=NULL;
	
	if(q->head==NULL){
	q->head=n;
	q->tail=n;
	}
	else{
		q->tail->next=n;
		q->tail=n;
	}
	
	q->count++;
	return(-1);
}

node *dequeue(queue *q){
	node *n=q->head;
	if(q->count<=0){
		return NULL;
	} else if(q->count<=1){
		q->head=NULL;
		q->tail=NULL;
	}
	else{
		q->head=q->head->next;
	}
	q->count--;
	return(n);
}

int queue_is_empty(queue *q){
	if(q->head==q->tail==NULL){
		return 1;
	}
	else{
		return 0;
	}
}

int main(){
	queue *q=queue_create(10);
	
	int i;
	for(i=0;i<10;i++){
		data *d=(data *)malloc(sizeof(data));
		d->id=i;
		enqueue(q,d);
	}
	
	while(q->head!=NULL){
		node *n=dequeue(q);
		printf("³ö¶Ó£º%d\n",n->d->id);
	}
}
