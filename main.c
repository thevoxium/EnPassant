#include "src/model.h"

int main(){
  Board* board = initBoard();
  if(board == NULL){
    return -1;
  }
  printBoard(board);
  freeBoard(board);
  return 0;
}
