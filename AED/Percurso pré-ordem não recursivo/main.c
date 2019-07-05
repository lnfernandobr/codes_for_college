#include<stdio.h>
#include<stdlib.h>
#define bool int

/* A binary tree tNode has data, pointer to left child 
   and a pointer to right child */
struct tNode {
    int data;
    struct tNode* left;
    struct tNode* right;
};



/* Helper function that allocates a new tNode with the 
   given data and NULL left and right pointers. */
struct tNode* newtNode(int data) {
    struct tNode* tNode = (struct tNode*) malloc(sizeof(struct tNode));
    tNode->data = data;
    tNode->left = NULL;
    tNode->right = NULL;

    return(tNode);
}


int qtd_no_subarvore(struct tNode *pt) {
    int x = 0, y = 0;

    if(pt->left == NULL && pt->right == NULL) {
        printf("Tamanho da subarvore %d: 1\n", pt->data);
        return 1;
    }

    else {
        if(pt->left != NULL)
            x = qtd_no_subarvore(pt->left);

        if(pt->right != NULL)
            y = qtd_no_subarvore(pt->right);

        printf("O numero de nó da subarvore %d é: %d\n", pt->data,  x + y + 1);

        return (x + y + 1);
    }

}

/* Driver program to test above functions*/
int main() {

    struct tNode *root        = newtNode(1);
    root->left                = newtNode(2);
    root->right               = newtNode(3);

    root->right->right        = newtNode(6);

    root->left->left          = newtNode(4);

    root->left->right         = newtNode(5);

    root->left->right->left   = newtNode(7);

    root->left->right->right  = newtNode(8);

    int res = qtd_no_subarvore(root);
    printf("Número total de nós: %d\n", res);

    getchar();

    return 0;
} 