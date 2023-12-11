#include <ctype.h>
#include <uchar.h>
#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//command for festival text-to-speech to file
static const char ttsCommand[] = "echo \"%s:.........:\" | text2wave -f 32000 -eval '(voice_cmu_us_slt_arctic_hts)'";
	//char ttsCommand[]="echo \"%s:.........:\" | text2wave -f 32000 -o \"ttsOut%d.wav\" ";
double speed=3.0;
int volume=100;

char * trimLine(char * str){
   char *end;
  // Trim leading space
  while(isspace((unsigned char)*str)) str++;
  if(*str == 0){return str;}
  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;
  end[1] = '\0';
  return str;
}
char **splitTxt(char *file){
	int max_lines=4096;
	char dels[3]=".,";
	char ** ProcessedLines= malloc(sizeof(char *) * max_lines);
	int ProcessedI = 0;
	ssize_t read;
	char * subline;
	char * line = NULL;
	size_t len = 0;
	FILE * InputFile = fopen((char *)file, "r");
	if (InputFile == NULL){printf("error opening file %s \n", (char *)file);return NULL;}
	while ((read = getline(&line, &len, InputFile)) != -1) {
		if(strlen(line)<2){continue;}
		subline = strtok(line, dels);
		for (;subline!=NULL;subline = strtok(NULL, dels)){
			if(strlen(subline)<2){continue;}
			if(asprintf(&ProcessedLines[ProcessedI], trimLine(subline)) > 1){
				ProcessedI+=1;
				if (ProcessedI>=max_lines){break;}
			}
		}
		if (ProcessedI>=max_lines){break;}
	}
	fclose(InputFile);
	if (line){free(line);}
	return ProcessedLines;
}
char * strToCommand(char *txt){
	char * parcedtxt;
	asprintf(&parcedtxt,ttsCommand ,txt);
	return parcedtxt;;
}
int done=0;
int section = 0;
FILE * temp = tmpfile();
void *textToAudio(void *tt){
	char ** lines = splitTxt(tt);
	for(section = 0; lines[section];section++){
		temp = tmpfile();
		printf(":%s\n", lines[section]);
		system(strToCommand(lines[section]));
		free(lines[section]);
	}
	done=1;
	free(lines);
	return NULL;
}

void speak(){
	int sectionSpeaking=0;
	while(1){
		if(section>sectionSpeaking){
			char *ss;
			if(0 > asprintf(&ss,"mpv -ao=alsa --speed=%f --volume=%d ttsOut%d.wav",speed,volume, sectionSpeaking)){continue;}
			sectionSpeaking+=1;
			system(ss);
			free(ss);
		}else{
		}
		if(done && section==sectionSpeaking){
			break;
		}
	}
}
void clean(){
   //real unsafe stuff, TODO make temp folder
	system("rm -f *ttsOut*");
}

#ifndef UNIT_TESTING
int main(int argc, char *argv[]){
	pthread_t t_id;
	clean();
   	// if one argument, assume that to be input file, otherwise use stdin
	if(argc<2){
		pthread_create(&t_id, NULL,textToAudio, "/dev/stdin\0");
	}else{
		pthread_create(&t_id, NULL,textToAudio, argv[1]);
	}
	//speak();	
	//done
	pthread_join(t_id, NULL);
	printf("Done\n");
	system("rm -f *ttsOut*");
	exit(0);
	return 1;
}
#endif
