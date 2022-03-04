/**
 * Author: Justin Li, 669651335, Spring 2022
 * 
 * Program for the ADTs of balloon pop game
 * 
 * /
// file:  bpgame.c


/** #include statements... **/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "bpgame.h"


/** TYPEDEFS / STRUCTs HERE ***/
typedef struct bpstack ElemType;
#define FORMAT_STRING "  %lf\n"
#define CAPACITY 128

struct bpstack {
   int prevscore;
   char **prevboard;
};

struct bpgame {


   // YOU DECIDE WHAT TO PUT IN HERE TO CAPTURE THE STATE OF
   //   A "RUN" OF THE GAME.....
   int rows;
   int cols;
   char **board;
   int score;
   struct stack_struct *boardstack;
}BPGAME;

/*** STACK IMPLEMENTATION HERE ***/

struct stack_struct {
    ElemType items[CAPACITY];
    int top;
};

typedef struct stack_struct *StackPtr;

StackPtr stk_create(){
    StackPtr s = malloc(sizeof(struct stack_struct));

    s->top = -1;  // stack initially empty
    return s;
}

// TODO
// StackPtr stk_clone(StackPtr s) {


//   return NULL;  // temporary placeholder



// }

void stk_free(StackPtr s) {
    free(s);
}

int stk_push(StackPtr s, ElemType val){
    if(s->top == CAPACITY - 1)
	return 0;
    s->top++;
    s->items[s->top] = val;
    return 1;
}

ElemType stk_pop(StackPtr s){
    if(s->top == -1)
        abort();  // library function which terminates program!!!
    s->top--;
    return s->items[s->top+1];
}

int stk_is_full(StackPtr s){
    return s->top == CAPACITY-1;
}

int stk_is_empty(StackPtr s){
    return s->top == -1;
}

int stk_size(StackPtr s) {
    return s->top+1;
}

void stk_clear(StackPtr s){
    s->top = -1;
}

// void stk_print(StackPtr s) {
//     int i;
//     printf("\n----TOP-----\n");

//     for(i=s->top; i>=0; i--) {
//         bp_display(s->items[i]);
//     }
//     printf("---BOTTOM---\n");
// }
/*** IMPLEMENTATION OF bp_XXXX FUNCTIONS HERE  ****/

void bp_display_STD(BPGame *b) {
   int nrows = b->rows;
   int ncols = b->cols;
    for (int row = 0; row < nrows; row++) {
        for (int col = 0; col < ncols; col++) {
            printf("%c", b->board[row][col]);
        }
        printf("\n");
    }
}


extern BPGame * bp_create(int nrows, int ncols){
   char assets[4] = {Red, Blue, Green, Yellow};
   if (nrows>MAX_ROWS || ncols>MAX_COLS) {
      printf("Exceeded maximum values for dimensions.\n");
      return NULL;
   }
   char **newboard = (char **)malloc(nrows*sizeof(char *));
   for (int i = 0; i < nrows; ++i) {
      newboard[i] = (char*)malloc(ncols*sizeof(char));
   }
   srand(time(NULL));
   for (int row = 0; row < nrows; row++) {
      for (int col = 0; col < ncols; col++) {
         newboard[row][col] = assets[rand()%4];
      }
   }
   BPGame * bp = (BPGame*)malloc(sizeof(BPGame));
   bp->rows = nrows;
   bp->cols = ncols;
   bp->board = newboard;
   bp->score = 0;
   // while(bp_is_compact(bp) != 1) {
   //    bp_float_one_step(bp);
   // }
   bp->boardstack = stk_create();
   return bp;
}

