#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
int done=0;
int section = 0;
void *toaud(void *vargp){


	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	char fileToRead="";

	char ttsCommand[]="echo \"%s\" | text2wave -f 32000  -o \"ttsOut%d.wav\" %s ";
	char ttsAppend[]="-eval '(voice_cmu_us_slt_arctic_hts)'";
	fp = fopen("/home/madanie/scripts/run/ttsc/ttstxt", "r");
	if (fp != NULL){
		while ((read = getline(&line, &len, fp)) != -1) {
			char *s;
			if(0 > asprintf(&s, ttsCommand,line, section, ttsAppend)){
			       	done=1;
				printf("error converting to audio\n");
				printf("%s\n",s);
				return NULL;
			}
			system(s);
			free(s);
			section+=1;
		}
		fclose(fp);
		if (line){free(line);}
	}else{printf("error opening file %s \n", fileToRead);}
	done=1;
	return NULL;
}
void *listen(void *vargp){
	return NULL;
}

void speak(double speak, double volume){
	int sectionSpeaking=0;
	while(1){
		if(section<=sectionSpeaking){
			//printf("waiting...\n");
		}else{
			sectionSpeaking+=1;
			printf("DING\n");
		}
		if(done){
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
	double speed=2.5;
	double volume=1.0;

	//convert tts
	pthread_t t_id;
	pthread_create(&t_id, NULL,toaud, NULL);
	speak(speed,volume);	
	//speak tts
	//done
	pthread_join(t_id, NULL);
	printf("After Thread\n");
	exit(0);
	printf("hello World");
	return 1;
}
