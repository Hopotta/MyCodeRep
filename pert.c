#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

/*作业数据——由三个链表构成 4.10飞天云课堂*/
typedef struct _data{
	int id;
	char name[20];
	int duration;	//最小单位：天 
	int tes;		//early start	最早时间：顺排计划 
	int tef;		//early finish
	int tls;		//late start	最迟时间：倒排计划 
	int tlf;		//late finish
	int critical;	//关键作业 
	int done;		//结束标记 
}data;

/*作业关系*/ 
typedef struct _relation{
	data *d;  /*指向数据的指针*/
	struct _relation *prev,*next; /*指向紧前和紧后作业*/ 
}relation;

/*作业*/
typedef struct _task{
	data *d;
	relation *predecessor,*successor; /*紧前，紧后作业双向链表的控制块,指向头的指针*/
	struct _task *prev,*next;  /*作业中的前后指针*/
}task;

/*项目*/ 
typedef struct _pert{
	int id; 
	char name[20];
	int tes;		//项目最早开个时间 
	int tlf;		//项目合同完工时间 
	task *head,*tail;
}pert;

int max(int a,int b){
	return(a>b ? a:b);
}

int min(int a,int b){
	return(a<b ? a:b);
}
/*创建并初始化*/ 
pert *pert_init(){
	pert *p=(pert *)malloc(sizeof(pert));
	p->id=1;
	p->tes=0;
	p->tlf=0;
	p->head=NULL;
	p->tail=NULL;
	
	return(p);
}

void pert_empty(pert *p){ /*清空先需要清空紧前作业和紧后作业，所以先套一个大的循环，循环嵌套*/
	while(p->head!=NULL){
		task *t=p->head;
		p->head=p->head->next;
		
				
		while(t->predecessor!=NULL){ /*释放紧前作业，不要释放数据部分*/ 
			relation *r=t->predecessor;
			t->predecessor=t->predecessor->next;
			
			free(r);			
		}
		
		while(t->successor!=NULL){
			relation *r=t->successor;
			t->successor=t->successor->next;
			
			free(r);			
		}
		
		free(t->d);
		free(t);
	}
	
	p->head=NULL;
	p->tail=NULL;
}

void pert_destroy(pert *p){
	pert_empty(p);/*清空所有内容*/ 
	
	free(p);/*释放控制块*/ 
}

int pert_add_task(pert *p,data *d){
	task *t=(task *)malloc(sizeof(task));
	if(t==NULL){  /*创建一个新的节点*/ 
		return(0);
	}
	t->d=d;/*初始化*/ 
	t->prev=NULL;
	t->next=NULL;
	t->predecessor=NULL;
	t->successor=NULL;
	
	if(p->head==NULL){
		p->head=t;
		p->tail=t;
	}else{
		t->prev=p->tail;
		p->tail->next=t;
		p->tail=t;
	}
	
	return(-1);
} 

void pert_iterate(pert *p){
	task *t=p->head;
	while(t!=NULL){		
		/*显示紧前作业*/
		relation *pr=t->predecessor;
		while(pr!=NULL){
			printf("%d<-",pr->d->id);
			pr=pr->next;
		}
		
		printf("[%d]",t->d->id);/*作业部分*/ 
		
		/*显示紧后作业*/
		relation *sr=t->successor;
		while(sr!=NULL){
			printf("->%d",sr->d->id);
			sr=sr->next;
		}
		
		printf("\n");
		t=t->next;
	}
	
	/*
	task *t=p->head;
	while(t!=NULL){
		printf("\n%d:tes=%d,tef=%d,tls=%d,tlf=%d,critical:%d",t->d->id,t->d->tes,t->d->tef,t->d->tls,t->d->tlf,t->d->critical);
		t=t->next;
	}*/
	
}

task *pert_find_task(pert *p,int id){ /*查找作业*/ 
	task *t=p->head;
	while(t!=NULL){
		if(t->d->id==id){
			return(t);
		} 
		t=t->next;
	}
	
	return(NULL);
}

relation *pert_find_predecessor(pert *p,int id,int pid){ /*给定某个作业，查找其紧前作业，pid指的是紧前作业的数据*/
	task *t=pert_find_task(p,id);/*先找到该作业*/ 
	if(t==NULL){
		return(NULL);
	}
	
	relation *pr=t->predecessor;
	while(pr!=NULL){
		if(pr->d->id==pid){
			return(pr);
		} 
		pr=pr->next;
	}
	
	return(NULL);
}

