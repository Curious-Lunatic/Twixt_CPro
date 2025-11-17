#ifndef board_h
#define board_h
#include "defination.h"


void createboard(Board *board);
void setcells(Board *board);
void legalmoves(Board *board);
void neighbours(Board *board, Position position, Cell *cell);

int offboard(Position postion);
int onborder(int player, Position position);
Position convert(int action);
int reverse(Position position);

Cell* viewcell(Board *board, Position position);
int linkage(Board *board, Position here, int direction);
void peg(Board* board, int player, Position position);

void update(Board *board, Position position, int player);
int checkwin(Board* board, int plauer);
void makemove(Board* board, int player, int action);
void checkstate(Board* board, int player, Position positon);
int showresult(Board* board);

Action* legality(Board* board, int player);
void removelegal(Board* board, int player, Position position);
void print(Board *board);

void actionlist(Action *list);
void addaction(Action *list, int action);
void removeaction(Action *list, int action);
int checkaction(Action *list, int action);
#endif
