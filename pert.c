#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

/*��ҵ���ݡ��������������� 4.10�����ƿ���*/
typedef struct _data{
	int id;
	char name[20];
	int duration;	//��С��λ���� 
	int tes;		//early start	����ʱ�䣺˳�żƻ� 
	int tef;		//early finish
	int tls;		//late start	���ʱ�䣺���żƻ� 
	int tlf;		//late finish
	int critical;	//�ؼ���ҵ 
	int done;		//������� 
}data;

/*��ҵ��ϵ*/ 
typedef struct _relation{
	data *d;  /*ָ�����ݵ�ָ��*/
	struct _relation *prev,*next; /*ָ���ǰ�ͽ�����ҵ*/ 
}relation;

/*��ҵ*/
typedef struct _task{
	data *d;
	relation *predecessor,*successor; /*��ǰ��������ҵ˫������Ŀ��ƿ�,ָ��ͷ��ָ��*/
	struct _task *prev,*next;  /*��ҵ�е�ǰ��ָ��*/
}task;

/*��Ŀ*/ 
typedef struct _pert{
	int id; 
	char name[20];
	int tes;		//��Ŀ���翪��ʱ�� 
	int tlf;		//��Ŀ��ͬ�깤ʱ�� 
	task *head,*tail;
}pert;

int max(int a,int b){
	return(a>b ? a:b);
}

int min(int a,int b){
	return(a<b ? a:b);
}
/*��������ʼ��*/ 
pert *pert_init(){
	pert *p=(pert *)malloc(sizeof(pert));
	p->id=1;
	p->tes=0;
	p->tlf=0;
	p->head=NULL;
	p->tail=NULL;
	
	return(p);
}

