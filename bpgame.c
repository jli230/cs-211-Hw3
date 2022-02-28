
// file:  bpgame.c


/** #include statements... **/
#include <stdlib.h>
#include <stdio.h>
#include "bpgame.h"


/** TYPEDEFS / STRUCTs HERE ***/


struct bpgame {


   // YOU DECIDE WHAT TO PUT IN HERE TO CAPTURE THE STATE OF
   //   A "RUN" OF THE GAME.....
   int rows;
   int cols;
   char **board;
};





/*** IMPLEMENTATION OF bp_XXXX FUNCTIONS HERE  ****/

extern BPGame * bp_create(int nrows, int ncols){
   printf("testing bp\n");
   if (nrows>MAX_ROWS || ncols>MAX_COLS) {
      printf("Exceeded maximum values for dimensions.\n");
      return NULL;
   }
   char **newboard= (char **)malloc(nrows*sizeof(char *));
   for (int i = 0; i < nrows; ++i) {
      newboard[i] = (char*)malloc(ncols*sizeof(char));
   }
   printf("testing bp\n");
   BPGame * bp = (BPGame*)malloc(sizeof(BPGame));
   bp->rows = nrows;
   bp->cols = ncols;
   bp->board = newboard;
   return bp;
}
extern BPGame * bp_create_from_mtx(char mtx[][MAX_COLS], int nrows, int ncols){
   if (nrows>MAX_ROWS || ncols>MAX_COLS) {
      printf("Exceeded maximum values for dimensions.\n");
      return NULL;
   } else if (nrows < 0 || ncols < 0) {
      printf("Invalid row or column dimensions.\n");
      return NULL;
   }
   char **newboard= (char **)malloc(nrows*sizeof(char *));
   for (int i = 0; i < nrows; ++i) {
      newboard[i] = (char*)malloc(ncols*sizeof(char));
   }
   BPGame * bp = (BPGame*)malloc(sizeof(BPGame));
   bp->rows = nrows;
   bp->cols = ncols;
   bp->board = newboard;
   for (int row = 0; row < nrows; row++) {
      for (int col = 0; col < ncols; col++) {
         newboard[row][col] = mtx[row][col];
      }
   }
   return bp;
}

extern void bp_destroy(BPGame * b){
   for (int i = 0; i < b->rows; ++i) {
      free(b->board[i]);
   }
}

int main() {
   struct bpgame *newbp = bp_create(35,40);
   newbp->board[2][2] = 'a';
   printf("testing");
   printf("Character at 2 2 %c\n Rows: %i Cols: %i\n", newbp->board[2][2], newbp->rows, newbp->cols);
   bp_destroy(newbp);
   printf("Character at 2 2 %c\n", newbp->board[2][2]);
}