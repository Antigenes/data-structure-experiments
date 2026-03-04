#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <map>
#include <cstdlib>
using namespace std;
struct CharNode{//用来存每个char字符和它的频数
    char ch;       
    int count;
};

typedef struct{
    int weight;
    int parent,lchild,rchild;
}HTNode,*HuffmanTree;//动态分配数组存储赫夫曼树
typedef char** HuffmanCode;//动态分配数组存储赫夫曼编码表


void Initialization(HuffmanTree &,CharNode* &);
void Encoding(HuffmanTree &,HuffmanCode &,CharNode* &);
void Decoding();
void print();
void TreePrinting();
int GetHuffmanTree(HuffmanTree &,CharNode* &);
void separate(string&,string&);
void compound(string,string);

int main(){
    HuffmanTree HT = NULL;
    HuffmanCode HC = NULL;
    CharNode *CharUnion = NULL;
    // int n = GetHuffmanTree(HT,CharUnion);
    // cout << n;
    char c = '6';
    while(c != 'Q' && c != 'q'){
        cout << "1.需要构建新的huffman树?" << endl;
        cout << "2.进行编码?" << endl;
        cout << "3.进行译码?" << endl;
        cout << "4.进行打印代码文件?" << endl;
        cout << "5.进行打印?" << endl;
        cout << "Q.退出" << endl;
        
        cin >> c;
        if(c == '1')
            Initialization(HT,CharUnion);
        if(c == '2')
            Encoding(HT,HC,CharUnion);
        if(c == '3')
            Decoding();
        if(c == '4')
            print();
        if(c == '5')
            TreePrinting();
        //cout << "\x1b[2J\x1b[H";//ANSI转义字符清屏
    }
    return 0;
}


//初始化部分
int FileInput(char* &p){//读取文本文件输入
    int MAX_SIZE = 100000;
    int INCREMENT = 50000;
    p = (char *)malloc(MAX_SIZE * sizeof(char));
    if(!p){
        cout << "内存分配失败!" << endl;
        return 0;
    }//防止内存没有正确分配
    unsigned char ch;
    int i = 0;
    cout << "请输入文本文件名:";
    string filename;
    cin >> filename;
    ifstream file(filename,ios::binary);//以二进制形式打开文件
    if (!file.is_open()) {
        cout << "文件打开失败!" << endl;
        p = nullptr;
        return 0;
    }//打开文件失败，直接返回

    while(file.read((char*)&ch,1)){//读取每个字节
        if(i >= (MAX_SIZE)){//对原数组进行扩容
            MAX_SIZE += INCREMENT;
            char *r = (char *)malloc(MAX_SIZE * sizeof(char));
            if(!r){
                cout << "内存分配失败!" << endl;
                free(p);
                p = NULL;
                file.close();
                return 0;
            }
            for(int j = 0;j < i;j++){
                r[j] = p[j];
            }//将p复制到r
            free(p);//释放原本的空间
            p = r;//将p指向现在的数组
        }
        p[i] = ch;
        i++;
    }//现在的i就是字符数组长度
    cout << "已成功读取文本!" << endl;//输出提示 
    file.close();
    return i;
}

void CalculatChars(char* p,int &n,CharNode* &CharUnion,int Len){//解析二进制字符串获得字符集
    CharUnion = (CharNode*)malloc(257 * sizeof(CharNode));//最多出现256个字符，不妨先创建出来(第一位不用)
    if(!CharUnion){
        cout << "解析失败!字符集空间未能正确分配!" << endl;
        return;
    }//分配失败直接退出
    n = 0;
    int flag = 0;//标记，1找到，0没找到
    for(int i = 0;i < Len;i++){
        //查找每一个元素
        int j;
        for(j = 1;j <= n;j++){
            if(CharUnion[j].ch == p[i]){//找到相同字符
                flag = 1;
                break;
            }
        }
        if(flag == 0){
            n++;
            CharUnion[n].ch = p[i];
            CharUnion[n].count = 1;
        }
        if(flag == 1){
            CharUnion[j].count++;
        }
        flag = 0;//置零标记
    }//得到字符集CharUnion
    return;
}

