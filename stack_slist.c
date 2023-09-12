#include <stdio.h>
#include <stdlib.h>
 
typedef struct _data{
	int id;
}data;

typedef struct _snode{
	data *d;
	struct _snode *next;
}snode;

typedef struct _stack{
	snode *head,*tail;
	int maxsize;
	int count;
}stack;

stack *stack_create(int maxsize){
	stack *s=(stack *)malloc(sizeof(stack));
	
	s->count=0;
	s->maxsize=maxsize;
	
	s->head=NULL;
	s->tail=NULL;
	
	return(s);	
}

stack *stack_destroy(stack *s){
	/*释放所有节点*/
	snode *n=NULL;
	while(s->head!=NULL){
		n=s->head;
		s->head=s->head->next;
		
		free(n->d);
		free(n);
	}
	s->tail=NULL;
	
	/*释放控制块*/
	free(s);
	
	return(NULL);
} 

int stack_push(stack *s,int id){
	if(s->count>=s->maxsize){
		return(0);
	}
	
	snode *n=(snode *)malloc(sizeof(snode));
	data *d=(data *)malloc(sizeof(data));
	d->id=id;
	n->d=d;
	n->next=NULL;
	
	if(s->count<=0){
		s->head=n;
		s->tail=n;
	}else{
		n->next=s->head;
		s->head=n;
	}
	s->count++;
	return (-1);
}

snode *stack_pop(stack *s){
	if(s->count<=0){
		return(NULL);
	}
	snode *n=s->head; 
	if(s->count<=1){
        s->head=NULL;
        s->tail=NULL;
    }else{
		s->head=s->head->next;
	}
	s->count--;
	return(n);
}
	
snode *stack_get(stack *s){
	if(s->count<=0){
		return(NULL);
	}else{
		return(s->head);
	}
}

void stack_empty(stack *s){
	snode *n=s->head;
	while(s->count>=0){
		s->head=s->head->next;
		free(n->d);
		free(n);
		n=s->head;
		s->count--;
	}	
	s->tail=NULL;
}

int stack_is_empty(stack *s){
	return(s->count<=0 ? -1:0);
}

int stack_is_full(stack *s){
	return(s->count>=s->maxsize ? -1:0);
}

void main() {
	int i=0,j=0,k=0;
	
	printf("\n1.create...");
	stack *s=stack_create(10);
	if(s!=NULL){
		printf("\nsuccess...");
	}
	
	printf("\n\n2.push...");
	for(i=0;i<12;i++){
		if(stack_push(s,i)==0){
			k++;
			printf("\nfull");
		}else{
			j++;
		}
	}
	printf("\ntotal:12, success:%d, fail:%d",j,k);
	
	printf("\n\n3.pop...");
	snode *n=stack_pop(s);
	while(n!=NULL){
		printf("\n%d",n->d->id);
		n=stack_pop(s);
	}
	
	printf("\n\n4.destroy...");
	stack_destroy(s);
	
	getch();
}
