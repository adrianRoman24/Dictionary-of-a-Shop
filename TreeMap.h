#ifndef TREEMAP_H_
#define TREEMAP_H_

#include <stdlib.h>

#define MAX(a, b) (((a) >= (b))?(a):(b))

//-------------------------------------------------------------------------

typedef struct node{
	void* elem; // Node key
	void* info; // Node satellite information
	struct node *pt; // Parent link
	struct node *lt; // left child link
	struct node *rt; // right child link
	struct node* next; // next link in the list of duplicates
	struct node* prev; // prev link in the list of duplicates
	struct node* end; // pointer to the end of the duplicates list
	long height;
}TreeNode;

typedef struct TTree{
	TreeNode *root; // pointer to the root of the tree
	void* (*createElement)(void*); // create element method
	void (*destroyElement)(void*); // destroy element method
	void* (*createInfo)(void*); // create info method
	void (*destroyInfo)(void*); // destroy info method
	int (*compare)(void*, void*); // comparison method between elements
	long size;
}TTree;


TTree* createTree(void* (*createElement)(void*), void (*destroyElement)(void*),
		void* (*createInfo)(void*), void (*destroyInfo)(void*),
		int compare(void*, void*)){

	// TODO: Cerinta 1 
	TTree *t = (TTree *)malloc(sizeof(TTree));
	t->size = 0;
	t->compare = compare;
	t->destroyInfo = destroyInfo;
	t->createInfo = createInfo;
	t->destroyElement = destroyElement;
	t->createElement = createElement;
	t->root = NULL;
	return t;
}

int isEmpty(TTree* tree){

	// TODO: Cerinta 1
	return tree->size == 0;
}


TreeNode* search(TTree* tree, TreeNode* x, void* elem){
	// TODO: Cerinta 1
	TreeNode *aux = x;
	while(aux != NULL){
		if(tree->compare(aux->elem, elem) > 0)
			aux = aux->lt;
		else
			if(tree->compare(aux->elem, elem) < 0)
				aux = aux->rt;
			else
				return aux;
	}
	return NULL;
}


TreeNode* minimum(TreeNode* x){
	// TODO: Cerinta 1
	if(x == NULL)
		return NULL;
	TreeNode *aux = x, *parent = x;
	while(1)
	{
		aux = aux->lt;
		if(aux == NULL)
			return parent;
		parent = parent->lt;
	}
}

TreeNode* maximum(TreeNode* x){
	// TODO: Cerinta 1
	if(x == NULL)
		return NULL;
	TreeNode *aux = x, *parent = x;
	while(1)
	{
		aux = aux->rt;
		if(aux == NULL)
			return parent;
		parent = parent->rt;
	}
}

TreeNode* successor(TreeNode* x){
	// TODO: Cerinta 1
	// Check function arguments
	if(x == NULL)
		return NULL;
	if(x->rt != NULL)
		return minimum(x->rt);

	TreeNode *parent = x->pt, *child = x;
	while(parent != NULL)
	{
		if(parent->lt == child)
			return parent;
		parent = parent->pt;
		child = child->pt;
	}
	return NULL;
}

TreeNode* predecessor(TreeNode* x){
	// TODO: Cerinta 1
	if(x == NULL)
		return NULL;
	if(x->lt != NULL)
		return maximum(x->lt);
	TreeNode *parent = x->pt, *child = x;
	while(parent != NULL)
	{
		if(parent->rt == child)
			return parent;
		parent = parent->pt;
		child = child->pt;
	}
	return NULL;
}

void updateHeight(TreeNode* x){
	int leftHeight = 0;
	int rightHeight = 0;
	if(x != NULL){
		if(x->lt != NULL) leftHeight = x->lt->height;
		if(x->rt != NULL) rightHeight = x->rt->height;
		x->height = MAX(leftHeight, rightHeight) + 1;
	}
}

