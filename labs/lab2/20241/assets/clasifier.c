#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int na, char *argv[]){
	int n,i,pid;
	
	if(na != 2) {
		fprintf(stderr, "Uso: %s <n>\n",argv[0]);
		exit(1);
	}
	n = atoi(argv[1]);
	for(i=0;i<3;i++) {
		if(!(pid = fork())) {
			int j,r, mypid;
			
			mypid = getpid();
			srand(mypid);			
			for(j=0;j<n;j++) {
			    r = rand() % 528;
			    printf("%d %d\n",mypid,r);
			}    
			exit(0);    
		}	
	}
	exit(0);	
}	


	
