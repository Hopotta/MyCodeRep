#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

typedef struct _data{
	int id;
}data;

typedef struct _node{
	data *d;
	struct _node *parent,*first_child,*last_child,*next_sibling,*prev_sibling;
}node;

typedef struct _tree{
	node *root;
}tree;

tree *tree_create(){
	tree *t=(tree *)malloc(sizeof(tree));
	t->root=NULL;
	
	return(t);
}

node *tree_get_last_child(node *n){
	while(n!=NULL && n->last_child!=NULL){
		n=n->last_child;
	}
	
	return(n);
}

node *tree_get_prev_node(node *n){
	if(n->prev_sibling!=NULL){
    	if(n->prev_sibling->last_child!=NULL){
      		node *t=n->prev_sibling->last_child;
      		while(t->last_child!=NULL){ 
        		t=t->last_child;
        	}
        	return(t); 
    	}else{
    		return(n->prev_sibling);
		} 
  	}else{
  		return(n->parent);
	}
}

void tree_empty(tree *t){ /*�����һ��Ҷ�ӽڵ㿪ʼ��һֱ��ǰ����*/ 
	node *n=tree_get_last_child(t->root);
	while(n!=NULL){
		node *n1=n;
		n=tree_get_prev_node(n);
		
		free(n1->d);
		free(n1);
	}
	t->root=NULL;
}

void tree_destroy(tree *t){
	tree_empty(t);
	
	free(t); 
}
/*����10��*/ 
node *tree_get_next_node(node *n){
	if(n==NULL){
		return(NULL);
	}
	
	if(n->first_child!=NULL){
		return(n->first_child);
	}else{
		if(n->next_sibling!=NULL){
        	return(n->next_sibling);
		}else{
      		while(n->parent!=NULL){
         		if(n->parent->next_sibling!=NULL){
				 	return(n->parent->next_sibling);
				}
           		n=n->parent;
			}
        }
        return(NULL);
    }
}

node *tree_find_node(tree *t,int id){
	node *n=t->root;
	while(n!=NULL){
		if(n->d->id==id){
			return(n);
		}
		n=tree_get_next_node(n);
	}
	
	return(NULL);
}

void tree_iterate(tree *t){
	node *n=t->root;
	while(n!=NULL){
		printf("\n\t%d",n->d->id);
		n=tree_get_next_node(n);
	}
}

int tree_append_node(tree *t,int id,data *d){
	node *p=tree_find_node(t,id);
	if(p==NULL && t->root!=NULL){
		return(0);
	}
	
	node *n=(node *)malloc(sizeof(node));
	if(n==NULL){
		return(0);
	}
	n->d=d;
	n->parent=NULL;
	n->first_child=NULL;
	n->last_child=NULL;
	n->next_sibling=NULL;
	n->prev_sibling=NULL;
	
	n->parent=p;
	if(p==NULL){
		t->root=n;
	}else if(p->first_child==NULL){
		p->first_child=n;
		p->last_child=n;
	}else{
		/*append node before the first child
		n->next_sibling=p->first_child;
		p->first_child->prev_sibling=n;
		p->first_child=n;
		*/
		
		/*append node after the last child*/
		n->prev_sibling=p->last_child;
		p->last_child->next_sibling=n;
		p->last_child=n;
	}
	
	return(-1);
}

node *tree_extract(tree *t,int id){
	node *n=tree_find_node(t,id); 
	if(n==NULL){ /*�Ҳ���Ŀ��ڵ�*/
		return(NULL);
	}
	
	if(n->parent==NULL){ /*Ŀ��ڵ�Ϊ���ڵ�*/
		return(NULL);
	}
	
	if(n->prev_sibling==NULL && n->next_sibling==NULL){
		n->parent->first_child=NULL;
		n->parent->last_child=NULL;
	}else if(n->prev_sibling==NULL){
		n->parent->first_child=n->next_sibling;
		n->parent->first_child->prev_sibling=NULL;
		/*n->next_sibling->prev_sibling=NULL*/
	}else if(n->next_sibling==NULL){
		n->parent->last_child=n->prev_sibling;
		n->parent->last_child->next_sibling=NULL;
	}else{
		n->next_sibling->prev_sibling=n->prev_sibling;
		n->prev_sibling->next_sibling=n->next_sibling;
	}
	n->parent=NULL;
	n->next_sibling=NULL;
	n->prev_sibling=NULL;
	
	return(n);
}

int tree_cut(tree *t,int id){   /*1.���ң�����ɾ�����ڵ�  2.����ָ�룬���루extract��  3.�ͷ������Ŀռ�*/
	node *n=tree_extract(t,id);
	if(n==NULL){
		return(0);
	}
	
	n=tree_get_last_child(n);
	while(n!=NULL){
		node *n1=tree_get_prev_node(n);
		free(n->d);
		free(n);
		n=n1;
	}
	
	return(-1);
}	

int tree_has_as_parent(tree *t,int id,int pid){
	if(id==pid){
		return(0);
	} 
	
	node *n=tree_find_node(t,id);
	if(n==NULL){
		return(0);
	}
	
	n=n->parent;
	while(n!=NULL){
		if(n->d->id==pid){
			return(-1);
		}
		n=n->parent;
	}	
	
	return(0);
} 

