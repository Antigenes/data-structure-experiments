#define WALK 0
#define NORMAL 1
#include <iostream>
#include <string>
using namespace std;
//邻接表存储结构
typedef struct InfoType{//弧的信息
    int roadkind;//WALK人行道，NORMAL不区分
    int length;
    string direction;
    int level;
}InfoType;
typedef struct ArcNode{
    int adjvex;
    struct ArcNode *nextarc;
    InfoType *info;
}ArcNode;
typedef struct VertexType{//顶点信息
    string name;
    string info;
}VertexType;
typedef struct VNode{
    VertexType data;
    ArcNode *firstarc;
}VNode;
typedef struct {
    VNode* vertices;
    int vexnum,arcnum;//顶点数、弧数
}ALGraph;//邻接表

void addArc(ALGraph&,int,int,int,int,string,int);
void initGraph(ALGraph&);
void floydPathFind(ALGraph,int,int);
void addPoint(ALGraph&);
int ValidTest(int,int,int,int,int,string,int);
int findPointNum(ALGraph,int&,string);

int main(){
    //把信息都填入邻接表
    ALGraph orig;//申明一个正邻接表
    initGraph(orig);
    while(1){
        cout << "请输入功能：" << endl;
        cout << "1:查询景点信息" << endl;
        cout << "2:查询两个景点间的最短路径" << endl;
        cout << "3:添加景点/道路" << endl;
        cout << "q:退出" << endl;
        cout << ":";
        char c;
        cin >> c;

        if(c == 'q')
            break;

        else if(c == '1'){
            cout << "请输入需要查询的景点:";
            string locate;
            cin >> locate;
            int flag = 0;//标记是否找到景点
            int find;//标记找到的序号
            if(!findPointNum(orig,find,locate));
            else{
                cout << "已查询到该景点的信息：" << endl;
                cout << locate << "：";
                cout << orig.vertices[find].data.info << endl;
                cout << "相邻景点：";
                ArcNode* p = orig.vertices[find].firstarc;
                while(p){
                    cout << orig.vertices[p->adjvex].data.name << "     ";//打印相邻景点
                    p = p->nextarc;
                }
                cout << endl;
                cout << endl;
                cout << endl;
                cin.get();
                cout << "回车以继续" << endl;
                cin.get();
            }
        }

        else if(c == '2'){
            //找到需要查找的起点与终点的序号
            cout << "请输入要查询的路径的起点：";
            string start;
            cin >> start;
            int startNum;
            findPointNum(orig,startNum,start);
            cout << "请输入要查询的路径的终点：";
            string end;
            cin >> end;
            int endNum;//标记找到终点的序号
            findPointNum(orig,endNum,end);
            //现在开始查找一条从startNUm到endNum的最短简单路径
            floydPathFind(orig,startNum,endNum);
        }

        if(c == '3'){//添加景点/道路功能
            cout << "a:添加景点" << endl;
            cout << "b:添加道路" << endl;
            char t;
            cin >> t;
            if(t == 'a'){
                addPoint(orig);
            }
            else if(t == 'b'){
                int u,v,roadkind,length,level;
                string direction;
                string start;
                string end;
                cout << "请输入起点：";
                cin >> start;
                findPointNum(orig,u,start);
                cout << "请输入终点：";
                cin >> end;
                findPointNum(orig,v,end);
                cout << "请输入长度：";
                cin >> length;
                cout << "请输入道路类型(0.WALK/1.NORMAL)：";
                cin >> roadkind;
                cout << "请输入方向：";
                cin >> direction;
                cout << "请输入等级：";
                cin >> level;
                if(ValidTest(u,v,roadkind,length,level,direction,orig.vexnum))//输入有效时添加边
                    addArc(orig,u,v,roadkind,length,direction,level);
            }            
        }

        else
            continue;//继续循环
    }
}

