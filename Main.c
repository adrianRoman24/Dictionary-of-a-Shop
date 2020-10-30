#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TreeMap.h"

/* Lungimea maxima a unui buffer pentru citire */
#define BUFLEN 1024

/* Lungimea unui element din arbore de modele (fara terminator)*/
#define ELEMENT_TREE_LENGTH 3

/* Range of models*/
typedef struct Range{
	int *index; // Array of models in the range
	int size; // Number of array elements
	int capacity; // Array capacity
}Range;

void* createStrElement(void* str){
	char* elem = (char*)malloc(ELEMENT_TREE_LENGTH + 1);
	strncpy(elem, (char*)str, ELEMENT_TREE_LENGTH);
	elem[ELEMENT_TREE_LENGTH] = '\0';
	return elem;
}

void destroyStrElement(void* elem){
	free((char*)elem);
}


void* createPriceElement(void* price){
	long *p = (long*) malloc(sizeof(long));
	*p = *((long*) price);
	return p;
}

void destroyPriceElement(void* price){
	free(price);
}

void* createIndexInfo(void* index){
	int *idx = (int*) malloc(sizeof(int));
	*idx = *((int*) index);
	return idx;
}

void destroyIndexInfo(void* index){
	free(index);
}

int compareStr(void* str1, void* str2){
	if(strncmp((char*)str1,(char*)str2,ELEMENT_TREE_LENGTH) > 0)
		return 1;
	else if(strncmp((char*) str1,(char*) str2, ELEMENT_TREE_LENGTH) < 0)
		return -1;
	return 0;
}

int comparePrice(void* price1, void* price2){
	if(*((int*) price1) > *((int*) price2))
		return 1;
	else if(*((int*) price1) < *((int*) price2))
		return -1;
	return 0;
}

void printFile(char* fileName){
	printf("---------\n");
	if(fileName == NULL) return;
	FILE * file = fopen(fileName,"r");
	if (file == NULL)
		return;

	char *buf = (char*) malloc(BUFLEN+1);
	while(fgets(buf,BUFLEN,file) != NULL){
		buf[strlen(buf) - 1] = '\0';
		printf("%s",buf);
	}
	printf("\n");
	printf("---------\n");
	free(buf);
	fclose(file);
}

void inorderModelTreePrint(TreeNode* node){
	if(node != NULL){
		inorderModelTreePrint(node->lt);
		TreeNode* begin = node;
		TreeNode* end = node->end->next;
		while(begin != end){
			printf("%d:%s  ",*((int*)begin->info),(char*)begin->elem);
			begin = begin->next;
		}
		inorderModelTreePrint(node->rt);
	}
}

void inorderPriceTreePrint(TreeNode* node){
	if(node != NULL){
		inorderPriceTreePrint(node->lt);
		TreeNode* begin = node;
		TreeNode* end = node->end->next;
		while(begin != end){
			printf("%d:%ld  ",*((int*)begin->info),*((long*)begin->elem));
			begin = begin->next;
		}
		inorderPriceTreePrint(node->rt);
	}
}

void printRange(Range* range, char* fileName){
	if(fileName == NULL) return;
	FILE * file = fopen(fileName,"r");
	if (file == NULL) return;

	char *buf = (char*) malloc(BUFLEN+1);

	for(int i = 0; i < range->size;i++){
		fseek(file,range->index[i],SEEK_SET);
		if(fgets(buf,BUFLEN,file) != NULL){
			char* model = strtok(buf,",");
			long price = atol(strtok(NULL,","));
			printf("%s:%ld  ", model, price);
		}
	}
	printf("\n");
	free(buf);
	fclose(file);
}

//-------TODO: Cerinta 2: You can use auxilary functions where needed----------
void buildTreesFromFile(char* fileName, TTree* modelTree, TTree* priceTree){
	// TODO: Cerinta 2a
	FILE *in = fopen(fileName, "rt");
	long *price = (long *)malloc(sizeof(long));
	int *index_sum = (int *)malloc(sizeof(int));
	*index_sum = 0;
	int i;
	char *string = (char *)calloc(20, sizeof(char));
	for(i = 0; i < 40; i ++)
	{
		fscanf(in, "%s", string);
		insert(modelTree, modelTree->createElement(string), modelTree->createInfo((index_sum)));
		char *initial_string = string;
		while((*string) != ',')
			string ++;
		string ++;
		*price = atol(string);

		insert(priceTree, priceTree->createElement(price), modelTree->createInfo((index_sum)));
			
		*index_sum = *index_sum + strlen(initial_string) + 2;
		string = initial_string;
	}
	fclose(in);
	free(string);
	free(price);
	free(index_sum);
}

