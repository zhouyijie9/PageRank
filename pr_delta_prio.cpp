#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <ctime>
#include <cstdlib>
#include <algorithm>

using namespace std;

double d = 0.85;
double threshold = 1e-7;

struct Page
{
    vector<int> outPage; //出邻居
    double value = 0;
    double oldDelta= 0;
    double recvDelta = 1-d;
};

//把每个页面的信息（outPage，value，oldDelta，recvDelta）存到向量里
void getPagesVec(int N, vector<Page>& pages)
{
    ifstream inFile("../dataset/web.txt");
    int u, v;
    while(inFile >> u >> v)
        pages[u].outPage.push_back(v);
}

int main()
{
    int N = 281903; // 可以去get_N.cpp获取顶点个数N
    vector<Page> pages(N); //pages存放页面向量，大小为N
    getPagesVec(N, pages);

    int cnt = 0; //统计迭代几轮

    int sampleAmount = 1000; //采样数量
    vector<double> sampleDelta(sampleAmount); //sampleDelta存的是采样到的顶点的delta值
    double proportion = 0.3; //前多少比例为阈值
    double sampleThreshold; //阈值具体数字
    unsigned seed = 123; //随机生成器种子

    // 开始迭代
    while(1)
    {
        int shouldStop = 0; //根据oldPR与newPR的差值 判断是否停止迭代

        int randomNum;
        srand(seed); //在调用rand()之前设置随机生成器种子

        for(int i = 0; i < sampleAmount; i++) //sampleAmount=1000次采样
        {
            randomNum = rand() % N; //0～N-1的随机整数
            sampleDelta[i] = pages[randomNum].oldDelta;
        }
        sort(sampleDelta.rbegin(), sampleDelta.rend()); //降序排序
        sampleThreshold = sampleDelta[sampleAmount * proportion - 1]; //获取阈值，第200个的值是阈值
        //cout << sampleThreshold << endl;

        for(auto &page: pages)
        {
            if(page.oldDelta < sampleThreshold) //不传播
                continue;

            int outDegree = page.outPage.size();
            double tmpDelta = page.oldDelta / outDegree;
            for(int j = 0; j < outDegree; j++)
                pages[page.outPage[j]].recvDelta += tmpDelta * d;
            page.oldDelta = 0;
        }

        for(auto &page: pages)
        {
            if(page.recvDelta < threshold)
                shouldStop++;
            page.value += page.recvDelta;
            page.oldDelta += page.recvDelta;
            page.recvDelta = 0;
        }

        if(shouldStop == N)
            break;

        cnt++;
    }

    printf("%s%d%s\n", "共迭代", cnt, "轮");
    //for(auto page: pages)
    //    printf("%.9lf\n", page.value/N);
    cout << "运行时间：" << (double)clock() /CLOCKS_PER_SEC<< "s\n";

    string outPath = "../result/pr_priority.txt";
    cout << "out path: " << outPath << endl;
    ofstream fout(outPath);
    for(int i=0; i < N; i++)
        fout << i <<' ' << pages[i].value << endl;

    return 0;
}


