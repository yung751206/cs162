#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void wc(FILE *ofile, FILE *infile, char *inname) {
	int c;
	int byte_count = 0;int word_count = 0;
	int line_count = 0;int  word_flag = 0;
	if(!infile){
		char *line = NULL;
		size_t len = 0;
		ssize_t read;
		while( (read = getline(&line,&len,stdin)) != -1){
			size_t size = strlen(line);
			int i;
			for(i=0;i<size;i++){	
				byte_count++;
				char c = line[i];
				if( (c == ' ' || c == '\n')  && word_flag){
					word_count++;
					word_flag = 0;
				}
				else if (c != ' ' || c != '\n'){
					word_flag = 1;
				}
				if(c == '\n'){
					line_count++;
				}
			}	
		}
		free(line);
	}			
	else if(infile){
		while( (c=fgetc(infile)) != EOF ){
			byte_count++;
			if( (c == ' ' || c == '\n')  && word_flag){
				word_count++;
				word_flag = 0;
			}
			else if (c != ' ' || c != '\n'){
				word_flag = 1;
			}
			if(c == '\n'){
				line_count++;
			}
		}
		fclose(infile);
	}
	if(ofile){
		fprintf(ofile,"%d %d %d %s\n",line_count,word_count,byte_count,inname);	
		fclose(ofile);
	}
	else{
		if(inname)
			printf("%d %d %d %s\n",line_count,word_count,byte_count,inname);
		else	
			printf("%d %d %d\n",line_count,word_count,byte_count);
	}
}

int main (int argc, char *argv[]) {
	char* out_file_name = NULL;
	char* inname = NULL;
	if(argc == 1) wc(NULL,NULL,NULL);
	else if(argc == 2){ 
		inname = argv[1];
		FILE *infile = fopen(inname,"r");
		if(!infile){
			perror("Input file opening failed");
			exit(1);
		}
		wc(NULL,infile,inname);
	}
	else if(argc == 3){	
		inname = argv[1];
		FILE *infile = fopen(inname,"r");
		if(!infile){
			perror("Input file opening failed");
			exit(1);
		}
		FILE *out_file = fopen(argv[2],"w");
		if(!out_file){
			perror("Output file opening failed");
			exit(1);
		}
		wc(out_file,infile,inname);
	}
	else{
		printf("arguments error\n");
		exit(1);
	}
	return 0;
}