int verify (char compared) {
   char assets[5] = {None, Red, Blue, Green, Yellow};
   for (int i = 0; i < 5; i++) {
      //printf("Character checked: %c\n", assets[i]);
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
         //printf("Asset to check: %c\n", mtx[row][col]);
         if (verify(mtx[row][col]) != 1) {
            printf("Invalid character detected! %i %i\n", row, col);
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
   stk_free(b->boardstack);
   free(b->board);
}

int cluster_pop(BPGame * b, int r, int c){
   int nrows = b->rows;
   int ncols = b->cols;
   char bal = b->board[r][c];
   int clusterscore = 0;
   if (b->board[r][c] == '.') {
      return 0;
   }
   clusterscore += 1;
   b->board[r][c] = '.';
   if(r != nrows-1 && bal == b->board[r+1][c]){
      clusterscore += cluster_pop(b, r+1, c);
   }
   if(r !=0 && bal == b->board[r-1][c]){
      clusterscore += cluster_pop(b, r-1, c);
   }
   if(c !=ncols-1 && bal == b->board[r][c+1]){
      clusterscore += cluster_pop(b, r, c+1);
   }
   if(c !=0 && bal == b->board[r][c-1]){
      clusterscore += cluster_pop(b, r, c-1);
   }
   return clusterscore;
}

int neighborcheck(BPGame * b, int r, int c){
   char balloon = b->board[r][c];
   if(r >= 1) {
      if (b->board[r-1][c] == balloon) {
         return 1;
      }
   }
   if(r <= b->rows-2) {
      if (b->board[r+1][c] == balloon) {
         return 1;
      }
   }
   if(c >= 1) {
      if (b->board[r][c-1] == balloon) {
         return 1;
      }
   }
   if(c <= b->cols-2) {
      if (b->board[r][c+1] == balloon) {
         return 1;
      }
   }
   return 0;
}

extern int bp_pop(BPGame * b, int r, int c){
   printf("Checking coordinate (%i, %i)\n", r, c);
   int nrows = b->rows;
   int ncols = b->cols;
   char bal = b->board[r][c];
   int clusterscore = 0;
   if (bal == '.' || neighborcheck(b, r, c) == 0) {
      return 0;
   }
   struct bpstack * entry = (struct bpstack*)malloc(sizeof(struct bpstack)); 
   entry->prevscore = b->score;
   char **prevboard = (char **)malloc(nrows*sizeof(char *));
   for (int i = 0; i < nrows; ++i) {
      prevboard[i] = (char*)malloc(ncols*sizeof(char));
   }
   for (int i = 0; i < nrows; i++) {
      for (int j = 0; j < ncols; j++) {
         prevboard[i][j] = b->board[i][j];
      }
   }
   entry->prevboard = prevboard;
   stk_push(b->boardstack, *entry);
   clusterscore += 1;
   b->board[r][c] = '.';
   if(r != nrows-1 && bal == b->board[r+1][c]){
      clusterscore += cluster_pop(b, r+1, c);
   }
   if(r !=0 && bal == b->board[r-1][c]){
      clusterscore += cluster_pop(b, r-1, c);
   }
   if(c !=ncols-1 && bal == b->board[r][c+1]){
      clusterscore += cluster_pop(b, r, c+1);
   }
   if(c !=0 && bal == b->board[r][c-1]){
      clusterscore += cluster_pop(b, r, c-1);
   }
   while(bp_is_compact(b) != 1) {
      bp_float_one_step(b);
   }
   b->score += clusterscore * (clusterscore-1); 
   return 1;
}

extern int comp(BPGame * b, int row, int col){
   if(b->board[row-1][col] == '.'){
      return 0;
   }
   return 1;
}
extern int bp_is_compact(BPGame * b){
   int nrows = b->rows;
   int ncols = b->cols;
   for(int row = 1; row<nrows; row++){
      for (int col = 0; col<ncols; col++){
         if(b->board[row][col] != '.'){
            if(comp(b, row, col) == 0){
               return 0;
            }
         }
      
      }
   }
   return 1;
}

extern void bp_float(BPGame * b, int col, int row, char balloon){
  if (b->board[row-1][col] == '.') {
     b->board[row-1][col] = balloon;
     b->board[row][col] = '.';
  }
}
extern void bp_float_one_step(BPGame * b){
  int nrows = b->rows;
  int ncols = b->cols;
  char bal; 
  for(int row = 1; row<nrows; row++){
    for(int col = 0; col<ncols; col++){
       //printf("Testing %i %i\n", row, col);
      bal=b->board[row][col];
      if(bal != '.'){
         bp_float(b, col, row, bal);
      }
    }  
  } 
}


extern void bp_display(BPGame * b) {
   printf("   +-");
   for (int i = 0; i < b->cols; i++) {
      printf("----");
   }
   printf("-+\n");
   for (int row = 0; row < b->rows; row++) {
      if (row < 10) {
         printf("0%i |  ", row);
      } else {
         printf("%i |  ", row);
      }
      for (int col = 0; col < b->cols; col++) {
         printf("%c   ", b->board[row][col]);
      }
      printf("|\n");
   }
   printf("   +-");
   for (int i = 0; i < b->cols; i++) {
      printf("----");
   }
   printf("-+\n     ");
   for (int col = 0; col < b->cols; col++) {
      if (col < 10) {
         printf("0%i  ", col);
      } else {
         printf("%i  ", col);
      }
   }
   printf("\n");
}

extern int bp_score(BPGame * b){
   return b->score;
}

extern int bp_get_balloon(BPGame * b, int r, int c){
   if (r < 0 || c < 0 || r > b->rows-1 || c > b->cols-1 ){
      return -1;
   } else {
      return b->board[r][c];
   }
}
extern int bp_can_pop(BPGame * b){
   int nrows = b->rows;
   int ncols = b->cols;
   for (int row = 0; row < nrows; row++) {
      for (int col = 0; col < ncols; col++) {
         char bal = b->board[row][col];
         if (bal != '.') {
            if (neighborcheck(b, row, col) ==1) {
               return 1;
            }
         }
      }
   }
   return 0;
}
extern int bp_undo(BPGame * b){
   if (b->boardstack->top == -1) {
      return 0;
   }
   ElemType previous = stk_pop(b->boardstack);
   b->score = previous.prevscore;
   b->board = previous.prevboard;
   return 1;
}

/*int main() {
   struct bpgame *newbp = bp_create(2,5);
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
   printf("Score is: %i\n", bp_score(newbp));
   bp_pop(newbp, 1, 2);
   bp_pop(newbp, 1, 5);
   bp_display(newbp);
   printf("Score is: %i\n", bp_score(newbp));
   bp_undo(newbp);
   bp_display(newbp);
   printf("Score is: %i\n", bp_score(newbp));
   bp_undo(newbp);
   bp_display(newbp);
   printf("bp_undo status at start: %i\n", bp_undo(newbp));
   bp_display(newbp);
   printf("Score is: %i\n", bp_score(newbp));
   printf("Scoreboard compact status is: %i\n", bp_is_compact(newbp));
   bp_display(newbp);
   bp_pop(newbp, 1, 0);
   bp_pop(newbp, 0, 1);
   bp_pop(newbp, 0, 3);
   bp_display(newbp);
   printf("Current status of game is %i\n", bp_can_pop(newbp));
   //bp_destroy(newbp);
}*/