void Select(HuffmanTree HT,int n,int &s1,int &s2){//找到赫夫曼树中最小的两个未分配节点的位置
    int m1 = -1;
    int m2 = -1;//先给两个初始权值
    for(int i = 1;i <= n;i++){
        if(HT[i].parent == 0){
            if(m1 == -1){
                m1 = HT[i].weight;
                s1 = i;
            }//第一个有效数给s1
            else if((HT[i].weight < m1)){
                m1 = HT[i].weight;
                s1 = i;
            }
        }
    }//先找出最小的s1
    for(int i = 1;i <= n;i++){
        if(i == s1)
            continue;
        if(HT[i].parent == 0){
            if(m2 == -1){
                m2 = HT[i].weight;
                s2 = i;
            }//第一个有效数给s1
            else if((HT[i].weight < m2)){
                m2 = HT[i].weight;
                s2 = i;
            }
        }
    }//再找出s2
    return;
}

void saveHuffmanTree(HuffmanTree &HT,int n,CharNode *CharUnion){
    const char* filename = "hfmTree.bin";//固定文件名，二进制文件存放赫夫曼树
    ofstream out(filename,ios::binary);//以二进制的形式进行输入
    if (!out.is_open()){
        cerr << "创建文件失败!" << endl;
        return;
    }//创建失败直接返回
    out.write((const char*)(&n),sizeof(int));//将n写入文件开头表示字符集大小
    int i;
    for(i = 1;i <= 2 * n - 1;i++){
        if(i <= n)//out.write(const char*p,int);函数原型
            out.write((const char*)(&CharUnion[i].ch),sizeof(char));//输入前n个节点代表的字符
        out.write((const char*)(&HT[i].weight),sizeof(int));
        out.write((const char*)(&HT[i].parent),sizeof(int));
        out.write((const char*)(&HT[i].lchild),sizeof(int));
        out.write((const char*)(&HT[i].rchild),sizeof(int));//依次输入每个节点的数据
    }
    return;
}

void Initialization(HuffmanTree &HT,CharNode* &CharUnion){
    char *pointer = NULL;//得到的文本存入这个字符数组里
    int Len;
    Len = FileInput(pointer);//以文件形式输入
    if(!pointer)
        return;//文件打开失败直接退出
    int n;
    
    CalculatChars(pointer,n,CharUnion,Len);//统计字符数目,字符集大小,各字符权值和频率
    if(!CharUnion)
        return;//未能获得字符集直接退出
    free(pointer);
    
    //构造Huffman树HT
    int i;
    int w[n + 1];
    for(i = 1;i <= n;i++){
        w[i] = CharUnion[i].count;
    }//将所有字符的权值给数组w

    int m = 2 * n - 1;//计算赫夫曼树的结点数
    HT = (HuffmanTree)malloc((m+1) * sizeof(HTNode));//0号不用
    HuffmanTree p;
    p = HT;
    p++;//指向HT[1]
    for(i = 1;i <= n;i++,p++){
        *p = {w[i],0,0,0};
    }//初始化所有树叶节点
    for(;i <= m;i++,p++){
        *p = {0,0,0,0};
    }//初始化其他所有节点

    for(i = n + 1;i <= m;i++){//构建赫夫曼树
        int s1,s2;
        Select(HT,i - 1,s1,s2);//选出HT中parent为0且weight最小的两个节点，序号赋给s1，s2
        HT[s1].parent = i;
        HT[s2].parent = i;
        HT[i].lchild = s1;
        HT[i].rchild = s2;
        HT[i].weight = HT[s1].weight + HT[s2].weight;
    }

//test：打印hfm树
    // for(int i = 1;i <= m;i++){
    //     if(i <= n)
    //         cout << CharUnion[i].ch << "   ";
    //     cout << HT[i].parent << "   " << HT[i].lchild << "    " << HT[i].rchild << "    " << HT[i].weight << endl;

    // }




    saveHuffmanTree(HT,n,CharUnion);
    free(HT);
    free(CharUnion);
    HT = NULL;
    CharUnion = NULL;

    //cout << n;
    return;
}


