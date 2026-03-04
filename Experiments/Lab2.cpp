#define arrive 1
#define depart 2
#include <iostream>


typedef struct{
    int arriveTime;
    int type;
    int money;
    int duration; 
}client;//客户
typedef struct{
    int occurTime;
    int type;
    client *cust;
}event;//事件

typedef struct linkList{
    event data;
    linkList *next;
}*eventList;//建立链表eventList用于存放事件,带头结点
struct Node{
    client data;
    Node *next;
};//队列节点
typedef struct{
    Node *front;
    Node *rear;
}Queue;//队列

Queue q[2];//两个银行窗口队列
eventList ev;//事件表头结点
event en;//事件
int total = 10000;//定义银行总额
int closeTime = 600;//定义关门时间
int customerNum = 0,totalTime = 0;//累计客户数与逗留时间

void eventInsert(eventList&,event&);//插入事件表
void checkqueue(int&,int);//检查队列2,返回处理时间
void closeBank();
int main(){
    //初始化
    int winfree = 1;//窗口1是否空闲
    ev = (eventList)malloc(sizeof(linkList));
    ev->next = NULL;//初始化置空事件链表
    q[0].front = NULL;
    q[0].rear = NULL;
    q[1].front = NULL;
    q[1].rear = NULL;//初始化队列
    eventInsert(ev,en);//将第一个事件插入事件表

    std::cout << "输入所有客户到达的时间，存款或取款，金额数，交易时间\n";
    std::cout << "输入EOF以结束输入\n";
    int arriveTime;
    int type;
    int money;
    int duration;
    while(std::cin >> arriveTime >> type >> money >> duration){
        if(arriveTime >= closeTime)
            continue;
        client* tmp = (client*)malloc(sizeof(client));
        tmp->arriveTime = arriveTime;
        tmp->type = type;
        tmp->money = money;
        tmp->duration = duration;
        event temp;
        temp.cust = tmp;
        temp.type = arrive;
        temp.occurTime = tmp->arriveTime;
        eventInsert(ev,temp);//将关门前进入银行的事件插入ev事件表
    }

    //进入每次循环
    while(ev->next != NULL){
        en = ev->next->data;
        eventList p = (ev->next)->next;
        ev->next = p;//事件从事件表出队列

        if(en.type == depart){
            int currentTime = en.occurTime;
            //存钱type0
            if(en.cust->type == 0){
                int totalTemp = total;//保存存钱之前的总额
                total += en.cust->money;
                totalTime += (currentTime - en.cust->arriveTime);
                Node *p = q[0].front;
                q[0].front = q[0].front->next;
                free(p);
                std::cout << en.occurTime;
                std::cout << "离开队列1" << std::endl;
                checkqueue(totalTemp,currentTime);//每处理完一个存钱的就检查队列2
            }

            //取钱type1
            else{
                if(total >= en.cust->money){//钱够
                    total -= en.cust->money;
                    totalTime += (currentTime - en.cust->arriveTime);
                    Node *p = q[0].front;
                    q[0].front = q[0].front->next;
                    free(p);
                    std::cout << en.occurTime;
                    std::cout << "离开队列1" << std::endl;
                }
                else{//钱不够了
                    Node *p = q[0].front;
                    q[0].front = q[0].front->next;
                    p->next = NULL;
                    if(q[1].front != NULL){
                        q[1].rear->next = p;
                        q[1].rear = p;
                    }
                    else{
                        q[1].front = p;
                        q[1].rear = p;
                    }//排入队列2
                    std::cout << en.occurTime;
                    std::cout << " 离开队列1,排入队列2" << std::endl;
                }
            }    
            if(q[0].front != NULL){//创建下一个离开事件
                Node nextq = *(q[0].front);
                event newEvent;
                newEvent.occurTime = currentTime + nextq.data.duration; 
                if(newEvent.occurTime <= closeTime){
                    newEvent.type = depart;
                    newEvent.cust = (client*)malloc(sizeof(client));
                    *newEvent.cust = nextq.data;
                    eventInsert(ev,newEvent);
                }
            }
            else{//否则窗口空闲
                winfree = 1;
            }     
        }//客户离开队列1
        else if(en.type == arrive){
            Node* cursor = (Node*)malloc(sizeof(Node));
            cursor->next = NULL;
            cursor->data = *(en.cust);
            if(q[0].front != NULL){
                q[0].rear->next = cursor;
                q[0].rear = cursor;
            }
            else{
                q[0].front = cursor;
                q[0].rear = cursor;
            }//先排队1
            std::cout << en.occurTime << "    排入队列1" << std::endl;
            if(winfree == 1){
                event nextEvent;
                nextEvent.occurTime = en.occurTime + en.cust->duration;  // 服务结束时间
                if(nextEvent.occurTime <= closeTime){
                    nextEvent.type = depart;
                    nextEvent.cust = (client*)malloc(sizeof(client));
                    *(nextEvent.cust) = *(en.cust);
                    eventInsert(ev, nextEvent);//生成离开事件插入事件表
                }
                winfree = 0;
            }
            customerNum++;
        }//客户到达银行
    }
    closeBank();
    int averageTime = totalTime / customerNum;
    std::cout << "平均逗留时间" << averageTime;
    return 0;
}
void eventInsert(eventList &ev,event &en){
    eventList pre = ev;
    eventList p = pre->next;
    eventList insert = (eventList)malloc(sizeof(linkList));
    insert->next = NULL;
    insert->data = en;
    while(pre->next != NULL){
        if(insert->data.occurTime > p->data.occurTime){
            p = p->next;
            pre = pre->next;
            continue;
        }
        else{
            pre->next = insert;
            insert->next = p;
            return;
        }//按发生时间从小到大的顺序插入
    }
    pre->next = insert;
    return;//否则直接插入末尾
}
void checkqueue(int &totalTemp,int currentTime){
    if(q[1].front == NULL)
        return;//空队列直接返回
    Node *p = q[1].front;
    Node *r = q[1].rear;
    do{
        if(p->data.money > total){
            q[1].front = q[1].front->next;
            p->next = NULL;
            q[1].rear->next = p;
            q[1].rear = q[1].rear->next;
            p = q[1].front;
        }
        else{
            if(currentTime + p->data.duration <= closeTime){
                total -= p->data.money;
                currentTime += p->data.duration;
                totalTime += (currentTime - p->data.arriveTime);
                q[1].front = q[1].front->next;
                free(p);
                std::cout << "离开队列2" << std::endl;
                if(total < totalTemp)
                    return;      
            }
        }
    }while(p != r && p != NULL);
    return;//每次检查未满足的排到队尾
}
void closeBank(){
    while(q[0].front != NULL){
        totalTime += (closeTime - q[0].front->data.arriveTime);
        Node *p = q[0].front;
        q[0].front = q[0].front->next;
        free(p);
    }
    while(q[1].front != NULL){
        totalTime += (closeTime - q[1].front->data.arriveTime);
        Node *p = q[1].front;
        q[1].front = q[1].front->next;
        free(p);
    }
    std::cout << "CLOSE" << std::endl;
    return;
}