#include <stdio.h>
#include <stdlib.h>

typedef struct _data{
    int id;
}data;

typedef struct _node{
    data *d;
    struct _node *next,*prev;
}node;

typedef struct _stack{
    node *head,*tail;
    int count;
    int maxsize;
}stack;

stack *stack_create(int maxsize){
    stack *s=(stack *)malloc(sizeof(stack));
    s->count=0;
    s->maxsize=maxsize;
    s->head=NULL;
    s->tail=NULL;
    
    return (s);
}

int stack_push(stack *s,data *d){
    node *n=NULL;
    if(s->count>=s->maxsize){
        return (0);
    }
    
    n=(node *)malloc(sizeof(node));
    if(n==NULL){
        return (0);
    }
    n->d=d;
    n->next=NULL;
    n->prev=NULL;
    
    if(s->head==NULL){
        s->head=n;
        s->tail=n;
    }else{
        /*n->prev=s->tail;
        s->tail->next=n;
        s->tail=n;*/
        s->head->prev=n;
		n->next=s->head;
        s->head=n;
    }
    
    s->count++;/*�����ǵü�һ*/
    
    return (-1);
}

node *stack_pop(stack *s){ /*����ջ����ͬʱ��Ҫ�����ÿգ�������ָ��*/ 
    node *n=s->head;
    /*node *n=s->tail;*/
    if(s->count<=0){
        return(NULL);
    }
    
    if(s->count<=1){
        s->head=NULL;
        s->tail=NULL;
    }else{
        /*s->tail->prev->next=NULL;
        s->tail=s->tail->prev;*/
        s->head->next->prev=NULL;
        s->head=s->head->next; 
    }
    
    s->count--;
    
    return(n);
}

stack *stack_destroy(stack *s){	/*�ͷ����нڵ�*/
	node *n=s->head;
	while(n!=NULL){
		s->head=s->head->next;
		
		free(n->d);
		free(n);
		
		n=s->head;
	}
	s->tail=NULL;
	
	/*�ͷſ��ƿ�*/
	free(s);
	
	return(NULL);
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
		data *d=(data *)malloc(sizeof(data));
		d->id=i;
		if(stack_push(s,d)==0){
			k++;
			printf("\nfull");
		}else{
			j++;
		}
	}
	printf("\ntotal:12, success:%d, fail:%d",j,k);
	
	printf("\n\n3.pop...");
	node *n=stack_pop(s);
	while(n!=NULL){
		printf("\n%d",n->d->id);
		n=stack_pop(s);
	}
	
	printf("\n\n4.destroy...");
	stack_destroy(s);
	
	getch();
}