//编码部分
int GetHuffmanTree(HuffmanTree &HT,CharNode* &CharUnion){
    const char* filename = "hfmTree.bin";//赫夫曼树文件名固定
    ifstream file(filename,ios::binary);//以二进制形式读取文件
    if (!file.is_open()){
        cout << "文件打开失败!" << endl;
        return 0;
    }//打开文件失败，直接返回

    int n;
    file.read((char*)(&n),sizeof(int));//读取文件开头存放的叶子数
    int m = 2 * n - 1;//得到结点数

    CharUnion = (CharNode*)malloc((n + 1) * sizeof(CharNode));
    HT = (HuffmanTree)malloc((m + 1) * sizeof(HTNode));
    int i;
    for(i = 1;i <= m;i++){
        if(i <= n)
            file.read((char*)(&CharUnion[i].ch),sizeof(char));
        file.read((char*)(&HT[i].weight),sizeof(int));
        file.read((char*)(&HT[i].parent),sizeof(int));
        file.read((char*)(&HT[i].lchild),sizeof(int));
        file.read((char*)(&HT[i].rchild),sizeof(int));//依次读取每个节点的数据
    }
    return n;
}

void Encoding(HuffmanTree &HT,HuffmanCode &HC,CharNode* &CharUnion){
    int n;
    CharNode* p = CharUnion;
    //统一从文件中读取赫夫曼树
    n = GetHuffmanTree(HT,CharUnion);
    if(!HT)
        return;//说明文件打开失败或没有文件，直接返回
    //现在得到了HT和包含字符信息的CharUnion

    //得到赫夫曼编码
    //直接copy的教材上面的（给伪代码改成代码），变量名也一样
    HC = (HuffmanCode)malloc((n + 1) * sizeof(char *));//分配n个字符编码的头指针向量
    char *cd = (char*)malloc(n * sizeof(char));//分配求编码的工作空间
    int start;
    int c,f;
    cd[n - 1] = '\0';//编码结束符
    for(int i = 1;i <= n;i++){//逐个字符求编码
        start = n - 1;
        for(c = i,f = HT[i].parent;f != 0;c = f,f = HT[f].parent){//叶子到根逆向求编码
            if(HT[f].lchild == c)
                cd[--start] = '0';
            else
                cd[--start] = '1';
        }
        HC[i] = (char*)malloc((n - start) * sizeof(char));
        strcpy(HC[i],&cd[start]);
    }
    free(cd);

    //map构建映射便于查找
    map<char,string> codeMap;
    for(int i = 1;i <= n;i++){
        codeMap[CharUnion[i].ch] = string(HC[i]);
    }

    //对文件进行编码
    string originalFilename;
    cout << "请输入文件名: ";
    cin >> originalFilename;

    ifstream inFile(originalFilename,ios::binary);
    if (!inFile.is_open()){
        cout << "文件打开失败!" << endl;
        return;
    }

    //构造输出文件名，去掉原后缀加输出文件后缀
    string Filename = originalFilename;
    int k = originalFilename.find_last_of('.');//找到后缀‘.’的位置
    if (k > 1) {
        Filename = originalFilename.substr(0,k);//得到无后缀的文件名
    }
    string extension = "";
    for(int i = k + 1;i < originalFilename.length();i++){
        extension += originalFilename[i];//不含点号
    }

    string outFilename = Filename + ".hfm";//得到输出文件名
    ofstream outFile(outFilename,ios::binary);//二进制形式写入压缩文件
    if(!outFile.is_open()){
        cerr << "输出失败!" << endl;
        inFile.close();
        return;
    }
    //开始编码
    unsigned char ch;
    int bitsCount = 0;//记录读到的bit数目
    unsigned char byteNow = 0;//拼接的字节
    outFile.write((const char*)(&byteNow),1);//把末尾补了几个0写在开头，这里先占位
    while(inFile.read((char*)(&ch),1)){//逐个字节读入
        map<char,string>::iterator it = codeMap.find(ch);
        if (it == codeMap.end()) {
            cout << "编码失败!" << endl;
            HT = NULL;
            CharUnion = NULL;
            HC = NULL;//置空指针
            break;
        }
        string hfmCode = it->second;//得到当前的hfm编码
        for(int i = 0;i < hfmCode.length();i++){
            if(hfmCode[i] == '1'){
                byteNow <<= 1;//字节左移一位并补0 
                byteNow |= 1;//与1或运算，相当于后面填个1
            }
            else{
                byteNow <<= 1;//后面不动
            }
            bitsCount++;
            if(bitsCount == 8){//满一个字节，写入
                outFile.write((const char*)(&byteNow),1);
                bitsCount = 0;
                byteNow = 0;
            }
        }
    }
    if(bitsCount != 0){
        byteNow <<= (8 - bitsCount);
        outFile.write((const char*)(&byteNow),1);//剩余bit补满0后输出
        byteNow = 1;
        byteNow <<= (7 - bitsCount);//得到要左移的位数
        outFile.seekp(0);//回到文件开头
        outFile.write((const char*)(&byteNow),1);//重新写入
    }
    outFile.close();
    inFile.close();
    free(HT);
    free(CharUnion);
    free(HC);
    HT = NULL;
    CharUnion = NULL;
    HC = NULL;//置空，这些已经被写入文件了
    compound(Filename,extension);//合成
    return;
}


