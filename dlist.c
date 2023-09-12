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
	list *l=(list *)malloc(sizeof(list));  /*分配空间*/ 
	if(l==NULL){   /*判定是否为空指针*/ 
		return(NULL);
	} 
	l->head=NULL; /*将链表头和链表尾分别置空*/
	l->tail=NULL;
	
	return(l); 
}

void list_destroy(list *l){
	node *t=NULL;  /*定义临时变量*/
	while(l->head!=NULL){  /*判定链表头是否为空，如果不为空，说明还有节点，需要释放，然后释放临时变量*/
		t=l->head;  /*指向链表头*/
		l->head=l->head->next; /*将链表头向后移*/
		
		free(t->d);   /*释放节点中的数据*/ 
		free(t);   /*释放节点本身*/ 
	}
	
	free(l); /*释放控制块*/
}
/*增加节点步骤：1.创建节点  2.节点初始化 3.调整指针，把节点增加到相应位置*/
int list_append_node(list *l,data *d){
	node *n=(node *)malloc(sizeof(node));   /*创建一个节点*/ 
	if(n==NULL){  /*动态分配，所以需要判定*/
		printf("%s","memory overflow!");
		return(0);
	}	
	n->d=d;  /*对节点做初始化操作，数据部分和指针*/  
	n->next=NULL;
	n->prev=NULL; 
	
	if(l->head==NULL){  /*链表无节点，为空*/
		l->head=n;
		l->tail=n;
	}else{
		n->prev=l->tail;  /*调整指针位置，n指向前面的指针应该指向原来的链尾*/  
		l->tail->next=n;  /*原来的链尾指向后面的指针应该指向新的*/ 
		l->tail=n; /*控制块指向链尾的指针,指向新增加的链尾*/ 
	}
	
	return(-1);
}

node *list_find(list *l,int id){
	node *t=l->head;  /*定义临时变量*/ 
	while(t!=NULL){
		if(t->d->id==id){
			return(t);
		}
		t=t->next;
	}	
	
	return(NULL);
}

int list_insert_before(list *l,int id,data *d){  /*在插入节点前需找到目标节点*/ 
	node *n;  /*创建新的节点*/ 
	node *t=l->head;
	while(t!=NULL){
		if(t->d->id==id){   /*判断当前要找的节点是不是目标节点*/
			break;
		}
		t=t->next;
	}	
	if(t==NULL){   /*目标节点不存在*/
		return(0);
	}
	
	n=(node *)malloc(sizeof(node));
	if(n==NULL){
		return(0);
	}
	n->d=d;    /*初始化*/
	n->next=NULL;
	n->prev=NULL;
	
	if(t->prev==NULL){  /*判断是不是链表头*/ 
		n->next=t;       /*插入在链表头*/ 
		t->prev=n;
		l->head=n;
	}else{ 
		n->prev=t->prev;   /*插入在链表中间或者尾部，算法是一样的*/ 
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
	
	if(t->next==NULL){      /*判断是不是链表尾*/ 
		n->prev=l->tail;   /*插入在链表尾*/ 
		l->tail->next=n;
		l->tail=n;
	}else{  
		n->prev=t;   /*插入在链表中间或者链表头，算法是一样的*/
		n->next=t->next;
		t->next->prev=n;
		t->next=n;
	}
	
	return(-1);
}

int list_remove(list *l,int id){   /*删除*/ 
	node *t=list_find(l,id);
	if(t==NULL){
		return(0);
	}	
	
	printf("\nfind %d",t->d->id);
	
	if(t->next==NULL && t->prev==NULL){  /*目标节点前后都为空，则直接置空整个链表*/ 
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

void list_iterate(list *l){  /*遍历*/ 
	node *t=l->head;  /*定义指向链表头的临时变量*/ 
	while(t!=NULL){
		printf("\n\t%d",t->d->id);  /*显示当前节点的数据*/
		t=t->next;  /*指向下一个*/ 
	}
}

void main() {
	int i; /*定义局部变量*/
	data *d=NULL; /*定义数据部分*/ 
	
	list *l=list_create();
	if(l==NULL){
		return;
	}
	
	for(i=0;i<10;i++){
		d=(data *)malloc(sizeof(data));
		d->id=i; /*创建好数据部分*/ 
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