int ValidTest(int u,int v,int roadkind,int length,int level,string direction,int k){//道路合法性检验
    int flag = 1;
    if(u <= 0 || u > k)
        flag = 0;
    if(v <= 0 || u > k)
        flag = 0;
    if(roadkind != 0 && roadkind != 1)
        flag = 0;
    if(level <= 0)
        flag = 0;
    if(direction != "东" && direction != "南" && direction != "西" && direction != "北"
        && direction != "东北" && direction != "东南" && direction != "西北" && direction != "西南")
        flag = 0;
    if(length < 0)
        flag = 0;
    //输出
    if(flag == 0){
        cout << "无效输入！";
        return 0;
    }
    else
        return 1;
}

void addPoint(ALGraph& orig){
    cout << "输入要加入的景点的名称：";
    string name;
    cin >> name;
    cout << "输入该顶点的信息：";
    string info;
    cin >> info;

    VNode* old = orig.vertices;
    orig.vexnum++;//顶点数加1
    orig.vertices = new VNode[orig.vexnum + 1];//重新建立链表
    for(int i = 1;i <= orig.vexnum - 1;i++){//复制前面的
        orig.vertices[i].data = old[i].data;
        ArcNode* p = old[i].firstarc;
        ArcNode* q = NULL;
        ArcNode* rear = NULL;
        while(p){//复制所有弧
            if(q == NULL){
                q = new ArcNode;//复制弧
                q->adjvex = p->adjvex;
                InfoType *info = new InfoType;//深拷贝
                *info = *(p->info);
                q->info = info;
                q->nextarc = NULL;//置空末尾
                rear = q;//尾指针指向尾节点
            }
            else{
                ArcNode* Node;
                Node = new ArcNode;//复制弧
                Node->adjvex = p->adjvex;
                InfoType *info = new InfoType;//深拷贝
                *info = *(p->info);
                Node->info = info;
                Node->nextarc = NULL;
                rear->nextarc = Node;
                rear = Node;//尾插法
            }
            p = p->nextarc;
        }
        orig.vertices[i].firstarc = q;//指向头结点
    }
    delete [] old;//释放内存
    orig.vertices[orig.vexnum].data.name = name;
    orig.vertices[orig.vexnum].data.info = info;//新景点信息复制上去
    orig.vertices[orig.vexnum].firstarc = NULL;//置空
    return;
}

void addArc(ALGraph& orig,int u,int v,int roadkind,int length,string direction,int level){//添加弧的函数
    ArcNode* q = orig.vertices[u].firstarc;
    ArcNode* p = new ArcNode;
    InfoType *info = new InfoType;
    *info = {roadkind,length,direction,level};//添加弧的信息
    p->adjvex = v;
    p->nextarc = NULL;
    p->info = info;
    p->nextarc = q;//头插法，新节点直接放到最前面来
    orig.vertices[u].firstarc = p;//头指针
    return;
}

