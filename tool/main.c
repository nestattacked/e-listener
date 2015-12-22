#include<stdio.h>
#define BUFFER_SIZE 200

int dealSubtitle(const char *filename, const char *outfilename){
	FILE *infp = NULL;
	FILE *outfp = NULL;
	infp = fopen(filename, "r");
	outfp = fopen(outfilename,"w");
	if(infp == NULL || outfp == NULL)
		return -1;
	char buffer[BUFFER_SIZE];
	while(!feof(infp)){
		fgets(buffer, BUFFER_SIZE, infp);
		fgets(buffer, BUFFER_SIZE, infp);
		buffer[12]='\0';
		fputs(buffer, outfp);
		fputc('\n', outfp);
		fputs(buffer+17, outfp);
		fgets(buffer, BUFFER_SIZE, infp);
		fgets(buffer, BUFFER_SIZE, infp);
		fputs(buffer,outfp);
		fgets(buffer, BUFFER_SIZE, infp);
	}
	fclose(infp);
	fclose(outfp);
	return 0;
}

int main(){
	int res = dealSubtitle("The.Big.Bang.Theory.S01E01.720p.BluRay.DD5.1.x264-DON.srt","The.Big.Bang.Theory.S01E01.720p.BluRay.DD5.1.x264-DON.srts");
	if(res == -1)
		return -1;
	else
		return 0;
}
