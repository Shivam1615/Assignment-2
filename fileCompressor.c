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
	struct data* next;
} data;

int wordCount=0;
//adds word into the linked list or iterates the frequancies
void addWord(char *word, data* frequencies){
	data *trans=frequencies;
	while(trans!=NULL){
		if( strcmp(trans->word,word)==0 ){
			trans->freq+=1;
			return;
		}
		trans=trans->next;
	}
	data *temp=(data *)malloc(sizeof(data));
	temp->word=word;
	temp->freq=1;
	temp->next=NULL;

	trans=frequencies;
        while(trans->next!=NULL){
                trans=trans->next;
        }
        trans->next=temp;
	wordCount++;
        return;
}
//tokenizes a string sent from a file
void countFreq(char *file, data* frequencies){
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
				addWord(temp,frequencies);
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

			addWord(whitespace,frequencies);
		}else{
			char letter[2];
			letter[0]=c;
			letter[1]='\0';
			strcat(word,letter);
		}
	
	}
        return;
}

//creates a deCompressed file
void deCompression(char *file, char *Hfile){

	int codes=0;
	int words;
	struct stat check;
        int original=open(file, O_RDONLY);
	int OfileSize;
        if(stat(file,&check)==0)
                OfileSize=check.st_size;
        char *Ofile=(char*)malloc(sizeof(char)*OfileSize+1);
        read(original,Ofile,OfileSize);
        *(Ofile+OfileSize)='\0';
        close(original);

	int i;
	for(i=0;i<strlen(Hfile);i++){
		if(*(Hfile+i)=='\t'){
			codes++;
		}
	}
	//put info of HuffmanCodebook into arrays
	char **codeArray=(char**)malloc((sizeof(char*)*codes));
	char **wordArray=(char**)malloc((sizeof(char*)*codes));
	int indexC=0;
	int indexW=0;
	char stuff[50];
	char type='c';
	strcpy(stuff,"");
	for(i=2;i<strlen(Hfile);i++){
	if(type=='c'){
		char x[2];
		if((Hfile[i]=='\t')){
			char *code=(char*)malloc(sizeof(char)*strlen(stuff));
			strcpy(code,stuff);
			codeArray[indexC]=code;
			indexC++;
			strcpy(stuff,"");
			type='w';
		}else{
			x[0]=Hfile[i];
			x[1]='\0';
			strcat(stuff,x);
		}
	}
        else if(type=='w'){
                char y[2];
                if(Hfile[i]=='\n'){
                        char *word=(char*)malloc(sizeof(char)*strlen(stuff));
                        strcpy(word,stuff);
                        wordArray[indexW]=word;
                        indexW++;
                        strcpy(stuff,"");
                        type='c';
                }else{
                        y[0]=Hfile[i];
                        y[1]='\0';
                        strcat(stuff,y);
                }
        }
	}
	//creat the new decompressed file
	char newFile[20];
	strncpy(newFile,file,strlen(file)-4);
	int new=open(newFile, O_CREAT | O_WRONLY, 0600);		

	strcpy(stuff,"");
	for(i=0;i<OfileSize;i++){
                char x[2];
                int k; 
		for(k=0;k<codes;k++){
			if(strcmp(stuff,codeArray[k])==0){
				if( strcmp(wordArray[k],"\\n")==0 ){
					write(new,"\n",1);
				}else if( strcmp(wordArray[k],"\\t")==0){
					write(new,"\t",1);
				}else{
					write(new,wordArray[k],strlen(wordArray[k]));
				}
				strcpy(stuff,"");
			}
		}      
                x[0]=Ofile[i];
                x[1]='\0';
                strcat(stuff,x);
	}
	close(new);

}
// creates a compressed file
void Compression(char *file, char *Hfile){
	
	int codes=0;
	int words;
	struct stat check;
        int original=open(file, O_RDONLY);
	int OfileSize;
        if(stat(file,&check)==0)
                OfileSize=check.st_size;
        char *Ofile=(char*)malloc(sizeof(char)*OfileSize+1);
        read(original,Ofile,OfileSize);
        *(Ofile+OfileSize)='\0';
        close(original);

	int i;
	for(i=0;i<strlen(Hfile);i++){
		if(*(Hfile+i)=='\t'){
			codes++;
		}
	}
	//put info of Codebook into arrays
	char **codeArray=(char**)malloc((sizeof(char*)*codes));
	char **wordArray=(char**)malloc((sizeof(char*)*codes));
	int indexC=0;
	int indexW=0;
	char stuff[100];
	char type='c';
	strcpy(stuff,"");
	for(i=2;i<strlen(Hfile);i++){
	if(type=='c'){
		char x[2];
		if((Hfile[i]=='\t')){
			char *code=(char*)malloc(sizeof(char)*strlen(stuff));
			strcpy(code,stuff);
			codeArray[indexC]=code;
			indexC++;
			strcpy(stuff,"");
			type='w';
		}else{
			x[0]=Hfile[i];
			x[1]='\0';
			strcat(stuff,x);
		}
	}
        else if(type=='w'){
                char y[2];
                if(Hfile[i]=='\n'){
                        char *word=(char*)malloc(sizeof(char)*strlen(stuff));
                        strcpy(word,stuff);
                        wordArray[indexW]=word;
                        indexW++;
                        strcpy(stuff,"");
                        type='c';
                }else{
                        y[0]=Hfile[i];
                        y[1]='\0';
                        strcat(stuff,y);
                }
        }
	}

	//creates new compressed file
	char newFile[20];
	strcpy(newFile,file);
	strcat(newFile,".hcz");
	int new=open(newFile, O_CREAT | O_WRONLY, 0600);		

	strcpy(stuff,"");
	for(i=0;i<OfileSize;i++){

                char x[2];
                if( (Ofile[i]=='\t') || (Ofile[i]=='\n') || (Ofile[i]==' ') ){
                        int k;
			char whitespace[3];
			if(Ofile[i]=='\n'){
				strcpy(whitespace,"\\n");
			}else if(Ofile[i]=='\t'){
				strcpy(whitespace,"\\t");
			}else{
				whitespace[0]=Ofile[i];
				whitespace[1]='\0';
			}
			for(k=0;k<codes;k++){	
				if(strcmp(stuff,wordArray[k])==0){
					write(new,codeArray[k],strlen(codeArray[k]));
					break;
				}
			}
			for(k=0;k<codes;k++){
				if(strcmp(whitespace,wordArray[k])==0){
					write(new,codeArray[k],strlen(codeArray[k]));
					break;
				}
			}
		                   
                        strcpy(stuff,"");
                }else{
                        x[0]=Ofile[i];
                        x[1]='\0';
		
                        strcat(stuff,x);
                }
	}
	close(new);
}
//recursively traverse the directories
void findFiles(char *dir, data* frequencies, char flag,char *Hfile)
{	
    DIR *d;
    struct dirent *entry;
    struct stat check;
    if((d = opendir(dir)) == NULL) {
        printf("cannot open directory: %s\n", dir);
	exit(1);
        return;
    }
    chdir(dir);
    while((entry = readdir(d)) != NULL) {
        lstat(entry->d_name,&check);
        if(S_ISDIR(check.st_mode)) {
            if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0)
                continue;
            
            findFiles(entry->d_name,frequencies,flag,Hfile);
        }
        else {
		if( strcmp("fileCompressor.c",entry->d_name)==0 || strcmp("Makefile",entry->d_name)==0 ||
                strcmp("fileCompressor",entry->d_name)==0 || strcmp("huffman",entry->d_name)==0 || strcmp("huffman.o",entry->d_name)==0||
                strcmp("huffman.c",entry->d_name)==0 || strcmp("huffman.h",entry->d_name)==0 || strcmp("HuffmanCodebook",entry->d_name)==0||
		strcmp("readme.pdf",entry->d_name)==0||strcmp("testplan.txt",entry->d_name)==0)	
		continue;
		int name=strlen(entry->d_name);
	if(flag=='b'){

                if( entry->d_name[name-3]=='h' & entry->d_name[name-2]=='c' & entry->d_name[name-1]=='z'){continue;}

		int fileSize;

                if(stat(entry->d_name,&check)==0)
                        fileSize = check.st_size;
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
		
		countFreq(file,frequencies);
		free(file);
	}else if(flag=='c'){
		
		if( entry->d_name[name-3]=='h' & entry->d_name[name-2]=='c' & entry->d_name[name-1]=='z'){continue;}
			Compression(entry->d_name,Hfile);

	}else if(flag=='d'){

		if( entry->d_name[name-3]=='h' & entry->d_name[name-2]=='c' & entry->d_name[name-1]=='z'){
			deCompression(entry->d_name,Hfile);
		}
		}else{
			continue;
		}

	}
    }
   chdir(".."); 
    closedir(d);
	return;
}

