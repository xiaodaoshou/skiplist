sk#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <ctime>

const int MAX_LEVEL = 16; // 最大层数

// 节点定义
struct Node {
    int value;
    Node **forward; // 指向下一个节点的指针数组

    Node(int value, int level) {
        this->value = value;
        // 创建一个指针数组，每层对应一个指针
        forward = new Node*[level + 1];
        memset(forward, 0, sizeof(Node*) * (level + 1));
    }
    ~Node() {
        delete[] forward;
    }
};

// 跳表定义
class SkipList {
private:
    Node *header;  // 跳表头节点
    int level;     // 当前跳表的层数
    float probability; // 控制是否向上建立索引的概率

    // 生成随机层数
    int randomLevel() {
        int lvl = 1;
        while ((std::rand() / double(RAND_MAX)) < probability && lvl < MAX_LEVEL) {
            lvl++;
        }
        return lvl;
    }

public:
    SkipList(float prob = 0.5) {
        this->level = 1;
        this->probability = prob;
        // 创建一个头节点，值为-1（或其他无效值），层数为MAX_LEVEL
        header = new Node(-1, MAX_LEVEL);
    }

    ~SkipList() {
        Node *current = header;
        while (current) {
            Node *next = current->forward[0];
            delete current;
            current = next;
        }
    }

    // 插入值
    void insert(int value) {
        Node *current = header;
        Node *update[MAX_LEVEL + 1];
        memset(update, 0, sizeof(Node*) * (MAX_LEVEL + 1));

        // 查找插入位置
        for (int i = level; i >= 0; i--) {
            while (current->forward[i] != nullptr && current->forward[i]->value < value) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        // 插入新节点
        int lvl = randomLevel();
        if (lvl > level) {
            for (int i = level + 1; i <= lvl; i++) {
                update[i] = header;
            }
            level = lvl;
        }

        Node *newNode = new Node(value, lvl);
        for (int i = 0; i <= lvl; i++) {
            newNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = newNode;
        }

        std::cout << "Inserted value " << value << " with level " << lvl << std::endl;
    }

    // 删除值
    void remove(int value) {
        Node *current = header;
        Node *update[MAX_LEVEL + 1];
        memset(update, 0, sizeof(Node*) * (MAX_LEVEL + 1));

        // 查找要删除的位置
        for (int i = level; i >= 0; i--) {
            while (current->forward[i] != nullptr && current->forward[i]->value < value) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        current = current->forward[0];
        if (current != nullptr && current->value == value) {
            for (int i = 0; i <= level; i++) {
                if (update[i]->forward[i] != current) break;
                update[i]->forward[i] = current->forward[i];
            }

            delete current;

            // 更新层数
            while (level > 1 && header->forward[level] == nullptr) {
                level--;
            }

            std::cout << "Removed value " << value << std::endl;
        }
    }

    // 查找值
    bool search(int value) {
        Node *current = header;
        for (int i = level; i >= 0; i--) {
            while (current->forward[i] != nullptr && current->forward[i]->value < value) {
                current = current->forward[i];
            }
        }

        current = current->forward[0];
        if (current != nullptr && current->value == value) {
            std::cout << "Found value " << value << std::endl;
            return true;
        }
        std::cout << "Value " << value << " not found" << std::endl;
        return false;
    }

    // 打印跳表
    void display() {
        for (int i = level; i >= 0; i--) {
            Node *current = header->forward[i];
            std::cout << "Level " << i << ": ";
            while (current != nullptr) {
                std::cout << current->value << " ";
                current = current->forward[i];
            }
            std::cout << std::endl;
        }
    }
};

int main() {
    // 初始化随机数种子
    std::srand(std::time(nullptr));

    SkipList skiplist;

    skiplist.insert(3);
    skiplist.insert(6);
    skiplist.insert(7);
    skiplist.insert(9);
    skiplist.insert(12);
    skiplist.insert(19);
    skiplist.insert(17);
    skiplist.insert(26);
    skiplist.insert(21);
    skiplist.insert(25);

    skiplist.display();

    skiplist.search(19);
    skiplist.remove(19);
    skiplist.search(19);

    return 0;
}
