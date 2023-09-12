#include <stdio.h>
#include <stdlib.h>

typedef struct _data{
	int id;
}data;

typedef struct _snode{
	data *d;
	struct _snode *next;
}snode;

typedef struct _slist{
	snode *head,*tail;
}slist;

slist *slist_create(){
	slist *l=(slist *)malloc(sizeof(slist));
	if(l==NULL){
		return(NULL);
	} 
	l->head=NULL;
	l->tail=NULL;
	
	return(l); 
}

void slist_destroy(slist *l){
	snode *t=NULL;
	while(l->head!=NULL){
		t=l->head;
		l->head=l->head->next;
		
		free(t->d);
		free(t);
	}
	
	free(l); 
}

int slist_append_node(slist *l,data *d){
	snode *n=(snode *)malloc(sizeof(snode));
	if(n==NULL){
		printf("%s","memory overflow!");
		return(0);
	}	
	n->d=d;
	n->next=NULL;
	
	if(l->head==NULL){
		l->head=n;
		l->tail=n;
	}else{
		l->tail->next=n;
		l->tail=n;
	}
	
	return(-1);
}

snode *slist_find(slist *l,int id){
	snode *t=l->head;
	while(t!=NULL){
		if(t->d->id==id){
			return(t);
		}
		t=t->next;
	}	
	
	return(NULL);
}

snode *slist_find_previous(slist *l,int id){  /*找到目标节点t前面的节点p，方便插入的时候调整指针*/ 
	snode *p=NULL;
	if(l->head!=NULL && l->head->d->id==id){
		p=NULL;
	}else{
		p=l->head;
		while(p!=NULL && p->next!=NULL){
			if(p->next->d->id==id){
				break;
			}
			p=p->next;
		}	
	}
	
	return(p);
} 

int slist_insert_before(slist *l,int id,data *d){
	snode *n;
	snode *p=NULL;
	snode *t=NULL;
	if(l->head!=NULL && l->head->d->id==id){  /*find_previous函数的调用*/ 
		p=NULL;
		t=l->head;
	}else{
		t=NULL;
		p=l->head;
		while(p!=NULL && p->next!=NULL){
			if(p->next->d->id==id){
				t=p->next;
				break;
			}
			p=p->next;
		}	
	}
	if(t==NULL){
		return(0);
	}
	
	n=(snode *)malloc(sizeof(snode));
	if(n==NULL){
		return(0);
	}
	n->d=d;
	n->next=NULL;
	
	if(p==NULL){
		n->next=t;
		l->head=n;
	}else{
		n->next=t;
		p->next=n;
	}
	
	return(-1);
}

int slist_insert_after(slist *l,int id,data *d){
	snode *n;
	snode *t=l->head;
	while(t!=NULL){
		if(t->d->id==id){
			break;
		}
		t=t->next;
	}	
	if(t==NULL){
		return(0);
	}
	
	n=(snode *)malloc(sizeof(snode));
	if(n==NULL){
		return(0);
	}
	n->d=d;
	n->next=NULL;
	
	if(t->next==NULL){
		l->tail->next=n;
		l->tail=n;
	}else{
		n->next=t->next;
		t->next=n;
	}
	
	return(-1);
}

int slist_remove(slist *l,int id){
	snode *p=NULL;
	snode *t=l->head;
	if(l->head!=NULL && l->head->d->id==id){
		p=NULL;
		t=l->head;
	}else{
		t=NULL;
		p=l->head;
		while(p!=NULL && p->next!=NULL){
			if(p->next->d->id==id){
				t=p->next;
				break;
			}
			p=p->next;
		}	
	}
	if(t==NULL){
		return(0);
	}

	if(p==NULL && t->next==NULL){  /*t是链表头，且链表只有链表头非空*/
		l->head=NULL;
		l->tail=NULL;
	}else if(p==NULL){ /*t是链表头*/ 
		l->head=t->next; 
	}else if(t->next==NULL){ /*t是链表尾*/ 
		p->next=NULL;
		l->tail=p;
	}else{
		p->next=t->next;
	}
	
	free(t->d); /*释放节点和地址*/ 
	free(t);
	
	return(-1);
} 

snode *slist_extract(slist *l,int id){
	snode *p=NULL;
	snode *t=l->head;
	if(l->head!=NULL && l->head->d->id==id){
		p=NULL;
		t=l->head;
	}else{
		t=NULL;
		p=l->head;
		while(p!=NULL && p->next!=NULL){
			if(p->next->d->id==id){
				t=p->next;
				break;
			}
			p=p->next;
		}	
	}
	if(t==NULL){
		return(NULL);
	}

	if(p==NULL && t->next==NULL){
		l->head=NULL;
		l->tail=NULL;
	}else if(p==NULL){
		l->head=t->next;
	}else if(t->next==NULL){
		p->next=NULL;
		l->tail=p;
	}else{
		p->next=t->next;
	}
	
	return(t);
} 

void slist_iterate(slist *l){
	snode *t=l->head;
	while(t!=NULL){
		printf("\n\t%d",t->d->id);
		t=t->next;
	}
}

void slist_test() {
	int i;
	data *d=NULL;
	
	slist *l=slist_create();
	if(l==NULL){
		return;
	}
	
	for(i=0;i<10;i++){
		d=(data *)malloc(sizeof(data));
		d->id=i;
		slist_append_node(l,d);
	}
	
	/*insert_before*/
	d=(data *)malloc(sizeof(data));
	d->id=11;
	slist_insert_before(l,0,d);
	
	d=(data *)malloc(sizeof(data));
	d->id=55;
	slist_insert_before(l,5,d);

	d=(data *)malloc(sizeof(data));
	d->id=99;
	slist_insert_before(l,9,d);
	

	/*insert_after*/
	d=(data *)malloc(sizeof(data));
	d->id=111;
	slist_insert_after(l,0,d);
	
	d=(data *)malloc(sizeof(data));
	d->id=555;
	/*slist_insert_after(l,5,d);*/
	
	d=(data *)malloc(sizeof(data));
	d->id=999;
	slist_insert_after(l,9,d);
	
	/*slist_remove(l,0);
	slist_remove(l,5);
	slist_remove(l,999);*/

	slist_iterate(l);
	
	slist_destroy(l); 
	
	getch();
}

void main(){
	slist_test();
}

