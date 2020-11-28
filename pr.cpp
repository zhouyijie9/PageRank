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
    double oldPR = 0;
    double newPR = 0;
};

//把每个页面的信息（outPage，oldPR，newPR）存到向量里
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

    // 开始pr值的迭代更新
    while(1)
    {
        int shouldStop = 0; //根据oldPR与newPR的差值 判断是否停止迭代

        //对于i->j，根据公式，页面j能接收到的PR值为：PR(j) = d * sum(PR(i)/OUT(i)) + (1-d)
        //因此每个顶点i向出页面发送的PR值为：oldPR/outDegree
        for(auto &page: pages)
        {
            int outDegree = page.outPage.size();
            double tmpPR = page.oldPR / outDegree;
            for(int j = 0; j < outDegree; j++)
                pages[page.outPage[j]].newPR += tmpPR * d;
        }

        for(auto &page: pages)
        {
            page.newPR += 1-d;
            if(abs(page.newPR - page.oldPR) < threshold)
                shouldStop++;
            page.oldPR = page.newPR;
            page.newPR = 0;
        }

        if(shouldStop == N)
            break;

        cnt++;
    }

    printf("%s%d%s\n", "共迭代", cnt, "轮");
//    for(auto page: pages)
//        printf("%.9lf\n", page.oldPR/N);
    cout << "运行时间：" << (double)clock() /CLOCKS_PER_SEC<< "s\n";

    string outPath = "../result/pr.txt";
    cout << "out path: " << outPath << endl;
    ofstream fout(outPath);
    for(int i=0; i < N; i++)
        fout << i <<' ' << pages[i].oldPR << endl;

    return 0;
}


