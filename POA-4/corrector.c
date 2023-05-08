#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <dirent.h>

#include <assert.h>

#include <sys/stat.h>



#define BUFFER_SIZE 1024

//Arif Kemal Sarı 28999



int number_person = 0;
char ***people;





const char *txt_or_not(const char *filename) {
    
    const char *ext =  strrchr(filename, '.');
    if(!ext||ext==".")return"-";
    return ext+1;
    }
    
void writefile_indir(const char* dirname) {
    DIR* dir = opendir(dirname);
    if (dir == NULL) {
        return;
    }
    printf("Reading files in: %s\n", dirname);
    struct dirent* file;
    
    file = readdir(dir);
    while (file != NULL) {
        
        if (file->d_type == DT_DIR && strcmp(file->d_name, ".") != 0 && strcmp(file->d_name, "..") != 0) {
            
            char * str = malloc(strlen(dirname) + strlen(file->d_name) + 1);
            strcpy(str,dirname); strcat(str,"/"); strcat(str,file->d_name);
            writefile_indir(str);
            free(str);
            
        }
        
        if (strcmp(txt_or_not(file->d_name), "txt") == 0 && (strcmp(file->d_name, "database.txt") != 0 || strcmp(dirname,".") != 0)){
            
            char * file_name = malloc(strlen(dirname) + strlen(file->d_name) + 1);
            
            strcpy(file_name ,dirname); strcat(file_name ,"/"); strcat(file_name ,file->d_name);
            
            FILE * Fb = fopen(file_name,"r+");
            free(file_name);
            if (Fb== NULL){
                
                printf("Could not open txt file.");
                return;
            }
            
            // Allocate a buffer to hold each line of the file
            char *line = NULL;
            size_t len = 0;
            char token [1000];
            // Read each line of the file
            
            while(fscanf(Fb, "%s", token) > 0){
                // Tokenize the line using space as the delimiter
                
                for (int i = 0; i < number_person; i++) {
                    if(strcmp(people[i][1],token)==0){
                        char* gender;
                        if(strcmp(people[i][0], "m") == 0){
                            gender = "Mr.";}
                        else{ gender = "Ms.";
                        }
                        
                        int size_name = strlen(people[i][1]);
                        int location = ftell(Fb) - size_name;
                        fseek(Fb,location - 4 ,SEEK_SET);
                        fputs(gender,Fb);
                        fseek(Fb,ftell(Fb)+size_name + 2 ,SEEK_SET);
                        fputs(people[i][2],Fb);
                        }
                    }
                }
                free(line);
                fclose(Fb);
            }
            file = readdir(dir);
        }
    closedir(dir);
}





int main(int argc, char* argv[]){
    
    FILE *fp;
    
    char buffer[BUFFER_SIZE];
    
    fp = fopen("database.txt", "r");
    
    if (fp == NULL) {
        
        printf("Could not open the database.txt file.");
        return -1;
    }
    while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
    
    number_person++;
    }
    people = malloc(number_person * sizeof(char*));
    for (int i = 0; i < number_person; i++) {
        people[i] = malloc(3 * sizeof(char*));
        people[i][0] = malloc(sizeof(char*));
        people[i][1] = malloc(sizeof(char*));
        people[i][2] = malloc(sizeof(char*));
    }
    
    fseek(fp,0,SEEK_SET);
    int i = 0;
    while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
        printf("%s",buffer);
        char * gender = strtok(buffer, " ");
        char * name = strtok(NULL, " ");
        char * surname = strtok(NULL, " ");
        strcpy(people[i][0],gender);
        strcpy(people[i][1],name);
        strcpy(people[i][2],surname);
        i++;
    }
    fclose(fp);
    writefile_indir(".");
    return 0;

}









