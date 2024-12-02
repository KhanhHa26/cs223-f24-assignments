/*----------------------------------------------
 * Author: Khanh Ha Nguyen
 * Date: 11/26/2024
 * Description: implement a binary search tree
 ---------------------------------------------*/

#include "tree.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

struct tree_node* find(const char* name, struct tree_node* root)
{
  if(root == NULL )
    return NULL;
  
  int compare = strcmp(name, root->data.name);

  if(compare < 0) {
    return(find(name, root->left));
  } else if (compare > 0) {
    return(find(name, root->right));
  } else {
    return root; 
  }
}

struct tree_node* insert(const char* name, struct tree_node* root)
{
  if(root == NULL) { 
    root = malloc ( sizeof (struct tree_node) );
    if(root == NULL ) {
      printf("Out of space!!!");
    } else {
      strcpy(root->data.name, name);
      root->left = root->right = NULL;
    }
  } else {
    int compare = strcmp(name, root->data.name);
    if(compare < 0) {
      root->left = insert(name, root->left);
    } else if (compare > 0) {
      root->right = insert(name, root->right);
    }
  }
  return root;
}

void clear(struct tree_node* root)
{
  if (root == NULL) {
    return;
  }
  clear(root->left);
  clear(root->right);
}

void printHelper(struct tree_node* root, int depth, char side) {
  if (root == NULL)
    return;

  for (int i = 0; i < depth; i++) {
    printf(" ");
  }

  if (side == ' ') {
    printf("%s\n", root->data.name);
  } else if (side == 'l') {
    printf("l: %s\n", root->data.name);
  } else {
    printf("r: %s\n", root->data.name);
  }
  printHelper(root->left, depth + 1, 'l');
  printHelper(root->right, depth + 1, 'r');
}

void print(struct tree_node* root)
{
  if (root == NULL)
    return;
  
  printHelper(root, 0, ' ');
}

void printSorted(struct tree_node* root)
{
  if (root == NULL)
    return;
  printSorted(root->left);
  printf("%s\n", root->data.name);
  printSorted(root->right);
}
