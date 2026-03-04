#include <iostream>
#include <cmath>
#include <cstdlib>
#include <algorithm>
using namespace std;
typedef struct term{
    double factor;
    int degree;
    term* next;
}term,* termptr;
void createList(termptr *L);
void clearList(termptr *L);
void printlist(termptr L,int flag);
void checkzero(termptr *L);
bool compare(int a,int b){
    return a>b;
}
int main(){
    int flag=1;
    //输出提示
    cout << "请选择多项式输出模式(输入序号并回车):" <<endl;
    cout << "1.降次序列" << endl;
    cout << "2.类数学表达式" <<endl;
    //选择输出格式
    cin >> flag;
    cin.get();

    //创建两个单链表来降次存储初始的多项式
    termptr La=NULL;
    createList(&La);
    checkzero(&La);
    cout << "a"; 
    printlist(La,flag); 

    termptr Lb=NULL;
    createList(&Lb);
    checkzero(&Lb);
    cout << "b"; 
    printlist(Lb,flag);

    //选择进行的运算
    while(1){
        //提示模块
        cout << endl;
        cout << "进行的运算：" << endl;
        cout << "1.将x带值计算多项式a" << endl;
        cout << "2.对多项式a求导" << endl;
        cout << "3.计算多项式a+多项式b" << endl;
        cout << "4.计算多项式a-多项式b" << endl;
        cout << "5.计算多项式a*多项式b" << endl;
        cout << "输入0退出程序" << endl;

        int choice;
        cin >> choice;
        cin.get();
        if(choice==0){
            //清空链表
            clearList(&La);
            clearList(&Lb);
            break;
        }
            
        //下面进行代值
        if(choice==1){
            cout << "输入x的取值:\n";
            double x;
            cin >> x;
            cin.get();
            double sum=0;
            termptr p=La->next;
            while(p!=NULL){
                sum+=p->factor*pow(x,p->degree);
                p=p->next;
            }
            cout << "代值x=" << x << "得到多项式a的值为" << sum <<endl;
        }

        //下面进行求导
        if(choice==2){
            termptr p=La->next;
            termptr La_=(termptr)malloc(sizeof (term));
            La_->degree=La->degree;
            La_->next=NULL;
            termptr q=La_;
            while(p!=NULL){
                termptr node=(termptr)malloc(sizeof (term));
                node->next=NULL;
                if(p->degree!=0)
                    node->degree=p->degree-1;
                else
                    node->degree=0;
                node->factor=p->factor*p->degree;
                q->next=node;
                q=node;
                p=p->next;
            }
            checkzero(&La_);
            //打印导函数多项式
            printlist(La_,flag);
            //清空链表
            clearList(&La_);
        }
       
        //下面实现加法
        if (choice==3){
            termptr p = La->next;
            termptr q=Lb->next;
            int count=La->degree+Lb->degree;
            termptr add=(termptr)malloc(sizeof (term));
            termptr rear=add;
            add->next=NULL;
            add->degree=count;
            termptr h=add;
            while(p!=NULL && q!=NULL){
                if(p->degree > q->degree){
                    termptr node=(termptr)malloc(sizeof (term));
                    node->next=NULL;
                    node->degree=p->degree;
                    node->factor=p->factor;
                    h->next=node;
                    h=h->next;
                    p=p->next;
                }
                else if(p->degree < q->degree){
                    termptr node=(termptr)malloc(sizeof (term));
                    node->next=NULL;
                    node->degree=q->degree;
                    node->factor=q->factor;
                    h->next=node;
                    h=h->next;
                    q=q->next;
                }
                else if(p->degree == q->degree){
                    termptr node=(termptr)malloc(sizeof (term));
                    node->next=NULL;
                    node->degree=p->degree; 
                    node->factor=p->factor+q->factor;
                    h->next=node;
                    h=h->next;
                    p=p->next;
                    q=q->next;
                    add->degree--;
                }
                rear=h;
            }
            while(p!=NULL){
                termptr node=(termptr)malloc(sizeof(term));
                node->next=NULL;
                rear->next=node;
                node->degree=p->degree;
                node->factor=p->factor;
                rear=node;
                p=p->next;
            }
            while(q!=NULL){
                termptr node=(termptr)malloc(sizeof(term));
                node->next=NULL;
                rear->next=node;
                node->degree=q->degree;
                node->factor=q->factor;
                rear=node;
                q=q->next;
            }
            checkzero(&add);
            //输出add
            printlist(add,flag);
            //释放add
            clearList(&add);
        }

        //下面实现减法
        if(choice==4){
            termptr p=La->next;
            termptr q=Lb->next;
            int count=La->degree+Lb->degree;
            termptr reduce=(termptr)malloc(sizeof (term));
            reduce->next=NULL;
            reduce->degree=count;
            termptr h=reduce;
            termptr rear=h;
            while(p!=NULL && q!=NULL){
                if(p->degree > q->degree){
                    termptr node=(termptr)malloc(sizeof (term));
                    node->next=NULL;
                    node->degree=p->degree;
                    node->factor=p->factor;
                    h->next=node;
                    h=h->next;
                    p=p->next;
                }
                else if(p->degree < q->degree){
                    termptr node=(termptr)malloc(sizeof (term));
                    node->next=NULL;
                    node->degree=q->degree;
                    node->factor=-q->factor;
                    h->next=node;
                    h=h->next;
                    q=q->next;
                }
                else if(p->degree == q->degree){
                    termptr node=(termptr)malloc(sizeof (term));
                    node->next=NULL;
                    node->degree=p->degree; 
                    node->factor=p->factor-q->factor;
                    h->next=node;
                    h=h->next;
                    p=p->next;
                    q=q->next;
                    reduce->degree--;
                }
                rear=h;
            }
            while(p!=NULL){
                termptr node=(termptr)malloc(sizeof(term));
                node->next=NULL;
                rear->next=node;
                node->degree=p->degree;
                node->factor=p->factor;
                rear=node;
                p=p->next;
            }
            while(q!=NULL){
                termptr node=(termptr)malloc(sizeof(term));
                node->next=NULL;
                rear->next=node;
                node->degree=q->degree;
                node->factor=-q->factor;
                rear=node;
                q=q->next;
            }
            checkzero(&reduce);
            //输出reduce
            printlist(reduce,flag);
            //释放reduce
            clearList(&reduce);
        }
        
        //下面实现乘法
        if(choice==5){
            termptr p=La->next;
            termptr q=Lb->next;
            termptr Lc=(termptr)malloc(sizeof(term));
            termptr r=NULL;
            termptr s=NULL;
            Lc->next=NULL;
            Lc->degree=0;
            int m=La->degree,n=Lb->degree;
            for(int i=0;i<m;i++){
                for(int j=0;j<n;j++){
                    s=Lc;
                    r=s->next;
                    int deg=p->degree+q->degree;
                    if(r!=NULL){
                        while(r!=NULL && r->degree > deg){
                            r=r->next;
                            s=s->next;
                        }
                        if(r!=NULL && r->degree == deg){
                            r->factor+=p->factor*q->factor;
                            q=q->next;
                            continue;
                        }
                    }
                    termptr node=(termptr)malloc(sizeof(term));
                    Lc->degree++;
                    s->next=node;
                    node->next=r;
                    node->degree=deg;
                    node->factor=p->factor*q->factor;
                    q=q->next;
                }
                q=Lb->next;
                p=p->next;
            }
            //清除零项
            checkzero(&Lc);
            //输出链表
            printlist(Lc,flag);
            //清空链表
            clearList(&Lc);    
        }
    }
    return 0;
}
void clearList(termptr *L){
    if(*L==NULL)
        return;
    termptr p=NULL;
    while(*L!=NULL){
        p=*L;
        *L=(*L)->next;
        free(p);
    }
}
void printlist(termptr L,int flag){
    if(flag==1){
        cout << "按首数字为项数,后面每两个数字分别表示系数和次数并降次排列\n";
        cout << "序列为:";
        termptr pointer=L->next;
        cout << L->degree;
        while(pointer!=NULL){
            cout << "," << pointer->factor << "," << pointer->degree;
            pointer=pointer->next;
        }
        cout << endl;
    }
    if(flag==2){
        cout << "按照类数学表达式输出为:";
        if(L->next==NULL){
            cout << 0 << endl;
            return;
        }
        termptr p=L->next;
        if(p->degree!=0){
            if(p->factor!=1 && p->factor!=-1)
                cout << p->factor;
            if(p->factor==-1)
                cout << "-"; 
            cout << "x";//单独输出首项因为首项正系数不要正号
            if(p->degree!=1)
                cout <<"^ " << p->degree;
            p=p->next;
        }      
        else{
            cout << p->factor;
            p=p->next;
        }
        while(p!=NULL){
            if(p->factor>0)
                cout << "+";
            if(p->factor!=1 && p->factor!=-1)
                cout << p->factor;
            if(p->factor==-1)
                cout << "-";  
            if(p->factor==1 && p->degree==0)
                cout << 1; 
            if(p->degree!=0)
                cout << "x";
            if(p->degree!=1 && p->degree!=0)
                cout << "^ " << p->degree;
            p=p->next;
        }
        cout << endl;
    }
}
void checkzero(termptr *L){
    termptr p=*L;
    termptr q=p->next;
    while(q!=NULL){
        if(q->factor==0){
            p->next=q->next;
            free(q);
            q=p->next;
            (*L)->degree--;
        }
        else{
            p=q;
            q=q->next;
        }
    }
}//检查0系数项
void createList(termptr *L){
    cout << "请按照[系数]<space>[指数]的方式输入多项式,回车以终止输入。\n";
    cout << "注意,重复次数的项会合并。\n";
    (*L)=(termptr)malloc(sizeof (term));
    (*L)->next=NULL;
    (*L)->degree=0;
    while(1){
        termptr p=(termptr)malloc(sizeof(term));
        cin >> p->factor;
        cin >> p->degree;
        termptr cur=(*L)->next;
        termptr pre=*L;
        while(cur!=NULL && cur->degree > p->degree){
            pre=cur;
            cur=cur->next;
        }
        if(cur!=NULL && cur->degree == p->degree){
            cur->factor+=p->factor;
            free(p);
        }//合并输入时的同类项
        else{
            pre->next=p;
            p->next=cur;
            (*L)->degree++;
        }
        if(cin.get()=='\n'){
            break;//当输入最后一个数字后回车，回车进入缓冲区并被cin.get()读取以终止循环
            cout << endl;
        }
    }
}