int findPointNum(ALGraph orig,int& u,string name){//找到景点对应的序号
    int flag = 0;//标记是否找到景点
    int startNum;//标记找到起点的序号
    for(int i = 1;i <= orig.vexnum;i++){
        if(name == orig.vertices[i].data.name){
            flag = 1;
            startNum = i;
        }
    }
    if(flag == 0){
        cout << "该景点不存在！";
        return 0;
    }
    else{
        u = startNum;
        return 1;
    }
}
void initGraph(ALGraph& orig){//填入最初的信息
    orig.vexnum = 12;
    orig.vertices = new VNode[orig.vexnum + 1];
    orig.arcnum = 34;
    //添加所有的节点信息
    orig.vertices[1].data.name = "北门";
    orig.vertices[2].data.name = "行政楼";
    orig.vertices[3].data.name = "一号教学楼";
    orig.vertices[4].data.name = "二号教学楼";
    orig.vertices[5].data.name = "图书馆";
    orig.vertices[6].data.name = "人工湖";
    orig.vertices[7].data.name = "实验中心";
    orig.vertices[8].data.name = "学生食堂";
    orig.vertices[9].data.name = "男寝";
    orig.vertices[10].data.name = "女寝";
    orig.vertices[11].data.name = "体育馆";
    orig.vertices[12].data.name = "南门";
    orig.vertices[1].data.info = "宏伟的学校正门，交通枢纽。";
    orig.vertices[2].data.info = "办公区域，办事处。";
    orig.vertices[3].data.info = "主教学楼，历史悠久。";
    orig.vertices[4].data.info = "现代化多媒体教室。";
    orig.vertices[5].data.info = "藏书百万，学习圣地。";
    orig.vertices[6].data.info = "校园著名景点，风景秀丽。";
    orig.vertices[7].data.info = "包含各类理工科实验室。";
    orig.vertices[8].data.info = "提供各地美食。";
    orig.vertices[9].data.info = "学生休息区。";
    orig.vertices[10].data.info = "学生休息区。";
    orig.vertices[11].data.info = "室内篮球场与羽毛球馆。";
    orig.vertices[12].data.info = "靠近生活区的小门。";

    for(int i = 1;i <= orig.vexnum;i++){
        orig.vertices[i].firstarc = NULL;//先全部置空
    }
    //addArc(orig,u,v,roadkind,length,direction,level);
    //添加所有的弧
    addArc(orig,1,2,NORMAL,100,"南",2);
    addArc(orig,1,3,NORMAL,200,"东南",3);
    addArc(orig,2,1,NORMAL,100,"北",2);
    addArc(orig,2,3,WALK,150,"东",3);
    addArc(orig,2,5,NORMAL,400,"西南",2);
    addArc(orig,2,6,NORMAL,100,"南",2);
    addArc(orig,3,1,NORMAL,200,"西北",3);
    addArc(orig,3,2,WALK,150,"西",3);
    addArc(orig,3,4,NORMAL,100,"南",2);
    addArc(orig,4,3,NORMAL,100,"北",2);
    addArc(orig,4,5,NORMAL,350,"西",2);
    addArc(orig,4,6,WALK,150,"西",5);
    addArc(orig,4,7,NORMAL,300,"东",1);
    addArc(orig,5,2,NORMAL,400,"东北",2);
    addArc(orig,5,4,NORMAL,350,"东",2);
    addArc(orig,5,6,WALK,100,"东",5);
    addArc(orig,6,2,NORMAL,100,"北",2);
    addArc(orig,6,4,WALK,150,"东",5);
    addArc(orig,6,5,WALK,100,"西",5);
    addArc(orig,6,8,WALK,200,"南",4);
    addArc(orig,7,4,NORMAL,300,"西",1);
    addArc(orig,7,8,NORMAL,150,"西南",2);
    addArc(orig,8,6,WALK,200,"北",4);
    addArc(orig,8,7,NORMAL,150,"东北",2);
    addArc(orig,8,9,NORMAL,100,"南",2);
    addArc(orig,8,10,NORMAL,120,"东南",2);
    addArc(orig,9,8,NORMAL,100,"北",2);
    addArc(orig,9,12,NORMAL,80,"南",1);
    addArc(orig,10,8,NORMAL,120,"西北",2);
    addArc(orig,10,12,NORMAL,100,"西南",1);
    addArc(orig,11,12,NORMAL,300,"东南",1);
    addArc(orig,12,9,NORMAL,80,"北",1);
    addArc(orig,12,10,NORMAL,100,"东北",1);
    addArc(orig,12,11,NORMAL,300,"西北",1);

    cout << "已成功载入地图信息！" << endl;
    return;
}

