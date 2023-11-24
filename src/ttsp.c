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

void *toaud(void *tt){
	printf("%s\n",(char *)tt);

	//boring var declarations
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	
	char *subline;
	char dels[2]=".";
	//command to run to convert to tts
	char ttsCommand[]="echo \"%s:.........:\" | text2wave -f 32000 -o \"ttsOut%d.wav\" %s ";
	char ttsAppend[]="-eval '(voice_cmu_us_slt_arctic_hts)'";
	//char ttsAppend[]="";
	
	//loop to convert each line
	fp = fopen((char *)tt, "r");
	if (fp != NULL){
		while ((read = getline(&line, &len, fp)) != -1) {
			if(strlen(line)<2){continue;}
			subline = strtok(line, dels);
			while(subline!=NULL){

				if(strlen(subline)<2){
					subline = strtok(NULL, dels);
					continue;
				}
				char *s;
				if(0 > asprintf(&s, ttsCommand,subline, section, ttsAppend)){
					done=1;
					printf("error converting to audio\n");
					printf("%s\n",s);
					return NULL;
				}
				//printf("%s\n",s);
				system(s);
				free(s);
				section+=1;
				subline = strtok(NULL, dels);
			}
		}
		fclose(fp);
		if (line){free(line);}
	}else{printf("error opening file %s \n", (char *)tt);}
	printf("\ndone--------------------------\n");
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

#ifndef UNIT_TESTING
int main(int argc, char *argv[]){
	system("rm -f *ttsOut*");
	pthread_t t_id;
	if(argc!=2){
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
