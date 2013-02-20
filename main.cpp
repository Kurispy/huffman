#include <iostream>
#include <cstring>
#include <string>
#include "Huffman.h"

using namespace std;

int main(int argc, char* argv[]) {
    Huffman huff_tree;
    
    if (1 == argc) {
        //Compress
        
        //Read in characters and determine frequencies
        huff_tree.readData();
        
        //Create the Huffman tree
        huff_tree.createTree();
        
        //Record codes from the Huffman tree
        huff_tree.encode();
        
        //Write out the compressed data in the proper format
        huff_tree.writeOut();
        
    }
    else if (!strcmp("-d", argv[1])) {
        //Decompress
        
        huff_tree.readCompressedData();
    }
    
    return 0;
}