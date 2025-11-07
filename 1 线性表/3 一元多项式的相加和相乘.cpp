#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <map>

using namespace std;

#ifdef _MSC_VER
#pragma warning(disable:6031)
#endif

/* 错误定义 */
#define TRUE        1
#define FALSE        0
#define OK        1
#define ERROR        0
#define INFEASIBLE    -1
#define MYOVERFLOW    -2

typedef int Status;

class Ploy;    // 前向声明

class LNode {
public:
    int coef;
    int expo;
    LNode* next;    // 指针域
    friend class Ploy;
    friend ostream& operator<<(ostream& out, const Ploy& a);
    // 定义任何私有成员相当于 struct LNode
};

class Ploy {
protected:
    LNode* head;    // 头指针
public:
    /* 关于链表的基本操作转化为实际的 C++ 方法 */
    Ploy();    // 构造函数，完成 InitList
    Ploy(const Ploy &p);
    ~Ploy();    // 析构函数，完成 DestroyList
    void Ploy_add_ele(const int p, const int e);
    Ploy Ploy_mul_ele(const int c, const int e);
    Ploy& operator=(const Ploy& p);
    Ploy operator+(const Ploy& p);
    Ploy Ploy_mul(const Ploy& a);
    friend ostream & operator<<(ostream& out, const Ploy& a);
};



/* 构造函数，初始化成员变量 */
Ploy::Ploy()
{
    /* 生成头结点空间，并赋值头指针 */
    head = new LNode;
    if (head == NULL)
        exit(MYOVERFLOW);

    head->next = NULL;
}

/* 复制构造函数，初始化成员变量 */
Ploy::Ploy(const Ploy &p)
{
    /* 生成头结点空间，并赋值头指针 */
    head = new LNode;
    if (head == NULL)
        exit(MYOVERFLOW);

    LNode* q = head;
    LNode* r = p.head->next;
    while (r)
    {
        q->next = new(nothrow) LNode;
        if (!q->next)exit(MYOVERFLOW);
        q->next->coef = r->coef;
        q->next->expo = r->expo;
        q->next->next = NULL;
        q = q->next;
        r = r->next;
    }
}

/* 析构函数，删除成员变量 */
Ploy::~Ploy()
{
    LNode* q, * p = head;

    /* 从头结点开始，逐个删除（包括头结点） */
    while (p) {    // 当链表不为空时，循环并执行
        q = p->next; // 抓住链表的下一个节点
        delete p;
        p = q;
    }

    head = NULL;    // 头指针指向 NULL
}

Ploy& Ploy::operator=(const Ploy& p)
{
    if (this == &p)
        return *this;

    // 先清空当前链表
    LNode* t = head->next;
    LNode* s;
    while (t)
    {
        s = t->next;
        delete t;
        t = s;
    }
    head->next = NULL;

    // 复制 p 的链表
    LNode* q = head;
    LNode* r = p.head->next;
    while (r)
    {
        q->next = new(nothrow) LNode;
        if (!q->next)exit(MYOVERFLOW);
        q->next->coef = r->coef;
        q->next->expo = r->expo;
        q->next->next = NULL;
        q = q->next;
        r = r->next;
    }
    return *this;
}

/* 优化后的多项式相加，使用双指针合并两个有序链表 */
Ploy Ploy::operator+(const Ploy& p)
{
    Ploy res;
    LNode* l1 = head->next;
    LNode* l2 = p.head->next;
    LNode* tail = res.head; // 结果链表的尾指针

    while (l1 && l2)
    {
        if (l1->expo < l2->expo)
        {
            LNode* node = new LNode;
            node->coef = l1->coef;
            node->expo = l1->expo;
            node->next = NULL;
            tail->next = node;
            tail = node;
            l1 = l1->next;
        }
        else if (l1->expo > l2->expo)
        {
            LNode* node = new LNode;
            node->coef = l2->coef;
            node->expo = l2->expo;
            node->next = NULL;
            tail->next = node;
            tail = node;
            l2 = l2->next;
        }
        else // 指数相等
        {
            int sum_coef = l1->coef + l2->coef;
            if (sum_coef != 0)
            {
                LNode* node = new LNode;
                node->coef = sum_coef;
                node->expo = l1->expo;
                node->next = NULL;
                tail->next = node;
                tail = node;
            }
            l1 = l1->next;
            l2 = l2->next;
        }
    }
    // 处理剩余项
    while (l1)
    {
        LNode* node = new LNode;
        node->coef = l1->coef;
        node->expo = l1->expo;
        node->next = NULL;
        tail->next = node;
        tail = node;
        l1 = l1->next;
    }
    while (l2)
    {
        LNode* node = new LNode;
        node->coef = l2->coef;
        node->expo = l2->expo;
        node->next = NULL;
        tail->next = node;
        tail = node;
        l2 = l2->next;
    }
    return res;
}

