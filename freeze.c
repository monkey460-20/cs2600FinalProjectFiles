#include <stdio.h>

#define EMPTY ' '
#define maxDim 7
#define noCol -1

typedef struct{
  char cells[maxDim][maxDim];
  int rows;
  int cols;
  int frozenCol;   //noCol when nothing is frozen
  char frozenFor;   //piece of the blocked player
  int frozenTurnsLeft;   //how many of that player's turns remain blocked
} Board;

void clearFreeze(Board *b){
    b->frozenCol = noCol;
    b->frozenFor = EMPTY;
    b->frozenTurnsLeft = 0;
}

int columnFull(const Board *b, int col){
  return (b->cells[0][col] != EMPTY);
}
//freezes a column so 'victim' can't drop into it
int freezeColumn(Board *b, int col, char victim, int turns){ 
  if(col < 0 || col >= b->cols){
    return 0;
  }
  if(columnFull(b, col)){
    return 0; //nothing to freeze
  }
//make sure the victim still has at least other legal moves
  int otherOpen = 0;  
  for(int c = 0; c < b->cols; c++){
    if(c != col && !columnFull(b, c)){
      otherOpen = 1;
    }
  }
  if(!otherOpen){
    return 0;
  }
  b->frozenCol = col;
  b->frozenFor = victim;
  b->frozenTurnsLeft = turns;
  return 1;
}

int isFrozenFor(const Board *b, int col, char piece){
  return (b->frozenTurnsLeft > 0 && b->frozenCol == col && b->frozenFor == piece);
}
//Call this when a player finishes their turn
//Only the frozen player's turns count down the freeze
void tickFreeze(Board *b, char playerWhoMoved){
  if(b->frozenTurnsLeft > 0 && b->frozenFor == playerWhoMoved){
    b->frozenTurnsLeft--;
    if(b->frozenTurnsLeft == 0){
      clearFreeze(b);
    }
  }
}
//Returns the row the piece landed in
//-1 if the columnis full, -2 if it's frozen for this player
int dropPiece(Board *b, int col, char piece){
  if(isFrozenFor(b, col, piece)){
    return -2;
  }
  for(int r = b->rows - 1; r>= 0; r--){
    if(b->cells[r][col] == EMPTY){
      b->cells[r][col] = piece;
      return r;
    }
  }
  return -1;
}

void printBoard(const Board *b) {
  // marker row above the board for the frozen column
  for (int c = 0; c < b->cols; c++) {
    printf(b->frozenTurnsLeft > 0 && c == b->frozenCol ? " FRZ" : "    ");
  }
  printf("\n");

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

#ifdef abilityTest
int main(void) {
  Board board;
  board.rows = 6;
  board.cols = 7;
  for (int r = 0; r < maxDim; r++)
    for (int c = 0; c < maxDim; c++)
      board.cells[r][c] = EMPTY;
  clearFreeze(&board);

  dropPiece(&board, 3, 'X');
  dropPiece(&board, 4, 'O');
  printf("Starting position:\n");
  printBoard(&board);

  printf("X freezes column 4 against O...\n");
  if (freezeColumn(&board, 3, 'O', 1)) {
    printBoard(&board);
  }

  if (dropPiece(&board, 3, 'O') == -2) {
    printf("O tried column 4: blocked, it's frozen.\n");
  }
  dropPiece(&board, 2, 'O');         // O plays somewhere else instead
  tickFreeze(&board, 'O');           // O's turn is over, so the freeze expires

  dropPiece(&board, 3, 'X');         // X plays normally
  printf("\nAfter O's turn the freeze is gone:\n");
  printBoard(&board);

  if (dropPiece(&board, 3, 'O') >= 0) {
    printf("O can drop in column 4 again.\n");
  }
  printBoard(&board);

  return 0;
}
#endif