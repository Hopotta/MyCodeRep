/*������
�������ɽڵ�ͽڵ��ϵ��ɣ�����ֻ��һ�������ڵ�֮����ڸ�-�ӹ�ϵ���ֵܹ�ϵ����û�и��ڵ㣬Ҷ�ӽ��û���ӽڵ�
ÿһ���ڵ���ӽڵ����<=2������߳�Ϊ���ӽڵ㣬�ұ߳�Ϊ���ӽڵ�
1.���������
2.�����ӽڵ�
3.��֦��������ȡ��ɾ����
4.��ת�����ҡ����������ṹ�ع� ������������ǰ�򣺸�������ң���������������ң��ۺ�������ҡ�����*/
#include <stdio.h>
#include <stdlib.h>
/*3.20�����ƿ���*/ 
typedef struct _data{
	int id;
}data; 

typedef struct _bnode{
	data *d;
	struct _bnode *parent,*left,*right;
}bnode;

typedef struct _btree{
	bnode *root;
}btree;


btree *btree_create(){
	btree *t=(btree *)malloc(sizeof(btree));
	t->root=NULL;
	
	return(t);
}

void btree_destroy(btree *t){
	btree_empty(t->root);
	
	free(t);	
}

void btree_empty(bnode *n){
	if(n==NULL){
		return;
	}

	btree_empty(n->left);
	btree_empty(n->right);
		
	free(n->d);
	free(n);
}

void btree_iterate(bnode *n){
	if(n==NULL){
		return;
	}
	
	printf("\n\t%d",n->d->id);	/*ǰ����������ڵ�һ�������ӽڵ��ǰ��*/ 
	btree_iterate(n->left);
	//printf("\n\t%d",n->d->id);/*����������Ѷ������������ı�/ 
	btree_iterate(n->right);
	//printf("\n\t%d",n->d->id);/*�������*/ 
}

bnode *btree_find(bnode *n,int id){
	if(n==NULL){
		return(NULL);
	}
	
	if(n->d->id==id){
		return(n);
	}else{
		bnode *t=btree_find(n->left,id);
		if(t!=NULL){
			return(t);
		}else{ 
			return(t=btree_find(n->right,id));
		} 
	}
}

int btree_append(btree *t,int pid,int id){
	bnode *p;
	if(pid!=-1){
		p=btree_find(t->root,pid);/*����p*/
		if(p==NULL || (p->left!=NULL && p->right!=NULL)){ /*�Ҳ���Ŀ��ڵ���������ӽڵ���������������*/ 
			return(0);
		}
	}

	bnode *n=(bnode *)malloc(sizeof(bnode)); /*��ʼ��*/ 
	n->d=(data *)malloc(sizeof(data));
	n->d->id=id;
	n->parent=NULL;
	n->left=NULL;
	n->right=NULL;
	
	if(pid==-1){
		t->root=n;	/*������Ϊ�գ�������Ϊ���ڵ�*/ 
	}else{
		n->parent=p;
		if(p->left==NULL){
			p->left=n;	
		}else{
			p->right=n;
		}
	}
	
	return(-1);
} 

int btree_cut(btree *t,int id){ /*��֦������1.�Ȳ��� 2.��Ϊ���ڵ㣬���ָ���ÿ� 3.����ָ��ָ�� 4.�ͷſռ�*/ 
	bnode *n=btree_find(t->root,id);
	if(n==NULL){
		return(0);
	}
	
	if(n->parent==NULL){
		t->root=NULL; /*���ڵ�Ŀ��ƿ��ÿ�*/ 
	}else{
		if(n->parent->left->d->id==id){ /*��Ӧ�Ĳ��������ӽڵ�*/
			n->parent->left=n->parent->right;
			n->parent->right=NULL;
		}else{
			n->parent->right=NULL;
		}
	}
	
	btree_empty(n);	/*�ͷ�*/ 
	
	return(-1);
}

int btree_has_as_parent(btree *t,int id,int pid){ /*�ж��Ƿ��и��ӹ�ϵ*/
	bnode *n=btree_find(t->root,id);
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


int btree_reparent(btree *t,int id,int pid){ /*���ṹ�ع�������Ҫ�ع��Ľڵ㣬�����µĸ��ڵ�*/
	if(id==pid){   /*ͬһ���ڵ㣬�޷��ع�*/
		return(0);
	}
	bnode *p=btree_find(t->root,pid);  /*p���µĸ��ڵ�*/
	if(p==NULL || (p->left!=NULL && p->right!=NULL)){  /*���ڵ㲻���ڣ�p�������ӽڵ��Ѿ�����*/
		return(0);
	}
	
	int found=0;
	bnode *n1=p->parent;
	while(n1!=NULL){
		if(n1->d->id==id){ 
			found=-1;
			break;
		}
		n1=n1->parent;
	}
	if(found==-1){
		return(0); /*��������Ĵ����ж�p��n�ӽڵ㣬�����ع�*/ 
	}
	
	bnode *n=btree_find(t->root,id); /*n��Ҫ�ع��Ľڵ�*/
	if(n==NULL || n->parent==NULL){ /*nΪ�ջ���nΪ���ڵ�*/ 
		return(0);
	}
	
	if(n->parent->d->id==pid){ /*p�Ѿ���n�ĸ��ڵ�*/
		return(0);	
	}
	
	if(n->parent->left->d->id==id){  /*�Ƚ�ȡ������Ȼ�����ָ��*/
		n->parent->left=n->parent->right;
		n->parent->right=NULL;
	}else{
		n->parent->right=NULL;	
	}
	
	n->parent=p;/*����ָ�򸸽ڵ��ָ��*/
	if(p->left==NULL){/*�������ڵ��ָ��*/
		p->left=n;
	}else{
		p->right=n;
	}
	
	return(-1);
}

void main() {
	btree *t=btree_create();
	if(t==NULL){
		return;
	}
	
	btree_append(t,-1,1);
	btree_append(t,1,2);
	btree_append(t,1,3);
	btree_append(t,2,4);
	btree_append(t,2,5);
	btree_append(t,3,6);
	
	btree_iterate(t->root);
	
	printf("\n\n\n");
	btree_reparent(t,2,3);
	btree_iterate(t->root);
	
	btree_destroy(t);
	 
	getch();
}
