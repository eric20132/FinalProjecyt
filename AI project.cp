#include<iostream>
using namespace std;
typedef struct pancake* pointer;
struct pancake{
	int state[16];
	int g;
	int h;
	int f;
	int action;
	int order;
};
int gapNumber(int *, int );
struct pancake makeSuccessor(struct pancake , int , int );
bool isGoal(int *, int );
void insert(pointer , struct pancake , int *, int );
void equal(pointer ,pointer, int);
struct pancake getTop(pointer , int *, int );
int isEmpty(int );
void search(struct pancake, int , int, int *);
int best=100;
int step[100];

int main()
{
	int n, i, j, depth=1, sofar[100];
	cin >> n;
	struct pancake current;
	for(i=0;i<n;i++)
		cin >> current.state[i];
	current.g=1;
	current.h=gapNumber(current.state, n);
	current.f=current.g+current.h;
	current.action=1;
	search(current, depth, n, sofar);
	struct pancake tmp;
	equal(&tmp, &current, n);
	for(i=0;i<best-1;i++)
	{
		tmp=makeSuccessor(tmp, step[i], n);
		cout << "(" << step[i] << ") [" << tmp.state[0];	
		for(j=1;j<n;j++)
			cout << " " << tmp.state[j];
		cout << "]"<<endl;	
	}
	return 0;
}
 
bool isGoal(int *pancake, int n)
{
	int True=1, j;
	for(j=1;j<n;j++)
			if(pancake[j]!=pancake[j-1]+1)
				True=0;
	if(True==1)
		return true;
	else
	 	return false;
}

int gapNumber(int * pancake, int n)
{
	int i, gap=0;
	for(i=1;i<n;i++)
		if(pancake[i]!=pancake[i-1]+1 && pancake[i]!=pancake[i-1]-1)
			gap++;
	return gap;
}

struct pancake makeSuccessor(struct pancake current, int action, int n)
{
	int i;
	struct pancake successor;
	for(i=action-1;i>=0;i--)
		successor.state[action-1-i]=current.state[i];
	for(i=action;i<n;i++)
		successor.state[i]=current.state[i];
	successor.g=current.g+1;
	successor.h=gapNumber(successor.state, n);
	successor.f=successor.g+successor.h;
	successor.action=action;
	return successor;
}

void equal(pointer a,pointer b, int n)
{
	int tmp[n], i;
	for(i=0;i<n;i++)
		a->state[i]=b->state[i];
	a->g=b->g;
	a->h=b->h;
	a->f=b->f;
	a->action=b->action;
	a->order=b->order;
}

void insert(pointer heap, struct pancake node, int * num, int n)
{
	int i, parent, child=*num;
	for(i=0;i<n;i++)
		heap[*num].state[i]=node.state[i];
	heap[*num].g=node.g;
	heap[*num].h=node.h;
	heap[*num].f=node.f;
	heap[*num].action=node.action;
	heap[*num].order=node.order;
	struct pancake tmp;
	equal(&tmp, &heap[*num], n);
	for(parent=(*num)/2;parent>=1;)
	{
		if(tmp.f<heap[parent].f || (tmp.f==heap[parent].f && tmp.order<heap[parent].order))
		{
			equal(&heap[child], &heap[parent], n);
			parent/=2;
			child/=2;
		}
		else
			break;
	}
	if(parent>=1)
		equal(&heap[child], &tmp, n);
	else
		equal(&heap[1], &tmp, n);
	(*num)++;	
}

struct pancake getTop(pointer heap, int * num, int n){
	
	int i, next=1, parent=1;
	struct pancake tmp, better;
	equal(&better, &heap[1], n);
	equal(&heap[1], &heap[(*num)-1], n);
	equal(&tmp, &heap[1], n);
	(*num)--;
	while(parent<((*num)+1)/2)
	{
	//	cout << "EWERWQWERQWERQWERQ"<<endl;
		//cout << tmp.order<<" "<<heap[parent*2].order << " " << heap[parent*2+1].order << endl;
		if(heap[parent*2].f<heap[parent*2+1].f || (heap[parent*2+1].f==heap[parent*2].f && heap[parent*2].order < heap[parent*2+1].order))
		{
			if(tmp.f<heap[parent*2].f || (tmp.f==heap[parent*2].f && tmp.order < heap[parent*2].order))
				break;
			else
			{
				equal(&heap[parent], &heap[parent*2], n);
				parent*=2;
			}
		}	
		else if(heap[parent*2].f > heap[parent*2+1].f || (heap[parent*2+1].f==heap[parent*2].f && heap[parent*2].order > heap[parent*2+1].order))
		{
			if(tmp.f < heap[parent*2+1].f || (tmp.f==heap[parent*2+1].f && tmp.order < heap[parent*2+1].order))
				break;
			else
			{
				equal(&heap[parent], &heap[parent*2+1], n);
				parent=parent*2+1;
			}
		}	
	}
	equal(&heap[parent], &tmp, n);
	return better;
}

int isEmpty(int num)
{
	if(num==1)
		return 1;
	else
		return 0;
}

void search(struct pancake current, int depth, int n, int *sofar)
{
//	cout << " "<< depth <<endl;
	int num=1, i, index=0;
	struct pancake heap[20];
/*	for(i=0;i<n;i++)
		cout << current.state[i]<<" ";*/
	if(isGoal(current.state, n))
	{
		//cout << sofar[0]<<"EE" << depth;
			 
		//cout <<endl;
		if(depth<best)
		{
				
			best=depth;
			for(i=0;i<best;i++)
				step[i]=sofar[i];
		}
		return ;	
	}
	if(depth>best)
		return;	
	for(int check=0, i=1;i<n;i++)
	{
		if(current.action==i+1 && check==0)
			continue;
		else if(current.state[i]==current.state[i-1]-1||current.state[i]==current.state[i-1]+1)
		{
			if(i==n-1)
			{
				check=0;
				struct pancake node=makeSuccessor(current, i+1, n);
				node.order=index;
				index++;
				insert(heap, node, &num, n);
			}
			else
			{
				check=1;
				continue;
			}	
		}
		else
		{
			if(check!=0)
			{
				check=0;
				struct pancake node=makeSuccessor(current, i, n);
				node.order=index;
				index++;
				insert(heap, node, &num, n);
			}
			if(i+1==current.action)
				continue;
			struct pancake node=makeSuccessor(current, i+1, n);
			node.order=index;
			index++;
			insert(heap, node, &num, n);
		}
	}
	struct pancake better;
	equal(&better, &heap[1],  n);
//	cout<<"@#@#$"<<endl;
	while(!isEmpty(num))
	{
		if(better.f!=heap[1].f)
			break;
		better=getTop(heap, &num, n);
		//cout << "SDFSDFSDFSd"<<endl;
		sofar[depth-1]=better.action;
		search(better, depth+1, n, sofar);
	}
//	cout << "EEEEWQWERQWERQWER"<<endl;
}
