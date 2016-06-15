
#include <time.h>
#include "route.h"
#include "lib_record.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include<iostream>


using namespace std;

#define MAX         600
#define INF         (~(0x1<<31))        // 最大值(即0X7FFFFFFF)
#define isLetter(a) ((((a)>='a')&&((a)<='z')) || (((a)>='A')&&((a)<='Z')))
#define LENGTH(a)   (sizeof(a)/sizeof(a[0]))

int static auth=0;

// 邻接表中表对应的链表的顶点
typedef struct _ENode//边节点
{   int id;
	int ivex;                   // 该边的顶点的位置
    int weight;                 // 该边的权
    //struct _ENode *next_edge;   // 指向下一条弧的指针
	struct _ENode *next_edge;
}ENode, *PENode;


// 边的结构体
typedef struct _edata
{   
	int  num;  //边的序号
  //   start; // 边的起点
 //    end;   // 边的终点
	int start;
	int end;
    int weight; // 边的权重
}EData;


// 邻接表中表的顶点
typedef struct _VNode//顶点节点
{
    int data;              // 顶点名称
	int flag;
    ENode *first_edge;      // 指向第一条依附该顶点的弧
}VNode;

// 邻接表
typedef struct _LGraph
{
    int vexnum;             // 图的顶点的数目
    int edgnum;             // 图的边的数目
    VNode vexs[MAX];//所有顶点集合
}LGraph;


int *char2int (char *c)
{   
	int i,j;
	int a;
	int bowl[MAX];
	int b=0;
	int tmp[4];
	for(i=0;i<MAX;i++)
		bowl[i]=0; 
for(i=0,j=0;i<MAX;i++)
       {   
		   	   
		     if(c[i]=='\0')
           {
			   switch(j)
			  {
			  case 1:  bowl[b]=tmp[0];b++;break;
			  case 2:  bowl[b]=tmp[0]*10 + tmp[1];b++;break;
			  case 3:  bowl[b]=tmp[0]*100 + tmp[1]*10+tmp[2];b++	 ;break;
              case 4:  bowl[b]=tmp[0]*1000 + tmp[1]*100+tmp[2]*10+tmp[3];	 b++;break;
			  }
			   bowl[b+1]=-1;
			   break;
           }
     a=int(c[i])-int('0');
           if(a>=0 && a<= 9)
		       { 
				   tmp[j]=a;
				   j++;
		       }
		   	else
		   {  
			  switch(j)
			  {
			  case 1:  bowl[b]=tmp[0];b++;break;
			  case 2:  bowl[b]=tmp[0]*10 + tmp[1];b++;break;
			  case 3:  bowl[b]=tmp[0]*100 + tmp[1]*10+tmp[2];b++	 ;break;
              case 4:  bowl[b]=tmp[0]*1000 + tmp[1]*100+tmp[2]*10+tmp[3];	 b++;break;
			  }
			   j=0;
		   }

}
return bowl;
}


static void link_last(ENode *list, ENode *node)
{
    ENode *p = list;
    
    while(p->next_edge != NULL)
        {
			p = p->next_edge;
		}
    p->next_edge = node;
	p->next_edge->next_edge=NULL;
}


LGraph* create_example_lgraph( int  gVexs[MAX],EData gEdges[5000],int vexnum, int edge_num)
{   int i, p1, p2,num;
	int c1,c2;

	int vlen = vexnum;

	int elen = edge_num;
    
    int weight;
    ENode *node1;
    LGraph* pG;

    if ((pG=(LGraph*)malloc(sizeof(LGraph))) == NULL )
        return NULL;
    memset(pG, 0, sizeof(LGraph));

    // 初始化"顶点数"和"边数"
    pG->vexnum = vlen;
    pG->edgnum = elen;
    // 初始化"邻接表"的顶点
    for(i=0; i<pG->vexnum; i++)
    {   
		pG->vexs[i].flag =0;
        pG->vexs[i].data = gVexs[i];//gVexs是全局变量
        pG->vexs[i].first_edge = NULL;
    }

    for(i=0; i<pG->edgnum; i++)
    {
        // 读取边的序号,起始顶点,结束顶点,权
		num=gEdges[i].num;
        c1 = gEdges[i].start;
        c2 = gEdges[i].end;//c2代表结束顶点
        weight = gEdges[i].weight;//gEdges是全局变量

		p1=c1;
		p2=c2;
        // 初始化node1
        node1 = (ENode*)malloc(sizeof(ENode));
		node1->id=num;
        node1->ivex = p2;//结束顶点
        node1->weight = weight;
        // 将node1链接到"p1所在链表的末尾"
        if(pG->vexs[p1].first_edge == NULL)
            {pG->vexs[p1].first_edge = node1;
		    pG->vexs[p1].first_edge->next_edge = NULL;
		     }
        else
            link_last(pG->vexs[p1].first_edge, node1);
  
    }

    return pG;
}




