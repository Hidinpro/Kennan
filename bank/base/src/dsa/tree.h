#ifndef TREE_H
#define TREE_H
#include <stdio.h>

/*
疑难问题：
	如何把一棵二叉搜索树保存到文件中。
	如何把文件中存储的二叉搜索树还原。
*/

typedef struct TreeNode
{
	void* ptr;
	struct TreeNode* left;
	struct TreeNode* right;
}TreeNode;

typedef struct Tree
{
	TreeNode* root;
	size_t size;
	int (*nodecmp)(const void*,const void*);
}Tree;

// 创建结点
TreeNode* create_tree_node(void* ptr);

// 创建树
Tree* create_tree(int (*comper)(const void*,const void*));

// 初始化树
void init_tree(Tree* tree,void* arr[],size_t len);

// 销毁树
void destroy_tree(Tree* tree);

// 添加结点
void add_tree(Tree* tree,void* ptr);

// 删除结点
void* del_tree(Tree* tree,void* ptr);

// 查找
void* query_tree(Tree* tree,void* ptr);

// 遍历
void fput_tree(Tree* tree,void (*fput_ptr)(FILE*,const void*),FILE* fp);
#endif//TREE_H