int tree_reparent(tree *t,int id,int pid){
	if(id==pid){
		return(0);
	} 
	
	node *n=tree_find_node(t,id);
	if(n==NULL){
		return(0);
	}
	
	node *p=tree_find_node(t,pid);
	if(p==NULL){
		return(0);
	}
	
	if(n->parent==NULL || n->parent->d->id==pid){ /*nΪ���ڵ����n�Ѿ���p���ӽڵ�*/
		return(0);
	}
	
	node *pn=p->parent;   /*�����ع����Լ����ӽڵ���*/
	while(pn!=NULL){
		if(pn->d->id==id){
			return(0);
		}
		pn=pn->parent;
	}
	
	/*����ָ��*/ 
	if(n->prev_sibling==NULL && n->next_sibling==NULL){
		n->parent->first_child=NULL;
		n->parent->last_child=NULL;
	}else if(n->prev_sibling==NULL){
		n->parent->first_child=n->next_sibling;
		n->parent->first_child->prev_sibling=NULL;
	}else if(n->next_sibling==NULL){
		n->parent->last_child=n->prev_sibling;
		n->parent->last_child->next_sibling=NULL;
	}else{
		n->next_sibling->prev_sibling=n->prev_sibling;
		n->prev_sibling->next_sibling=n->next_sibling;
	}
	n->parent=NULL;
	n->next_sibling=NULL;
	n->prev_sibling=NULL;	 
	
	n->parent=p;
	if(p->first_child==NULL){
		p->first_child=n;
		p->last_child=n;
	}else{
		/*append subtree before the first child
		n->next_sibling=p->first_child;
		p->first_child->prev_sibling=n;
		p->first_child=n;
		*/
		
		/*append subtree after the last child*/
		n->prev_sibling=p->last_child;
		p->last_child->next_sibling=n;
		p->last_child=n;
	}	
	
	return(-1); 
}

/*�������*/
void tree_reverse_iterate(tree *t){
	node *n=tree_get_last_child(t->root);
	while(n!=NULL){
		printf("\n%d",n->d->id);
		n=tree_get_prev_node(n);
	}
}

void tree_get_leafs1(tree *t){  /*������������Ҷ�ӽڵ㣬���������������*/
	node *n=t->root;  
	while(n!=NULL){  /*�����޸��ж����������󲿼�����n->parent!=null&&n->first_child!=null*/ 
		if(n->first_child==NULL){
			printf("\n%d",n->d->id);
		}
		n=tree_get_next_node(n);
	}	
}

void tree_get_leafs(tree *t,int id){  /*�ҵ�û���ӽڵ�Ľڵ㣬��ΪҶ�ӽڵ�*/
	node *n=tree_find_node(t,id);  /*id��Ӧ�Ľڵ��Ƿ����*/
	while(n!=NULL && n->last_child!=NULL){   /*�ҵ������Ǹ��ӽڵ㣬������*/ 
		n=n->last_child;
	}
	
	while(n!=NULL){  
		if(n->first_child==NULL){ /*�ж�n�ǲ���Ҷ�ӽڵ㣬���������ʾ����*/ 
			printf("\n%d",n->d->id);
		}
		if(n->d->id==id){ /*�ֻص����Լ��ˣ��Ϳ��Խ�������ѭ��*/ 
			break;
		}else{
			n=tree_get_prev_node(n);/*��Ȼ�ͽ�����ǰ��*/ 
		}
	}
}


typedef struct _test_data{
	int id;
	int pid;
}test_data;

/*              1                    */
/*       ------------                */
/*      /      |     \               */
/*     2        3      4             */
/*    --       --                    */
/*   /  \     /  \                   */
/*  5    6   7    8                  */
/*                                   */

test_data ds[]={
	{1,-1},
	{2,1},
	{3,1},
	{4,1},
	{5,2},
	{6,2},
	{7,3},
	{8,3}
};

void test_tree(){
	printf("\n\ninit");
	tree *t=tree_create();
	
	printf("\n\nadd nodes");
	int i;
	for(i=0;i<8;i++){
		data *d=(data *)malloc(sizeof(data));
		d->id=ds[i].id;
		tree_append_node(t,ds[i].pid,d);
	}
	tree_iterate(t);
	
	printf("\n\nreverse iterate");
	tree_reverse_iterate(t);
	
	printf("\n\nget leafs");
	tree_get_leafs(t,1);
	
	printf("\n\ncut node:2");
	tree_cut(t,2);
	tree_iterate(t);
	
	printf("\n\nreparent:7->4");
	tree_reparent(t,7,4);
	tree_iterate(t);
	
	printf("\n\ndestroy");
	tree_destroy(t);
	
	getch();
}

/*���㣺 
1.���Ҳ�Ʒ������������Ҷ�ӽڵ�ĸ���
2.���Ҳ�Ʒ����Ĳ�������n->parent!=null&&n->first_child!=null
3.��һ���������м��������ÿ������������Ƕ���:����������ж�id�Ƿ����p1��Ȼ����� 
4.������ȱʧĳ���������Ӱ��Ĳ�����������һ���ڵ㿪ʼ����ǰ���ң�����ҵ�Ŀ�꣬��ô���ϵĸ��ڵ㶼�ᱻӰ�죬Ȼ�������ǰ�� 
*/ 



