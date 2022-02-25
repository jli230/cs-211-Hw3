
#define MAX_ROWS 40
#define MAX_COLS 40

/********
* FILE:  bpgame.h
* DESC:  This file specifies the interface/ADT for 
*	managing the "balloon-pop" game.
*
*	 A client/app to actually play the game must
*	 manage the user interface itself, but would
*	 use the ADT/interface here to perform the
*	 interface independent operations on the state
* 	 of the game.
*
* NOTES:  The type BPGame (and struct bpgame) are "incomplete":
*	 The actual contents of the structure to capture
*	 the state of a game instance are hidden from
*	 client programs.  Client programs work with a BPGame
*	 strictly by passing a pointer to functions of the ADT.
*
*	 This allows alternative implementation strategies
*	 while maintaining the same interface.
*
*	 A primitive client program bpop.c is provided and 
*	 illustrates how the BPGame ADT is used.
*/



/** 
    BPGame is incompletely specified as far as any client program 
	is concerned.
    A BPGame "instance" captures everything about a particular run
	of a balloon pop game:  the current state of the board, 
	the score, any additional info for the undo feature, etc.

    The definition of actual struct bpgame is hidden in bboard.c 
**/
typedef struct bpgame BPGame;

/** somewhat arbitrary characters to represent 
	"empty"/"air", red, blue, green and yellow.
**/

#define None '.'    // "air"
#define Red  '^'
#define Blue '='
#define Green 'o'
#define Yellow '+'


/**
* Function:  bp_create
* Parameters: number of rows and columns
* Description: returns pointer to newly created
*              board with given number of rows
*              and columns.
*
*              Populates board with random balloons.
*
*              If nrows or ncols is out of range, 
*              an error message is printed to stderr
*              and NULL is returned.
*
*/
extern BPGame * bp_create(int nrows, int ncols);

/**
* Function:  bp_create_from_mtx
* Parameters: number of rows and columns and 2D array of char
* Description: returns pointer to newly created
*              board with given number of rows
*              and columns.
*
*              Populates board contents of given 2D matrix (assumes
*	       given dimensions).
*
*	       Error conditions:
*              		If nrows or ncols is out of range, an error message 
*			is printed to stderr and NULL is returned.
*
*			If any element in the matrix is not one of the understood
*			color symbols (from the #define-s at top of this file),
*			an error message is pritnted to stderr and NULL is returned.
*			
*/
extern BPGame * bp_create_from_mtx(char mtx[][MAX_COLS], int nrows, int ncols);

/**
* Function:  bp_destroy
* Description:  deallocates all dynamically allocated 
*  		memory associated with the board.
*		
*		Comment:  this might start out as
*		a one-liner.  
*
*/
extern void bp_destroy(BPGame * b);


/**
* Function:  bp_display
* Description:  pretty-prints the current board.
*
*		uses '|', '+' and '-' to "draw" the boundaries
*               of the board.
*
*		For full credit rows and columns are labeled with
* 		their indices.  Column indices displayed "vertically"
*
*		Examples:
*
*  This board is full of balloons (like at the beginning of a game).
* 
*       +-----------------------+
*		  0 | + + + = + ^ ^ o + = = |
*		  1 | ^ = = o + o + + + ^ = |
*		  2 | ^ + ^ o + o + = = = = |
*		  3 | = ^ o o = o + + + = = |
*		    +-----------------------+
* 		    0 0 0 0 0 0 0 0 0 0 1
*		      0 1 2 3 4 5 6 7 8 9 0
*
*
*  This board has some empty slots represented by  '.'
*       +-----------------------+
*		  0 | + + + = + ^ ^ o + = = |
*		  1 | ^ = o o + o + + + ^ = |
*		  2 | ^ + . o + . + = . . = |
*		  3 | . . . o . . + + . . = |
*		    +-----------------------+
* 		    0 0 0 0 0 0 0 0 0 0 1
*		      0 1 2 3 4 5 6 7 8 9 0
*
*
*  Here is the same board using a space ' ' to 
*  represent an empty slot.
*
*       +-----------------------+
*		  0 | + + + = + ^ ^ o + = = |
*		  1 | ^ = o o + o + + + ^ = |
*		  2 | ^ +   o +   + =     = |
*		  3 |       o     + +     = |
*		    +-----------------------+
* 		    0 0 0 0 0 0 0 0 0 0 1
*		      0 1 2 3 4 5 6 7 8 9 0
*
*/
extern void bp_display(BPGame * b);

