#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>

int main(int argc, char **argv){

DIR *directory = opendir(".");
struct dirent *directories;

if(directory){

while((directories = readdir(directory)) != NULL){
        printf("%s\n", directories->d_name);
}
close(directory);
}

return 0;
}