void add_keys_to_range(TreeNode *node, Range *range, char *q)
{
	if(node == NULL)
		return;
	if(strstr((char *)(node->elem), q) != NULL)
	{
		range->index[range->size] = *(int *)(node->info);
		range->size ++;
		if(range->size == range->capacity)
		{
			range->capacity = range->capacity * 2;
			range->index = realloc(range->index, range->capacity * sizeof(int));
			if(range->index == NULL)
				exit(-1);
		}
	}
	add_keys_to_range(node->next, range, q);
}
TreeNode *first_node(TTree *tree)
{
	TreeNode *aux = tree->root;
	if(aux == NULL)
		return NULL;
	while(aux->prev != NULL)
		aux = aux->prev;
	return aux;
}
Range* modelGroupQuery(TTree* tree, char* q){
	// TODO: Cerinta 2b
	Range *range = (Range *)malloc(sizeof(Range));
	range->index = (int *)malloc(sizeof(int));
	range->size = 0;
	range->capacity = 1;
	add_keys_to_range(first_node(tree), range, q);
	return range;
}
void add_price_to_range(TreeNode *node, Range *range, long p, long q)
{
	if(node == NULL)
		return;
	if( (p >= (*(long *)(node->elem)) ) && (q <= (*(long *)(node->elem)) ))
	{
		range->index[range->size] = *(int *)(node->info);
		range->size ++;
		if(range->size == range->capacity)
		{
			range->capacity = range->capacity * 2;
			range->index = realloc(range->index, range->capacity * sizeof(int));
			if(range->index == NULL)
				exit(-1);
		}
	}
	add_price_to_range(node->next, range, p, q);
}
Range* priceRangeQuery(TTree* tree, long q, long p){
	// TODO: Cerinta 2c
	Range *range = (Range *)malloc(sizeof(Range));
	range->index = (int *)malloc(sizeof(int));
	range->size = 0;
	range->capacity = 1;
	add_price_to_range(first_node(tree), range, p, q);
	return range;
}

void add_range2_to_range1(Range *range1, Range *range2)
{
	int i, j, found = 0, initial_size = range1->size;
	for(i = 0; i < range2->size; i ++)
	{
		found = 0;
		for(j = 0; j < initial_size; j ++)
			if(range1->index[j] == range2->index[i])
			{
				found = 1;
				break;
			}
		if(found == 0)
		{
			range1->index[range1->size] = range2->index[i];
			range1->size ++;
			if(range1->size == range1->capacity)
			{
				range1->capacity = range1->capacity * 2;
				range1->index = realloc(range1->index, range1->capacity * sizeof(int));
				if(range1->index == NULL)
					exit(-1);
			}
		}
	}
}

Range* modelRangeQuery(TTree* tree, char* q, char* p){
	// TODO: Cerinta 2d
	Range *range1 = modelGroupQuery(tree, q);
	Range *range2 = modelGroupQuery(tree, p);
	add_range2_to_range1(range1, range2);
	free(range2->index);
	free(range2);
	return range1;
}



long getModelPrice(FILE* file, int index){
	long price = -1;
	char buf[BUFLEN+1];
	fseek(file, index, SEEK_SET);
	if(fgets(buf,BUFLEN,file) != NULL){
		strtok(buf, ",\n");
		price = atol(strtok(NULL,",\n"));
	}
	return price;
}



Range* modelPriceRangeQuery(char* fileName, TTree* tree, char* m1, char* m2, long p1, long p2){
	// TODO: Cerinta 2e
	FILE *in = fopen(fileName, "rt");
	Range *range = modelRangeQuery(tree, m1, m2);
	//delete indexes that don't have price between p1 and p2
	int i, j;
	for(i = 0; i < range->size; i ++)
	{
		long price = getModelPrice(in, range->index[i]);
		if(price < p1 || price > p2)
		{
			for(j = i; j < range->size; j ++)
				range->index[j] = range->index[j + 1];
			range->size --;
			i --;
		}
	}
	fclose(in);
	return range;
}

void destroy(TTree **tree)
{
	destroyTree(*tree);
}
int main(void) {

	TTree* modelTree = createTree(createStrElement, destroyStrElement,
	 createIndexInfo, destroyIndexInfo, compareStr);
	if(modelTree == NULL) goto NullPointerException;

	TTree* priceTree = createTree(createPriceElement, destroyPriceElement, 
		createIndexInfo, destroyIndexInfo, comparePrice);
	if(priceTree == NULL) goto NullPointerException;

	buildTreesFromFile("input.csv", modelTree, priceTree);
	if(isEmpty(modelTree) || isEmpty(priceTree)) goto EmptyTreeException;

	printf("Model Tree In Order:\n");
	inorderModelTreePrint(modelTree->root);
	printf("\n\n");

	printf("Price Tree In Order:\n");
	inorderPriceTreePrint(priceTree->root);
	printf("\n\n");

	printf("Group Model Search:\n");
	Range *range = modelGroupQuery(modelTree,"MG3");
	if(range == NULL) goto NullPointerException;
	printRange(range,"input.csv");
	printf("\n\n");

	printf("Price Range Search:\n");
	Range *range2 = priceRangeQuery(priceTree,100,400);
	if(range2 == NULL) goto NullPointerException;
	printRange(range2,"input.csv");
	printf("\n\n");

	printf("Model Range Search:\n");
	Range *range3 = modelRangeQuery(modelTree,"L2","M");
	if(range3 == NULL) goto NullPointerException;
	printRange(range3,"input.csv");
	printf("\n\n");

	printf("Model Price Range Search:\n");
	Range *range4 = modelPriceRangeQuery("input.csv",modelTree,"L2","M", 300, 600);
	if(range4 == NULL) goto NullPointerException;
	printRange(range4,"input.csv");
	destroy(&modelTree);
	destroy(&priceTree);
	free(range->index);
	free(range);
	free(range2->index);
	free(range2);
	free(range3->index);
	free(range3);
	free(range4->index);
	free(range4);
	// TODO: DO NOT FORGET TO DE-ALLOCATE EVERYTHING
	
	return 0;

NullPointerException:
	printf("NULL pointer exception!\n");
	return -1;

EmptyTreeException:
	printf("Emtpty tree exception!\n");
	return -2;
}
