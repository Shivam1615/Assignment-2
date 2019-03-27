fileCompressor: fileCompressor.c
        gcc -g -Wall -Werror -fsanitize=address -o fileCompressor fileCompressor.c -lm
clean:
        rm -f fileCompressor