int get_weight(LGraph G, int start, int end)
{

	int w;
	int i=1;
    ENode *node,*prenode;
	int min=INF;
    if (start==end)
       {
	     
		      w=0;
	          return w;
     	}

	if(start==8888)
	{
            return INF;	
	}

    node = G.vexs[start].first_edge;

    while (node!=NULL)
    { i++;
        if (end==node->ivex)
		{
			if(node->weight<min)
			{
				min=node->weight;
			
			}
		
		}
           
		prenode=node;	
        node = node->next_edge;

		if(node==NULL)
		{
			w=min;
		    return w;
		}
    }

	w= INF;
    return w;
}




void swap(int *a, int *b) //交换两个数
{     
    int m;     
    m = *a;     
    *a = *b;     
    *b = m; 
}  



int *MaoPath(EData gEdges[5000],int *prev,int v,LGraph *G,int e)
{   int  i,j,w;
    int  weight=0; 
	w=0;
	int re[MAX];
	for(i=0;i<MAX;i++)
	{
	
	re[i]=-1;
	}
	int tp[MAX];
	int*p=re;
	 for(i=e; i!= v ; i=prev[i])
    {
        	
       for(j=0; j<(*G).edgnum; j++)
		if( (prev[i] == gEdges[j].start) && (i==  gEdges[j].end))
	   {

		   w++;
		   tp[w]=gEdges[j].num;
		   (*G).vexs[gEdges[j].start].flag=1;
	       (*G).vexs[gEdges[j].end].flag=1;
			weight=weight+gEdges[j].weight;

			
	   }
		
    }
	 i=0;
	 for(w;w>0;w--)
	 {re[i]=tp[w];

	 i++;

	 }
	 
	 p[MAX-1]=weight;
	 return p;

}


int *dijkstra(EData gEdges[5000],LGraph  *G, int vs, int e, int prev[], int dist[] )
{   
	
	int *p;
    int *p1;
    int i,j,k;
	k=8888;
    int min;
    int tmp;
	int t,w;
    int flag[MAX];      // flag[i]=1表示"顶点vs"到"顶点i"的最短路径已成功获取。
	for (i = 0; i < (*G).vexnum; i++)
    {

        flag[i] = 0;                    // 顶点i的最短路径还没获取到。
        prev[i] = -1;                    // 顶点i的前驱顶点为0。
   
		dist[i] = get_weight(*G, vs, i);

    }

    flag[vs] = 1;
    dist[vs] = 0;
	int o[MAX];
	
     int r;
     for(r=0;r<MAX;r++)
     o[r]=vs;

	  for (i =0; i < (*G).vexnum; i++)
    {     
		if(vs!=(*G).vexnum-1)
	{

        min = INF;
        for (j = 0; j < (*G).vexnum; j++)
        {

			              if((*G).vexs[j].first_edge!=NULL)
						  {
			                            if (flag[j]==0 && dist[j]<min &&  (  (*G).vexs[j].flag==0 || ( (*G).vexs[j].flag==1) && ((*G).vexs[j].first_edge->next_edge==NULL)))
                                       {
                                              min = dist[j];//在未获取最短路径的顶点中，找到离vs最近的顶点(k)。
                                               k = j;
		                                    
										}


						  }
            }
   }
	
	else
	{
	
	k=vs;
	
	
	
	}
        // 标记"顶点k"为已经获取到最短路径
		
        flag[k] = 1;
		prev[k]=o[k];

        for (j = 0; j < (*G).vexnum; j++)
        {  
			tmp = get_weight(*G, k, j);
            tmp = (tmp==INF ? INF : (min + tmp)); // 防止溢出

if((*G).vexs[j].first_edge==NULL && auth==1)
{

	int x=8888;
		   p=&x;
		return p;

}
            if (flag[j] == 0 && (tmp  < dist[j]) && (  (*G).vexs[j].flag==0 || ( (*G).vexs[j].flag==1) && ((*G).vexs[j].first_edge->next_edge==NULL)))
            {
                dist[j] = tmp;
				if(k!=0)
				{
					o[j]=k;
				prev[j] = o[j];
		
				}
        
            }
		

			if(flag[e]==1)
			{	
	
				break;
				
			}


        }
	}

	if(dist[e]!=INF)
		{
			p=MaoPath(gEdges,prev ,vs,G,e);		
	    }
	else
       {
		   int x=8888;
		   p=&x;
	   }
	   	
	return p;
}