relation *pert_find_successor(pert *p,int id,int sid){/*给定某个作业，查找其紧后作业，sid指的是紧后作业的数据*/
	task *t=pert_find_task(p,id);
	if(t==NULL){
		return(NULL);
	}
	
	relation *sr=t->successor;
	while(sr!=NULL){
		if(sr->d->id==sid){
			return(sr);
		} 
		sr=sr->next;
	}
	
	return(NULL);
}

int pert_relation_exists(pert *p,int pid,int sid){ /*判断是否存在紧前紧后关系*/ 
	if(pert_find_successor(p,pid,sid)!=NULL){
		return(-1);
	}else{
		return(0);
	}
}

/*pid-predecessor, sid-successor*/
int pert_add_relation(pert *p,int pid,int sid){ /*增加作业的紧前紧后关系，需要先判断是否存在*/
	task *pt=pert_find_task(p,pid);
	if(pt==NULL){
		printf("\n%d not exists!",pid);
		return(0);
	}	

	task *st=pert_find_task(p,sid);
	if(st==NULL){
		printf("\n%d not exists!",sid);
		return(0);
	}
	
	/*是否已经存在紧前紧后关系？*/
	if(pert_find_successor(p,pid,sid)!=NULL){
		return(0);
	}
	 
	/*增加紧前作业,pt为原来的紧前作业，st为原来的紧后作业*/
	relation *pr=(relation *)malloc(sizeof(relation));	
	if(pr==NULL){
		return(0);
	}
	pr->d=pt->d;
	pr->next=NULL;
	pr->prev=NULL;
	
	if(st->predecessor==NULL){
		st->predecessor=pr; 
	}else{
		pr->next=st->predecessor;
		st->predecessor->prev=pr;
		st->predecessor=pr;/*直接加在原来的链表头*/ 
	}
	
	/*增加紧后作业*/
	relation *sr=(relation *)malloc(sizeof(relation));	
	if(sr==NULL){
		return(0);
	}
	sr->d=st->d;
	sr->next=NULL;
	sr->prev=NULL;
	
	if(pt->successor==NULL){
		pt->successor=sr;
	}else{
		sr->next=pt->successor;
		pt->successor->prev=sr;
		pt->successor=sr;/*直接加在链表头*/ 
	}
	
	return(-1);
}
/*删除作业关系，包括紧前紧后*/ 
int pert_delete_relation(pert *p,int pid,int sid){
	task *pt=pert_find_task(p,pid);	
	if(pt==NULL){
		return(0);
	}
	
	/*删除紧后作业*/
	relation *r=pt->successor;
	while(r!=NULL){
		if(r->d->id==sid){
			if(r->prev==NULL){
				pt->successor=r->next;
				if(pt->successor!=NULL){
					pt->successor->prev=NULL;
				}
			}
			
			if(r->next!=NULL){
				r->next->prev=r->prev;
			} 
			
			if(r->prev!=NULL){
				r->prev->next=r->next;
			}
			
			free(r);
			
			break; 
		}
		r=r->next;	
	}
	
	/*删除紧前作业*/
	task *st=pert_find_task(p,sid);	
	if(st==NULL){
		return(0);
	}
	r=st->predecessor;
	while(r!=NULL){
		if(r->d->id==pid){
			if(r->prev==NULL){
				st->successor=r->next;
				if(st->successor!=NULL){
					st->successor->prev=NULL;
				}
			}
			
			if(r->next!=NULL){
				r->next->prev=r->prev;
			} 
			
			if(r->prev!=NULL){
				r->prev->next=r->next;
			}
			
			free(r);
			
			break; 
		}
		r=r->next;	
	}
	
	return(-1);	
}

