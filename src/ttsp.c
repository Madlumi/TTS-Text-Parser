#include <ctype.h>
#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int done=0;
int section = 0;

double speed=3.0;
int volume=100;

char * cleanLine(char * str){
   char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator character
  end[1] = '\0';

  return str;

}
char **splitTxt(char *file){
   char ** ProcessedLines= malloc(sizeof(char *) * 1024);
   int ProcessedI = 0;
	//boring var declarations
	ssize_t read;
	char dels[3]=".,";
	char *subline;

   printf("a1\n");
	//loop to convert each line
	char * line = NULL;
	size_t len = 0;
	FILE * InputFile = fopen((char *)file, "r");
	if (InputFile == NULL){
      printf("error opening file %s \n", (char *)file);
      return NULL;
   }
   while ((read = getline(&line, &len, InputFile)) != -1) {
      if(strlen(line)<2){continue;}
      subline = strtok(line, dels);
      for (;subline!=NULL;subline = strtok(NULL, dels)){
         if(strlen(subline)<2){continue;}
         if(asprintf(&ProcessedLines[ProcessedI], cleanLine(subline)) > 1){
            printf("%s\n",ProcessedLines[ProcessedI]);
            ProcessedI+=1;
         }

      }
   }
   fclose(InputFile);
   if (line){free(line);}
   return ProcessedLines;
}
void txtToAud(void *tt){

}
void *toaud(void *tt){
   //TODO OLD
   char ** lines = splitTxt(tt);
   char * l=lines[0];
   for(int i = 0; !strcmp(l, "");i++,l=lines[i] ){
      printf("%s\n", l);
   }
   free(lines);
   return NULL;;

	
	//command to run to convert to tts
	char ttsCommand[]="echo \"%s:.........:\" | text2wave -f 32000 -o \"ttsOut%d.wav\" %s ";
	char ttsAppend[]="-eval '(voice_cmu_us_slt_arctic_hts)'";
	//char ttsAppend[]="";
	
	done=1;
	return NULL;
}

void speak(){
	int sectionSpeaking=0;
	while(1){
		if(section>sectionSpeaking){
			//printf("b %d >= %d\n",section,sectionSpeaking);
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
   // TODO allow more arguments
   // if one argument, assume that to be input file, otherwise use stdin
	if(argc<2){
		pthread_create(&t_id, NULL,toaud, "/dev/stdin\0");
	}else{
		pthread_create(&t_id, NULL,toaud, argv[1]);
	}
	//todo make  args

	//convert tts
	//speak tts
	speak();	
	//done
	pthread_join(t_id, NULL);
	printf("Done\n");
	system("rm -f *ttsOut*");
	exit(0);
	return 1;
}
#endif
