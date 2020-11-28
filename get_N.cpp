//
// Created by 周依杰 on 2020/11/28.
//

#include <iostream>
#include <set>
#include <fstream>
using namespace std;

int main()
{
    set<int> s;
    ifstream inFile("../dataset/web.txt");
    int u, v;
    while(inFile >> u >> v) //假设每一行插入一条边u->v
    {
        s.insert(u);
        s.insert(v);
    }
    int N = s.size();
    printf("一共有%d个顶点。", N);
}