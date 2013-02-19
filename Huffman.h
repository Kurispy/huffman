#ifndef HUFFMAN_H
#define	HUFFMAN_H

#include <string>

using namespace std;

struct HuffNode {
    int min_char;
    unsigned int weight;
    HuffNode *left;
    HuffNode *right;
    HuffNode(int _min_char, unsigned int _weight, HuffNode *_left, HuffNode *_right);
    bool operator<(HuffNode &rhs);
};

class PriorityQueue {
    HuffNode *front_[256];
public:
    int size_;
    PriorityQueue();
    void insert(HuffNode *node);
    void deletefront();
    HuffNode* front();
    void siftUp(int child, int parent);
    void siftDown(int parent);
};

class Huffman {
private:
    unsigned int frequency_list_[256];
    string code_table_[256];
    PriorityQueue queue_;
public:
    Huffman();
    void readData();
    void createTree();
    void encode();
    void writeOut();
    int findMinChar(HuffNode *huffnode1, HuffNode *huffnode2);
    void DFS(HuffNode *huffnode, string code);
    int asBits(int character, char *bits);
};

#endif	/* HUFFMAN_H */

