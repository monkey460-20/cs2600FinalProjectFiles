#include <stdio.h>
#include <ctype.h>

#define EMPTY  ' '
#define maxDim 7

typedef struct {
  char cells[maxDim][maxDim];
  int rows;
  int cols;
} Board;

typedef enum {
  CLOCKWISE, COUNTERCLOCKWISE
} flipDirection;

void applyGravity(Board *b) {
  for (int c = 0; c < b->cols; c++) {
    int write = b->rows - 1;
    for (int r = b->rows - 1; r >= 0; r--) {
      if (b->cells[r][c] != EMPTY) {
        char piece = b->cells[r][c];
        b->cells[r][c] = EMPTY;
        b->cells[write][c] = piece;
        write--;
      }
    }
  }
}

void rotateBoard(Board *b, flipDirection dir) {
  char tmp[maxDim][maxDim];

  for (int r = 0; r < b->rows; r++) {
    for (int c = 0; c < b->cols; c++) {
      if (dir == CLOCKWISE) {
        tmp[c][b->rows - 1 - r] = b->cells[r][c];
      } else {
        tmp[b->cols - 1 - c][r] = b->cells[r][c];
      }
    }
  }

  int oldRows = b->rows;
  b->rows = b->cols;
  b->cols = oldRows;

  // copy the rotated result back into the board
  for (int r = 0; r < b->rows; r++) {
    for (int c = 0; c < b->cols; c++) {
      b->cells[r][c] = tmp[r][c];
    }
  }
}

void gravityFlip(Board *b, flipDirection dir) {
  rotateBoard(b, dir);
  applyGravity(b);
}

void printBoard(const Board *b) {
  printf("(%d rows x %d cols)\n", b->rows, b->cols);
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

int askDirection(flipDirection *dir) {
  char choice;

  while (1) {
    printf("Flipping Direction (R = Clockwise; L = Counterclockwise; Q = quit): ");
    if (scanf(" %c", &choice) != 1) {
      return 0;
    }
    choice = (char)toupper((unsigned char)choice);

    if (choice == 'R') {
      *dir = CLOCKWISE;
      return 1;
    }
    if (choice == 'L') {
      *dir = COUNTERCLOCKWISE;
      return 1;
    }
    if (choice == 'Q') {
      return 0;
    }

    printf("Invalid choice\n");
  }
}

#ifdef abilityTest
int main(void) {
  // Sample 6x7 board, row 0 is the top
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

  flipDirection dir;
  while (askDirection(&dir)) {
    gravityFlip(&board, dir);
    printf("\nAfter %s flip:\n",
           dir == CLOCKWISE ? "clockwise" : "counterclockwise");
    printBoard(&board);
  }

  return 0;
}
#endif