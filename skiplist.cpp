#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <limits>

class Node {
public:
    int value;
    std::vector<Node*> forward;

    Node(int val, int level) : value(val), forward(level, nullptr) {}
};

class SkipList {
private:
    int maxLevel;
    float probability;
    int currentLevel;
    Node* head;

    // 随机生成一个层数
    int randomLevel() {
        int level = 1;
        while (static_cast<float>(rand()) / RAND_MAX < probability && level < maxLevel)
            level++;
        return level;
    }

public:
    SkipList(int maxLvl, float prob) : maxLevel(maxLvl), probability(prob), currentLevel(1) {
        head = new Node(-1, maxLevel);  // 头节点
    }

    // 插入一个元素
    void insert(int value) {
        std::vector<Node*> update(maxLevel, nullptr);
        Node* current = head;

        // 找到每一层的插入位置
        for (int i = currentLevel - 1; i >= 0; i--) {
            while (current->forward[i] != nullptr && current->forward[i]->value < value) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        current = current->forward[0];

        // 如果值不存在则插入新节点
        if (current == nullptr || current->value != value) {
            int newLevel = randomLevel();

            if (newLevel > currentLevel) {
                for (int i = currentLevel; i < newLevel; i++) {
                    update[i] = head;
                }
                currentLevel = newLevel;
            }

            Node* newNode = new Node(value, newLevel);
            for (int i = 0; i < newLevel; i++) {
                newNode->forward[i] = update[i]->forward[i];
                update[i]->forward[i] = newNode;
            }
        }
    }

    // 删除一个元素
    void remove(int value) {
        std::vector<Node*> update(maxLevel, nullptr);
        Node* current = head;

        for (int i = currentLevel - 1; i >= 0; i--) {
            while (current->forward[i] != nullptr && current->forward[i]->value < value) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        current = current->forward[0];

        if (current != nullptr && current->value == value) {
            for (int i = 0; i < currentLevel; i++) {
                if (update[i]->forward[i] != current) break;
                update[i]->forward[i] = current->forward[i];
            }

            delete current;

            while (currentLevel > 1 && head->forward[currentLevel - 1] == nullptr) {
                currentLevel--;
            }
        }
    }

    // 查询一个元素
    bool search(int value) {
        Node* current = head;
        for (int i = currentLevel - 1; i >= 0; i--) {
            while (current->forward[i] != nullptr && current->forward[i]->value < value) {
                current = current->forward[i];
            }
        }
        current = current->forward[0];

        return current != nullptr && current->value == value;
    }

    // 打印跳表
    void print() {
        for (int i = 0; i < currentLevel; i++) {
            Node* node = head->forward[i];
            std::cout << "Level " << i + 1 << ": ";
            while (node != nullptr) {
                std::cout << node->value << " ";
                node = node->forward[i];
            }
            std::cout << "\n";
        }
    }

    // 析构函数
    ~SkipList() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->forward[0];
            delete current;
            current = next;
        }
    }
};

int main() {
    srand(time(nullptr));

    SkipList skiplist(6, 0.5);

    skiplist.insert(3);
    skiplist.insert(6);
    skiplist.insert(7);
    skiplist.insert(9);
    skiplist.insert(12);
    skiplist.insert(19);

    std::cout << "Skip List after insertions:\n";
    skiplist.print();

    std::cout << "Search for 6: " << (skiplist.search(6) ? "Found" : "Not found") << "\n";
    std::cout << "Search for 15: " << (skiplist.search(15) ? "Found" : "Not found") << "\n";

    skiplist.remove(6);
    std::cout << "Skip List after deletion of 6:\n";
    skiplist.print();

    return 0;
}