//译码部分
void Decoding(){
    string extension;
    string filename;
    separate(filename,extension);//从zip文件得到两个文件
    int n;
    HuffmanTree HT;
    CharNode* CharUnion;
    n = GetHuffmanTree(HT,CharUnion);//调用函数获得所有文件中的赫夫曼树
    //直接copy的教材上面的（给伪代码改成代码），变量名也一样
    HuffmanCode HC = (HuffmanCode)malloc((n + 1) * sizeof(char *));//分配n个字符编码的头指针向量
    char *cd = (char*)malloc(n * sizeof(char));//分配求编码的工作空间
    int start;
    int c,f;
    cd[n - 1] = '\0';//编码结束符
    for(int i = 1;i <= n;i++){//逐个字符求编码
        start = n - 1;
        for(c = i,f = HT[i].parent;f != 0;c = f,f = HT[f].parent){//叶子到根逆向求编码
            if(HT[f].lchild == c)
                cd[--start] = '0';
            else
                cd[--start] = '1';
        }
        HC[i] = (char*)malloc((n - start) * sizeof(char));
        strcpy(HC[i],&cd[start]);
    }
    free(cd);

    // for(int i=1;i <= n;i++){
    //     cout << HC[i] << endl;
    // }

    //反向map构建映射便于查找
    map<string,char> DecodeMap;
    for(int i = 1;i <= n;i++){
        DecodeMap[string(HC[i])] = CharUnion[i].ch;
    }
    //直接copy译码部分获得反向映射表

    //开始解码
    ifstream transFile(filename,ios::binary);//二进制形式读取
    unsigned char ch; 
    transFile.read((char*)(&ch),1);//先读取第个字节获取存放的末尾补零数
    int supp = 0;
    for(int i = 1;i <= 7;i++){//只检查前7位
        if(ch & 1){
            supp = i;
            ch >>= 1;
            break;
        }
    }//获得补零数
    string file = "";
    while(transFile.read((char*)(&ch),1)){
        for(int j = 7;j >= 0;j--){
            if((ch >> j) & 1)//当前位数为1
                file += "1";//末尾＋1；
            else
                file += "0";//否则末尾添0
        }
    }
    transFile.close();
    //得到的file现在以字符串形式存下了所有二进制数
    file = file.substr(0,file.length() - supp);//删去末尾添的0
    string decode = "";
    string nowCode = "";
    for(int i = 0;i < file.length();i++){//逐一比对获得译码
        nowCode += file[i];
        map<string, char>::iterator it = DecodeMap.find(nowCode);
        if(it != DecodeMap.end()){
            decode += it->second;
            nowCode = "";//情况当前字符串
        }
    }
    
    //构造输出文件名，去掉原后缀加输出文件后缀
    string Filename = filename;
    int k = filename.find_last_of('.');//找到后缀‘.’的位置
    if (k > 1) {
        Filename = filename.substr(0,k);//得到无后缀的文件名
    }
    
    filename = Filename + "." + extension;//得到输出文件名

    ofstream outDecode(filename,ios::binary);
    if(!outDecode.is_open()){
        cout << "译码时创建文件失败!" << endl;
        return;
    }
    outDecode.write(decode.data(),decode.length());//用.data()取string decode的指针，将decode对应的二进制写入文件
    return;
}


