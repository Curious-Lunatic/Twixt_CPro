#ifndef defination_h
#define defination_h

typedef enum{
    red=0,
    blue=1
}Player;

typedef enum{
    open=0,
    red_win=1,
    blue_win=2,
    draw=3
}Result;

typedef enum{
    redpeg=0,
    bluepeg=1,
    empty=2,
    outboard=3
}Peg;

typedef enum{
    NE=0,
    EN=1,
    ES=2,
    SE=3,
    SW=4,
    WS=5,
    WN=6,
    NW=7
}Knight;

typedef struct{
    int x;
    int y;
}Position;

typedef struct{
    int horizontal;
    int vertical;
}Offset;

typedef struct Cell{
    Peg colour;        
    char links;
    char blocked;
    Position neigbours[8];
    int start[2]; 
    int end[2];
}Cell;

typedef struct Action{
    int list[572];   // no. of cells in the thing //
    int count;
}Action;

typedef struct Board{
    Cell cells[24][24];
    int move;
    Position first;
    Result result;
    Action legal[2];
}Board;

typedef struct State{
    Board board;
    int current;
    int gameover;
}State;

static int diff[8][2] = {{1,-2},{2,-1},{2,1},{1,2},{-1,2},{-2,1},{-2,-1},{-1,-2}};

int oppdir(int direction);
int samepos(Position a, Position b);

#endif