/* 优化后的多项式相乘，使用 map 存储中间结果 */
Ploy Ploy::Ploy_mul(const Ploy& a)
{
    // 创建 map 存储乘积结果，键为指数，值为系数
    std::map<int, int> terms; // exponent -> coefficient

    for (LNode* p1 = head->next; p1 != NULL; p1 = p1->next)
    {
        for (LNode* p2 = a.head->next; p2 != NULL; p2 = p2->next)
        {
            int expo = p1->expo + p2->expo;
            int coef = p1->coef * p2->coef;
            terms[expo] += coef;
        }
    }

    // 构建结果多项式
    Ploy res;
    LNode* tail = res.head;

    for (std::map<int, int>::iterator it = terms.begin(); it != terms.end(); ++it)
    {
        if (it->second != 0)
        {
            LNode* node = new LNode;
            node->coef = it->second;
            node->expo = it->first;
            node->next = NULL;
            tail->next = node;
            tail = node;
        }
    }

    return res;
}

void Ploy::Ploy_add_ele(const int c, const int e)
{
    if (!c)return;
    LNode* l = head;
    LNode* r = l->next;
    while (r)
    {
        if (r->expo == e)// 若指数等于当前节点指数
        {
            r->coef += c;
            if (r->coef == 0)// 加后系数为 0，删除该节点
            {
                l->next = r->next;
                delete r;
            }
            return;
        }
        else if (r->expo > e)// 若未找到指数且当前指数更大，插入到 l 和 r 之间
        {
            LNode* s = new(nothrow) LNode;
            if (!s)exit(MYOVERFLOW);
            s->expo = e;
            s->coef = c;
            l->next = s;
            s->next = r;
            return;
        }

        l = l->next;
        r = r->next;
    }

    // r 移动到了 NULL，即未插入，需在尾节点（即 l）之后添加
    l->next = new(nothrow) LNode;
    if (!l->next)exit(MYOVERFLOW);
    l->next->expo = e;
    l->next->coef = c;
    l->next->next = NULL;
    return;
}

Ploy Ploy::Ploy_mul_ele(const int c, const int e)
{
    Ploy temp;
    if (!c)return temp;
    LNode* q = head->next;
    while (q)
    {
        temp.Ploy_add_ele(q->coef * c, q->expo + e);
        q = q->next;
    }

    return temp;
}

ostream& operator<<(ostream& out, const Ploy& a)
{
    LNode* p = a.head->next;
    if (!p)
        return out;

    while (p)
    {
        out << p->coef << " " << p->expo;
        p = p->next;
        if (p)
            out << " ";
    }
    out << endl;

    return out;
}


int main()
{
    Ploy PA, PB;
    int n, m;
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        int c, e;
        scanf("%d%d", &c, &e);
        PA.Ploy_add_ele(c, e);
    }
    scanf("%d", &m);
    for (int i = 0; i < m; i++)
    {
        int c, e;
        scanf("%d%d", &c, &e);
        PB.Ploy_add_ele(c, e);
    }

    int command;
    scanf("%d", &command);
    if (command == 0 || command == 2)
    {
        cout << PA + PB;
        if (command == 2)
        {
            Ploy PC = PA.Ploy_mul(PB);
            cout << PC;
        }
    }
    else if (command == 1)
    {
        Ploy PC = PA.Ploy_mul(PB);
        cout << PC;
    }

    return 0;
}