#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void readAll(){
	FILE *file;
	file = fopen("database.txt", "r");
	if (file == NULL){
		printf("Cannot open database\n");
		exit(1);
	}
	char *line_buf = NULL;
    size_t line_buf_size = 0;
    ssize_t lineSize;
    lineSize = getline(&line_buf, &line_buf_size, file);

	while(lineSize >= 0){
        printf("%s", line_buf);
        lineSize = getline(&line_buf, &line_buf_size, file);
	}
	fclose(file);
	line_buf = NULL;
}

void getRecord(char key[]){	
	// read in current database
	FILE *file;
	file = fopen("database.txt", "r");
	if (file == NULL){
        printf("Cannot open database");
		exit(1);
	}
	
    char *line_buf = NULL;
    size_t line_buf_size = 0;
    ssize_t lineSize;
	int keyfound = 0;
		
    lineSize = getline(&line_buf, &line_buf_size, file);
    while(lineSize >= 0){
		char *tempString;
		int num = 0;
		int found = 0;
		
		while( (tempString = strsep(&line_buf, ",")) != NULL){	
			if (num == 0){
				if(strcmp(tempString, key) == 0){
					found = 1;
				}else{
					break;
				}
			}else{
				if (found == 1){
					printf("%s,%s", key, tempString);	
					keyfound = 1;
					break;
				}
			}
			num++;
		}
		
		if(found == 1){
			break;
		}
		
        lineSize = getline(&line_buf, &line_buf_size, file);
	}

	if(keyfound == 0){
		printf("%s not found\n", key);
	}
	line_buf = NULL;
    fclose(file);
}

void clearAll(){
	FILE *file;
	file = fopen("database.txt", "w");
	if(file == NULL){
		printf("Cannot open database\n");
		exit(1);
	}

	fprintf(file, "");

	fclose(file);
}
void writeToFile(char key[], char value[]){
    FILE *file;
    file = fopen("database.txt", "r");
    if (file == NULL){
        printf("Cannot open database\n");
        exit(1);
    }

	FILE *newFile;
	newFile = fopen("tempDatabase.txt", "w");
	if (newFile == NULL){
		printf("Cannot open database\n");
		exit(1);
	}
    
    char *line_buf = NULL;
    size_t line_buf_size = 0;
    ssize_t lineSize;
    lineSize = getline(&line_buf, &line_buf_size, file);
	while(lineSize >= 0){
        char *tempString;
        int num = 0;
        char *tempKey = "";
        while( (tempString = strsep(&line_buf, ",")) != NULL){
            if(num == 0){
                if(strcmp(tempString, key) == 0){
                    break;
                }else{
	                tempKey = tempString;
				}
            }else{
                fprintf(newFile, "%s,%s", tempKey, tempString);
            }
            num++;
        }
        lineSize = getline(&line_buf, &line_buf_size, file);
    }
	fprintf(newFile, "%s,%s\n", key, value);
	int changedName = rename("tempDatabase.txt", "database.txt");
    if (changedName != 0){
        printf("Something went wrong deleting the record\n");
        exit(1);
	}
	fclose(file);
    fclose(newFile);
	line_buf = NULL;
}

void deleteRecord(char key[]){
	int recordFound = 0;
	FILE *file;
	file = fopen("database.txt", "r");
	if (file == NULL){
		printf("Cannot open database\n");
		exit(1);
	}

	FILE *newFile;
	newFile = fopen("tempDatabase.txt", "w");
	if (newFile == NULL){
		printf("Cannot open database\n");
		exit(1);
	}

	char *line_buf = NULL;
	size_t line_buf_size = 0;
	ssize_t lineSize;
	
	lineSize = getline(&line_buf, &line_buf_size, file);
	while(lineSize >= 0){
		char *tempString;
		int num = 0;
		char *tempKey = "";
		
		while( (tempString = strsep(&line_buf, ",")) != NULL){
			if(num == 0){
				if(strcmp(tempString, key) == 0){
					recordFound = 1;
					break;
				}else{
					tempKey = tempString;
				}
			}else{
				fprintf(newFile, "%s,%s", tempKey, tempString);
			}
			num++;
		}
		lineSize = getline(&line_buf, &line_buf_size, file);
	}
	
	int changedName = rename("tempDatabase.txt", "database.txt");
	if (changedName != 0){
		printf("Something went wrong deleting the record\n");
		exit(1);
	}

	fclose(file);
	fclose(newFile);
	line_buf = NULL;

	if (recordFound == 0){
		printf("%s not found\n", key);
	}
}

int main(int argc, char* argv[]){	
	// read in commands and process the
	int put = 0;
	int delete = 0;
	int get = 0;
	for (int i = 1; i < argc; i++){

		if(put == 1 || delete == 1 || get == 1){
			printf("bad command\n");
			put = 0;
			delete = 0;
			get = 0;
		} 
	
		char *comps;
		int counter = 0;
		char *key = "";
		char *value = "";
		int badKey = 0;
		char *wholeString;
		wholeString = malloc(strlen(argv[i]) + 1);
		strcpy(wholeString, argv[i]);
		
		while( (comps = strsep(&argv[i], ",")) != NULL ){
			if (counter == 0){
				// initial command
				if(strcmp(comps, "p") == 0){
					put = 1;
				}else if(strcmp(comps, "g") == 0){
					get = 1;
					int numCommas = 0;
					for(int j = 0; j < strlen(wholeString); j++){
						if(wholeString[j] == ','){
							numCommas++;
						}
					}
					if(numCommas != 1){
						badKey = 1;
						get = 0;
					}
				}else if(strcmp(comps, "a") == 0){
					readAll();
				}else if(strcmp(comps, "d") == 0){
					delete = 1;
					int numCommas = 0;
					for(int j = 0; j < strlen(wholeString); j++){
						if (wholeString[j] == ','){
							numCommas++;
						}
					}
					if(numCommas != 1){
						badKey = 1;
						delete = 0;
					}
				}else if(strcmp(comps, "c") == 0){
					clearAll();
				}else{
					printf("bad command\n");
				}
			}else if (counter == 1) {
				// key
				key = comps;

				for (int j = 0; j < strlen(key); j++){
					if (isdigit(key[j]) == 0){
						badKey = 1;
						break;
					}
				}

				if(get == 1){
					getRecord(key);
					get = 0;
				}else if(delete == 1){
					deleteRecord(key);
					delete = 0;
				}
			}else {
				// value
				value = comps;
				if (put == 1){
					// go to put
					writeToFile(key, value);
					put = 0;
					
				}else{
					if(badKey != 1){
						printf("bad command\n");
					}
				}
			}
			counter++;
		}
		free(wholeString);	
		if (badKey == 1){
			printf("bad command\n");
		}
		
	}
	if(put == 1 || delete == 1 || get == 1){
		printf("bad command\n");
	}
}
