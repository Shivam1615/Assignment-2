all: fileCompressor.c huffman.o
        gcc -o fileCompressor fileCompressor.c huffman.o

huffman.o: huffman.c huffman.h
        gcc -c huffman.c

clean:
        rm ./fileCompressor ./*.o
            echo Clean done
