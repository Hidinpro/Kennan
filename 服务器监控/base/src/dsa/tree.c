#include <stdlib.h>
#include <string.h>
#include "tree.h"

// 创建结点
TreeNode* create_tree_node(void* ptr)
{
	TreeNode* node = malloc(sizeof(TreeNode));
	node->ptr = ptr;
	node->left = NULL;
	node->right = NULL;
	return node;
}

// 创建树
Tree* create_tree(int (*comper)(const void*,const void*))
{
	Tree* tree = malloc(sizeof(Tree));
	tree->nodecmp = comper;
	tree->root = NULL;
	tree->size = 0;
	return tree;
}

// 初始化树，前序结果初始化的
TreeNode* _init_tree(void* arr[],int* pos)
{
	if(NULL == arr[*pos])
		return NULL;

	TreeNode* root = create_tree_node(arr[*pos]);
	
	*pos += 1;
	root->left = _init_tree(arr,pos);

	*pos += 1;
	root->right = _init_tree(arr,pos);

	return root;
}

void init_tree(Tree* tree,void* arr[],size_t len)
{
	int pos = 0;
	tree->root = _init_tree(arr,&pos);

	for(int i=0; i<len; i++)
	{
		if(NULL != arr[i])
			tree->size++;
	}
}

// 销毁树
void _destroy_tree(TreeNode* root)
{
	if(NULL == root)
		return;

	_destroy_tree(root->left);
	_destroy_tree(root->right);
	free(root);
}

void destroy_tree(Tree* tree)
{
	_destroy_tree(tree->root);
	free(tree);
}

// 添加结点
void _add_tree(TreeNode** root,TreeNode* node,int (*nodecmp)(const void*,const void*))
{
	if(NULL == *root)
	{
		*root = node;
		return;
	}

	if(1 == nodecmp((*root)->ptr,node->ptr))
		_add_tree(&(*root)->left,node,nodecmp);
	else
		_add_tree(&(*root)->right,node,nodecmp);
}

void add_tree(Tree* tree,void* ptr)
{
	_add_tree(&tree->root,create_tree_node(ptr),tree->nodecmp);
}

// 删除结点
TreeNode* _del_tree(TreeNode** root,void* ptr,int (*nodecmp)(const void*,const void*))
{
	if(NULL == *root)
		return NULL;

	int result = nodecmp((*root)->ptr,ptr);
	if(0 == result)
	{
		TreeNode* temp = *root;
		*root = temp->right;
		_add_tree(root,temp->left,nodecmp);
		return temp;
	}

	if(1 == result)
		return _del_tree(&(*root)->left,ptr,nodecmp);
	else
		return _del_tree(&(*root)->right,ptr,nodecmp);
}

void* del_tree(Tree* tree,void* ptr)
{
	TreeNode* node = _del_tree(&tree->root,ptr,tree->nodecmp);
	if(NULL == node)
		return NULL;

	void* tmp = node->ptr;
	free(node);
	return tmp;
}

// 查找
TreeNode* _query_tree(TreeNode* root,void* ptr,int (*nodecmp)(const void*,const void*))
{
	if(NULL == root)
		return NULL;

	int result = nodecmp(root->ptr,ptr);
	if(0 == result)
		return root;
	
	if(1 == result)
		return _query_tree(root->left,ptr,nodecmp);
	else
		return _query_tree(root->right,ptr,nodecmp);
}

void* query_tree(Tree* tree,void* ptr)
{
	TreeNode* node = _query_tree(tree->root,ptr,tree->nodecmp);
	if(NULL == node)
		return NULL;
	return node->ptr;
}

// 前序遍历
void _fput_tree(TreeNode* root,void (*fput_ptr)(FILE*,const void*),FILE* fp)
{
	if(NULL == root)
	{
		fput_ptr(fp,NULL);
		return;
	}

	fput_ptr(fp,root->ptr);
	_fput_tree(root->left,fput_ptr,fp);
	_fput_tree(root->right,fput_ptr,fp);
}

void fput_tree(Tree* tree,void (*fput_ptr)(FILE*,const void*),FILE* fp)
{
	_fput_tree(tree->root,fput_ptr,fp);
}