/**
* Function:  bp_pop
* Description:  (attempts to) "pop" the balloon at (r,c) (if any)
*               and its associated "color cluster"
*
*               Cluster must be of AT LEASE SIZE 2 (a balloon that
*   		        is its own cluster will not be popped).
*
* Returns:  number of balloons popped (which may be zero).
*
* Side effects:  
*		locations of popped balloons become None (as in the "color"
*			None in the #define at the top of this file).
*
*		adds n*(n-1) to score if n balloons were popped.
*
* Notes:  does not require that the board be "compact".  But presumably, 
*	most client applicationw would only call on a compact board.
*
*/
extern int bp_pop(BPGame * b, int r, int c);

/**
* Function:  bp_is_compact
* Description:  determines if all balloons are as "high" as possible.
*               Returns 1 if this is the case
*               Returns 0 otherwise.
*
*               Note:  a balloon is as high as possible if and only
*     		if all slots in its column ABOVE it are also balloons
*		(non-empty)
*
*/
extern int bp_is_compact(BPGame * b);

/**
* Function:  bp_float_one_step
* Description:  moves all balloons that that are NOT AS HIGH AS POSSIBLE
*               up one spot.  
*
*   Example:
*
*		before
*
*       +-----------------------+
*		  0 | + + + = + ^     + = = |
*		  1 | ^       + o + +   ^ = |
*		  2 | ^ +   o +   + =     = |
*		  3 |       o     + + =   = |
*		    +-----------------------+
* 		    0 0 0 0 0 0 0 0 0 0 1
*		      0 1 2 3 4 5 6 7 8 9 0
*
*		after
*
*       +-----------------------+
*		  0 | + + + = + ^ + + + = = |
*		  1 | ^ +   o + o + =   ^ = |
*		  2 | ^     o +   + + =   = |
*		  3 |                     = |
*		    +-----------------------+
* 		    0 0 0 0 0 0 0 0 0 0 1
*		      0 1 2 3 4 5 6 7 8 9 0
*
*
* Note:  think animation by repeating this until it
*   is compact (perhaps displaying each intermediate
*   configuration).
*/
extern void bp_float_one_step(BPGame * b);

/**
* Function:  bp_score
* Description:  returns the current score 
*
*/
extern int bp_score(BPGame * b);


/**
* Function:   bp_get_balloon
* Description:  returns color of balloon at given location as a char.
*		if (r,c) is out of range, -1 is returned.
*
*/
extern int bp_get_balloon(BPGame * b, int r, int c);


/**
* Function:   bp_can_pop
* Description:  returns 1 if the player can pop some balloons;
*		        0 otherwise.
*
*		If the board is empty, of course nothing can
*			be popped.
*		If there are no adjacent balloons of the same
*		color, there also are no moves.
*	
* Notes:  probable usage is to determine when game is over.
*/
extern int bp_can_pop(BPGame * b);


/**
* Function:  bp_undo
* Description:  "undoes" most recent successful pop (if any), 
*		restoring the board to the exact state it was 
*		in immediately prior to that pop.
*
*               Also restores score to corresponding previous value.
*
*               Returns 1 if successful; 0 otherwise (i.e., when in the initial state --
*		no moves have been made to be undone).
*
*		NOTE:  bp_undo can be called repeatedly to back up all the way to the beginning
*		of the game.
*
*/
extern int bp_undo(BPGame * b);