//打印部分
void print(){
    cout << "请输入要打印的二进制文件:";
    string Filename;
    cin >> Filename;
    char ch;
    ifstream file(Filename,ios::binary);//二进制形式读取文件
    int count = 0;
    while(file.read((char*)(&ch),1)){//读取每一个字节
        for(int i = 7;i >= 0;i--){//从左到右读取每一个比特
            if((ch >> i) & 1){//当前位是1
                cout << '1';
            }
            else{
                cout << '0';
            }
            count++;
            if(count == 50){
                count = 0;
                cout << endl;//满50个换行
            }
        }
    }
    cout << endl;
    return;
}


//打印赫夫曼树部分
void CSTreePrintRecessed(HuffmanTree T,int depth,HuffmanTree HT,CharNode *CharUnion,int layer,int n){
    //递归算法，把孩子全部打印完后再打印兄弟
    int j;
    for(j = 0;j < depth;j++){
        cout << '\t';
    }
    cout << "------     ";//打印当前节点
    if(layer >= 1 && layer <= n)
        cout << CharUnion[layer].ch;
    cout << endl;
    if((*T).rchild != 0){
        HuffmanTree rT = &HT[(*T).rchild];
        CSTreePrintRecessed(rT,depth + 1,HT,CharUnion,(*T).rchild,n);
    }
    if((*T).lchild != 0){
        HuffmanTree lT = &HT[(*T).lchild];
        CSTreePrintRecessed(lT,depth + 1,HT,CharUnion,(*T).lchild,n);//先打印右孩子再打印左孩子以满足凹入表法
    }
    return;
}//根据作业题改的

void TreePrinting(){
    HuffmanTree HT = NULL;
    CharNode* CharUnion = NULL;
    int n;
    n = GetHuffmanTree(HT,CharUnion);//从文件读取hfm树信息
    //确实成功读取了
    // cout << n << endl;
    int m = n * 2 - 1;    
    HuffmanTree T = &HT[m];//指向根节点
    int depth = 0;
    CSTreePrintRecessed(T,depth,HT,CharUnion,m,n);
    return;
}

void separate(string &filename,string &extension){
    cout << "请输入要解压的文件名:";
    cin >> filename;
    ifstream zip (filename,ios::binary);
    extension = "";
    char ch;
    for(int i = 0;i < 3;i++){
        zip.read((char*)(&ch),1);
        if(ch != '\0')
            extension += ch;
    }//得到文件后缀名
    int length;
    zip.read((char*)(&length),sizeof(int));//读取树文件字节数
    ofstream tree ("hfmTree.bin",ios::binary);
    for(int i = 0;i < length;i++){
        zip.read((char*)(&ch),1);
        tree.write((const char*)(&ch),1);
    }
    tree.close();
    string hfmname = filename;
    int k = filename.find_last_of('.');//找到后缀‘.’的位置
    if (k > 1) {
        hfmname = filename.substr(0,k);//得到无后缀的文件名
    }
    hfmname += ".hfm";
    ofstream hfm (hfmname,ios::binary);
    while(zip.read((char*)(&ch),1)){
        hfm.write((const char*)(&ch),1);
    }
    filename = hfmname;
    hfm.close();
    zip.close();
    return;
}

void compound(string Filename,string extension){//压缩文件前三个字节存文件格式，第四个文件存赫夫曼树文件的字节长度
    string treename = "hfmTree.bin";
    string ifilename = Filename + ".hfm";
    ifstream tree (treename,ios::binary);//读取hfm树文件
    tree.seekg(0,ios::end);//定位到文件末尾
    int length = tree.tellg();//获取字节数
    tree.seekg(0,ios::beg);//回到开头

    ifstream hfm (ifilename,ios::binary);//读取编码文件
    string zipname = Filename + ".zip";
    ofstream zip (zipname,ios::binary);//生成最终文件
    char ch;
    for(int i = 0;i < 3;i++){
        if(i < extension.length())
            ch = extension[i];
        else
            ch = '\0';
        zip.write((const char*)(&ch),1);
    }//文件开头三个字节写入文件后缀
    zip.write((const char*)(&length),sizeof(int));//接着存入bin文件长度
    while(tree.read((char*)(&ch),1)){
        zip.write((const char*)(&ch),1);//逐个字节写入赫夫曼树文件
    }
    while(hfm.read((char*)(&ch),1)){
        zip.write((const char*)(&ch),1);//逐个字节写入编码文件
    }
    tree.close();
    hfm.close();
    zip.close();
    return;
}   