#include<stdio.h>
#include<stdlib.h>

typedef struct node
{
	int data;
	struct node* left;
	struct node* right;
	int height;
}Node;

Node* newNode(int data) 
{ 
	Node* node = (Node*)malloc(sizeof(Node)); 
	node->data   = data; 
	node->left   = NULL;
	node->right  = NULL; 
	node->height = 1;
	return(node);
};

int height(Node* N)
{
	if(N==NULL)
		return 0;
	return N->height;
}

int getBalance(Node* N) 
{ 
    if (N == NULL) 
            return 0; 
    return height(N->left) - height(N->right); 
}

int max(int a, int b) 
{ 
	    return (a > b)? a : b; 
} 

Node* LL(Node* node)
{
	Node *tempRight,*tempNode;

	tempRight = node->right;
	tempNode = tempRight->left;

	tempRight->left = node;
	node->right = tempNode;

	node->height = max(height(node->left),height(node->right)) + 1; 
	tempRight->height = max(height(tempRight->left),height(tempRight->right)) + 1;
	return tempRight;
}

Node* RR(Node* node)
{
	Node *tempLeft,*tempNode;

	tempLeft = node->left;
	tempNode = tempLeft->right;

	tempLeft->right = node;
	node->left = tempNode;

	node->height = max(height(node->left),height(node->right)) + 1; 
    	tempLeft->height = max(height(tempLeft->left),height(tempLeft->right)) + 1;
	return tempLeft;
}

Node* minFromRight(Node* root)
{
	while(root->left != NULL)
	{
		root = root->left;
	}
	return root;
}

Node* delete(Node* root,int key)
{
	Node* temp;
	if(key < root->data)
		root->left = delete(root->left,key);
	else if(key > root->data)
		root->right = delete(root->right,key);

	else	//node to be deleted is found
	{
		if(root->right==NULL || root->left==NULL)
		{
			temp = (root->left)? root->left : root->right;
			if(temp == NULL)
				root = NULL;	// No child case, delete root directly
			else
				*root = *temp;	//copy contents from child node to root node
		}

		else //both child present
		{
			temp = minFromRight(root->right);
			root->data = temp->data;
			root->right = delete(root->right, temp->data);
		}
		free(temp);
	}

	if(root == NULL)
		return root;

	root->height = max(height(root->left),height(root->right)) + 1;

	int balance = getBalance(root);
	if(balance > 1 && getBalance(root->right) >= 0)
		return RR(root);
	if(balance > 1 && getBalance(root->right) < 0)
	{
		root->right = LL(root);
		return RR(root);
	}
	if(balance < -1 && getBalance(root->left) <= 0)
		return LL(root);
	if(balance < -1 && getBalance(root->left) > 0)
	{
		root->left = RR(root->left);
		return LL(root);	
	}
       	return root;		
}


Node* insert(Node* root,int key)
{
	if (root==NULL)
		return newNode(key);

	if(key < root->data)
		root->left =insert(root->left, key);
	else if (key > root->data) 
	        root->right = insert(root->right, key); 
    	else 
         	return root;


	root->height = 1 + max(height(root->left),height(root->right)); 

	int balance= getBalance(root);
	if(balance > 1 && key < root->left->data)
		return RR(root);

	if(balance > 1 && key > root->left->data)
	{
		root->left = LL(root->left);
		return RR(root);
	}

	if(balance < -1 && key < root->right->data)
	{
		root->right = RR(root->right);
		return LL(root);
	}

	if(balance < -1 && key > root->right->data)
	{
		return LL(root);
	}
	return root;
}

void preOrder(Node* root) 
{ 
	if(root != NULL) 
	{ 
		printf("%d ", root->data); 
		preOrder(root->left); 
		preOrder(root->right); 
	}				     
} 

void inOrder(Node* root) 
{ 
	if(root != NULL) 
	{ 
		inOrder(root->left); 
		printf("%d ", root->data); 
		inOrder(root->right); 
	}				     
} 

int main()
{
	Node* root = NULL;


	root = insert(root, 9);
	    root = insert(root, 5);
	        root = insert(root, 10);
		    root = insert(root, 0);
		        root = insert(root, 6);
			    root = insert(root, 11);
			        root = insert(root, -1);
				    root = insert(root, 1);
				        root = insert(root, 2);
	preOrder(root);
	printf("\n");
	inOrder(root);
	printf("\n\n");
	root = delete(root, 10);
	preOrder(root);
	printf("\n");
	inOrder(root);
	printf("\n\n");
	return 0;
}
