#ifndef HUFFMAN_H
#define	HUFFMAN_H

#include <iostream>

using namespace std;

struct Node {
    int min_char;
    unsigned int weight;
    Node *left;
    Node *right;
};

class Huffman {
private:
    unsigned int frequency_list_[256];
    int code_table_[256]; //Store codes as ints maybe? Convert to string on print?
    Node *char_queue_[256]; //Need to either make a queue or min heap
    Node *inter_queue_[256];
public:
    void readData();
    void createTree();
    void encode();
    void writeOut();
};

#endif	/* HUFFMAN_H */

