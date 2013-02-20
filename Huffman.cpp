#include <cstdlib>
#include <iostream>
#include "Huffman.h"

using namespace std;

HuffNode::HuffNode(int _min_char, unsigned int _weight, HuffNode *_left, HuffNode *_right)
: min_char(_min_char), weight(_weight), left(_left), right(_right) {
    
}

bool HuffNode::operator<(HuffNode &rhs) {
    if (weight > rhs.weight)
        return 1;
    else if (weight < rhs.weight)
        return 0;
    else {
        if (min_char > rhs.min_char)
            return 1;
        else
            return 0;
    }
}

PriorityQueue::PriorityQueue()
: size_(0) {
    
}

void PriorityQueue::insert(HuffNode *node) {
    if (size_ == 0) {
        front_[size_++] = node;
    }
    else {
        front_[size_++] = node;
        siftUp(size_ - 1, (size_ - 2) / 2);
    }
}

void PriorityQueue::deletefront() {
    front_[0] = front_[size_ - 1];
    front_[size_ - 1] = NULL;
    size_--;
    siftDown(0);
}

HuffNode* PriorityQueue::front() {
    return front_[0];
}

void PriorityQueue::siftUp(int child, int parent) {
    HuffNode *temp;
    
    if (child == 0)
        return;
    
    if (*(front_[parent]) < *(front_[child])) {
        temp = front_[parent];
        front_[parent] = front_[child];
        front_[child] = temp;
        siftUp(parent, (parent - 1) / 2);
    }
}

void PriorityQueue::siftDown(int parent) {
    HuffNode *temp;
    
    if (parent * 2 + 2 <= size_ - 1) {
        if (*(front_[parent]) < *(front_[parent * 2 + 1]) || *(front_[parent]) < *(front_[parent * 2 + 2])) {
            if (*(front_[parent * 2 + 1]) < *(front_[parent * 2 + 2])) {
                temp = front_[parent];
                front_[parent] = front_[parent * 2 + 2];
                front_[parent * 2 + 2] = temp;
                siftDown(parent * 2 + 2);
            }
            else {
                temp = front_[parent];
                front_[parent] = front_[parent * 2 + 1];
                front_[parent * 2 + 1] = temp;
                siftDown(parent * 2 + 1);
            }
        }
    }
    else if (parent * 2 + 1 <= size_ - 1) {
        if (*(front_[parent]) < *(front_[parent * 2 + 1])) {
            temp = front_[parent];
            front_[parent] = front_[parent * 2 + 1];
            front_[parent * 2 + 1] = temp;
            siftDown(parent * 2 + 1);
        }
    }
}

Huffman::Huffman()
: size_(0) {
    for (int i = 0; i < 256; i++) {
        frequency_list_[i] = 0;
    }
    
    for (int i = 0; i < 256; i++) {
        code_table_[i] = "";
    }
}

void Huffman::readData() {
    while (!cin.eof()) {
        frequency_list_[cin.get()]++;
        size_++;
    }
    
    //Reset get pointer to the beginning of the stream
    cin.clear();
    cin.seekg(0);
}

void Huffman::readCompressedData() {
    //Magic cookie
    for (int i = 0; i < 8; i++)
        cin.ignore();
    
    //Frequencies
    for (int i = 0; i < 256; i++) {
        cin.read((char*) &frequency_list_[i], 4);
        size_ += frequency_list_[i];
    }
    
    createTree();
    
    //Data
    decode();
}

void Huffman::createTree() {
    HuffNode *huffnode;
    
    //Create the priority queue
    for (int i = 0; i < 256; i++) {
        if (frequency_list_[i] > 0) {
            huffnode = new HuffNode(i, frequency_list_[i], NULL, NULL);
            queue_.insert(huffnode);
        }
    }
    
    //Combine nodes from the queue until a single tree is left
    while (queue_.size_ > 1) {
        huffnode = queue_.front();
        queue_.deletefront();
        if (*(huffnode) < *(queue_.front()))
            huffnode = new HuffNode(findMinChar(huffnode, queue_.front()), huffnode->weight + queue_.front()->weight, huffnode, queue_.front());
        else
            huffnode = new HuffNode(findMinChar(huffnode, queue_.front()), huffnode->weight + queue_.front()->weight, queue_.front(), huffnode);
        
        queue_.deletefront();
        queue_.insert(huffnode);
    }
}

void Huffman::encode() {
    DFS(queue_.front(), "");
}

void Huffman::decode() {
    unsigned char buffer;
    unsigned int numprinted = 0;
    HuffNode *huffnode = queue_.front();
    
    //Single character
    if (huffnode->left == NULL && huffnode->right == NULL) {
        for (unsigned int i = 0; i < frequency_list_[huffnode->min_char]; i++) {
            cout << (char) huffnode->min_char;
            numprinted++;
        }
    }
    if (numprinted == size_)
        return;
    
    while (!cin.eof()) {
        cin.read((char*) &buffer, 1);
        
        for (int i = 0; i < 8; i++) {
            if (buffer % 2)
                huffnode = huffnode->right;
            else
                huffnode = huffnode->left;

            buffer = buffer >> 1;
            
            if (huffnode->left == NULL && huffnode->right == NULL) {
                cout << (char) huffnode->min_char;
                numprinted++;
                huffnode = queue_.front();
                if (numprinted == size_)
                    return;
            }
        }
    }
}

void Huffman::writeOut() {
    //Magic cookie
    cout << "HUFFMA3" << '\0';
    
    //Frequencies
    for (int i = 0; i < 256; i++) {
        cout.write((char*) &frequency_list_[i], sizeof(frequency_list_[i]));
    }
    
    //Data
    asBits();
}

int Huffman::findMinChar(HuffNode *huffnode1, HuffNode *huffnode2) {
    if (huffnode1->min_char < huffnode2->min_char)
        return huffnode1->min_char;
    else
        return huffnode2->min_char;
}

void Huffman::DFS(HuffNode *huffnode, string code) {
    if (huffnode->left != NULL) {
        DFS(huffnode->left, code += '0');
        code.erase(code.end() - 1);
    }
    if (huffnode->right != NULL) {
        DFS(huffnode->right, code += '1');
        code.erase(code.end() - 1);
    }
    if (huffnode->left == NULL && huffnode->right == NULL)
        code_table_[huffnode->min_char] = code;
}

void Huffman::asBits() {
    int next, width = 0;
    unsigned char buffer = 0;
    
    while (!cin.eof()) {
        next = cin.get();
        
        if (next < 0)
            break;
    
        for (unsigned int i = 0; i < code_table_[next].size(); i++) {
            buffer += (((int) code_table_[next].at(i)) - '0') << 7;
            width++;
            if (width == 8) {
                cout.write((char*) &buffer, 1);
                width = 0;
                buffer = 0;
            }
            else
                buffer = buffer >> 1;
        }
    }
    
    if (width != 0) {
        buffer = buffer >> (7 - width);
        cout.write((char*) &buffer, 1);
    }
}