void search_route(char *topo[5000], int edge_num, char *demand)
{   clock_t start;
    clock_t finish1;
	clock_t finish2;
    start = clock(); 

	int final=1;
	int crack=1;
	int swapkey;
   EData gEdges[5000];
	int i;
	int f;
	int j;
	int *t;//找次级点时用到
    int weight=0;
    int test=0; 
    int prev[MAX] = {0};
    int dist[MAX] = {0};
    LGraph* pG;
	int *p;
	int q=0;
	int d[54];
	int k=0;
	int *two;
	unsigned short result[MAX];
	unsigned short resulttp[MAX];
	int minweight=1000000;

	int vexnum=0;

	for(i=0;i<54;i++)
	{
	    d[i]=-1;
	}

	i=0;
	while(char2int(demand)[i]!=-1)
	{ 
		
		d[i]=char2int(demand)[i];

		i++;
	}

	for(i=0;i<edge_num;i++)
	{

	          if(char2int(topo[i])[2]>vexnum)
			  {
			       vexnum=char2int(topo[i])[2];

			  }

	  
	}
	vexnum++;
	int  gVexs[MAX];

	for(i=0;i<MAX;i++)
	{
	     gVexs[i]=-1;
	}


	for(i=0;i<5000;i++)
	{
		gEdges[i].num=-1;

	}

	for(i=0;i<vexnum;i++)
	{
	     gVexs[i]=i;
	}
	for(i=0;i<edge_num;i++)
	{

		gEdges[i].num=char2int(topo[i])[0];
		gEdges[i].start=char2int(topo[i])[1];
		gEdges[i].end=char2int(topo[i])[2];
		gEdges[i].weight=char2int(topo[i])[3];
	}


    pG = create_example_lgraph( gVexs ,gEdges ,vexnum,edge_num);
	

	int m[54];
	for(i=0;i<MAX;i++)//初始化
	{
	  resulttp[i]=8888; 
	  result[i]=8888; 
	  
	}

	for(i=0;i<54;i++)//初始化
	{
	  m[i]=-1;
	}
	int mtp;
    m[0]=d[0];
	(*pG).vexs[d[1]].flag=1;
	for(i=1;i<54;i++)
	{
        
		m[i]=d[i+1];
	   if(m[i]==-1)
	      {

			  m[i-1]=-1;
			  mtp=i-1;
			  break;
	      }

	}

int itp;
int iitp;

//对M[]排序
     for(j=1;j<mtp;j++) 
      {for(i=1;i<mtp-1-j;i++)
              {  if(m[i]>m[i+1]) 
                {
                 f=m[i];
                 m[i]=m[i+1];
                  m[i+1]=f;
                   }
	             }
         }                              
	
	 j=0;





while(final)
{	final=0;
while(crack)
{   crack=0;
	i=0;

bool ifff=0;
bool elll=0;


	while(m[i+1] != -1)
	{   
		finish1 = clock();

if(finish1-start<7000)
{		if( pG->vexs[m[i+1]].flag == 0 )
		     {     q=i+1;
				 t=p=dijkstra(gEdges,pG,m[i],m[i+1],prev, dist);		


				                            if((*t)==8888)
										   {

											crack=1;
											swapkey=i+1;
										    break;




										   }

			
				   weight=weight+p[MAX-1];
				   ifff=1;

		     }
		else
		     {     int j=2;

		             while(i+j<mtp)
		           {  


					   if((*pG).vexs[m[i+j]].flag == 0)
				     {
				          
				          t=p=dijkstra(gEdges,pG,m[i],m[i+j],prev, dist);
						                 

						               if((*t)==8888)
										   {

										  crack=1;
										  swapkey=i+1;
										  break;
									   
									   
									       }


						   weight=weight+p[MAX-1];
						   q=i+j;
						   elll=1;
			            break;                                                      
				     }

					   if(crack==1)
					   {
					        break;
					   }

					   j++;
				   }
				              

              if(crack==1)
			 {
					   break;
			 }  



			if(i+j==mtp)
			{
			
			     break;
			
			}

		     }



	if(ifff)
	{
       ifff=0;
	  i++;
	}
	else if(elll)
	{   
		elll=0;
	   i=q;
	}
	else
	{
	   i++;
	}

if(*p ==8888)
		{
		
		break;
		
		}                  


		 while(*p != -1)
	              {    
					  resulttp[k]=*p;
					  k++;
	                  p++;
     	           }

	}
	else{break;}
}
if(crack==0)
{	
	
	(*pG).vexs[d[1]].flag=0;
	t=p=dijkstra(gEdges,pG,m[q],d[1],prev, dist);
	 if(*p==8888)
	 {
	 
	 crack=1;
	 swapkey=q;

	 
	 }
	 else
	 {
		 weight=weight+t[MAX-1];
	 }

	   while(*p != -1  && *p!= 8888)
	    {     
					  resulttp[k]=*p;
					  k++;
	                  p++;
     	 }
	                
	 
}





 finish = clock(); 
 if(finish-start<7000)
 {

                        if(crack==0)
                    {
	                             
								 if(weight<minweight)
								 {  minweight=weight;

								 	        for(i=0;i<MAX;i++)
	                                    {
	                                           
	                                             result[i]=8888; 
	  
                                     	}
                                        i=0;
		                       while (resulttp[i] != 8888)
	                                      {
											     
											     result[i]=resulttp[i];
					
		                                        i++;
                                           }
						                   auth=1;							      
								 }
								         srand( (unsigned)time( NULL ) );
							              itp= rand()%(mtp-2)+2;
		                                       iitp=m[1];
		                                       m[1]=m[itp];
		                    	                m[itp]=iitp;

				                   for(i=0;i<MAX;i++)
                                    {pG->vexs[i].flag=0;
								   }

										for(i=0;i<MAX;i++)
	                             {
	                                            
									      resulttp[i]=8888;
												 
	  
	                              }
								k=0;

								(*pG).vexs[d[1]].flag=1;

								weight=0;


								
								crack=1;

                    }   
				  
				       else
				  {
				                     
				                       itp=m[1];

                                   if(swapkey<=2)
									{
									    swapkey=mtp-1;
									}
		                              for(i=2;i<=swapkey;i++)
		                            {   
			                                    iitp=m[i];
		                                       m[i]=itp;
		                    	                itp=iitp;
              
		                             }
	                    	                    m[1]=itp;
				                   for(i=0;i<MAX;i++)
                                    {pG->vexs[i].flag=0;
								   }

										for(i=0;i<MAX;i++)
	                             {
	                                            
									      resulttp[i]=8888;
												 
	  
	                              }
								k=0;

								(*pG).vexs[d[1]].flag=1;

								weight=0;
								
				  }
               final=1;


                   
 }
 else 

 {                                            
	   
	   final=0;
	   crack=0;
	   i=0;
      while (result[i] != 8888)
	  {
           record_result(result[i]);													
		   i++;
        }
 
 }



}

}

}







