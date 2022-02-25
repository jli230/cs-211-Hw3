
/**
* Very simple program giving demo of very crude terminal
*    based animation.
*
* Key ideas:
*
*   using nanosleep library function to control animation
*	rate.
*   mechanism for clearing terminal by printing a magic
*	character to the terminal.
*
*   What it does:  displays a message which moves back and
*	forth (left and right); animation moves message
*	one column left of right for each "frame"
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>   // for nanosleep

#define SLEEP_MS 200  // defines delay between "frames" in
		      //   animation


/**
* makes program sleep the given number of milliseconds
*
* Citation:  adapted from stackoverflow:
*
*    http://stackoverflow.com/a/28827188
*/
void sleep_ms(int milliseconds) // cross-platform sleep function
{
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = milliseconds * 1000000;
    nanosleep(&ts, NULL);
}



void clear(){

  printf("\033c\n");

}

void pspaces(int n) {
  while(n) {
	printf(" ");
	n--;
  }
}


void animate(char *msg) {
int lboundary = 10;
int steps = 8;
int i;

  clear();
  while(1) {
    for(i = lboundary; i< lboundary+steps; i++){
	pspaces(i);
	printf("%s\n",  msg);
	sleep_ms(SLEEP_MS);
	clear();
    }
    for(; i>lboundary; i--){
	pspaces(i);
	printf("%s\n",  msg);
	sleep_ms(SLEEP_MS);
	clear();
    }
  }
}
	
	

int main(){

char *message = "A MAN A PLAN A CANAL PANAMA";

  animate(message);
}
  
