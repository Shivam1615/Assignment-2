#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "huffman.h"

typedef struct data{
	char *word;
	int freq;
} data;

int wordCount=0;
int size=500;

data* addWord(char *word, data* frequencies){
        int i=0;
	
	while( (*(frequencies+i)).freq!=0){
		if( strcmp( (*(frequencies+i)).word,word)==0){
			(*(frequencies+i)).freq+=1;
			return frequencies;
		}
		i++;
	}
    
	if( (*(frequencies+i)).freq==0){
        	(*(frequencies+i)).word=word;
        	(*(frequencies+i)).freq=1;
	}
	wordCount++;

	return frequencies;
}

data* countFreq(char *file, data* frequencies){

	char c;
	char word[25];
	strcpy(word,"");
	int i;
        for(i=0;i<strlen(file);i++){
        	c=*(file+i);
        	if( c==' '  || c=='\n'|| c=='\t'){
			if( strcmp(word,"")!=0){
				char *temp=(char*)malloc(sizeof(char)*(strlen(word)+1));
				strcpy(temp,word);
				frequencies=addWord(temp,frequencies);
				strcpy(word,"");
			}
			
			char *whitespace=(char*)malloc(sizeof(char)*4);
			
			if(c==' '){ 
				strcpy(whitespace," ");
			}
			else if(c=='\n'){
				strcpy(whitespace,"\\n");
			}
			else if(c=='\t'){
				strcpy(whitespace,"\\t");
			}

			frequencies=addWord(whitespace,frequencies);
		}else{
			char letter[2];
			letter[0]=c;
			letter[1]='\0';
			strcat(word,letter);
		}
	
	}
        return frequencies;
}


void Compression(char *file){

        int codes;
        int words;

        int huffman=open("HuffmanCodebook", O_RDONLY);
        struct stat check;
        int HfileSize;
        if(stat("HuffmanCodebook",&check)==0)
                HfileSize=check.st_size;
        char *Hfile=(char*)malloc(sizeof(char)*HfileSize+1);
        read(huffman,Hfile,HfileSize);
        *(Hfile+HfileSize)='\0';
        close(huffman);


        int original=open(file, O_RDONLY);
        int OfileSize;
        if(stat(file,&check)==0)
                OfileSize=check.st_size;
        char *Ofile=(char*)malloc(sizeof(char)*OfileSize+1);
        read(original,Ofile,OfileSize);
        *(Ofile+OfileSize)='\0';
        close(original);

        int i;
        for(i=0;i<HfileSize;i++){
                if(*(Hfile+i)=='\t'){
                        codes++;
                }
        }
        for(i=0;i<OfileSize;i++){
                if(*(Ofile+i)==' '){
                        words++;
                }
        }



}

data* findFiles(char *dir, data* frequencies)
{	
    DIR *d;
    struct dirent *entry;
    struct stat check;
    if((d = opendir(dir)) == NULL) {
        printf("cannot open directory: %s\n", dir);
	exit(1);
        return frequencies;
    }
    chdir(dir);
    while((entry = readdir(d)) != NULL) {
        lstat(entry->d_name,&check);
        if(S_ISDIR(check.st_mode)) {
            if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0)
                continue;
            
            frequencies=findFiles(entry->d_name,frequencies);
        }
        else {
		if( strcmp("fileCompressor.c",entry->d_name)==0 || strcmp("Makefile",entry->d_name)==0 ||
                strcmp("fileCompressor",entry->d_name)==0 || strcmp("huffman",entry->d_name)==0 ||
                strcmp("huffman.c",entry->d_name)==0 || strcmp("huffman.h",entry->d_name)==0 || strcmp("HuffmanCodebook",entry->d_name)==0)	
		continue;

		int fileSize;

		if(stat(entry->d_name,&check)==0)
			fileSize = check.st_size;		
		char *file=(char*)malloc(sizeof(char)*fileSize+1);
		int fd=open(entry->d_name,O_RDONLY);

		if(fd==-1){
			printf("Failed to open a file. \n");
			exit(1);		

		}		

		read(fd,file,fileSize);
		*(file+fileSize)='\0';
		
		close(fd);
		
		frequencies=countFreq(file,frequencies);
		free(file);
	}
    }
   chdir(".."); 
    closedir(d);
	return frequencies;
}

int main(int argc, char* argv[])
{
	data *frequencies=(data*)calloc(500,sizeof(data)); 

	if( strcmp(argv[1],"-R")==0){
		if( strcmp(argv[2],"-b")==0){
			frequencies=findFiles(argv[3],frequencies);
		}

		int i;
		for(i=0;i<5;i++){
			chdir("Asst2");
			chdir("..");
			chdir("Asst2");
		}
        	
	}else if((strcmp(argv[1], "-b") == 0) && (strcmp(argv[2], "-R") == 0)){
                frequencies=findFiles(argv[3], frequencies);

                int i;
                for(i=0;i<5;i++){
                        chdir("Asst2");
                        chdir("..");
                        chdir("Asst2");
		}
	}else if( strcmp(argv[1],"-b")==0){
		int fd=open(argv[2],O_RDONLY);

		if(fd==-1){
			printf("Failed to open the file. \n");		
			exit(1);
		}	

		struct stat check;
		int fileSize;
                if(stat(argv[2],&check)==0)
                        fileSize = check.st_size;
                char *file=(char*)malloc(sizeof(char)*fileSize+1);

                read(fd,file,fileSize);
                *(file+fileSize)='\0';
                close(fd);

                frequencies=countFreq(file,frequencies);
		free(file);
	}
	int i;


	char **arr=(char**)malloc(sizeof(char *)*wordCount);
	int *freq=(int*)malloc(sizeof(int)*wordCount);
	
	for(i=0;i<wordCount;i++){
        	arr[i]=(*(frequencies+i)).word;
		*(freq+i)=(*(frequencies+i)).freq; 
	}
		
        
	getCodeBook(arr,freq,wordCount);
	
	free(frequencies);
	free(arr);
	free(freq);
	Compression(argv[2]);
        return 0;
}