void avlRotateLeft(TTree* tree, TreeNode* x){
	// TODO: Cerinta 1
	TreeNode *k1 = x->rt;
	TreeNode *k2 = k1->lt;
	if(k2 != NULL)
		k2->pt = x;
	k1->lt = x;
	x->rt = k2;
	k1->pt = x->pt;
	x->pt = k1;
	if(k1->pt != NULL)
		if(k1->pt->rt == x)
			k1->pt->rt = k1;
		else
			k1->pt->lt = k1;
	else
		tree->root = k1;
	updateHeight(k1);
	updateHeight(x);
}

void avlRotateRight(TTree* tree, TreeNode* y){
	// TODO: Cerinta 1
	TreeNode *k1 = y->lt;
	TreeNode *k2 = k1->rt;
	if(k2 != NULL)
		k2->pt = y;
	k1->rt = y;
	y->lt = k2;
	k1->pt = y->pt;
	y->pt = k1;
	if(k1->pt != NULL)
		if(k1->pt->rt == y)
			k1->pt->rt = k1;
		else
			k1->pt->lt = k1;
	else
		tree->root = k1;
	updateHeight(k1);
	updateHeight(y);
}


long height_node(TreeNode *t)
{
	if(t == NULL)
		return 0;
	return t->height;
}

TreeNode* createTreeNode(TTree *tree, void* value, void* info){
	// Allocate node
	TreeNode* newNode = (TreeNode*) malloc(sizeof(TreeNode));

	// Set its element field
	newNode->elem = tree->createElement(value);

	//Set its info
	newNode->info = tree->createInfo(info);

	// Set its tree links
	newNode->pt = newNode->rt = newNode->lt = NULL;

	// Set its list links
	newNode->next = newNode->prev = newNode->end = NULL;
	newNode->end = newNode;
	/*
	 *  The height of a new node is 1,
	 *  while the height of NULL is 0
	 */
	newNode->height = 1;

	return newNode;
}

void destroyTreeNode(TTree *tree, TreeNode* node){
	// Check arguments
	if(tree == NULL || node == NULL) return;

	// Use object methods to de-alocate node fields
	tree->destroyElement(node->elem);
	tree->destroyInfo(node->info);

	// De-allocate the node
	free(node);
}

void add_to_list(TTree *tree, TreeNode *node, TreeNode *root)
{
	if(root == NULL)
		return;
	//initial root is tree->root
	TreeNode *aux = root;
	//case after
	while(tree->compare(node->elem, aux->elem) > 0)
	{
		aux = aux->next;
		if(aux == NULL)
		{
			root->end->next = node;
			node->prev = root->end;
			return;
		}
		else
		if(tree->compare(node->elem, aux->elem) == 0)
		{
			if(aux->end->next != NULL)
				aux->end->next->prev = node;
			node->next = aux->end->next;
			aux->end->next = node;
			node->prev = aux->end;
			TreeNode *aux2 = aux;
			while(tree->compare(aux2->elem, node->elem) == 0)
			{
				aux2->end = node;
				aux2 = aux2->next;
				if(aux2 == NULL)
					break;
			}
			return;
		}
		root = root->next;
	}
	//case before
	while(tree->compare(node->elem, aux->elem) < 0)
	{
		aux = aux->prev;
		if(aux == NULL)
		{
			root->prev = node;
			node->next = root;
			return;
		}
		else
		if(tree->compare(node->elem, aux->elem) == 0)
		{
			if(aux->end->next != NULL)
				aux->end->next->prev = node;
			node->next = aux->end->next;
			aux->end->next = node;
			node->prev = aux->end;
			TreeNode *aux2 = aux;
			while(tree->compare(aux2->elem, node->elem) == 0)
			{
				aux2->end = node;
				aux2 = aux2->next;
			}
			return;
		}
		root = root->prev;
	}

	//duplicate case
	if(root->end->next != NULL)
		root->end->next->prev = node;
	node->next = root->end->next;
	root->end->next = node;
	node->prev = root->end;
	TreeNode *aux2 = root;
	while(tree->compare(aux2->elem, node->elem) == 0)
	{
		aux2->end = node;
		aux2= aux2->next;
		if(aux2 == NULL)
			break;
	}
	return;
}

void update_all_heights(TreeNode *aux){
	while(aux != NULL){
		updateHeight(aux);
		updateHeight(aux->lt);
		updateHeight(aux->rt);
		aux = aux->pt;
	}
}

