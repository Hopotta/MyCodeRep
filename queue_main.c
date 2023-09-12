#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

typedef struct _data{
	int id;
}data;

typedef struct _queue{
	data **d;
	int size;
	int count;
}queue;

queue *queue_create(int size){
	queue *s=(queue *)malloc(sizeof(queue));
	s->size=size;
	s->count=0;
	s->d=(data **)malloc(sizeof(data *)*size);
	
	return(s);
}

void queue_destroy(queue *s){
	free(s->d);
	free(s);	
} 

int queue_enter(queue *s,data *d){  /*���*/ 
	if(s->count>=s->size){
		return(0);
	}
	
	s->d[s->count]=d;
	s->count++;
	
	return(-1); 
} 

data *queue_leave(queue *s){  /*����*/ 
	data *t=s->d[0];/*��һ��Ԫ�س��Ӻ���Ҫ���������ǰ��*/
	for(int i=0;i<s->count;i++){
		s->d[i]=s->d[i+1];
	}
	s->count--;
	return(t);
}

data *queue_get(queue *s){
	return(s->d[0]);
} 

int queue_is_empty(queue *s){
	return(s->count<=0 ? -1:0);
}

int queue_is_full(queue *s){
	return(s->count>=s->size ? -1:0);
}

void main() {
	data *d;
	queue *s=queue_create(10);
	
	for(int i=0;i<10;i++){
		d=(data *)malloc(sizeof(data));
		d->id=i;
		queue_enter(s,d);
	}
	
	while(queue_is_empty(s)!=-1){
		d=queue_leave(s);
		printf("\n%d",d->id);
	}
	
	queue_destroy(s);
	
	getch(); 
}
