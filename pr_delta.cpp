#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <ctime>

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
    int N = 281903; // N是页面总数
    vector<Page> pages(N); //pages存放页面向量，大小为N
    getPagesVec(N, pages);

    int cnt = 0; //统计迭代几轮

    // 开始迭代
    while(1)
    {
        int shouldStop = 0; //根据oldPR与newPR的差值 判断是否停止迭代

        //对于i->j，根据公式，页面j能接收到的delta值为：
        //recvDelta(j) = oldDelta(i) / outDegree
        //因此每个顶点i向出邻居发送的delta值为：oldDelta(i)
        for(auto &page: pages)
        {
            int outDegree = page.outPage.size();
            double tmpDelta = page.oldDelta / outDegree;
            for(int j = 0; j < outDegree; j++)
                pages[page.outPage[j]].recvDelta += tmpDelta * d;
        }

        for(auto &page: pages)
        {
            if(page.recvDelta < threshold)
                shouldStop++;
            page.value += page.recvDelta;
            page.oldDelta = page.recvDelta;
            page.recvDelta = 0;
        }

        if(shouldStop == N)
            break;

        cnt++;
    }

    printf("%s%d%s\n", "共迭代", cnt, "轮");
//    for(auto page: pages)
//        printf("%.9lf\n", page.value/N);
    cout << "运行时间：" << (double)clock() /CLOCKS_PER_SEC<< "s\n";

    string outPath = "../result/pr_delta.txt";
    cout << "out path: " << outPath << endl;
    ofstream fout(outPath);
    for(int i=0; i < N; i++)
        fout << i <<' ' << pages[i].value << endl;

    return 0;
}