void equilibrate(TTree *tree, TreeNode *parent1)
{
	while(parent1 != NULL)
	{
		if(abs(height_node(parent1->lt) - height_node(parent1->rt)) > 1)
		{
			if(height_node(parent1->rt) > height_node(parent1->lt))
			{
				// right right
				if(height_node(parent1->rt->rt) >= height_node(parent1->rt->lt))
					avlRotateLeft(tree, parent1);
				//right left
				else
				{
					avlRotateRight(tree, parent1->rt);
					avlRotateLeft(tree, parent1);
				}
			}
			else
			{
				// left left
				if(height_node(parent1->lt->lt) >= height_node(parent1->lt->rt))
					avlRotateRight(tree, parent1);
				//left right
				else
				{
					avlRotateLeft(tree, parent1->lt);
					avlRotateRight(tree, parent1);
				}
			}
			TreeNode *aux = parent1;
			//update all heights
			update_all_heights(aux);
		}
		parent1 = parent1->pt;
	}
}

void insert(TTree* tree, void* elem, void* info) {
	// TODO: Cerinta 1
	//case empty
	if(tree == NULL)
		return;
	if(isEmpty(tree)){
		tree->root = createTreeNode(tree, elem, info);
		tree->size ++;
		return;
	}
	TreeNode *node = createTreeNode(tree, elem, info);
	TreeNode *aux = tree->root, *parent2 = NULL, *parent1 = NULL;
	//while(1), but it has conditions to exit the loop
	//parent1 > parent2
	while(1)
	{
		//case insert right
		if(tree->compare(elem, aux->elem) > 0)
		{
			parent2 = aux;
			aux = aux->rt;
			if(aux == NULL)
			{
				add_to_list(tree, node, tree->root);
				tree->size ++;
				parent2->rt = node;
				node->pt = parent2;
				aux = parent2;
				update_all_heights(aux);
				equilibrate(tree, node);
				return;
			}
		}
		else
			//case insert left
			if(tree->compare(elem, aux->elem) < 0)
			{
				parent2 = aux;
				aux = aux->lt;
				if(aux == NULL)
				{
					add_to_list(tree, node, tree->root);
					tree->size ++;
					parent2->lt = node;
					node->pt = parent2;
					aux = parent2;
					update_all_heights(aux);
					equilibrate(tree, node);
					return;
				}
			}
			//case duplicate
			else
			{
				add_to_list(tree, node, tree->root);
				return;
			}	
	}
}

//puts the node "node" on the last level
void delete_right(TTree *tree, TreeNode *aux){
	aux->rt->pt = aux->pt;
	if(aux->pt != NULL)
		if(aux->pt->lt == aux)
			aux->pt->lt = aux->rt;
		else
			aux->pt->rt = aux->rt;
	else
		tree->root = aux->rt;
	TreeNode *aux2 = aux->pt;
	update_all_heights(aux2);
	destroyTreeNode(tree, aux);
}

void delete_left(TTree *tree, TreeNode *aux){
	aux->lt->pt = aux->pt;
	if(aux->pt != NULL)
		if(aux->pt->lt == aux)
			aux->pt->lt = aux->lt;
		else
			aux->pt->rt = aux->lt;
	else
		tree->root = aux->lt;
	TreeNode *aux2 = aux->pt;
	update_all_heights(aux2);
	destroyTreeNode(tree, aux);
}

void delete_no_child(TTree *tree, TreeNode *aux){
	if(aux->pt != NULL)
		if(aux->pt->lt == aux)
			aux->pt->lt = NULL;
		else
			aux->pt->rt = NULL;
	else
		tree->root = NULL;
	TreeNode *aux2 = aux->pt;
	update_all_heights(aux2);
	destroyTreeNode(tree, aux);
}

