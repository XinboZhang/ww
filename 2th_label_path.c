#include <utility>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <iostream>
#include <string.h>
#include <vector>
#include <fstream>
#include <algorithm>
using namespace std;


map<string,int>mapNode;//URL to the number of node 
map<int,string>remapNode;//the number of node to URL
pair<map<string,int>::iterator,bool>Insert_Pair;
map<int,int>mapGraphNode;//the number of node to the number in the graph
map<int,int>remapGraphNode;//the number in the graph to the number of the node
pair<map<int,int>::iterator,bool>Insert_GPair;
map<string,int>mapUniqueLabel;//the number of the edge to the different number 
map<int,string>remapUniqueLabel;//different URL number to the name of the edge
map<int,string>remapLabel;//the number of edge to URL
map<int,int>mapGraphLabel;//the number of edge to the number in the graph
map<int,int>remapGraphLabel;//the number in the graph tp the number of edge
//ofstream fout("LabelPath1226.txt");
ofstream fout("LabelPath03241_2.txt");
ofstream fout1("tmp1_2.txt");


vector<vector<int> >show;
vector<vector<int> >io;
vector<bool>visit;
vector<vector<int> >IncludeNode(60000);
vector<int> countt;
int edgecnt=0;
int entitycnt=1;
int UniqueEdgeCnt=0;

struct iedge{
	int u;
	int v;
	int e;//record the unique number of edge
};

iedge * edge=new iedge[200000000];
int myanswerNo;
int jianzhi=0xffff;
int get_file_line(char *pInputName){//read the file by lines and get the URLs and map them as different nodes and edges,record the start node and the end node of each edge
	FILE * fp;
	int i=1;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	char *pOut;
	char needle[1];
	needle[0]=(char)9;
	char *result1;
	char *result2;
	char *result3;
	fp = fopen(pInputName, "r");
	if (fp == NULL)
	return -1;
	visit.push_back(false);
	while((read=getline(&line,&len,fp))!=-1){ 
		if((int)line[strlen(line)-2]==10||(int)line[strlen(line)-2]==13)
		line[strlen(line)-2]='\0';
		else if((int)line[strlen(line)-1]==10||(int)line[strlen(line)-1]==13)
		line[strlen(line)-1]='\0';
		result1=strtok(line,needle);
		result2=strtok(NULL,needle);
		result3=strtok(NULL,needle);
		if(result3[strlen(result3)-1]=='.')
		result3[strlen(result3)-1]='\0';
		edgecnt++;//the edge count,read one then add one,from 1,finally edgecnt means the number of edges

		remapLabel.insert(pair<int,string>(edgecnt,result2));//map the each edge to a true No.
		visit.push_back(false);	
		Insert_Pair=mapUniqueLabel.insert(pair<string,int>(result2,UniqueEdgeCnt));
		//map the edge to unique label
		if(Insert_Pair.second==true){ 

			remapUniqueLabel.insert(pair<int,string>(UniqueEdgeCnt,result2));
			//fout<<"T"<<edgecnt<<"F"<<remapLabel[edgecnt]<<endl;
			edge[edgecnt].e=UniqueEdgeCnt;
			UniqueEdgeCnt++;//UniqueEdgeCnt from 1,recording different edges
		}
		else{
			map<string,int>::iterator iter;
			iter=mapUniqueLabel.find(result2);
			edge[edgecnt].e=iter->second;
		}

		if(strcmp(result2,"<http://www.w3.org/1999/02/22-rdf-syntax-ns#type>")==0){ //cout<<"type"<<edge[edgecnt].e<<endl;
			jianzhi=edge[edgecnt].e;
		}
		Insert_Pair=mapNode.insert(pair<string,int>(result1,entitycnt));
		if(Insert_Pair.second==true){ 
			edge[edgecnt].u=mapNode[result1];
			remapNode.insert(pair<int,string>(entitycnt,result1));
		//cout<<entitycnt<<" "<<remapNode[entitycnt]<<endl;
			entitycnt++;
		}
		else{
			map<string,int>::iterator iter;
			iter=mapNode.find(result1);
			edge[edgecnt].u=iter->second;
		}
		Insert_Pair=mapNode.insert(pair<string,int>(result3,entitycnt));
		if(Insert_Pair.second==true){ 
			edge[edgecnt].v=mapNode[result3];
			remapNode.insert(pair<int,string>(entitycnt,result3));
			//cout<<entitycnt<<" "<<remapNode[entitycnt]<<endl;
			entitycnt++;
		}
		else{
			map<string,int>::iterator iter;
			iter=mapNode.find(result3);
			edge[edgecnt].v=iter->second;
		}//finally,the number of nodes equal to entitycnt-1
		IncludeNode[edge[edgecnt].e].push_back(edge[edgecnt].v);
	}

	fclose(fp);
}
void build_index(char *pInputName){//record which edge the node exists in

	int jj;
	show.resize(entitycnt+3);
	io.resize(entitycnt+3);
	for(int j=0;j<=entitycnt;j++){
		countt.push_back(0);
	}
	for(int j=1;j<=edgecnt;j++){//edge j
		jj=edge[j].e;

		show[edge[j].u].push_back(j);
		io[edge[j].u].push_back(1);
		countt[edge[j].u]++;
		
		
		show[edge[j].v].push_back(j);
		io[edge[j].v].push_back(0);
		countt[edge[j].v]++;
//cout<<j<<endl;

	}
}
struct EdgeNode{
	int adjvex;
	EdgeNode*next;
	int EdgeData;
};
struct VertexNode{
	EdgeNode*firstedge;
	int VertexData;
	bool stop;
};
struct AdjList{
	int VerNum;
	int ArcNum;
	VertexNode vertex[100000000];
};
int ncount;
int ecount;
	//EdgeNode *q=NULL;
