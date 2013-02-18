#ifndef HUFFMAN_H
#define	HUFFMAN_H

#include <iostream>

using namespace std;

struct HuffNode {
    int min_char;
    unsigned int weight;
    HuffNode *left;
    HuffNode *right;
    bool operator<(HuffNode &rhs);
};

struct QueueNode {
    HuffNode *huff;
    QueueNode *left;
    QueueNode *right;
};

class PriorityQueue {
    QueueNode *front_;
public:
    int size_;
    void insert(QueueNode *node);
    void deletefront();
    QueueNode* front();
};

class Huffman {
private:
    unsigned int frequency_list_[256];
    int code_table_[256];
    PriorityQueue queue_;
public:
    void readData();
    void createTree();
    void encode();
    void writeOut();
    int findMinChar();
    void DFS(HuffNode *huffnode, int code);
};

#endif	/* HUFFMAN_H */

