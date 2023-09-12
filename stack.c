#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
/*3.13 3.15·ÉÌìÔÆ¿ÎÌÃ*/
/*
typedef struct _data{
	int id;
}data;

typedef struct _stack{
	data *d;
	int size;
	int count;
}stack;

stack *stack_create(int size){
	stack *s=(stack *)malloc(sizeof(stack));
	s->size=size;
	s->count=0;
	s->d=(data *)malloc(sizeof(data)*size);
	
	return(s);
}

void stack_destroy(stack *s){
	free(s->d);
	free(s);	
}

int stack_push(stack *s,data d){
	if(s->count>=s->size){
		return(0);
	}
	
	s->d[s->count]=d;
	s->count++;
	
	return(-1); 
} 

data stack_pop(stack *s){
	s->count--;
	return(s->d[s->count]);
}

data stack_get(stack *s){
	return(s->d[s->count-1]);
} 

int stack_is_empty(stack *s){
	return(s->count<=0 ? -1:0);
}

int stack_is_full(stack *s){
	return(s->count>=s->size ? -1:0);
}

void main() {
	data d;
	stack *s=stack_create(10);
	
	for(int i=0;i<10;i++){
		d.id=i;
		stack_push(s,d);
	}
	
	while(stack_is_empty(s)!=-1){
		d=stack_pop(s);
		printf("\n%d",d.id);
	}
	
	stack_destroy(s);
	
	getch(); 
}
*/

typedef struct _data{
	int id;
}data;

typedef struct _stack{
	data **d;
	int size;
	int count;
}stack;

stack *stack_create(int size){
	stack *s=(stack *)malloc(sizeof(stack));
	s->size=size;
	s->count=0;
	s->d=(data **)malloc(sizeof(data *)*size);
	
	return(s);
}

void stack_destroy(stack *s){
	free(s->d);
	free(s);	
} 

int stack_push(stack *s,data *d){
	if(s->count>=s->size){
		return(0);
	}
	
	s->d[s->count]=d;
	s->count++;
	
	return(-1); 
} 

data *stack_pop(stack *s){
	s->count--;
	return(s->d[s->count]);
}

data *stack_get(stack *s){
	return(s->d[s->count-1]);
} 

int stack_is_empty(stack *s){
	return(s->count<=0 ? -1:0);
}

int stack_is_full(stack *s){
	return(s->count>=s->size ? -1:0);
}

void main() {
	data *d;
	stack *s=stack_create(10);
	
	for(int i=0;i<10;i++){
		d=(data *)malloc(sizeof(data));
		d->id=i;
		stack_push(s,d);
	}
	
	while(stack_is_empty(s)!=-1){
		d=stack_pop(s);
		printf("\n%d",d->id);
	}
	
	stack_destroy(s);
	
	getch(); 
}
