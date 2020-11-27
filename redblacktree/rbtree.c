#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

/**
 * Red-Black TREE - 자료구조 교재 10장의 내용 참고
 */
#define RED 0
#define BLACK 1
//Red-Black tree를 구성하기 위해 사용하는 node 구조체
typedef struct node* nodePointer;
typedef struct node{
    int data;
    int color;
    nodePointer left, right, parent;
}NODE;
//null 대신 nil이라는 구조체 배열을 놔두어서 트리가 변하는 것에 대응한다.
nodePointer nil;
nodePointer root;
void LR(nodePointer x);
void RR(nodePointer x);
void insertToBST(int data);
void makeUpRBT(nodePointer x);
void delete(nodePointer z);
void fixRBT(nodePointer x);
void tree_transplant(nodePointer u,nodePointer v);
nodePointer search(int data);
nodePointer find_minimum(nodePointer x){
    while(x->left != nil) x = x->left;
    return x;
}
void deallocate(nodePointer n){
    if( n == nil) return ;
    deallocate(n->left);
    deallocate(n->right);

    free(n);
}
int main(int argc, char* argv[]){
    clock_t start_time = clock();
    FILE *fp = NULL, *fp1 = NULL, *sizeFp;//fp : input file pointer, fp1 = output file pointer,sizeFp : count the number of operand
    int data = 0;
    nodePointer temp;
    nil = malloc(sizeof(NODE));
    nil->color = BLACK;
    root = nil;
    char op;
    int count = 0;//파일 안의 명령어의 갯수를 읽어옴.
    char buf[256];
    fp = fopen(argv[1],"r");
    if(fp == NULL){
        printf("File open error!\n");
        exit(-1);
    }
    fp1 = fopen("db_result.txt","w");
    while(!(EOF == fscanf(fp,"%c %d\n",&op,&data))){
        switch(op){
            case 'i' : 
                insertToBST(data);
                break;
            case 'd' :
                search(data) != nil ? delete(search(data)) : fprintf(fp1,"Ignore\n");
                break;
            case 's' :
                fprintf(fp1,(search(data) == nil) ? "False\n" : "True\n");
                break;
            default:
                printf("You can only input next 3 operation : i, d, s\n");
        }
    }
    deallocate(root);
    free(nil);
    fclose(fp);
    fclose(fp1);
    printf("%lf\n",(double)(clock()-start_time)/CLOCKS_PER_SEC);

    return 0;
}
void insertToBST(int data){

    nodePointer z = (nodePointer)malloc(sizeof(NODE));
    nodePointer x,y;
    z->data = data;
    z->color = RED;
    z->left = nil;
    z->right = nil;
    
    x = root;
    y = nil;

    while(x != nil){
        y = x;
        if(z->data < x->data) x = x->left;
        else x = x->right;
    }
    if( y == nil) root = z;
    else if(z-> data < y->data) y->left = z;
    else y->right = z;

    z->parent = y;
    makeUpRBT(z);
}
void makeUpRBT(nodePointer x){
    while((x->parent)->color == RED){
        if(x->parent == (x->parent->parent)->left){
            if(x->parent->parent->right->color == RED){
                x->parent->color = BLACK;
                x->parent->parent->right->color = BLACK;
                x->parent->parent->color = RED;
                x = x->parent->parent;
            }
            else{
                if(x == (x->parent)->right){
                    x = x->parent;
                    LR(x);
                }

                (x->parent)->color = BLACK;
                (x->parent->parent)->color = RED;

                RR((x->parent->parent));
            }
        }
        else{
            if(x->parent->parent->left->color == RED){
                (x->parent)->color = BLACK;
                x->parent->parent->left->color = BLACK;
                (x->parent->parent)->color = RED;
                x = (x->parent->parent);
            }
            else{
                if(x == (x->parent)->left){
                    x = x->parent;
                    RR(x);
                }

                (x->parent)->color = BLACK;
                (x->parent->parent)->color = RED;

                LR((x->parent->parent));
            }
        }
    }
    (root)->color = BLACK;
}
void delete(nodePointer target){
    nodePointer x,y;
    char targetColor;
    targetColor = target->color;
    if(target->left == nil){
        x = target->right;
        tree_transplant(target,target->right);
    }
    else if(target->right == nil){
        x = target->left;
        tree_transplant(target,target->left);
    }
    else{
        y = find_minimum(target->right);

        targetColor = y->color;
        x = y->right;
        if(y->parent == target) x->parent = y;
        else{
            tree_transplant(y,y->right);
            y->right = target->right;
            (y->right)->parent = y;
        }

        tree_transplant(target,y);
        y->left = target->left;
        (y->left)->parent = y;
        y->color = target->color;
    }
    if(targetColor == BLACK) fixRBT(x);
    free(target);
}
void fixRBT(nodePointer x){
    nodePointer s = NULL;
    while((x != (root)) && (x->color == BLACK)){
        if(x == (x->parent)->left){
            s = (x->parent)->right;
            if(s->color == RED){
                s->color = BLACK;
                (x->parent)->color = RED;
                LR(x->parent);
                s = (x->parent)->right;
            }
            //case2
            if((s->left)->color == BLACK && (s->right)->color == BLACK){
                s->color = RED;
                x = x->parent;
            }
            //case3
            else{
                if((s->right)->color == BLACK){
                    s->color = RED;
                    s->left->color = BLACK;
                    RR(s);
                    s = (x->parent)->right;
                }
            //case4
                s->color = (x->parent)->color;
                x->parent->color = BLACK;
                (s->right)->color = BLACK;
                //(x->parent)->color = BLACK;
                LR(x->parent);
                x = root;
            }
        }
        else{
            s = (x->parent)->left;
            if(s->color == RED){
                s->color = BLACK;
                (x->parent)->color = RED;
                RR(x->parent);
                s = (x->parent)->left;
            }
            //case2
            if((s->left)->color == BLACK && (s->right)->color == BLACK){
                s->color = RED;
                x = x->parent;
            }
            //case3
            else{
                if((s->left)->color == BLACK){
                    s->color = RED;
                    (s->right)->color = BLACK;
                    LR(s);
                    s = (x->parent)->left;
                }
            //case4
                s->color = (x->parent)->color;
                x->parent->color = BLACK;
                (s->left)->color = BLACK;
                //(x->parent)->color = BLACK;
                RR(x->parent);

                x = root;
            }
        }
    }
    x->color = BLACK;
}
nodePointer search(int data){
    nodePointer target;
    target = root;
    while(target != nil && target->data != data){
        if(target->data > data) target = target->left;
        else target = target->right;
    }
    return target;
}
void LR(nodePointer x){
    if(!x || !x->right) return ;
    nodePointer y = x->right;

    x->right = y->left;

    if(y->left != nil) y->left->parent = x;

    y->parent = x->parent;
    if(y->parent == nil) (root) = y;
    else if(x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;

    y->left = x;
    x->parent = y;
}
void RR(nodePointer x){
    if(!x || !x->left) return ;

    nodePointer y = x->left;
    x->left = y->right;

    if(y->right != nil) y->right->parent = x;

    y->parent = x->parent;

    if(y->parent == nil) (root) = y;
    else if(x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;

    y->right = x;
    x->parent = y;
}
void tree_transplant(nodePointer u, nodePointer v){
    if(u->parent == nil) root = v;
    else if(u == u->parent->left) u->parent->left = v;
    else u->parent->right = v;

    v->parent = u->parent;
}
