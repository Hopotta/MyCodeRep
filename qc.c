#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

void Swap(int *x, int *y) {
    int Temp;
    
    Temp = *x;
    *x = *y;
    *y = Temp;
}
/*���������������*/
int c=0;

void quick_sort(int a[], int first, int last) {
    if(first < last) {
    	c++;
	    printf("\n\n#%d\n",c);
	    int k=first;
		for(k=first;k<=last;k++){
			printf("\t%d",a[k]);
		}

        int pivot = first;
        int i = first;
        int j = last;
        while (i < j) {
            while(a[i] <= a[pivot] && i < last){/*���������ߣ��жϵ�i���Ƿ�С���ֵ��ֵ�����Ҳ���������ķ�Χ*/ 
                i++;                   /*ֱ���ҵ����ֵ����Ǹ�ֵ*/
        	}
        
            while(a[j] > a[pivot]){ /*���������ߣ��жϵ�j���Ƿ�����ֵ��ֵ��ֱ���ҵ����ֵ�С���Ǹ�ֵ*/
                j--;
        	}
                
            if(i < j) { 
            	printf("\n\n\t[%d]<->[%d]: %d<->%d",i,j,a[i],a[j]);
                Swap(&a[i], &a[j]);/*��i��j��Ӧλ�õ�ֵ��һ��������Ȼ�����ѭ����ֱ��i>=jֹͣ����Ϊһ���������*/ 
                printf("\n");
                int k=first;
				for(k=first;k<=last;k++){
					printf("\t%d",a[k]);
				}
            }
        }
        if(pivot!=j){
        	printf("\n\n\t[%d]<->[%d]: %d<->%d",pivot,j,a[pivot],a[j]);
			Swap(&a[pivot], &a[j]);
		}
        
	    printf("\n");
		for(k=first;k<=last;k++){
			printf("\t%d",a[k]);
		}

        quick_sort(a, first, j - 1); 
        quick_sort(a, j + 1, last);
    }
}

int test_qc() {
	/*int a[]={3,2,4,10,6,20,7,21,8,15,9};*/
	int a[]={5,2,1,16,7,13,9,4,12,6};
	quick_sort(a,0,9); 
	
	printf("\n");
	int i=0;
	for(i=0;i<10;i++){
		printf("\t%d",a[i]);
	}
	
	getch();
}
