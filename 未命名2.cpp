#include <stdio.h>
typedef int KeyType;
typedef struct{
	KeyType key;
}RecType;
void swap(int *a,int *b){
	int x;
	x=*a;
	(*a)=*b;
	*b=*a;
}
void select(RecType r[],int n,int m){
	int k;
	for(int i=0;i<m;i++){
		k=i;
		for(int j=i+1;j<n;j++){
			if(r[j].key>r[k].key){
				k=j;
			}
		}
		if(k!=i){
			swap(&r[i].key,&r[k].key);
		}
	}
}
int mian(){
	int a[4]={1,2,3,4};
	RecType r[4];
	for(int i=0;i<4;i++){
		r[i].key=a[i];
	}
	select(r,4,2);
	return 0;
}
