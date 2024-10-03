
// rotate arbitrary given node so that height of that node from root reduces by 1 (inorder should be preserved)

#include<stdio.h>
#include<stdlib.h>

typedef struct node
{
	int data;
	struct node* left;
	struct node* right;
}Node;

Node* newNode(int data) 
{ 
	Node* node = (Node*)malloc(sizeof(Node)); 
	node->data   = data; 
	node->left   = NULL;
	node->right  = NULL; 
	return(node);
}


Node* build()
{
	Node* newRoot;
	Node* root = newNode(10);
	root->left = newNode(5);
	root->right= newNode(13);
	root->right->left = newNode(55);
	root->left->left =newNode(3);
	root->left->left->left =newNode(1);
	root->right->right =newNode(17);
	root->right->right->right =newNode(20);
	root->right->right->left =newNode(15);
	return root;
}

void preorder(Node* root)
{
	if(root==NULL)
		return;
	printf("%d ", root->data);
	preorder(root->left);
	preorder(root->right);
}
void inorder(Node* root)
{
	if(root==NULL)
		return;
	inorder(root->left);
	printf("%d ", root->data);
	inorder(root->right);
}

Node* findParent(Node* root,int key)
{
	Node* parent;
	if(root==NULL || root->data==key)
		return NULL;
	
	if((root->left!=NULL && (root->left->data == key)) ||(root->right!=NULL && (root->right->data == key)))
		return root;

	parent = findParent(root->left,key);
	if(parent != NULL)
		return parent;

	return findParent(root->right,key);
}

Node* RR(Node* node)
{
	Node* subtree;
	Node* newroot;
	 
	newroot = node->left;
	subtree = newroot->right;
	newroot->right = node;
	node->left = subtree;

	return newroot;
}

Node* LL(Node* node)
{
	Node* subtree;
	Node* newroot;

	newroot = node->right;
	subtree = newroot->left;
	newroot->left = node;
	node->right = subtree;

	return newroot;
}

Node* rotateNode(Node* root,int key)
{
	Node* parent;
	Node* tempNode;
	
	if(root != NULL)
	{
		if(root->data == key)
			return root;
		else
		{
			parent =findParent(root,key);
			if(parent!=NULL)
			{
				Node* grandparent;
				grandparent = findParent(root,parent->data);	
			
				if(parent->left!=NULL && parent->left->data == key)
				{	
					if(grandparent != NULL && grandparent->left==parent)
							grandparent->left = RR(parent);
					
					else if(grandparent != NULL && grandparent->right==parent)
							grandparent->right = RR(parent);
					
					else
						root = RR(parent);
				}
				else if(parent->right!=NULL && parent->right->data == key)
				{
					if(grandparent != NULL && grandparent->right == parent)
							grandparent->right = LL(parent);

					else if(grandparent != NULL && grandparent->left == parent)
							grandparent->left = LL(parent);
		
					else
						root = LL(parent);
				}
			}
		}
	}
	return root;
}

int main()
{
	int height;
	Node* root = build();
	inorder(root);
	printf("\n");
	preorder(root);

	printf("\n");
	root= rotateNode(root,17);
	
	inorder(root);
	printf("\n");
	preorder(root);	

	return 0;
}