int main(int argc, char* argv[])
{	
	//linked list for frequancies
	char head[5];
	strcpy(head,"@#&$");
	data *frequencies=(data*)malloc(sizeof(data));
	frequencies->word=head;
	frequencies->freq=-1;
	frequencies->next=NULL;

	if( strcmp(argv[2],"-R")==0){
		printf("Flags in wrong order, -R should be first\n");
		exit(1); 
	}

	if( strcmp(argv[1],"-R")==0){
		if( strcmp(argv[2],"-b")==0){ // recursive -b flag
			
			findFiles(argv[3],frequencies,'b',argv[2]);
		
		int i =0;
		frequencies=frequencies->next;	
		char **arr=(char**)malloc(sizeof(char *)*wordCount);
		int *freq=(int*)malloc(sizeof(int)*wordCount);
	
		data* trans=frequencies;
		while(trans!=NULL){
			arr[i]=trans->word;	
			*(freq+i)=trans->freq;
			trans=trans->next;
			i++;
		}
        
		for(i=0;i<10;i++){
			chdir("Asst2");
			chdir("..");
			chdir("Asst2");
		}

		remove("HuffmanCodebook");
		getCodeBook(arr,freq,wordCount);	
		free(frequencies);
		free(arr);
		free(freq);

		}else if( strcmp(argv[2],"-c")==0){// recursive -c flag

			int huffman=open("HuffmanCodebook", O_RDONLY);
			struct stat check;
			int HfileSize;
			if(stat("HuffmanCodebook",&check)==0)
			HfileSize=check.st_size;
			char *Hfile=(char*)malloc(sizeof(char)*HfileSize+1);
			read(huffman,Hfile,HfileSize);
			*(Hfile+HfileSize)='\0';
			close(huffman);

			findFiles(argv[3],frequencies,'c',Hfile);

		}else if( strcmp(argv[2],"-d")==0){// recursive -d flag

			int huffman=open("HuffmanCodebook", O_RDONLY);
			struct stat check;
			int HfileSize;
			if(stat("HuffmanCodebook",&check)==0)
			HfileSize=check.st_size;
			char *Hfile=(char*)malloc(sizeof(char)*HfileSize+1);
			read(huffman,Hfile,HfileSize);
			*(Hfile+HfileSize)='\0';
			close(huffman);

			findFiles(argv[3],frequencies,'d',Hfile);

		}
        	
	}
	if( strcmp(argv[1],"-b")==0){// normal -b flag
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

                countFreq(file,frequencies);
		free(file);

	int i =0;
	frequencies=frequencies->next;	
	char **arr=(char**)malloc(sizeof(char *)*wordCount);
	int *freq=(int*)malloc(sizeof(int)*wordCount);
	
	data* trans=frequencies;
	while(trans!=NULL){
		arr[i]=trans->word;	
		*(freq+i)=trans->freq;
		trans=trans->next;
		i++;
	}
        
	getCodeBook(arr,freq,wordCount);	
	free(frequencies);
	free(arr);
	free(freq);


	}else if( strcmp(argv[1],"-c")==0){//normal -c flag





	int huffman=open("HuffmanCodebook", O_RDONLY);

	if(huffman==-1){
		printf("Failed to open a file. \n");
		exit(1);		

	}		

	struct stat check;
	int HfileSize;
	if(stat("HuffmanCodebook",&check)==0)
		HfileSize=check.st_size;
	char *Hfile=(char*)malloc(sizeof(char)*HfileSize+1);
	read(huffman,Hfile,HfileSize);
	*(Hfile+HfileSize)='\0';
	close(huffman);

		Compression(argv[2],Hfile);
	}else if( strcmp(argv[1],"-d")==0){

	int huffman=open("HuffmanCodebook", O_RDONLY);

	if(huffman==-1){
		printf("Failed to open a file. \n");
		exit(1);		

	}		

	struct stat check;
	int HfileSize;
	if(stat("HuffmanCodebook",&check)==0)
		HfileSize=check.st_size;
	char *Hfile=(char*)malloc(sizeof(char)*HfileSize+1);
	read(huffman,Hfile,HfileSize);
	*(Hfile+HfileSize)='\0';
	close(huffman);

		deCompression(argv[2],Hfile);
	}
	
        return 0;
}
