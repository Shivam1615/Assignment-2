#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

typedef struct data{
        char word;
        int freq;
} data;

int size=0;

void countFreq(FILE *pf, data* frequencies){

        char c;
        while((c=fgetc(pf))!=EOF){
                printf("%c",c);






        }
        fclose(pf);
        return;
}

void findFiles(char *dir)
{

data *frequencies=(data*)malloc(sizeof(data)*20);

    DIR *d;
    struct dirent *entry;
    struct stat check;

    if((d = opendir(dir)) == NULL) {
        fprintf(stderr,"cannot open directory: %s\n", dir);
        return;
    }
    chdir(dir);
    while((entry = readdir(d)) != NULL) {
        lstat(entry->d_name,&check);
        if(S_ISDIR(check.st_mode)) {
            if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0)
                continue;
            printf("%s/",entry->d_name);
            findFiles(entry->d_name);
        }
        else {
               
                if( strcmp("fileCompressor.c",entry->d_name)==0 || strcmp("Makefile",entry->d_name)==0 ||
                strcmp("fileCompressor",entry->d_name)==0 || strcmp("huffman",entry->d_name)==0 ||
                strcmp("huffman.c",entry->d_name)==0 || strcmp("huffman.h",entry->d_name)==0)
                        continue;
                printf("%s contains: ",entry->d_name);
                FILE *pf=fopen(entry->d_name,"r");
                countFreq(pf,frequencies);
        }
    }
    chdir("..");
    closedir(d);
}


int main(int argc, char* argv[])
{
        char *currentDir, x[2]=".";
        currentDir=x;

        findFiles(currentDir);

        return 0;
}
