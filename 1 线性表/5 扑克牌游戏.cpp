#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstdlib>

using namespace std;

// 定义扑克牌结构体
struct Card {
    int suit; // 0=黑桃, 1=红心, 2=梅花, 3=方块
    int rank; // 1=A, 2-10, 11=J, 12=Q, 13=K
    Card *prev; // 前一张牌
    Card *next; // 后一张牌
    // 构造函数
    Card(int s, int r) : suit(s), rank(r), prev(NULL), next(NULL) {}
};

// 将花色字符串转换为整数
int suitToInt(const string &suitStr) {
    if (suitStr == "Spade") return 0;
    if (suitStr == "Heart") return 1;
    if (suitStr == "Club") return 2;
    if (suitStr == "Diamond") return 3;
    return -1; // 无效花色
}

// 将牌面字符串转换为整数
int rankToInt(const string &rankStr) {
    if (rankStr == "A") return 1;
    if (rankStr == "J") return 11;
    if (rankStr == "Q") return 12;
    if (rankStr == "K") return 13;
    else return atoi(rankStr.c_str());
}

// 将整数转换为花色字符串
string intToSuit(int suitInt) {
    if (suitInt == 0) return "Spade";
    if (suitInt == 1) return "Heart";
    if (suitInt == 2) return "Club";
    if (suitInt == 3) return "Diamond";
    return "";
}

// 将整数转换为牌面字符串
string intToRank(int rankInt) {
    if (rankInt == 1) return "A";
    if (rankInt == 11) return "J";
    if (rankInt == 12) return "Q";
    if (rankInt == 13) return "K";
    else {
        char buf[4];
        sprintf(buf, "%d", rankInt);
        return string(buf);
    }
}

// 修剪字符串，去除首尾空白字符，包括空格、制表符、回车符和换行符
string trim(const string& s) {
    size_t first = s.find_first_not_of(" \t\r\n");
    if (first == string::npos)
        return "";
    size_t last = s.find_last_not_of(" \t\r\n");
    return s.substr(first, (last - first + 1));
}

// 将一张牌添加到牌堆底部
void append(Card *&head, Card *&tail, Card *newCard) {
    if (tail == NULL) {
        head = tail = newCard;
    } else {
        tail->next = newCard;
        newCard->prev = tail;
        tail = newCard;
    }
}

// 从牌堆顶部弹出一张牌
Card* pop(Card *&head, Card *&tail) {
    if (head == NULL) {
        return NULL;
    } else {
        Card *removedCard = head;
        head = head->next;
        if (head != NULL) {
            head->prev = NULL;
        } else {
            tail = NULL;
        }
        removedCard->next = NULL;
        removedCard->prev = NULL;
        return removedCard;
    }
}

// 逆转整个牌堆
void revert(Card *&head, Card *&tail) {
    Card *current = head;
    Card *temp = NULL;
    while (current != NULL) {
        temp = current->prev;
        current->prev = current->next;
        current->next = temp;
        current = current->prev; // 因为prev和next已经交换
    }
    // 交换头尾指针
    temp = head;
    head = tail;
    tail = temp;
}

// 比较两张牌的大小，用于排序
bool compareCards(const Card *a, const Card *b) {
    return a->rank < b->rank;
}

// 抽取某一花色的所有牌
void extract(Card *&head, Card *&tail, int suit) {
    vector<Card*> extractedCards;
    Card *current = head;
    while (current != NULL) {
        if (current->suit == suit) {
            Card *toRemove = current;
            current = current->next;
            // 从牌堆中移除该牌
            if (toRemove->prev != NULL) {
                toRemove->prev->next = toRemove->next;
            } else {
                head = toRemove->next;
            }
            if (toRemove->next != NULL) {
                toRemove->next->prev = toRemove->prev;
            } else {
                tail = toRemove->prev;
            }
            toRemove->prev = toRemove->next = NULL;
            extractedCards.push_back(toRemove);
        } else {
            current = current->next;
        }
    }
    // 按照牌面从小到大排序
    sort(extractedCards.begin(), extractedCards.end(), compareCards);
    // 将抽取的牌放到牌堆顶部
    for (int i = extractedCards.size() - 1; i >= 0; --i) {
        Card *card = extractedCards[i];
        if (head == NULL) {
            head = tail = card;
        } else {
            card->next = head;
            head->prev = card;
            head = card;
            card->prev = NULL;
        }
    }
}

int main() {
    int n;
    // 使用 cin >> n; 然后使用 cin.get(); 消耗换行符
    cin >> n;
    cin.get(); // 消耗掉换行符

    Card *head = NULL; // 牌堆顶部
    Card *tail = NULL; // 牌堆底部

    for (int i = 0; i < n; ++i) {
        string line;
        // 读取命令行，如果读取失败，跳过
        if (!getline(cin, line)) {
            break;
        }
        // 跳过空行
        while (line.empty()) {
            if (!getline(cin, line)) {
                break;
            }
        }
        line = trim(line); // 去除首尾空白字符
        if (line.empty()) {
            continue;
        }
        istringstream iss(line);
        string cmd;
        iss >> cmd;
        cmd = trim(cmd);
        if (cmd == "Append") {
            string suitStr, rankStr;
            iss >> suitStr >> rankStr;
            suitStr = trim(suitStr);
            rankStr = trim(rankStr);
            int suit = suitToInt(suitStr);
            int rank = rankToInt(rankStr);
            Card *newCard = new Card(suit, rank);
            append(head, tail, newCard);
        } else if (cmd == "Pop") {
            Card *poppedCard = pop(head, tail);
            if (poppedCard != NULL) {
                cout << intToSuit(poppedCard->suit) << " " << intToRank(poppedCard->rank) << endl;
                delete poppedCard;
            } else {
                cout << "NULL" << endl;
            }
        } else if (cmd == "Revert") {
            revert(head, tail);
        } else if (cmd == "Extract") {
            string suitStr;
            iss >> suitStr;
            suitStr = trim(suitStr);
            int suit = suitToInt(suitStr);
            extract(head, tail, suit);
        }
    }
    // 输出最终的牌堆
    if (head == NULL) {
        cout << "NULL" << endl;
    } else {
        Card *current = head;
        while (current != NULL) {
            cout << intToSuit(current->suit) << " " << intToRank(current->rank) << endl;
            Card *next = current->next;
            delete current;
            current = next;
        }
    }
    return 0;
}