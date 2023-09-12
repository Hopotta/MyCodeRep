#include <stdio.h>
#include <stdlib.h>

typedef struct _data{
	int id;
}data;

typedef struct _node{
	data *d;
	struct _node *prev,*next;
}node;

typedef struct _list{
	node *head,*tail;
}list;

list *list_create(){
	list *l=(list *)malloc(sizeof(list));  /*����ռ�*/ 
	if(l==NULL){   /*�ж��Ƿ�Ϊ��ָ��*/ 
		return(NULL);
	} 
	l->head=NULL; /*������ͷ������β�ֱ��ÿ�*/
	l->tail=NULL;
	
	return(l); 
}

void list_destroy(list *l){
	node *t=NULL;  /*������ʱ����*/
	while(l->head!=NULL){  /*�ж�����ͷ�Ƿ�Ϊ�գ������Ϊ�գ�˵�����нڵ㣬��Ҫ�ͷţ�Ȼ���ͷ���ʱ����*/
		t=l->head;  /*ָ������ͷ*/
		l->head=l->head->next; /*������ͷ�����*/
		
		free(t->d);   /*�ͷŽڵ��е�����*/ 
		free(t);   /*�ͷŽڵ㱾��*/ 
	}
	
	free(l); /*�ͷſ��ƿ�*/
}
/*���ӽڵ㲽�裺1.�����ڵ�  2.�ڵ��ʼ�� 3.����ָ�룬�ѽڵ����ӵ���Ӧλ��*/
int list_append_node(list *l,data *d){
	node *n=(node *)malloc(sizeof(node));   /*����һ���ڵ�*/ 
	if(n==NULL){  /*��̬���䣬������Ҫ�ж�*/
		printf("%s","memory overflow!");
		return(0);
	}	
	n->d=d;  /*�Խڵ�����ʼ�����������ݲ��ֺ�ָ��*/  
	n->next=NULL;
	n->prev=NULL; 
	
	if(l->head==NULL){  /*�����޽ڵ㣬Ϊ��*/
		l->head=n;
		l->tail=n;
	}else{
		n->prev=l->tail;  /*����ָ��λ�ã�nָ��ǰ���ָ��Ӧ��ָ��ԭ������β*/  
		l->tail->next=n;  /*ԭ������βָ������ָ��Ӧ��ָ���µ�*/ 
		l->tail=n; /*���ƿ�ָ����β��ָ��,ָ�������ӵ���β*/ 
	}
	
	return(-1);
}

node *list_find(list *l,int id){
	node *t=l->head;  /*������ʱ����*/ 
	while(t!=NULL){
		if(t->d->id==id){
			return(t);
		}
		t=t->next;
	}	
	
	return(NULL);
}

int list_insert_before(list *l,int id,data *d){  /*�ڲ���ڵ�ǰ���ҵ�Ŀ��ڵ�*/ 
	node *n;  /*�����µĽڵ�*/ 
	node *t=l->head;
	while(t!=NULL){
		if(t->d->id==id){   /*�жϵ�ǰҪ�ҵĽڵ��ǲ���Ŀ��ڵ�*/
			break;
		}
		t=t->next;
	}	
	if(t==NULL){   /*Ŀ��ڵ㲻����*/
		return(0);
	}
	
	n=(node *)malloc(sizeof(node));
	if(n==NULL){
		return(0);
	}
	n->d=d;    /*��ʼ��*/
	n->next=NULL;
	n->prev=NULL;
	
	if(t->prev==NULL){  /*�ж��ǲ�������ͷ*/ 
		n->next=t;       /*����������ͷ*/ 
		t->prev=n;
		l->head=n;
	}else{ 
		n->prev=t->prev;   /*�����������м����β�����㷨��һ����*/ 
		n->next=t;
		t->prev->next=n;
		t->prev=n;
	}
	
	return(-1);
}

int list_insert_after(list *l,int id,data *d){
	node *n;  
	node *t=l->head;
	while(t!=NULL){
		if(t->d->id==id){
			break;
		}
		t=t->next;
	}	
	if(t==NULL){   
		return(0);
	}
	
	n=(node *)malloc(sizeof(node));  
	if(n==NULL){
		return(0);
	}
	n->d=d;  
	n->next=NULL;  
	n->prev=NULL;
	
	if(t->next==NULL){      /*�ж��ǲ�������β*/ 
		n->prev=l->tail;   /*����������β*/ 
		l->tail->next=n;
		l->tail=n;
	}else{  
		n->prev=t;   /*�����������м��������ͷ���㷨��һ����*/
		n->next=t->next;
		t->next->prev=n;
		t->next=n;
	}
	
	return(-1);
}

int list_remove(list *l,int id){   /*ɾ��*/ 
	node *t=list_find(l,id);
	if(t==NULL){
		return(0);
	}	
	
	printf("\nfind %d",t->d->id);
	
	if(t->next==NULL && t->prev==NULL){  /*Ŀ��ڵ�ǰ��Ϊ�գ���ֱ���ÿ���������*/ 
		l->head=NULL;
		l->tail=NULL;
	}else if(t->prev==NULL){ 
		t->next->prev=NULL;
		l->head=t->next;
	}else if(t->next==NULL){
		printf("\ntail %d",t->d->id);
		t->prev->next=NULL;
		l->tail=t->prev;
	}else{
		t->prev->next=t->next;
		t->next->prev=t->prev;
	}
	
	printf("\nfree");
	free(t->d);
	free(t);
	
	return(-1);
} 

void list_iterate(list *l){  /*����*/ 
	node *t=l->head;  /*����ָ������ͷ����ʱ����*/ 
	while(t!=NULL){
		printf("\n\t%d",t->d->id);  /*��ʾ��ǰ�ڵ������*/
		t=t->next;  /*ָ����һ��*/ 
	}
}

void main() {
	int i; /*����ֲ�����*/
	data *d=NULL; /*�������ݲ���*/ 
	
	list *l=list_create();
	if(l==NULL){
		return;
	}
	
	for(i=0;i<10;i++){
		d=(data *)malloc(sizeof(data));
		d->id=i; /*���������ݲ���*/ 
		list_append_node(l,d);
	}
	
	/*insert_before*/
	d=(data *)malloc(sizeof(data));
	d->id=11;
	list_insert_before(l,0,d);
	
	d=(data *)malloc(sizeof(data));
	d->id=55;
	list_insert_before(l,5,d);

	d=(data *)malloc(sizeof(data));
	d->id=99;
	list_insert_before(l,9,d);
	

	/*insert_after*/
	d=(data *)malloc(sizeof(data));
	d->id=111;
	list_insert_after(l,0,d);
	
	d=(data *)malloc(sizeof(data));
	d->id=555;
	list_insert_after(l,5,d);
	
	d=(data *)malloc(sizeof(data));
	d->id=999;
	list_insert_after(l,9,d);
	
	list_remove(l,0);
	list_remove(l,5);
	list_remove(l,999);

	list_iterate(l);
	
	list_destroy(l); 
	
	getch();
}
