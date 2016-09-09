#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<map>
#include<iostream>
using namespace std;
map<int,string>VId2Label;
map<string,int>VLabel2Id;
pair<map<string,int>::iterator,bool>Insert_Pair;
struct EdgeNode{
	string valued;
	EdgeNode*next;
	int adjvex;
};
struct vertexNode{
	EdgeNode*firstedge;
	string data;
};
struct AdjList{
	int verNum;
	int ArcNum;
	vertexNode vertices[10];
};
int countnode;
void Create_GF(AdjList*G){
	countnode=0;
	int flag=0;
	string x,la,y;
	while(cin>>x>>la>>y){
		int len=y.length();
	
		
		if(y[len-1]=='}'){flag=1;y=y.substr(0,len-1);}
		
		len=y.length();
		
		if(y[len-1]=='.'){y=y.substr(0,len-1);}
		Insert_Pair=VLabel2Id.insert(pair<string,int>(x,countnode));
		if(Insert_Pair.second==true){
			VId2Label.insert(pair<int,string>(countnode,x));
			G->vertices[countnode].firstedge=NULL;
			countnode++;
		}
		Insert_Pair=VLabel2Id.insert(pair<string,int>(y,countnode));
		if(Insert_Pair.second==true){
			VId2Label.insert(pair<int,string>(countnode,y));
			G->vertices[countnode].firstedge=NULL;
			countnode++;
		}
		int st=VLabel2Id[x];
		int ed=VLabel2Id[y];
		EdgeNode*s=new EdgeNode;
		s->valued=la;
		s->adjvex=ed;
		s->next=G->vertices[st].firstedge;
		G->vertices[st].firstedge=s;
		if(flag==1)break;
	}
}
void Print_GF(AdjList *G){
	for(int i=0;i<countnode;i++){
		cout<<VId2Label[i];
		EdgeNode*p=G->vertices[i].firstedge;
		while(p){
			cout<<" -> "<<VId2Label[p->adjvex]<<" ( BY:"<<p->valued<<") ";
			p=p->next;
		}
		cout<<endl;
	}
}
void Delete_GF(AdjList*G){
	for(int i=0;i<countnode;i++){
		EdgeNode*q;
		EdgeNode*p=G->vertices[i].firstedge;
		while(p){
			q=p;
			p=p->next;
			delete q;
		}
		G->vertices[i].firstedge=NULL;
	}
}
int main(){
	char tmp;
	cin>>tmp;
	while(tmp!='{')cin>>tmp;
	getchar();
	AdjList G;
	Create_GF(&G);
	Print_GF(&G);
	Delete_GF(&G);
}