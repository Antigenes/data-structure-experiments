#define OVERFLOW -2
#include <iostream>
using namespace std;

typedef struct OLNode{
    int i,j;
    int e;
    OLNode *rigth,*down;
}OLNode,*OLink;//每个结点

typedef struct{
    OLink *rhead,*chead;
    int mu,nu,tu;//行数，列数，非零数的个数
}crossList;//行，列链表的头指针

void createMatrix(crossList &M);
void addMatrix(crossList &M,crossList &N);
void substractMatrix(crossList &M,crossList &N);

int main(){
    crossList M;
    crossList N;
    cout << "输入矩阵的行数，列数，非零元个数" << endl;
    createMatrix(M);
    cout << "输入矩阵的行数，列数，非零元个数" << endl;
    createMatrix(N);
    cout << "输入要执行的操作：" << endl;
    while(1){
        cout << endl;
        cout << "1相加,2相减,0退出程序" << endl;
        int f;
        cin >> f;
        if(f == 0)
            break;
        if(f == 1){
            addMatrix(M,N);
        }
        if(f == 2){
            substractMatrix(M,N);
        }
    }
    return 0;
}

void createMatrix(crossList &M){
    cin >> M.mu >> M.nu >> M.tu;
    M.rhead = (OLink *)malloc((M.mu + 1) * sizeof(OLink));
    M.chead = (OLink *)malloc((M.nu + 1) * sizeof(OLink));
    if(!M.rhead || !M.chead)
        exit(OVERFLOW);
    for(int i = 0;i <= M.mu;i++){
        M.rhead[i] = NULL;
    }
    for(int i = 0;i <= M.nu;i++){
        M.chead[i] = NULL;
    }//初始化清空每条链表
    int i = 1,j,e;
    cout << "输入0回车以退出输入" << endl;
    while(1){
        cin >> i;
        if(!i)
            break;
        cin >> j >> e;
        if((i <= M.mu) && (j <= M.nu)){
            OLink p = (OLink)malloc(sizeof(OLNode));
            if(!p)
                exit(OVERFLOW);
            p->i = i;
            p->j = j;
            p->e = e;//创建结点并填入数据
            if(!M.rhead[i] || M.rhead[i]->j > j){
                p->rigth = M.rhead[i];
                M.rhead[i] = p;
            }
            else{//寻找插入位置
                OLink q;
                for(q = M.rhead[i];(q->rigth) && (q->rigth->j < j);q = q->rigth);
                p->rigth = q->rigth;
                q->rigth = p;//行插入
            }
            if(!M.chead[j] || M.chead[j]->i > i){
                p->down = M.chead[j];
                M.chead[j] = p;
            }
            else{
                OLink q;
                for(q = M.chead[j];(q->down) && (q->down->i < i);q = q->down);
                p->down = q->down;
                q->down = p;//列插入
            }
        }
    }
    return;
}   
void addMatrix(crossList &M,crossList &N){
    crossList O;
    O.mu = M.mu;
    O.nu = M.nu;
    O.tu = M.tu;
    O.rhead = (OLink *)malloc((O.mu + 1) * sizeof(OLink));
    O.chead = (OLink *)malloc((O.nu + 1) * sizeof(OLink));
    if(!O.rhead || !O.chead)
        exit(OVERFLOW);
    for(int i = 0;i <= O.mu;i++){
        O.rhead[i] = NULL;
    }
    for(int i = 0;i <= O.nu;i++){
        O.chead[i] = NULL;
    }//初始化清空每条链表
    for(int i = 1;i <= M.mu;i++){
        OLink p = M.rhead[i];
        if(p != NULL){
            OLink insertPTR = (OLink)malloc(sizeof(OLNode));
            insertPTR->i = p->i;
            insertPTR->j = p->j;
            insertPTR->e = p->e;//复制结点数据
            if(!O.rhead[i] || O.rhead[i]->j > p->j){
                insertPTR->rigth = O.rhead[i];
                O.rhead[i] = insertPTR;
            }
            else{//寻找插入位置
                OLink q;
                for(q = O.rhead[i];(q->rigth) && (q->rigth->j < p->j);q = q->rigth);
                insertPTR->rigth = q->rigth;
                q->rigth = insertPTR;
            }//行插入
            int j = insertPTR->j;
            if(!O.chead[j] || O.chead[j]->i > p->i){
                insertPTR->down = O.chead[j];
                O.chead[j] = insertPTR;
            }
            else{//寻找插入位置
                OLink q;
                for(q = O.chead[j];(q->down) && (q->down->i < p->i);q = q->down);
                insertPTR->down = q->down;
                q->down = insertPTR;
            }//列插入
            p = p->rigth;
        }
        else
            break;
    }//复制整个M

    for(int i = 1;i <= O.mu;i++){
        OLink OR = O.rhead[i];
        OLink NR = N.rhead[i];
        while(NR != NULL){       
            if(OR == NULL || (OR->j) != (NR->j)){
                int j = NR->j;
                OLink insertPTR = (OLink)malloc(sizeof(OLNode));
                insertPTR->i = NR->i;
                insertPTR->j = NR->j;
                insertPTR->e = NR->e;//复制结点数据 
                OLink q;
                if(!O.rhead[i] || O.rhead[i]->j > j){
                    insertPTR->rigth = O.rhead[i];
                    O.rhead[i] = insertPTR;
                }
                else{  
                    for(q = O.rhead[i];(q->rigth) && (q->rigth->j < j);q = q->rigth);
                    insertPTR->rigth = q->rigth;
                    q->rigth = insertPTR;
                }//行插入
                if(!O.chead[j] || O.chead[j]->i > i){
                    insertPTR->down = O.chead[j];
                    O.chead[j] = insertPTR;
                }
                else{
                    for(q = O.chead[j];(q->down) && (q->down->i < i);q = q->down);
                    insertPTR->down = q->down;
                    q->down = insertPTR;
                }//列插入
                O.tu++;
            }//不同的插入
            else{
                OR->e += NR->e;
            }//相同位置的直接加
            NR = NR->rigth;
        }
    }

    //测试模块
    /*
    cout << "m:" << O.mu <<endl;
    cout << "n:" << O.nu <<endl;
    cout << "t:" << O.tu <<endl;
    for(int i = 1;i <= O.mu;i++){
        OLink p = O.rhead[i];
        while(p){
            cout << p->i << " " << p->j << " " << p->e << " " << endl;
            p = p->rigth;
        }
    }
    cout << endl;
    */

    //输出模块
    for(int i = 1;i <= M.mu;i++){
        if(i > 1)
            cout << endl;
        OLink p = O.rhead[i];
        for(int j = 1;j <= M.nu;j++){
            if(p != NULL && j == p->j){
                cout << p->e << " ";
                p = p->rigth;
            }
            else{
                cout << 0 << " ";
            }
        }
    }//打印
}
void substractMatrix(crossList &M,crossList &N){
    crossList O;
    O.mu = M.mu;
    O.nu = M.nu;
    O.tu = M.tu;
    O.rhead = (OLink *)malloc((O.mu + 1) * sizeof(OLink));
    O.chead = (OLink *)malloc((O.nu + 1) * sizeof(OLink));
    if(!O.rhead || !O.chead)
        exit(OVERFLOW);
    for(int i = 0;i <= O.mu;i++){
        O.rhead[i] = NULL;
    }
    for(int i = 0;i <= O.nu;i++){
        O.chead[i] = NULL;
    }//初始化清空每条链表
    for(int i = 1;i <= M.mu;i++){
        OLink p = M.rhead[i];
        if(p != NULL){
            OLink insertPTR = (OLink)malloc(sizeof(OLNode));
            insertPTR->i = p->i;
            insertPTR->j = p->j;
            insertPTR->e = p->e;//复制结点数据
            if(!O.rhead[i] || O.rhead[i]->j > p->j){
                insertPTR->rigth = O.rhead[i];
                O.rhead[i] = insertPTR;
            }
            else{//寻找插入位置
                OLink q;
                for(q = O.rhead[i];(q->rigth) && (q->rigth->j < p->j);q = q->rigth);
                insertPTR->rigth = q->rigth;
                q->rigth = insertPTR;
            }//行插入
            int j = insertPTR->j;
            if(!O.chead[j] || O.chead[j]->i > p->i){
                insertPTR->down = O.chead[j];
                O.chead[j] = insertPTR;
            }
            else{//寻找插入位置
                OLink q;
                for(q = O.chead[j];(q->down) && (q->down->i < p->i);q = q->down);
                insertPTR->down = q->down;
                q->down = insertPTR;
            }//列插入
            p = p->rigth;
        }
        else
            break;
    }//复制整个M

    for(int i = 1;i <= O.mu;i++){
        OLink OR = O.rhead[i];
        OLink NR = N.rhead[i];
        while(NR != NULL){       
            if(OR == NULL || (OR->j) != (NR->j)){
                int j = NR->j;
                OLink insertPTR = (OLink)malloc(sizeof(OLNode));
                insertPTR->i = NR->i;
                insertPTR->j = NR->j;
                insertPTR->e = -NR->e;//复制结点数据 
                OLink q;
                if(!O.rhead[i] || O.rhead[i]->j > j){
                    insertPTR->rigth = O.rhead[i];
                    O.rhead[i] = insertPTR;
                }
                else{  
                    for(q = O.rhead[i];(q->rigth) && (q->rigth->j < j);q = q->rigth);
                    insertPTR->rigth = q->rigth;
                    q->rigth = insertPTR;
                }//行插入
                if(!O.chead[j] || O.chead[j]->i > i){
                    insertPTR->down = O.chead[j];
                    O.chead[j] = insertPTR;
                }
                else{
                    for(q = O.chead[j];(q->down) && (q->down->i < i);q = q->down);
                    insertPTR->down = q->down;
                    q->down = insertPTR;
                }//列插入
                O.tu++;
            }//不同的插入
            else{
                OR->e -= NR->e;
            }//相同位置的直接减


            
            NR = NR->rigth;
        }
    }

    //测试模块
    /*
    cout << "m:" << O.mu <<endl;
    cout << "n:" << O.nu <<endl;
    cout << "t:" << O.tu <<endl;
    for(int i = 1;i <= O.mu;i++){
        OLink p = O.rhead[i];
        while(p){
            cout << p->i << " " << p->j << " " << p->e << " " << endl;
            p = p->rigth;
        }
    }
    cout << endl;
    */

    //输出模块
    for(int i = 1;i <= M.mu;i++){
        if(i > 1)
            cout << endl;
        OLink p = O.rhead[i];
        for(int j = 1;j <= M.nu;j++){
            if(p != NULL && j == p->j){
                cout << p->e << " ";
                p = p->rigth;
            }
            else{
                cout << 0 << " ";
            }
        }
    }//打印
}
