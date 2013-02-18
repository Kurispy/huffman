#include "Huffman.h"

using namespace std;

void Huffman::readData() {
    while (!cin.eof())
        frequency_list_[cin.get()]++;
    
    //Reset get pointer to the beginning of the stream
    cin.seekg(0);
}

void Huffman::createTree() {
    HuffNode *huffnode;
    QueueNode *queuenode;
    
    //Create the priority queue
    for (int i = 0; i < 256; i++) {
        if (frequency_list_[i] > 0) {
            huffnode = new HuffNode(i, frequency_list_[i], NULL, NULL);
            queuenode = new QueueNode(huffnode);
            queue_.insert(queuenode);
            queue_.size_++;
        }
    }
    
    //Combine nodes from the queue until a single tree is left
    while (queue_.size_ > 1) {
        huffnode = queue_.front()->huff;
        queue_.deletefront();
        if (huffnode < queue_.front()->huff)
            huffnode = new HuffNode(findMinChar(huffnode, queue_.front()->huff), huffnode->weight + queue_.front()->huff->weight, huffnode, queue_.front()->huff);
        else
            huffnode = new HuffNode(findMinChar(huffnode, queue_.front()->huff), huffnode->weight + queue_.front()->huff->weight, queue_.front()->huff, huffnode);
        
        queue_.deletefront();
        queuenode = new QueueNode(huffnode)
        queue_.insert(queuenode);
    }
}

void Huffman::encode() {
    DFS(queue_.front()->huff, 0);
}

void Huffman::writeOut() {
    //Magic cookie
    cout << "HUFFMA3\0";
    
    //Frequencies
    for (int i = 0; i < 256; i++) {
        cout.write((char*) &frequency_list_[i], sizeof(frequency_list_[i]));
    }
    
    //Data
    //TODO: Bitwise endianness
}