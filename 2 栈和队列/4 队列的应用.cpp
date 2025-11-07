#include <iostream>
#include <queue>
using namespace std;

int n, m; //n行m列
int a[1000][1000]; //矩阵
bool visited[1000][1000]; //记录是否访问过
int area = 0; //记录连通块的个数

//四个方向
int dx[4] = { 0, 0, 1, -1 };
int dy[4] = { 1, -1, 0, 0 };

//检查坐标(x, y)是否有效且未访问过
bool isValid(int x, int y)
{
    return x >= 0 && x < n && y >= 0 && y < m && a[x][y] == 1 && !visited[x][y];
}

//检查坐标(x, y)是否在矩阵边缘
bool isEdge(int x, int y)
{
    return x == 0 || x == n - 1 || y == 0 || y == m - 1;
}
//遍历连通块
bool bfs(int x, int y)
{
    queue<pair<int, int>> q; //创建队列
    q.push({ x, y }); //将起始点加入队列
    visited[x][y] = true; //标记起始点为已访问
    bool hasInternal = false; //标记是否有内部节点

    while (!q.empty())
    {
        int cx = q.front().first; //获取当前节点的x坐标
        int cy = q.front().second; //获取当前节点的y坐标
        q.pop(); //移除队列中的当前节点
        if (!isEdge(cx, cy))
        {
            hasInternal = true; //如果当前节点不在边缘，标记为有内部节点
        }
        //遍历四个方向的邻居节点
        for (int i = 0; i < 4; i++)
        {
            int nx = cx + dx[i]; //计算邻节点的x坐标
            int ny = cy + dy[i]; //计算邻节点的y坐标

            if (isValid(nx, ny))
            {
                visited[nx][ny] = true; //标记邻节点为已访问
                q.push({ nx, ny }); //将邻节点加入队列
            }
        }
    }
    return hasInternal; //返回是否有内部节点
}

int main()
{
    cin >> n >> m; //输入矩阵的行数和列数
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            cin >> a[i][j]; //输入矩阵元素
            visited[i][j] = false; //初始化访问标记为false
        }
    } //输入矩阵

    //遍历矩阵中的每个元素
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            //如果当前元素为1且未访问过
            if (a[i][j] == 1 && !visited[i][j])
            {
                
                if (bfs(i, j))
                {
                    area++;
                }//如果有内部节点则计入区域数
            }
        }
    }
    cout << area << endl; //输出区域数
    return 0;
}