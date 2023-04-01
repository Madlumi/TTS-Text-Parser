#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
int done=0;
int section = 0;
double speed=2.5;
int volume=130;

void *toaud(void *tt){
	printf("%s\n",(char *)tt);

	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	//
	char ttsCommand[]="echo \"%s:.....................:\" | text2wave -f 32000  -o \"ttsOut%d.wav\" %s ";
	char ttsAppend[]="-eval '(voice_cmu_us_slt_arctic_hts)'";
	fp = fopen((char *)tt, "r");
	if (fp != NULL){
		while ((read = getline(&line, &len, fp)) != -1) {
			char *s;
			//printf("%s\n",line);
			if(line[0]=='\n'){continue;}
			if(0 > asprintf(&s, ttsCommand,line, section, ttsAppend)){
			       	done=1;
				printf("error converting to audio\n");
				printf("%s\n",s);
				return NULL;
			}
			//printf("%s\n",s);
			system(s);
			free(s);
			section+=1;
		}
		fclose(fp);
		if (line){free(line);}
	}else{printf("error opening file %s \n", (char *)tt);}
	done=1;
	return NULL;
}

void speak(){
	int sectionSpeaking=0;
	while(1){
		if(section>sectionSpeaking){
			//printf("b %d >= %d\n",section,sectionSpeaking);
			char *ss;
			if(0 > asprintf(&ss,"mpv --speed=%f --volume=%d ttsOut%d.wav",speed,volume, sectionSpeaking)){continue;}
			sectionSpeaking+=1;
			system(ss);
			free(ss);
		}
		if(done && section==sectionSpeaking+1){
			break;
		}
	}

}

int main(int argc, char *argv[]){
	if(argc!=2){
		printf("needs 1 args, not %d\n", argc-1);
		return 1;
	}
	//todo make  args
	system("rm -f *ttsOut*");
	
	//convert tts
	pthread_t t_id;
	pthread_create(&t_id, NULL,toaud, argv[1]);
	//speak tts
	speak();	
	//done
	pthread_join(t_id, NULL);
	printf("Done\n");
	system("rm -f *ttsOut*");
	exit(0);
	return 1;
}
