
// file:  bpgame.c


/** #include statements... **/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "bpgame.h"


/** TYPEDEFS / STRUCTs HERE ***/


struct bpgame {


   // YOU DECIDE WHAT TO PUT IN HERE TO CAPTURE THE STATE OF
   //   A "RUN" OF THE GAME.....
   int rows;
   int cols;
   char **board;
   char **prevboard;
};





/*** IMPLEMENTATION OF bp_XXXX FUNCTIONS HERE  ****/

extern BPGame * bp_create(int nrows, int ncols){
   printf("testing bp\n");
   char assets[5] = {None, Red, Blue, Green, Yellow};
   if (nrows>MAX_ROWS || ncols>MAX_COLS) {
      printf("Exceeded maximum values for dimensions.\n");
      return NULL;
   }
   char **newboard = (char **)malloc(nrows*sizeof(char *));
   char **previousboard = (char **)malloc(nrows*sizeof(char *));
   for (int i = 0; i < nrows; ++i) {
      newboard[i] = (char*)malloc(ncols*sizeof(char));
      previousboard[i] = (char*)malloc(ncols*sizeof(char));
   }
   srand(time(NULL));
   for (int row = 0; row < nrows; row++) {
      for (int col = 0; col < ncols; col++) {
         newboard[row][col] = assets[rand()%5];
         previousboard[row][col] = newboard[row][col];
      }
   }
   BPGame * bp = (BPGame*)malloc(sizeof(BPGame));
   bp->rows = nrows;
   bp->cols = ncols;
   bp->board = newboard;
   bp->prevboard = previousboard;
   return bp;
}

int verify (char compared) {
   char assets[5] = {None, Red, Blue, Green, Yellow};
   for (int i = 0; i < 5; i++) {
      printf("Character checked: %c\n", assets[i]);
      if (compared == assets[i]){
         return 1;
      }
   }
   return 0;
}

extern BPGame * bp_create_from_mtx(char mtx[][MAX_COLS], int nrows, int ncols){
    char assets[5] = {None, Red, Blue, Green, Yellow};
   if (nrows>MAX_ROWS || ncols>MAX_COLS) {
      printf("Exceeded maximum values for dimensions.\n");
      return NULL;
   } else if (nrows < 0 || ncols < 0) {
      printf("Invalid row or column dimensions.\n");
      return NULL;
   }
   char **newboard = (char **)malloc(nrows*sizeof(char *));
   char **previousboard = (char **)malloc(nrows*sizeof(char *));
   for (int i = 0; i < nrows; ++i) {
      newboard[i] = (char*)malloc(ncols*sizeof(char));
      previousboard[i] = (char*)malloc(ncols*sizeof(char));
   }
   BPGame * bp = (BPGame*)malloc(sizeof(BPGame));
   bp->rows = nrows;
   bp->cols = ncols;
   bp->board = newboard;
   for (int row = 0; row < nrows; row++) {
      for (int col = 0; col < ncols; col++) {
         newboard[row][col] = mtx[row][col];
         printf("Asset to check: %c\n", mtx[row][col]);
         if (verify(mtx[row][col]) != 1) {
            printf("Invalid character detected! %i %i", row, col);
            return NULL;
         } else {
            newboard[row][col] = mtx[row][col];
         }
      }
   }
   return bp;
}

extern void bp_destroy(BPGame * b){
   for (int i = 0; i < b->rows; ++i) {
      free(b->board[i]);
   }
   free(b->board);
}

extern void bp_display(BPGame * b) {
   printf("  +-");
   for (int i = 0; i < b->cols; i++) {
      printf("----");
   }
   printf("-+\n");
   for (int row = 0; row < b->rows; row++) {
      printf("%i |  ", row);
      for (int col = 0; col < b->cols; col++) {
         printf("%c   ", b->board[row][col]);
      }
      printf("|\n");
   }
   printf("  +-");
   for (int i = 0; i < b->cols; i++) {
      printf("----");
   }
   printf("-+\n    ");
   for (int col = 0; col < b->cols; col++) {
      if (col < 10) {
         printf("0%i  ", col);
      } else {
         printf("%i  ", col);
      }
   }
   printf("\n");
}

int main() {
   struct bpgame *newbp = bp_create(10,10);
   newbp->board[2][2] = None;
   printf("testing");
   printf("Character at 2 2 %c\n Rows: %i Cols: %i\n", newbp->board[2][2], newbp->rows, newbp->cols);
   bp_display(newbp);
   char m[40][40];
   m[0][0] = '.';
   m[0][1] = '.';
   m[0][2] = '^';
   m[1][0] = '.';
   m[1][1] = 'b';
   m[1][2] = '.';
   struct bpgame *nextbp = bp_create_from_mtx(m,2, 3);
   //bp_destroy(newbp);
   if (nextbp != NULL) {
      bp_display(nextbp);
   }
   printf("Character at 2 2 %c\n", newbp->board[2][2]);
}