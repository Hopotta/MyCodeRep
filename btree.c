/*操作：
二叉树由节点和节点关系组成，有且只有一个根，节点之间存在父-子关系和兄弟关系，根没有父节点，叶子结点没有子节点
每一个节点的子节点个数<=2，且左边称为左子节点，右边称为右子节点
1.构造和析构
2.增加子节点
3.截枝操作（抽取与删除）
4.旋转、查找、遍历、父结构重构 （遍历包括①前序：父→左→右；②中序：左→父→右；③后序：左→右→父）*/
#include <stdio.h>
#include <stdlib.h>
/*3.20飞天云课堂*/ 
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
	
	printf("\n\t%d",n->d->id);	/*前序遍历，父节点一定在其子节点的前面*/ 
	btree_iterate(n->left);
	//printf("\n\t%d",n->d->id);/*中序遍历，把二叉树拉开，拍扁/ 
	btree_iterate(n->right);
	//printf("\n\t%d",n->d->id);/*后序遍历*/ 
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
		p=btree_find(t->root,pid);/*查找p*/
		if(p==NULL || (p->left!=NULL && p->right!=NULL)){ /*找不到目标节点或者左右子节点已满，不能增加*/ 
			return(0);
		}
	}

	bnode *n=(bnode *)malloc(sizeof(bnode)); /*初始化*/ 
	n->d=(data *)malloc(sizeof(data));
	n->d->id=id;
	n->parent=NULL;
	n->left=NULL;
	n->right=NULL;
	
	if(pid==-1){
		t->root=n;	/*二叉树为空，则令其为根节点*/ 
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

int btree_cut(btree *t,int id){ /*截枝操作：1.先查找 2.若为根节点，则根指针置空 3.调整指针指向 4.释放空间*/ 
	bnode *n=btree_find(t->root,id);
	if(n==NULL){
		return(0);
	}
	
	if(n->parent==NULL){
		t->root=NULL; /*根节点的控制块置空*/ 
	}else{
		if(n->parent->left->d->id==id){ /*对应的部分在左子节点*/
			n->parent->left=n->parent->right;
			n->parent->right=NULL;
		}else{
			n->parent->right=NULL;
		}
	}
	
	btree_empty(n);	/*释放*/ 
	
	return(-1);
}

int btree_has_as_parent(btree *t,int id,int pid){ /*判断是否有父子关系*/
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


int btree_reparent(btree *t,int id,int pid){ /*父结构重构：查找要重构的节点，查找新的父节点*/
	if(id==pid){   /*同一个节点，无法重构*/
		return(0);
	}
	bnode *p=btree_find(t->root,pid);  /*p是新的父节点*/
	if(p==NULL || (p->left!=NULL && p->right!=NULL)){  /*父节点不存在，p的左右子节点已经满了*/
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
		return(0); /*根据上面的代码判断p是n子节点，不能重构*/ 
	}
	
	bnode *n=btree_find(t->root,id); /*n是要重构的节点*/
	if(n==NULL || n->parent==NULL){ /*n为空或者n为根节点*/ 
		return(0);
	}
	
	if(n->parent->d->id==pid){ /*p已经是n的父节点*/
		return(0);	
	}
	
	if(n->parent->left->d->id==id){  /*先截取下来，然后调整指针*/
		n->parent->left=n->parent->right;
		n->parent->right=NULL;
	}else{
		n->parent->right=NULL;	
	}
	
	n->parent=p;/*调整指向父节点的指针*/
	if(p->left==NULL){/*调整父节点的指针*/
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
