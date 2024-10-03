#include<stdio.h>
#include<stdlib.h>

#define MAX 3
#define MIN 1


typedef struct node
{
	int data[MAX+1];
	int count;
	struct node* child[MAX+1];
}Node;

Node* root;

Node* createNode(int data,Node* child)
{
	Node* newNode=(Node*)malloc(sizeof(Node));
	newNode->count = 1;
	newNode->data[1] = data;
	newNode->child[0] = root;
	newNode->child[1] = child;
	return newNode;
}

void addToNode(int data,int pos,Node* node,Node* child)
{
	int j=node->count;
	while(j > pos)
	{
		node->data[j+1] = node->data[j];
		node->child[j+1] = node->child[j];
		j--;
	}
	node->data[j+1]=data;
	node->child[j+1]=child;
	node->count++;
}

void splitNode(int data,int* pval,int pos,Node* node, Node* child, Node** newNode)
{
	int median,j;
	if(pos > MIN)
		median = MIN+1;
	else
		median = MIN;

	*newNode = (Node*)malloc(sizeof(Node));
	j=median+1;
	while(j <= MAX)
	{
		(*newNode)->data[j-median]= node->data[j];
		(*newNode)->child[j-median]= node->child[j];
		j++;
	}
	node->count = median;
	(*newNode)->count = MAX-median;

	if(pos <= MIN)
		addToNode(data,pos,node,child);
	else
		addToNode(data,pos-median,*newNode,child);

	*pval= node->data[node->count];
	(*newNode)->child[0]= node->child[node->count];
	node->count--;
}

int setValue(int data,int* pval,Node* node,Node** child)
{
	int pos;
	if(!node)
	{
		*pval= data;
		*child= NULL;
		return 1;
	}
	if(data < node->data[1])
		pos=0;
	else
	{
		for(pos=node->count;data < node->data[pos] && pos>1; pos--);
		if(data == node->data[pos])
		{
			return 0;
		}
	}

	if(setValue(data,pval,node->child[pos],child))
	{
		if(node->count < MAX)
			addToNode(*pval,pos,node,*child);
		else
		{
			splitNode(*pval,pval,pos,node,*child,child);
			return 1;
		}
	}
	return 0;
}

void insert(int data)
{
	int i;
	int flag;
	Node* child;
	flag=setValue(data,&i,root,&child);
	if(flag)
		root=createNode(i, child);
}

int copySuccessor(Node* dummy)
{
	while(dummy->child[0])
		dummy = dummy->child[0];
	return dummy->data[1];
}

int copyPredecessor(Node* dummy)
{
	int nodecnt= dummy->count;
	while(dummy->child[nodecnt])
		dummy = dummy->child[nodecnt];
	return dummy->data[nodecnt];
}

void mergeNodes(Node* node,int pos)
{
	Node *x1 = node->child[pos];
	Node* x2;
	if(node->child[pos-1])
		x2= node->child[pos-1];
	else
		x2= (Node*)malloc(sizeof(Node));

	x2->count++;
	x2->data[x2->count]= node->data[pos];
	x2->child[x2->count]= x1->child[0];

	int j=1;
	while(j<= x1->count)
	{
		x2->count++;
		x2->data[x2->count]=x1->data[j];
		x2->child[x2->count]=x1->child[j];
		j++;
	}

	j=pos;
	while(j < node->count)
	{
		node->data[j] = node->data[j+1];
		node->child[j] = node->child[j+1];
		j++;
	}

	node->count--;
	free(x1);
	node->child[pos-1]= x2;
}

void leftShift(Node* node,int pos)
{
	Node* x;
	x= node->child[pos-1];

	x->count++;
	x->data[x->count]= node->data[pos];
	x->child[x->count]= node->child[pos]->child[0];
	
	x= node->child[pos];
	node->data[pos]=x->data[1];
	x->child[0] = x->child[1];
	x->count--;

	int j;
	for(j=1; j<= node->child[pos]->count ;j++)
	{
		x->data[j]= x->data[j+1];
		x->child[j]= x->child[j+1];
	}
	return;
}

void rightShift(Node* node,int pos)
{
	Node* x;
	x= node->child[pos];
	int j=x->count;
	x->count++;
	while (j > 0) 
	{
	    	x->data[j + 1] = x->data[j];
		x->child[j + 1] = x->child[j];
		j--;
	}
	x->data[1] = node->data[pos];
	x->child[1] = x->child[0];
	
	
 	x = node->child[pos - 1];
   	node->data[pos] = x->data[x->count];
    	node->child[pos]->child[0] = x->child[x->count];
    	x->count--;
	return;
}

void adjustNode(Node* node,int pos)
{
	if(!pos)
	{
		if(node->child[1]->count > MIN)
			leftShift(node,1);
		else
			mergeNodes(node,1);
	}
	else if(pos == node->count)
	{
		if(node->child[pos-1]->count > MIN)
			rightShift(node,pos);
		else
			mergeNodes(node,pos);	
	}
	else
	{
		if(node->child[pos-1]->count > MIN)
			rightShift(node,pos);
		else if(node->child[pos+1]->count > MIN)
			leftShift(node,pos+1);
		else
		{
			mergeNodes(node,pos);
		}
	}
}

void removeVal(int pos,Node* node)
{
	int i;
	for(i=pos; i<node->count ;i++)
	{
		node->data[i] = node->data[i+1];
		node->child[i] = node->child[i+1];
	}
	node->count--;
}

int delVal(int key, Node* node)
{
	int pos,flag;
	if(node)
	{
		if(key < node->data[1])
		{
			pos = 0;
			flag = 0;
		}
		else
		{
			for(pos=node->count; key < node->data[pos] && pos>1 ; pos--);
			if(node->data[pos] == key)
				flag = 1;
			else
				flag = 0;
		}		

		if(flag)
		{
			if(node->child[pos-1])
			{
				node->data[pos] = copyPredecessor(node->child[pos-1]);
				flag= delVal(node->data[pos],node->child[pos-1]);
			}
			else if(node->child[pos])
			{
				node->data[pos] = copySuccessor(node->child[pos]);
				flag = delVal(node->data[pos],node->child[pos]);
			}
			else  
				removeVal(pos,node); //deletion from leaf node
		}
		else
			flag = delVal(key,node->child[pos]);

	//balancing after deletion
		if(pos)
		{
			 if (node->child[pos-1] && node->child[pos-1]->count < MIN)
			 {
				adjustNode(node, pos-1);//for predecessor check
			}
		}
	 	if (node->child[pos] && node->child[pos]->count < MIN)
		{
			adjustNode(node, pos);//for successor check
		}
	}
	return flag;	

}

void delete(int key,Node* node)
{
	Node* temp;
	if(!delVal(key,node))
	{
		return;
	}
	else
	{
		if(node->count == 0)
		{
			temp = node;
			node = node->child[0];
			free(temp);
		}
	}
	root = node;
	return;
}

int main()
{
	insert(8);
	insert(9);
    	insert(10);
      	insert(11);
       	insert(15);
	insert(16);
	insert(17);
	insert(18);
	insert(20);
	insert(23);
	insert(7);
	insert(6);
	insert(27);
	insert(30);

	delete(18,root);
	delete(17,root);
	delete(16, root);
	delete(23, root);
	delete(6, root);
	delete(7, root);
	delete(15, root);
	delete(20, root);
	return 0;
}
