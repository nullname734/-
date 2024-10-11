#include <stdio.h>
#include <stdlib.h>
#include<string.h> 
#define Maxsize 1024 //������ͨ�����ַ��������и��������ȵļ� 
typedef struct{
	char words[50]; //����Ͳ����˻��е�������ʮ����ĸ 
}Essay;
typedef struct{
	char s[50];  //�ؼ��� 
	int count; //�������� 
}RecType;  //����Ԫ�� 
typedef struct{
	char data[50]; //������� 
	int weight;  //Ȩ�� 
	int parent; //��ĸ��� 
	int lchild; //���� 
	int rchild; //�Һ��� 
}HTNode;
typedef struct{
    char cd[Maxsize]; //��ű��룬��ѽ������е�� 
	int start;  //������ʼ��ָ�� 
}HCode;
typedef struct{
	char data[Maxsize];
	char code[Maxsize];
}Code;
void writeSr(int a[],int *n){
	FILE*fp;
	*n=0;
	fp=fopen("������.txt","r");
	if(fp==NULL){
		printf("�޷����ļ�");
		exit(0);
	} 
	int u=0;
	while(!feof(fp)){ //��ǻس���λ�� 
		char s[Maxsize];
		fgets(s,Maxsize,fp);
		for(int i=0;s[i]!='\0';i++){
			if(s[i]==' '||s[i]==','||s[i]=='.'){ //�ո񡢱����ž�Ϊ�������ʵ�λ�� 
				//printf("%c",s[i]);
				u++;
			}
		}
		a[*n]=u;
		(*n)++;
		}
	fclose(fp);
}
void readFiles(Essay string[],int *n,int a[],int l){ //���ļ� 
	FILE*fp; //�����ļ�ָ�� 
	*n=0;
	int x=0;
	int t=0;
	fp=fopen("������.txt","r"); //��ֻ����ʽ���ļ�
	if(fp==NULL){ //���ļ�ָ���ǿ�ָ�� 
		printf("�޷����ļ�"); //��ʾ��ʧ�� 
		exit(0);
	}
	while(!feof(fp)){
	fscanf(fp,"%s",&string[*n].words); //���ļ���õ���
    int j=0;
	while(string[*n].words[j]!=0){ //��������ţ���Ӱ���ظ��ʵ�ͳ�ƣ� 
		if(string[*n].words[j]==','||string[*n].words[j]=='.'){
		  x=*n;
		  (*n)++;  
		  string[*n].words[0]=string[x].words[j];
		  string[x].words[j]=0;
		}
		j++;
	}
	for(int z=0;z<l;z++){  //�����ֻس���ʱ�� 
		if((*n)==(a[z]+t-1)){
			(*n)++;
			strcpy(&string[*n],"\n"); //д��س� 
			t++; //��¼��ԭ�����ӵĴ��� 
		}
	}
	(*n)++;
  }
}
void CountWords(RecType a[],int n,Essay b[],int *c){ //��¼ÿ���ʶ�Ӧ���ظ�����,c��¼�ڻ�ȡ�ظ��ʺ�õ�������ĳ��� 
  	(*c)=0;
	  for(int i=0;i<n;i++){
		char s[50]; //���ں�������ظ��ʶԱ� 
		strcpy(s,b[i].words);  
		int j=0;
    	if(strlen(b[i].words)!=0){ //������ʲ�Ϊ��ʱ 
			strcpy(a[*c].s,b[i].words); //���ƽ��� 
			(*c)++; //������ָ����� 
		} 
		int x=0; 
		while(j<n){ //�������������ڵĴ� 
		if(!strcmp(b[j].words,s)&&strlen(s)!=0){ //��ѯ��ͬ�Ĵʣ������Ҳ��ǿմ�ʱ 
			x++;
		//	memset(b[j].words,'\0',sizeof(b[j].words)); //�������ظ��Ĵ��ÿգ���ֹ�ظ����� 
			}
			a[*c-1].count=x; //����ôʵ�����,����������c�Ѿ�������Ϊ��ʹ�ô���������Ӧ��*c��Ҫ��һ
			j++;
		}
	}
}
void creatFile(RecType a[],int n){
	FILE *fp;
	fp=fopen("Cont.txt","w"); //�����ļ� 
	if(fp==NULL){
		printf("����ʧ��");
		exit(0);
	}
	for(int i=0;i<n;i++){
		fprintf(fp,"����:%s\t�������ļ��г��ֵĴ���:%d\n",a[i].s,a[i].count); //����Ϣд���ļ� 
	}
	fclose(fp); //�ر��ļ� 
}
void writeT(HTNode ht[],int n){
	FILE *fp;
	fp=fopen("hfmtree.txt","w");
	if(fp==NULL){
		printf("����ʧ��");
		return;
	}
	for(int i=1;i<2*n-1;i++){
		fprintf(fp,"%s\t%d\t%d\t%d\t%d\n",ht[i].data,ht[i].weight,ht[i].parent,ht[i].lchild,ht[i].rchild); //�������������й�����д�� 
	}
	fclose(fp); //�ر��ļ�
}
void CreatHT(HTNode ht[],int n,RecType a[]){ //������������ 
	int lnode,rnode,min1,min2;
	for(int i=0;i<2*n-1;i++){ //�������г�ʼ�� 
		ht[i].weight=a[i].count;
		strcpy(ht[i].data,a[i].s);
		ht[i].lchild=-1;
		ht[i].rchild=-1;
		ht[i].parent=-1;
	}
	for(int i=n;i<2*n-1;i++){ //������������ķ�֧��� 
		min1=min2=32767;
		lnode=rnode=-1;  //lnode,rnodeָ��Ȩֵ��С�Ľ��
		for(int k=0;k<=i-1;k++){
			if(ht[k].parent==-1){ 
				if(ht[k].weight<min1){ //����δ����������Ľ���в��� 
					min2=min1;
					rnode=lnode;
					min1=ht[k].weight;
					lnode=k;
				}
				else if(ht[k].weight<min2){
					min2=ht[k].weight;
					rnode=k;
				}
			}
		}
		ht[i].weight=ht[rnode].weight+ht[lnode].weight; //�ϲ�С�Ľ�� 
		ht[i].lchild=lnode;
		ht[i].rchild=rnode;
		ht[lnode].parent=i;
		ht[rnode].parent=i; 
	}
	writeT(ht,n);//����д���ļ� 
	
}
void writeCode(Essay b[],HCode hcd[],int n){
	FILE *fp;
	Code q[n];//����ÿһ�е��� 
	fp=fopen("codefile.txt","w");
	if(fp==NULL){
		printf("����ʧ��");
		exit(0);
	}
	for(int i=0;i<n;i++){ 
		int c=0;
		strcpy(q[i].data,b[i].words);
		for(int j=hcd[i].start;j<=n;j++){
			  q[i].code[c]=hcd[i].cd[j];
			  c++;
		}
	}
	
	for(int i=0;i<n;i++){  //д���ļ�
		fprintf(fp,"%s",q[i].code);
		fprintf(fp," ");
	}
	fprintf(fp,"-1"); //����ļ�ĩβ 
	fclose(fp); //�ر��ļ� 
}
void CreateHCode(HTNode ht[],int n,Essay string[]){
	int f,c;
	HCode hc;
	HCode hcd[n];
	for(int i=0;i<n;i++){
		hc.start=n;
		c=i;
		f=ht[i].parent;
	    while(f!=-1){ //һֱ�����ڵ� 
			if(ht[f].lchild==c){ //�������� 
				hc.cd[hc.start]='0';
				hc.start--;
			}
			else{
				hc.cd[hc.start]='1'; //�����Һ��� 
			    hc.start--;
			}
			c=f;
			f=ht[f].parent;
		}
		hc.start++;  //������ָ���ʼ���ַ���������һ�����ڵ�Ĵ��� 
	    hcd[i]=hc;
	}
	writeCode(string,hcd,n);
}
void writeDcode(Essay a[],int n){
	FILE *fp;
	fp=fopen("textfile.txt","w");
	if(fp==NULL){
		printf("��ʧ��");
		exit(0);
	}
	for(int i=0;i<n;i++){
		if((!strcmp(a[i+1].words,"."))||(!strcmp(a[i+1].words,","))||(!strcmp(a[i].words,"\n"))){ //Ϊ�˺�ԭ�ļ���ʾһģһ���Ĺ��ڱ����ŵ���Э 
			fprintf(fp,"%s",a[i].words);
		}
		else{
		fprintf(fp,"%s ",a[i].words); //�ǵ�д��Ӣ�ĵ��ʵĿո� 
		}
	}
	fclose(fp);
}
void Decoding(HTNode *T,int n){
	FILE *fp;
	Essay a[Maxsize],w[Maxsize];
	int y=0;
	fp=fopen("codefile.txt","rt");//��ȡ�ļ�
	if(fp==NULL){
		printf("��ȡʧ��");
		return;
	} 
	int i=0;
	char s[10]="0"; //��ʼ��һ���ַ��� 
	while(strcmp(s,"-1")){ //������ĩβ��ʶǰ 
		fscanf(fp,"%s",s); //���丳ֵ 
		if(strcmp(s,"-1")){ //�����ַ�������ĩβ��ʶ 
			strcpy(&a[i].words,&s); //�������ڽ�����ַ����� 
			i++;
		}
	}
	int t;
	int k=0,l;
	for(int j=0;j<=i;j++){  //����ÿһ�ι������� 
		l=0;
		for(k=0;a[j].words[k]!='\0';k++) 
		 while(l<k){
			t=2*n-2;
			while(T[t].lchild!=-1&&T[t].rchild!=-1){ //Ѱ����Ҷ�ӽ�� 
				if(a[j].words[l]=='0'){ //Ϊ0����� 
				 t=T[t].lchild;
				}
				if(a[j].words[l]=='1'){ //Ϊ1���ұ� 
					t=T[t].rchild;
				}
				l++; //����һ���������Ѱ�� 
			}
			strcpy(&w[y].words,&T[t].data); //��Ҷ�ӽ��д������
			y++;//��¼�ô�������ĳ���	 
		 }
		 
  }
  	writeDcode(w,y); //������д���ļ� 
  	
}
void Menu(){ //��ʾ�˵� 
	Essay string[Maxsize];
	int n,z,l;
	int a[Maxsize];
	RecType Cnt[Maxsize];
	HTNode ht[Maxsize];
	printf("\n\n");
	printf("               -----------------------------------�˵�-----------------------------------\n");
	printf("               |��������Ҫ�Ĳ���                                                        |\n");
	printf("               |1.�����ļ�                                                              |\n");
	printf("               |2.�Դʼ�����д���ļ�                                                    |\n");
	printf("               |3.������������                                                          |\n");
	printf("               |4.�Ի�����������                                                        |\n");
	printf("               |5.�Ի�������������                                                      |\n");
	printf("               |6.�˳�����                                                              |\n");
	printf("               --------------------------------------------------------------------------\n");	
	while(1){
	    printf("               ");
		int x;
		scanf("%d",&x);
		switch (x) {
			case 1:
			writeSr(a,&l);
			readFiles(string,&n,a,l); //��ȡ�ļ�����ļ��ڵĵ���
			printf("�������\n");
			break;
			case 2:
			CountWords(Cnt,n,string,&z); //����ÿ���ʵ�����
			creatFile(Cnt,z);  //������д���ļ�
			printf("�ļ������ɹ�\n");
			break;
			case 3:
			CreatHT(ht,z,Cnt); //������������
			printf("�ļ������ɹ�\n");
			break;
			case 4:
			CreateHCode(ht,z,string); //����������
			printf("�������\n");
			break;
			case 5:
			Decoding(ht,z); //���� 
			printf("�������\n");
			break;
			case 6:
			printf("�������");
			exit(0);
			break;
			default:
			printf("����������\n");
			break;		 
		}
	}
}
int main(){
	Menu();
	return 0;
}