void change_nodes(TTree *tree, TreeNode *aux, TreeNode *successor_aux){
	TreeNode *pt = aux->pt, *lt = aux->lt, *rt = aux->rt;
	aux->lt->pt = successor_aux;
	aux->rt->pt = successor_aux;
	if(successor_aux->rt)
		successor_aux->rt->pt = aux;
	if(successor_aux->lt)
		successor_aux->lt->pt = aux;
	aux->lt = successor_aux->lt;
	aux->rt = successor_aux->rt;
	successor_aux->rt = rt;
	successor_aux->lt = lt;
	if(aux->pt != NULL)
	{
		if(aux->pt->lt == aux)
			aux->pt->lt = successor_aux;
		else
			aux->pt->rt = successor_aux;
	}
	else
		tree->root = successor_aux;
	if(successor_aux->pt != NULL)
	{
		if(successor_aux->pt->lt == successor_aux)
			successor_aux->pt->lt = aux;
		else
			successor_aux->pt->rt = aux;
	}
	else
		tree->root = successor_aux;
	aux->pt = successor_aux->pt;
	successor_aux->pt = pt;
}

void delete_from_list(TTree *tree, TreeNode *node){
	if(node->end == node)
	{
		if(node->next != NULL)
			node->next->prev = node->prev;
		if(node->prev != NULL)
			node->prev->next = node->next;
		return;
	}
	//case duplicates
	TreeNode *aux = node, *end = node->end;
	if(end->next != NULL)
		end->next->prev = end->prev;
	end->prev->next = end->next;
	while(tree->compare(aux->elem, end->elem) == 0)
	{
		aux->end = end->prev;
		aux = aux->next;
		if(aux == NULL)
			break;
	}
}

void delete(TTree* tree, void* elem){
	// TODO: Cerinta 1
	if(tree == NULL)
		return;
	if(isEmpty(tree))
		return;
	TreeNode *aux = tree->root;
	while(aux != NULL)
	{
		if(tree->compare(elem, aux->elem) > 0)
			aux = aux->rt;
		else
		if(tree->compare(elem, aux->elem) < 0)
			aux = aux->lt;
		else
		{ 
			//case no duplicate
			if(aux->end == aux)
			{
				tree->size --;
				//case no children
				if(aux->rt == NULL && aux->lt == NULL)
				{
					TreeNode *parent = aux->pt, *parent2 = aux->pt;
					delete_from_list(tree, aux);
					delete_no_child(tree, aux);
					update_all_heights(parent);
					parent = parent2;
					equilibrate(tree, parent);
					return;
				}
				//case left child only
				if(aux->rt == NULL)
				{
					TreeNode *parent = aux->pt, *parent2 = aux->pt;
					delete_from_list(tree, aux);
					delete_left(tree, aux);
					update_all_heights(parent);
					parent = parent2;
					equilibrate(tree, parent);
					return;
				}
				//case right child only
				if(aux->lt == NULL)
				{
					TreeNode *parent = aux->pt, *parent2 = aux->pt;
					delete_from_list(tree, aux);
					delete_right(tree, aux);
					update_all_heights(parent);
					parent = parent2;
					equilibrate(tree, parent);
					return;
				}
				//case both children
				if(aux->lt != NULL && aux->rt != NULL)
				{
					TreeNode *successor_aux = successor(aux);
					change_nodes(tree, aux, successor_aux);
					updateHeight(successor_aux);
					updateHeight(aux);
					TreeNode *parent = aux->pt, *parent2 = aux->pt;
					if(aux->rt == NULL)
					{
						delete_from_list(tree, aux);
						delete_no_child(tree, aux);
					}
					else
					{
						delete_from_list(tree, aux);
						delete_right(tree, aux);
					}
					update_all_heights(parent);
					parent = parent2;
					equilibrate(tree, parent);
					return;
				}
			}
			//case duplicate
			else
			{
				TreeNode *del = aux->end;
				delete_from_list(tree, aux);
				destroyTreeNode(tree, del);
				return;
			}
		}
	}
}

void destroy_all_nodes(TTree *tree, TreeNode *root){
	if(root->rt != NULL)
		destroy_all_nodes(tree, root->rt);
	if(root->lt != NULL)
		destroy_all_nodes(tree, root->lt);
	if(root == NULL)
		return;
	destroyTreeNode(tree, root);
}

void destroyTree(TTree* tree){
	// TODO: Cerinta 1
	destroy_all_nodes(tree, tree->root);
	free(tree);
}

#endif /* TREEMAP_H_ */