void pert_empty(pert *p){ /*�������Ҫ��ս�ǰ��ҵ�ͽ�����ҵ����������һ�����ѭ����ѭ��Ƕ��*/
	while(p->head!=NULL){
		task *t=p->head;
		p->head=p->head->next;
		
				
		while(t->predecessor!=NULL){ /*�ͷŽ�ǰ��ҵ����Ҫ�ͷ����ݲ���*/ 
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
	pert_empty(p);/*�����������*/ 
	
	free(p);/*�ͷſ��ƿ�*/ 
}

int pert_add_task(pert *p,data *d){
	task *t=(task *)malloc(sizeof(task));
	if(t==NULL){  /*����һ���µĽڵ�*/ 
		return(0);
	}
	t->d=d;/*��ʼ��*/ 
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
		/*��ʾ��ǰ��ҵ*/
		relation *pr=t->predecessor;
		while(pr!=NULL){
			printf("%d<-",pr->d->id);
			pr=pr->next;
		}
		
		printf("[%d]",t->d->id);/*��ҵ����*/ 
		
		/*��ʾ������ҵ*/
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

task *pert_find_task(pert *p,int id){ /*������ҵ*/ 
	task *t=p->head;
	while(t!=NULL){
		if(t->d->id==id){
			return(t);
		} 
		t=t->next;
	}
	
	return(NULL);
}

relation *pert_find_predecessor(pert *p,int id,int pid){ /*����ĳ����ҵ���������ǰ��ҵ��pidָ���ǽ�ǰ��ҵ������*/
	task *t=pert_find_task(p,id);/*���ҵ�����ҵ*/ 
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

relation *pert_find_successor(pert *p,int id,int sid){/*����ĳ����ҵ�������������ҵ��sidָ���ǽ�����ҵ������*/
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

int pert_relation_exists(pert *p,int pid,int sid){ /*�ж��Ƿ���ڽ�ǰ�����ϵ*/ 
	if(pert_find_successor(p,pid,sid)!=NULL){
		return(-1);
	}else{
		return(0);
	}
}

/*pid-predecessor, sid-successor*/
int pert_add_relation(pert *p,int pid,int sid){ /*������ҵ�Ľ�ǰ�����ϵ����Ҫ���ж��Ƿ����*/
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
	
	/*�Ƿ��Ѿ����ڽ�ǰ�����ϵ��*/
	if(pert_find_successor(p,pid,sid)!=NULL){
		return(0);
	}
	 
	/*���ӽ�ǰ��ҵ,ptΪԭ���Ľ�ǰ��ҵ��stΪԭ���Ľ�����ҵ*/
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
		st->predecessor=pr;/*ֱ�Ӽ���ԭ��������ͷ*/ 
	}
	
	/*���ӽ�����ҵ*/
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
		pt->successor=sr;/*ֱ�Ӽ�������ͷ*/ 
	}
	
	return(-1);
}
/*ɾ����ҵ��ϵ��������ǰ����*/ 
int pert_delete_relation(pert *p,int pid,int sid){
	task *pt=pert_find_task(p,pid);	
	if(pt==NULL){
		return(0);
	}
	
	/*ɾ��������ҵ*/
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
	
	/*ɾ����ǰ��ҵ*/
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
	
	/*1)ɾ����ǰ�ͽ���*/
	relation *pr=t->predecessor;/*ָ���ǰ��ָ��*/ 
	while(pr!=NULL){
		task *pt=pert_find_task(p,pr->d->id);/*ָ���ǰ��Ӧ��ҵ��ָ��*/
		
		relation *sr=pt->successor;/*ָ���ǰ��Ӧ��ҵ�Ľ����ָ��*/ 
		while(sr!=NULL){
			if(sr->d->id==id){
				if(sr->prev==NULL){/*λ������ͷ*/
					pt->successor=sr->next;
					if(pt->successor!=NULL){
						pt->successor->prev=NULL;/*ԭ����nextָ��ǰ���ָ�����Ҫ�ÿ�*/
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
		task *st=pert_find_task(p,sr->d->id);/*ָ������Ӧ��ҵ��ָ��*/
		
		relation *pr=st->predecessor;/*ָ������Ӧ��ҵ�Ľ�ǰ��ָ��*/
		while(pr!=NULL){
			if(pr->d->id==id){
				if(pr->prev==NULL){/*λ������ͷ*/
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

	/*2)ɾ����ҵ����*/ 
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
1. ��ֵ����: tls-tes<d
2. ĳЩ��ҵ���ǵĿ��깤ʱ��������ȷ�������ܸı� 
3. ��ǰ�����ϵ��1)fs=finish-start; 2)ss=start-start; 3)ff=finish-finish; 4)sf=start-finish;
4. finish+delta=start
5. �������� ��worktime/working-shift 
6. ���ӻ���pertͼ��GANTTͼ 

TES/TEF,TLS/TLF 
˳�żƻ��� 
	1������û�н�ǰ��ҵ����ҵ��TES=0 
	2��TES=max{���н�ǰ��ҵ TEF} 
	3��TEF=TES+DURATION

���żƻ���
	1������û�н�����ҵ����ҵ��TLF=TEF
	2��TLF=min{���н�����ҵ TLS}
	3��TLS=TLF-DURATION 
	
�ؼ���ҵ��TES=TLS or TEF=TLF
�ؼ�·�����ɹؼ���ҵ��ɵ�·����·��� 

����ж��Ƿ���ڻ���
	һ��ѭ�������δ�������ҵ��>0����һ��ѭ�������������ҵ����Ϊ0����ô��δ�������ҵ�����ٴ���һ������ 
*/ 
int pert_schedule(pert *p){
	/*��ʼ��*/ 
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
	
	/*˳�żƻ�*/
	while(1){
		int c1=0,c2=0; /*c1-��ʾ��û�м������ҵ�ĸ�����c2-��ʾһ��ѭ����ɼ������ҵ����*/
		task *t=p->head;
		while(t!=NULL){	
			if(t->d->done==1){
				t=t->next;
				continue;
			}
			
			c1++;
			
			/*�ж�t�Ľ�ǰ��ҵ�Ƿ��Ѿ��������*/
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
		
		/*ȫ����ɼ���*/
		if(c1==0){
			break;
		}
		
		/*��⵽��*/
		if(c1>0 && c2==0){
			printf("\n���ڻ�: ");
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
	
	/*���żƻ�*/
	int maxTef=0;
	t=p->head;
	while(t!=NULL){
		maxTef=max(maxTef,t->d->tef);
		
		t->d->done=0;
		t=t->next;
	}

	while(1){
		int c1=0,c2=0; /*c1-��ʾ��û�м������ҵ�ĸ�����c2-��ʾһ��ѭ����ɼ������ҵ����*/
		task *t=p->head;
		while(t!=NULL){	
			if(t->d->done==1){
				t=t->next;
				continue;
			}
			
			c1++;
			
			/*�ж�t�Ľ�����ҵ�Ƿ��Ѿ��������*/
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
		
		/*ȫ����ɼ���*/
		if(c1==0){
			break;
		}
	}	
	
	/*�ؼ���ҵ*/ 
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