void AddNode(AdjList *adj,int add){
	int d;
		//cout<<"from"<<add<<endl;
	for(int ju=0;ju<countt[add];ju++){
			//cout<<io[add][ju]<<endl;
		if(io[add][ju]==1)
		d=edge[show[add][ju]].v;
		else if(io[add][ju]==0)
		d=edge[show[add][ju]].u;
			//cout<<"d"<<d<<endl;
		if(visit[show[add][ju]]==false){
			mapGraphLabel.insert(pair<int ,int>(ecount,show[add][ju]));
			visit[show[add][ju]]=true;
				//cout<<"bian"<<remapLabel[show[add][ju]]<<endl;
			ecount++;

			Insert_GPair=remapGraphNode.insert(pair<int,int>(d,ncount));
			if(Insert_GPair.second==true){
				mapGraphNode.insert(pair<int ,int>(ncount,d));
				adj->vertex[ncount].VertexData=d;
				adj->vertex[ncount].firstedge=NULL;
				int wh=mapUniqueLabel[remapLabel[show[add][ju]]];
				if(wh==jianzhi)
					adj->vertex[ncount].stop=true;
				else
					adj->vertex[ncount].stop=false;
					//cout<<"dian"<<remapNode[d]<<endl;
				ncount++;
			}
		}
	}		
}
			
			
void CreateGraph(AdjList *adj){		
	vector<bool>::iterator init;
	for(init=visit.begin();init!=visit.end();init++)
		*init=false;
	ncount=1;
	mapGraphNode.insert(pair<int ,int>(ncount,myanswerNo));
	remapGraphNode.insert(pair<int,int>(myanswerNo,ncount));
	adj->vertex[ncount].VertexData=myanswerNo;
	adj->vertex[ncount].firstedge=NULL;
	adj->vertex[ncount].stop=false;
	ncount++;
	EdgeNode*q=NULL;
	ecount=1;
	AddNode(adj,myanswerNo);
	int orincount=ncount-1;
	for(int fall=2;fall<=orincount;fall++){
		if(adj->vertex[fall].stop==true)
			continue;
		AddNode(adj,mapGraphNode[fall]);
	}


	adj->VerNum=ncount-1;
	adj->ArcNum=ecount-1;
	for(int ju=1;ju<ecount;ju++){
		int trueNo=mapGraphLabel[ju];
		
		int s,d;
		s=remapGraphNode[edge[trueNo].u];
		d=remapGraphNode[edge[trueNo].v];
		
		q=(EdgeNode*)malloc(sizeof(EdgeNode));
		if(q==NULL){
			cout<<"ERRORR1"<<endl;
			perror("wrong1:");
			return;
		}
		q->EdgeData=trueNo;
		q->adjvex=d;
		q->next=adj->vertex[s].firstedge;
		adj->vertex[s].firstedge=q;


		/*q=(EdgeNode*)malloc(sizeof(EdgeNode));
		if(q==NULL){
			cout<<"ERRORR2"<<endl;
			perror("wrong2:");
			return;
		}
		q->EdgeData=trueNo;
		q->adjvex=s;
		q->next=adj->vertex[d].firstedge;
		adj->vertex[d].firstedge=q;*/
	}
	fout1<<"ncount "<<ncount-1<<" ecount "<<ecount-1<<endl;
}
void DisplayGraph(AdjList*adj)
{
	int dis=adj->VerNum;
	EdgeNode *q1=NULL;
	EdgeNode *q2=NULL;
	EdgeNode *q3=NULL;
	q1=adj->vertex[1].firstedge;


	if(q1!=NULL){
		q2=q1;
		while(q2!=NULL){
			q3=adj->vertex[q2->adjvex].firstedge;
			while(q3!=NULL){ 
				fout<<mapUniqueLabel[remapLabel[q2->EdgeData]]<<"*";
				fout<<mapUniqueLabel[remapLabel[q3->EdgeData]]<<"*";
				q3=q3->next;
				fout<<endl;
			}
			fout<<mapUniqueLabel[remapLabel[q2->EdgeData]]<<"*"<<endl;
			q2=q2->next;
		} 
	}
} 
void Delete(AdjList* adj){
	int dis=adj->VerNum;
	EdgeNode *q1=NULL;
	EdgeNode *q2=NULL;
	for(int p=1;p<=dis;p++){
		q1=adj->vertex[p].firstedge;
		while(q1!=NULL){
			q2=q1->next;
//cout<<"DFG"<<endl;
			free(q1);
//cout<<"THG"<<endl;
			q1=q2;
		}
	}
}
int main()
{
//char name[100]="DBpedia2014_sorted_dbo_dbp_type_yago_type";
	char name[100]="try0826.txt";
	char buf[256]={0};
	memset(buf,0,256);
	get_file_line(name);
	cout<<"GETDOWN"<<endl;
	cout<<jianzhi<<endl;
	build_index(name);
	cout<<"BUILD"<<endl;
	FILE* fpa=NULL;
	char * myanswer = NULL;
	size_t l= 0;
	//char MYA_PATH[20]="gengxin.txt";
	char MYA_PATH[20]="t0826.txt";
	fpa=fopen(MYA_PATH,"r");
	if(NULL==fpa)
	return -1;
	ssize_t readd;
	while((readd=getline(&myanswer,&l,fpa))!=-1){ 
		if((int)myanswer[strlen(myanswer)-2]==10||(int)myanswer[strlen(myanswer)-2]==13)
		myanswer[strlen(myanswer)-2]='\0';
		else if((int)myanswer[strlen(myanswer)-1]==10||(int)myanswer[strlen(myanswer)-1]==13)
		myanswer[strlen(myanswer)-1]='\0';
		printf("%s\n",myanswer);
		fout<<myanswer<<endl;
		AdjList *adj=(AdjList*)malloc(sizeof(AdjList));
		if(adj!=NULL){
			map<string,int>::iterator it;
			it=mapNode.find(myanswer);
			myanswerNo=it->second;
			cout<<myanswerNo<<endl;
			CreateGraph(adj);
			cout<<"CREATE"<<endl;
			DisplayGraph(adj);
			cout<<"DISPLAY"<<endl;
			Delete(adj);
			cout<<"Delete"<<endl;
			if(adj)free(adj);
			adj=NULL;
			cout<<"YES"<<endl;
			mapGraphNode.erase(mapGraphNode.begin(),mapGraphNode.end());
			remapGraphNode.erase(remapGraphNode.begin(),remapGraphNode.end());
			mapGraphLabel.erase(mapGraphLabel.begin(),mapGraphLabel.end());
			remapGraphLabel.erase(remapGraphLabel.begin(),remapGraphLabel.end());
			fout<<"#"<<endl;
			
		}
		else {
			cout<<"ERRORR"<<endl;
			perror("wrong:");
		}
	}
	return 0;	
}