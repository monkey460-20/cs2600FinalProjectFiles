#include <stdio.h>
#define EMPTY ' '
#define maxDim 7

typedef struct{
  char cells[maxDim][maxDim];
  int rows;
  int cols;
} Board;

//every piece falls to the bottom of its column
void applyGravity(Board *b){
  for(int c = 0; c < b->cols; c++){
    int write = b->rows -1;
    for(int r = b->rows - 1; r >= 0; r--){
      if(b->cells[r][c] != EMPTY){
	char piece = b->cells[r][c];
	b->cells[r][c] = EMPTY;
	b->cells[write][c] = piece;
	write--;
      }
    }
  }
}
//removes the disc in the bottom row
//the column above falls down. Returns 1 on success, 0 if not allowed
int popOut(Board *b, int col){
  if(col < 0 || col >= b->cols){
    return 0;
  }
  int bottom = b->rows - 1;
  if(b->cells[bottom][col] == EMPTY){ //checks if their is anything to remove
    return 0;
  }
  b->cells[bottom][col] = EMPTY;
  return 1;
}

void printBoard(const Board *b) {
  for (int r = 0; r < b->rows; r++) {
    for (int c = 0; c < b->cols; c++) {
      printf("| %c ", b->cells[r][c]);
    }
    printf("|\n");
  }
  for (int c = 0; c < b->cols; c++) {
    printf("  %d ", c + 1);
  }
  printf("\n\n");
}

int main(void) {
  const char *start[6] = {
    "       ",
    "       ",
    "       ",
    "  X    ",
    " OXO   ",
    "XOXOX  ",
  };

  Board board;
  board.rows = 6;
  board.cols = 7;
  for (int r = 0; r < maxDim; r++)
    for (int c = 0; c < maxDim; c++)
      board.cells[r][c] = EMPTY;
  for (int r = 0; r < 6; r++)
    for (int c = 0; c < 7; c++)
      board.cells[r][c] = start[r][c];

  printf("Starting board:\n");
  printBoard(&board);

  printf("Popping out the bottom checker in column 3...\n");
  if (popOut(&board, 2)) {
    printBoard(&board);
  }

  if (!popOut(&board, 6)) {
    printf("Column 7 is empty, so there's nothing to pop out.\n");
  }

  return 0;
}
