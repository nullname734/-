#include <stdio.h>
#include <stdlib.h>
#include<string.h> 
#define Maxsize 1024 //就是普通的让字符串数组有个声明长度的家 
typedef struct{
	char words[50]; //我真就不信了还有单词有五十个字母 
}Essay;
typedef struct{
	char s[50];  //关键词 
	int count; //计算数量 
}RecType;  //查找元素 
typedef struct{
	char data[50]; //结点内容 
	int weight;  //权重 
	int parent; //父母结点 
	int lchild; //左孩子 
	int rchild; //右孩子 
}HTNode;
typedef struct{
    char cd[Maxsize]; //存放编码，妈呀这编码有点大 
	int start;  //编码起始的指针 
}HCode;
typedef struct{
	char data[Maxsize];
	char code[Maxsize];
}Code;
void writeSr(int a[],int *n){
	FILE*fp;
	*n=0;
	fp=fopen("霍夫曼.txt","r");
	if(fp==NULL){
		printf("无法打开文件");
		exit(0);
	} 
	int u=0;
	while(!feof(fp)){ //标记回车的位置 
		char s[Maxsize];
		fgets(s,Maxsize,fp);
		for(int i=0;s[i]!='\0';i++){
			if(s[i]==' '||s[i]==','||s[i]=='.'){ //空格、标点符号均为隔开单词的位置 
				//printf("%c",s[i]);
				u++;
			}
		}
		a[*n]=u;
		(*n)++;
		}
	fclose(fp);
}
void readFiles(Essay string[],int *n,int a[],int l){ //读文件 
	FILE*fp; //建立文件指针 
	*n=0;
	int x=0;
	int t=0;
	fp=fopen("霍夫曼.txt","r"); //以只读方式打开文件
	if(fp==NULL){ //若文件指针是空指针 
		printf("无法打开文件"); //提示打开失败 
		exit(0);
	}
	while(!feof(fp)){
	fscanf(fp,"%s",&string[*n].words); //从文件获得单词
    int j=0;
	while(string[*n].words[j]!=0){ //处理标点符号（会影响重复词的统计） 
		if(string[*n].words[j]==','||string[*n].words[j]=='.'){
		  x=*n;
		  (*n)++;  
		  string[*n].words[0]=string[x].words[j];
		  string[x].words[j]=0;
		}
		j++;
	}
	for(int z=0;z<l;z++){  //当出现回车的时候 
		if((*n)==(a[z]+t-1)){
			(*n)++;
			strcpy(&string[*n],"\n"); //写入回车 
			t++; //记录比原先增加的词数 
		}
	}
	(*n)++;
  }
}
void CountWords(RecType a[],int n,Essay b[],int *c){ //记录每个词对应的重复数量,c记录在获取重复词后得到的数组的长度 
  	(*c)=0;
	  for(int i=0;i<n;i++){
		char s[50]; //用于后面进行重复词对比 
		strcpy(s,b[i].words);  
		int j=0;
    	if(strlen(b[i].words)!=0){ //当这个词不为空时 
			strcpy(a[*c].s,b[i].words); //复制进词 
			(*c)++; //将数组指针递增 
		} 
		int x=0; 
		while(j<n){ //遍历整段文章内的词 
		if(!strcmp(b[j].words,s)&&strlen(s)!=0){ //查询相同的词，并且且不是空词时 
			x++;
		//	memset(b[j].words,'\0',sizeof(b[j].words)); //将后面重复的词置空，防止重复计数 
			}
			a[*c-1].count=x; //保存该词的数量,由于在上面c已经递增，为了使得词与数量对应，*c需要减一
			j++;
		}
	}
}
void creatFile(RecType a[],int n){
	FILE *fp;
	fp=fopen("Cont.txt","w"); //创建文件 
	if(fp==NULL){
		printf("创建失败");
		exit(0);
	}
	for(int i=0;i<n;i++){
		fprintf(fp,"单词:%s\t单词在文件中出现的次数:%d\n",a[i].s,a[i].count); //将信息写入文件 
	}
	fclose(fp); //关闭文件 
}
void writeT(HTNode ht[],int n){
	FILE *fp;
	fp=fopen("hfmtree.txt","w");
	if(fp==NULL){
		printf("创建失败");
		return;
	}
	for(int i=1;i<2*n-1;i++){
		fprintf(fp,"%s\t%d\t%d\t%d\t%d\n",ht[i].data,ht[i].weight,ht[i].parent,ht[i].lchild,ht[i].rchild); //将哈夫曼树的有关内容写入 
	}
	fclose(fp); //关闭文件
}
void CreatHT(HTNode ht[],int n,RecType a[]){ //建立哈夫曼树 
	int lnode,rnode,min1,min2;
	for(int i=0;i<2*n-1;i++){ //各结点进行初始化 
		ht[i].weight=a[i].count;
		strcpy(ht[i].data,a[i].s);
		ht[i].lchild=-1;
		ht[i].rchild=-1;
		ht[i].parent=-1;
	}
	for(int i=n;i<2*n-1;i++){ //构造哈夫曼树的分支结点 
		min1=min2=32767;
		lnode=rnode=-1;  //lnode,rnode指向权值最小的结点
		for(int k=0;k<=i-1;k++){
			if(ht[k].parent==-1){ 
				if(ht[k].weight<min1){ //在尚未构造二叉树的结点中查找 
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
		ht[i].weight=ht[rnode].weight+ht[lnode].weight; //合并小的结点 
		ht[i].lchild=lnode;
		ht[i].rchild=rnode;
		ht[lnode].parent=i;
		ht[rnode].parent=i; 
	}
	writeT(ht,n);//将树写入文件 
	
}
void writeCode(Essay b[],HCode hcd[],int n){
	FILE *fp;
	Code q[n];//保存每一行的码 
	fp=fopen("codefile.txt","w");
	if(fp==NULL){
		printf("创建失败");
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
	
	for(int i=0;i<n;i++){  //写入文件
		fprintf(fp,"%s",q[i].code);
		fprintf(fp," ");
	}
	fprintf(fp,"-1"); //标记文件末尾 
	fclose(fp); //关闭文件 
}
void CreateHCode(HTNode ht[],int n,Essay string[]){
	int f,c;
	HCode hc;
	HCode hcd[n];
	for(int i=0;i<n;i++){
		hc.start=n;
		c=i;
		f=ht[i].parent;
	    while(f!=-1){ //一直到根节点 
			if(ht[f].lchild==c){ //处理左孩子 
				hc.cd[hc.start]='0';
				hc.start--;
			}
			else{
				hc.cd[hc.start]='1'; //处理右孩子 
			    hc.start--;
			}
			c=f;
			f=ht[f].parent;
		}
		hc.start++;  //将编码指向最开始的字符，进行下一个根节点的处理 
	    hcd[i]=hc;
	}
	writeCode(string,hcd,n);
}
void writeDcode(Essay a[],int n){
	FILE *fp;
	fp=fopen("textfile.txt","w");
	if(fp==NULL){
		printf("打开失败");
		exit(0);
	}
	for(int i=0;i<n;i++){
		if((!strcmp(a[i+1].words,"."))||(!strcmp(a[i+1].words,","))||(!strcmp(a[i].words,"\n"))){ //为了和原文件显示一模一样的关于标点符号的妥协 
			fprintf(fp,"%s",a[i].words);
		}
		else{
		fprintf(fp,"%s ",a[i].words); //记得写入英文单词的空格 
		}
	}
	fclose(fp);
}
void Decoding(HTNode *T,int n){
	FILE *fp;
	Essay a[Maxsize],w[Maxsize];
	int y=0;
	fp=fopen("codefile.txt","rt");//读取文件
	if(fp==NULL){
		printf("读取失败");
		return;
	} 
	int i=0;
	char s[10]="0"; //初始化一个字符串 
	while(strcmp(s,"-1")){ //在遇到末尾标识前 
		fscanf(fp,"%s",s); //将其赋值 
		if(strcmp(s,"-1")){ //若该字符串不是末尾标识 
			strcpy(&a[i].words,&s); //放入用于解码的字符串内 
			i++;
		}
	}
	int t;
	int k=0,l;
	for(int j=0;j<=i;j++){  //读入每一段哈夫曼码 
		l=0;
		for(k=0;a[j].words[k]!='\0';k++) 
		 while(l<k){
			t=2*n-2;
			while(T[t].lchild!=-1&&T[t].rchild!=-1){ //寻找其叶子结点 
				if(a[j].words[l]=='0'){ //为0走左边 
				 t=T[t].lchild;
				}
				if(a[j].words[l]=='1'){ //为1走右边 
					t=T[t].rchild;
				}
				l++; //对下一个编码进行寻找 
			}
			strcpy(&w[y].words,&T[t].data); //将叶子结点写入数组
			y++;//记录该词语数组的长度	 
		 }
		 
  }
  	writeDcode(w,y); //将数组写入文件 
  	
}
void Menu(){ //显示菜单 
	Essay string[Maxsize];
	int n,z,l;
	int a[Maxsize];
	RecType Cnt[Maxsize];
	HTNode ht[Maxsize];
	printf("\n\n");
	printf("               -----------------------------------菜单-----------------------------------\n");
	printf("               |请输入想要的操作                                                        |\n");
	printf("               |1.读入文件                                                              |\n");
	printf("               |2.对词计数并写入文件                                                    |\n");
	printf("               |3.建立霍夫曼树                                                          |\n");
	printf("               |4.对霍夫曼树编码                                                        |\n");
	printf("               |5.对霍夫曼编码译码                                                      |\n");
	printf("               |6.退出程序                                                              |\n");
	printf("               --------------------------------------------------------------------------\n");	
	while(1){
	    printf("               ");
		int x;
		scanf("%d",&x);
		switch (x) {
			case 1:
			writeSr(a,&l);
			readFiles(string,&n,a,l); //读取文件获得文件内的单词
			printf("读入结束\n");
			break;
			case 2:
			CountWords(Cnt,n,string,&z); //计算每个词的数量
			creatFile(Cnt,z);  //将词数写入文件
			printf("文件创建成功\n");
			break;
			case 3:
			CreatHT(ht,z,Cnt); //建立霍夫曼树
			printf("文件建立成功\n");
			break;
			case 4:
			CreateHCode(ht,z,string); //霍夫曼编码
			printf("编码结束\n");
			break;
			case 5:
			Decoding(ht,z); //解码 
			printf("译码结束\n");
			break;
			case 6:
			printf("程序结束");
			exit(0);
			break;
			default:
			printf("请重新输入\n");
			break;		 
		}
	}
}
int main(){
	Menu();
	return 0;
}