int adj(ALGraph orig,int i,int j,int& length){//查找是否存在i到j的有向边，并获取长度
    ArcNode* p = orig.vertices[i].firstarc;
    while(p){
        if(p->adjvex == j){
            length = p->info->length;//获取长度
            return 1;//找到，直接返回1
        }
        p = p->nextarc;
    }
    return 0;//没找到，返回0
}
void floydFull(ALGraph orig,int u,int v){//全地图条件下的最短路径查找
    int k = orig.vexnum;
    int dist[k + 1][k + 1];//顶点序号从1到k
    int path[k + 1][k + 1];
    int length;
    //初始化
    for(int i = 1;i <= k;i++){
        for(int j = 1;j <= k;j++){
            if(i == j){
                dist[i][j] = 0;
                path[i][j] = -1;//自身
            }
            else if(adj(orig,i,j,length)){
                dist[i][j] =  length;
                path[i][j] = j;//可直接到达
            }
            else{
                dist[i][j] = INT_MAX;
                path[i][j] = -1;//无路径
            }
        }
    }
    //更新算法
    for(int z = 1;z <= k;z++){//遍历每一个点作为中间点
        for(int x = 1;x <= k;x++){
            for(int y = 1;y <= k;y++){
                if(dist[x][z] != INT_MAX && dist[z][y] != INT_MAX)//当存在无穷大时直接跳过
                    if(dist[x][z] + dist[z][y] < dist[x][y]){//有更短的路径直接换
                        path[x][y] = path[x][z];
                        dist[x][y] = dist[x][z] + dist[z][y];
                    }
            }
        }
    }
    //输出u到v的路径
    cout << "从" << orig.vertices[u].data.name << "到" << orig.vertices[v].data.name;
    if(path[u][v] == -1)//没有路径
        cout << "不存在路径";
    else{
        int cur = u;//当前顶点
        int next;
        cout <<  "的路线为：" << endl;
        cout << orig.vertices[u].data.name;//打印起点
        while(cur != v){//输出路线
            next = path[cur][v];
            cout << "---";
            cout << "向";
            ArcNode* p = orig.vertices[cur].firstarc;
            while(p){//遍历每一条边找到路径对应的边
                if(p->adjvex == path[cur][v]){
                    cout << (p->info)->direction;
                    break;
                }
                p = p->nextarc;
            }
            cout << "---";
            cout << orig.vertices[next].data.name;
            cur = next;//进入下一个节点
        }
    }
    cout << endl;
    return;
}   

void floydCar(ALGraph orig,int u,int v){//去除人行道的条件下的最短路径查找
    ALGraph car;//先赋值orig
    car.vexnum = orig.vexnum;
    car.vertices = new VNode[orig.vexnum + 1];//建立链表
    car.arcnum = 0;//先置为0
    for(int i = 1;i <= orig.vexnum;i++){//复制
        car.vertices[i].data = orig.vertices[i].data;
        ArcNode* p = orig.vertices[i].firstarc;
        ArcNode* q = NULL;
        ArcNode* rear = NULL;
        while(p){//复制所有的NORMAL弧
            if(p->info->roadkind == NORMAL){
                if(q == NULL){
                    q = new ArcNode;//复制第一个不是WALK的弧
                    q->adjvex = p->adjvex;
                    InfoType *info = new InfoType;//深拷贝
                    *info = *(p->info);
                    q->info = info;
                    q->nextarc = NULL;//置空末尾
                    rear = q;//尾指针指向尾节点
                    car.arcnum++;//弧数自增
                }
                else{
                    ArcNode* Node;
                    Node = new ArcNode;//复制第一个不是WALK的弧
                    Node->adjvex = p->adjvex;
                    InfoType *info = new InfoType;//深拷贝
                    *info = *(p->info);
                    Node->info = info;
                    Node->nextarc = NULL;
                    rear->nextarc = Node;
                    rear = Node;//尾插法
                    car.arcnum++;//弧数自增
                }
            }
            p = p->nextarc;
        }
        car.vertices[i].firstarc = q;//指向头结点
    }
    floydFull(car,u,v);//调用正常情况的最短路径查找
    return;
}

void floydPathFind(ALGraph orig,int u,int v){//floyd算法寻找最短路径
    cout << "输入交通工具：" << endl;
    cout << "1.步行" << endl;
    cout << "2.乘车" << endl;
    char c;
    cin >> c;
    if(c == '1'){//直接在全地图找最短路径
        floydFull(orig,u,v);
    }
    else if(c == '2'){//在禁止通过人行道的条件下寻找最短路径
        floydCar(orig,u,v);
    }
    else
        cout << "无效输入！" << endl;
    return;
}

//1.关节点、2.经过某几个景点的最短路径、3.景色等级最大路径、4.路程最大路径、5.所有路径