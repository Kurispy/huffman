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
        siftUp(size_ - 1, (size_ - 1) / 2);
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
    
    if (front_[parent] < front_[child]) {
        temp = front_[parent];
        front_[parent] = front_[child];
        front_[child] = temp;
        siftUp(parent, parent / 2);
    }
}

void PriorityQueue::siftDown(int parent) {
    HuffNode *temp;
    
    if (parent * 2 + 2 <= size_ - 1) {
        if (front_[parent] < front_[parent * 2 + 1] || front_[parent] < front_[parent * 2 + 2]) {
            if (front_[parent * 2 + 1] < front_[parent * 2 + 2]) {
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
        if (front_[parent] < front_[parent * 2 + 1]) {
            temp = front_[parent];
            front_[parent] = front_[parent * 2 + 1];
            front_[parent * 2 + 1] = temp;
            siftDown(parent * 2 + 1);
        }
    }
}

void Huffman::readData() {
    while (!cin.eof())
        frequency_list_[cin.get()]++;
    
    //Reset get pointer to the beginning of the stream
    cin.seekg(0);
}

void Huffman::createTree() {
    HuffNode *huffnode;
    
    //Create the priority queue
    for (int i = 0; i < 256; i++) {
        if (frequency_list_[i] > 0) {
            huffnode = new HuffNode(i, frequency_list_[i], NULL, NULL);
            queue_.insert(huffnode);
            queue_.size_++;
        }
    }
    
    //Combine nodes from the queue until a single tree is left
    while (queue_.size_ > 1) {
        huffnode = queue_.front();
        queue_.deletefront();
        if (huffnode < queue_.front())
            huffnode = new HuffNode(findMinChar(huffnode, queue_.front()), huffnode->weight + queue_.front()->weight, huffnode, queue_.front());
        else
            huffnode = new HuffNode(findMinChar(huffnode, queue_.front()), huffnode->weight + queue_.front()->weight, queue_.front(), huffnode);
        
        queue_.deletefront();
        queue_.insert(huffnode);
    }
}

void Huffman::encode() {
    DFS(queue_.front(), 0);
}

void Huffman::writeOut() {
    int next, size;
    char bits[4] = {0, 0, 0, 0};
    
    //Magic cookie
    cout << "HUFFMA3\0";
    
    //Frequencies
    for (int i = 0; i < 256; i++) {
        cout.write((char*) &frequency_list_[i], sizeof(frequency_list_[i]));
    }
    
    //Data
    while (!cin.eof()) {
        next = cin.get();
        size = asBits(next, bits);
        cout.write(bits, size);
    }
}

int Huffman::findMinChar(HuffNode *huffnode1, HuffNode *huffnode2) {
    if (huffnode1->min_char < huffnode2->min_char)
        return huffnode1->min_char;
    else
        return huffnode2->min_char;
}

void Huffman::DFS(HuffNode *huffnode, int code) {
    if (huffnode->left != NULL)
        DFS(huffnode->left, (code << 1));
    if (huffnode->right != NULL)
        DFS(huffnode->right, (code << 1) + 1);
    if (huffnode->left == NULL && huffnode->right == NULL)
        code_table_[huffnode->min_char] = code;
}

int Huffman::asBits(int character, char *bits) {
    int size, k = 1;
    bits[0] = 0;
    bits[1] = 0;
    bits[2] = 0;
    bits[3] = 0;
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            *(bits + i) += (code_table_[character] & (k << (i * 8 + j))) >> j;
            *(bits + i) = *(bits + i) << 1;
        }
        if (*(bits + i) != 0)
            size = i + 1;
    }
    
    return size;
}