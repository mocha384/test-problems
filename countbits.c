#include <stdio.h>
#include <sys/time.h>

#define STRLEN 1024
#define FILESTART "<FILE START>"
#define FILEEND   "<FILE END>"

int NumberOfBitsInNum(int i)
{
    i = i - ((i >> 1) & 0x55555555);
    i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
    return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

int main(int argc, char* argv[]) {
	char *revstring = NULL;
	int filestart_found = 0;
	int totalBits = 0;
	if(2 != argc) {
		printf("Usage: reverse input.txt");
		return 1;
	}
	
	struct timespec spec1,spec2;
	clock_gettime(CLOCK_REALTIME,&spec1);
	FILE *p = fopen(argv[1],"r");
	if (NULL == p) {
		printf("File cannot be opened");
		return 1;
	}
	char line[STRLEN] = "";
	while ( fgets(line, STRLEN,p) != NULL ){
		if ( 0 == filestart_found && 0 == strncmp(line,FILESTART,sizeof(FILESTART)-1)) {
			filestart_found = 1;
			continue;
		}
		if(0 == filestart_found) 
			continue;
		if(1 == filestart_found && 0 == strncmp(line,FILEEND, sizeof(FILEEND)-1)) {
			break;
		}
		line[strlen(line)-2] = '\0'; //convert newline to null terminated
		//printf("%s %d ",line,strlen(line));
		int num = atoi(line);
		if (0 != num) {
			totalBits += NumberOfBitsInNum(num);
			printf("%10d:%2d",num,NumberOfBitsInNum(num));
			clock_gettime(CLOCK_REALTIME,&spec2);
			printf(":%ld\n",spec2.tv_nsec-spec1.tv_nsec);
			spec1 = spec2;
		}
		if(NULL != revstring)
			free(revstring);
	}
	printf("Total Bits : %d", totalBits);
	close(p);
	return 0;
}