int pert_delete_task(pert *p,int id){
	task *t=pert_find_task(p,id); 
	if(t==NULL){
		return(0);
	}
	
	/*1)删除紧前和紧后*/
	relation *pr=t->predecessor;/*指向紧前的指针*/ 
	while(pr!=NULL){
		task *pt=pert_find_task(p,pr->d->id);/*指向紧前对应作业的指针*/
		
		relation *sr=pt->successor;/*指向紧前对应作业的紧后的指针*/ 
		while(sr!=NULL){
			if(sr->d->id==id){
				if(sr->prev==NULL){/*位于链表头*/
					pt->successor=sr->next;
					if(pt->successor!=NULL){
						pt->successor->prev=NULL;/*原来的next指向前面的指针就需要置空*/
					}
				}
				
				if(sr->next!=NULL){
					sr->next->prev=sr->prev;
				} 
				
				if(sr->prev!=NULL){
					sr->prev->next=sr->next;
				}
				
				free(sr);
				
				break; 
				
			}
			sr=sr->next;
		}
		
		relation *t1=pr->next;
		free(pr);
		pr=t1;
	}
	
	relation *sr=t->successor;
	while(sr!=NULL){
		task *st=pert_find_task(p,sr->d->id);/*指向紧后对应作业的指针*/
		
		relation *pr=st->predecessor;/*指向紧后对应作业的紧前的指针*/
		while(pr!=NULL){
			if(pr->d->id==id){
				if(pr->prev==NULL){/*位于链表头*/
					st->predecessor=st->predecessor->next;
					if(st->predecessor!=NULL){
						st->predecessor->prev=NULL;
					}
				}
				
				if(pr->next!=NULL){
					pr->next->prev=pr->prev;
				} 
				
				if(pr->prev!=NULL){
					pr->prev->next=pr->next;
				}
				
				free(pr);
				
				break; 
				
			}
			pr=pr->next;
		}
		
		relation *t1=sr->next;
		free(sr);
		sr=t1;
	}

	/*2)删除作业本身*/ 
	if(t->prev==NULL && t->next==NULL){
		p->head=NULL;
		p->tail=NULL;
	}else if(t->prev==NULL){ 
		p->head=p->head->next;
		if(p->head!=NULL){
			p->head->prev=NULL;
		}
	}else if(t->next==NULL){
		p->tail=p->tail->prev;
		if(p->tail!=NULL){
			p->tail->next=NULL;
		}
	}else{
		t->prev->next=t->next;
		t->next->prev=t->prev;
	}
	/*	
	if(t->prev==NULL){
		p->head=p->head->next;
		if(p->head!=NULL){
			p->head->prev=NULL;
		}else{
			p->tail=NULL;
		}
	}else{
		t->prev->next=t->next;
	}
	
	if(t->next==NULL){
		p->tail=p->tail->prev;
		if(t->tail!=NULL){
			p->tail->next=NULL;
		}else{
			t->head=NULL;
		}	
	}else{		
		t->next->prev=t->prev;
	} 
	*/
	
	free(t->d);
	free(t);

	return(-1);
}

