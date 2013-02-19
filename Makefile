# tech; Chen, Christopher

huffman : Huffman.o main.o 
	g++ -Wall -g -o huffman  Huffman.o main.o 

Huffman.o : Huffman.cpp Huffman.h 
	g++ -Wall -g -c  Huffman.cpp

main.o : main.cpp Huffman.h 
	g++ -Wall -g -c  main.cpp

clean:
	rm -f huffman Huffman.o main.o 