/*
1. 阈值问题: tls-tes<d
2. 某些作业它们的开完工时间是事先确定，不能改变 
3. 紧前紧后关系：1)fs=finish-start; 2)ss=start-start; 3)ff=finish-finish; 4)sf=start-finish;
4. finish+delta=start
5. 工厂日历 ：worktime/working-shift 
6. 可视化：pert图，GANTT图 

TES/TEF,TLS/TLF 
顺排计划： 
	1）所有没有紧前作业的作业：TES=0 
	2）TES=max{所有紧前作业 TEF} 
	3）TEF=TES+DURATION

倒排计划：
	1）所有没有紧后作业的作业：TLF=TEF
	2）TLF=min{所有紧后作业 TLS}
	3）TLS=TLF-DURATION 
	
关键作业：TES=TLS or TEF=TLF
关键路径：由关键作业组成的路径，路径最长 

如何判定是否存在环：
	一次循环，如果未计算的作业数>0，且一次循环计算出来的作业个数为0，那么在未计算的作业中至少存在一个环。 
*/ 
int pert_schedule(pert *p){
	/*初始化*/ 
	task *t=p->head;
	while(t!=NULL){
		t->d->tes=0;
		t->d->tef=0;
		t->d->tls=0;
		t->d->tlf=0;
		t->d->critical=0;
		t->d->done=0;
		
		t=t->next;
	}
	
	/*顺排计划*/
	while(1){
		int c1=0,c2=0; /*c1-表示还没有计算的作业的个数；c2-表示一次循环完成计算的作业个数*/
		task *t=p->head;
		while(t!=NULL){	
			if(t->d->done==1){
				t=t->next;
				continue;
			}
			
			c1++;
			
			/*判定t的紧前作业是否已经计算完成*/
			int done=1;
			int tes=0;
			relation *pr=t->predecessor;
			while(pr!=NULL){
				if(pr->d->done==0){
					done=0;
					break;
				}
				tes=max(tes,pr->d->tef);
				
				pr=pr->next;
			}
			
			if(done==1){
				t->d->done=1;
				t->d->tes=tes;
				t->d->tef=t->d->tes+t->d->duration;
				c2++;
			}
			
			t=t->next;
		}
		
		/*全部完成计算*/
		if(c1==0){
			break;
		}
		
		/*监测到环*/
		if(c1>0 && c2==0){
			printf("\n存在环: ");
			task *t=p->head;
			while(t!=NULL){
				if(t->d->done==0){
					printf("->%d",t->d->id);
				}
				t=t->next;
			}
			return(0); 
		} 
	}
	
	/*倒排计划*/
	int maxTef=0;
	t=p->head;
	while(t!=NULL){
		maxTef=max(maxTef,t->d->tef);
		
		t->d->done=0;
		t=t->next;
	}

	while(1){
		int c1=0,c2=0; /*c1-表示还没有计算的作业的个数；c2-表示一次循环完成计算的作业个数*/
		task *t=p->head;
		while(t!=NULL){	
			if(t->d->done==1){
				t=t->next;
				continue;
			}
			
			c1++;
			
			/*判定t的紧后作业是否已经计算完成*/
			int done=1;
			int tlf=maxTef;
			relation *sr=t->successor;
			while(sr!=NULL){
				if(sr->d->done==0){
					done=0;
					break;
				}
				tlf=min(tlf,sr->d->tls);
				
				sr=sr->next;
			}
			
			if(done==1){
				t->d->done=1;
				t->d->tlf=tlf;
				t->d->tls=t->d->tlf-t->d->duration;
				c2++;
			}
			
			t=t->next;
		}
		
		/*全部完成计算*/
		if(c1==0){
			break;
		}
	}	
	
	/*关键作业*/ 
	t=p->head;
	while(t!=NULL){
		t->d->critical=0;
		if(t->d->tls-t->d->tes<1){
			t->d->critical=1;
		} 
		t=t->next;
	}
	
	return(-1);
}

int pert_get_predecessor_count(pert *p,int id){
	task *t=p->head;
	while(t!=NULL){
		if(t->d->id==id){
			break;
		}
		t=t->next;
	}	
	if(t==NULL){
		return(0);
	}
	
	int count=0;
	relation *r=t->predecessor;
	while(r!=NULL){
		count++;
		r=r->next;
	}
	
	return(count);
}

int test_pert() {
	printf("\n\ninit\n");
	pert *p=pert_init();
		
	printf("\n\nadd tasks\n");
	for(int i=1;i<=5;i++){
		data *d=(data *)malloc(sizeof(data));
		d->id=i;
		d->duration=i;
		pert_add_task(p,d);
	}
	pert_iterate(p);
	
	printf("\n\nadd relations\n");
	pert_add_relation(p,1,2);
	pert_add_relation(p,1,3);
	pert_add_relation(p,2,4);
	pert_add_relation(p,3,4);
	pert_add_relation(p,4,5);
	pert_add_relation(p,5,2);
	pert_iterate(p);
	
	//printf("\n\ndelete relation:2->4\n");
	//pert_delete_relation(p,2,4);
	//pert_iterate(p);

	printf("\n\nschedule\n");
	pert_schedule(p);
	
	task *t=p->head;
	while(t!=NULL){
		printf("\n%d:tes=%d,tef=%d,tls=%d,tlf=%d,critical:%d",t->d->id,t->d->tes,t->d->tef,t->d->tls,t->d->tlf,t->d->critical);
		t=t->next;
	}
	
	//printf("\n\ndelete task:2\n");
	//pert_delete_task(p,2);
	//pert_iterate(p);
	
	
	/*
	printf("\nfind task:3");
	task *t=pert_find_task(p,3);
	if(t==NULL){
		printf("3 not found");
	}else{
		printf("found");
	}
	*/
	
	printf("\n\ndestroy\n");
	pert_destroy(p);
	
	getch();
}
