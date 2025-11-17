#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "board.h"

int oppdir(int direction){
    if (direction<4){
        return direction+4;
    }
    else {
        return direction-4;
    }
}

int samepos(Position a, Position b){
    return ((a.x == b.x) && (a.y == b.y));
}

void createboard(Board *board){
board->move=0;
board->first.x=-1;
board->first.y=-1;
board->result=open;
setcells(board);
legalmoves(board);
}

void setcells(Board *board){
Position position;
Cell *cell;
    for (int i=0; i<24; i++){
        for (int j=0; j<24; j++){
            position.x = i;
            position.y = j;
            cell = viewcell(board,position);

            cell->links = 0;
            cell->blocked = 0;

            for (int k=0; k<2; k++){
                cell->start[k]=0;
                cell->end[k]=0;
            }
        if (offboard(position)){
            cell->colour = outboard;
        }
        else{
            cell->colour = empty;
            if (j==0 && i>0 && i<23){
                cell->start[blue]=1;
            }
            if (j==23 && i>0 && i<23){
                cell->end[blue]=1;
            }
            if (i==0 && j>0 && j<23){
                cell->start[red]=1;
            }
            if (i==23 && j>0 && j<23){
                cell->end[red]=1;
            }
            neighbours(board,position,cell);
            }
        }
    }
}

void legalmoves(Board *board){
    Position position;
    actionlist(&board->legal[red]);
    actionlist(&board->legal[blue]);
    int act,rb,bb;
    for (int i=0; i<24; i++){
        for (int j=0; j<24; j++){
            position.x = j;
            position.y = i;
            act = reverse(position);

        if(offboard(position)){
            continue;
        }
        rb = onborder(red,position);
        bb = onborder(blue,position);
        if(bb){
            addaction(&board->legal[blue],act);
        }
        else if(rb){
            addaction(&board->legal[red],act);
        }
        else{
            addaction(&board->legal[blue],act);
            addaction(&board->legal[red],act);
            }
        }
    }
}

void neighbours(Board *board, Position position, Cell *cell){
    Position neighbour;
    for (int i=0; i<8; i++){
        neighbour.x = position.x + diff[i][0];
        neighbour.y = position.y + diff[i][1];

        if(!offboard(neighbour)){
            cell->neigbours[i] = neighbour;
        }
        else{
        cell->neigbours[i].y = -1;
        cell->neigbours[i].x = -1;
        }
    }
}

int offboard(Position position){
    if (position.x>23 || position.x<0 || position.y>23 || position.y<0 ){
        return 1;
    }
else if ((position.x == 0 && position.y == 23) || (position.x == 23 && position.y == 0) || (position.x == 0 && position.y == 0) || (position.x == 23 && position.y == 23)){
        return 1;
    }
    else return 0;
}

int onborder(int player, Position position){
    if (player == red){
        return ((position.y == 0 || position.y == 23) && (position.x > 0 && position.x < 23));
    }
    else if (player == blue){
        return ((position.x == 0 || position.x == 23) && (position.y > 0 && position.y < 23));       
    }
return 0;
}

Position convert(int action){
    Position curr;
    curr.x = action%24;
    curr.y = action/24;
    return curr;
}

int reverse(Position position){
    return (position.y*24)+(position.x);
}

Cell* viewcell(Board *board, Position position){
    return &board->cells[position.y][position.x];
}

int linkage(Board *board, Position here, int direction){
    Position there;
    Cell *chere, *cthere;
    Position check;
    Cell *ccheck;
    there.x = here.x + diff[direction][0];
    there.y = here.y + diff[direction][1];
    if (offboard(there)){
        return 0;
    }
    else{
        chere = viewcell(board,here);
        cthere = viewcell(board,there);
    if (cthere->colour != chere->colour){
        return 0;
        }
    else{
        for (int i=0; i<24; i++){
            for (int j=0; j<24; j++){
                check.x = i;
                check.y = j;
                if (offboard(check)){
                    continue;
                }
            }
        }
    }
}
return 1;
}

void peg(Board* board, int player, Position positon){
    Cell *cell;
    Position target;
    Cell *ctarget;
    cell = viewcell(board,positon);
    if (player == red){
        cell->colour = redpeg;
    }
    else if (player == blue){
        cell->colour = bluepeg;
    }
    for (int i =0; i<8; i++){
        target.x = positon.x + diff[i][0];
        target.y = positon.y + diff[i][1];
        if (offboard(target)){
        continue;
        }
ctarget = viewcell(board,target);
    if (ctarget->colour == cell->colour){
        if (linkage(board,positon,i)){
            cell->links |= (1<<i);
        ctarget->links |= (1<<oppdir(i));
    printf("BINGO!\n");
    printf("You have created a link! : %c%d <==> %c%d\n", 'a'+positon.x,positon.y+1,'a'+target.x, target.y+1);
        }
    else {
        cell->blocked |= (1<<i);
    ctarget->blocked |= (1<<oppdir(i));
            }
        }
    }
update(board,positon,player);
}

void update(Board *board, Position position, int player){
    Cell* cell;
    for(int i=0; i<24; i++){
        for(int j=0; j<24; j++){
            board->cells[i][j].start[player]=0;
            board->cells[i][j].end[player]=0;
        }
    }
    for (int i=0; i<24; i++){
        for (int j=0; j<24; j++){
            Position p;
            p.x=j;
            p.y=i;
            cell = &board->cells[i][j];
            Peg buffer;
            if (player == red){
                buffer = redpeg;
            }
            else if (player == blue){
                buffer = bluepeg;
            }
            if (player == red && i==0 && j>0 && j<23){
            cell->start[player]=1;
            }
            else if (player == blue  && j==0 && i>0 && i<23){
            cell->start[player]=1;
            }
            else if (player == red && i==23 && j>0 && j<23){
            cell->end[player]=1;
            }
            else if (player == red && j==0 && i>0 && i<23){
            cell->end[player]=1;
                }
            }
        }
    int flag=1,a=0;
    while (flag && a<10000){
        flag=0;
        a++;

        for (int i=0; i<24; i++){
            for (int j=0; j<24; j++){
                cell = &board->cells[i][j];
                if (cell->start[player]){
                    for(int k=0; k<8; k++){
                        if(cell->links & (1<<k)){
                            Position next = cell->neigbours[k];
                            if (next.x >=0 && next.y>=0){
                                Cell *cnext = &board->cells[next.y][next.x];
                                if (!cnext->start[player]){
                                    cnext->start[player]=1;
                                    flag=1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
   flag=1,a=0;
    while (flag && a<10000){
        flag=0;
        a++;
        for (int i=0; i<24; i++){
            for (int j=0; j<24; j++){
                cell = &board->cells[i][j];
                if (cell->end[player]){
                    for(int k=0; k<8; k++){
                        if(cell->links & (1<<k)){
                            Position next = cell->neigbours[k];
                            if (next.x >=0 && next.y>=0){
                                Cell *cnext = &board->cells[next.y][next.x];
                                if (!cnext->end[player]){
                                    cnext->end[player]=1;
                                    flag=1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


int checkwin(Board* board, int plauer){
    Cell *cell;
    for (int i=0; i<24; i++){
        for (int j=0; j<24; j++){
            cell = &board->cells[i][j];
            if (cell->start[plauer] && cell->end[plauer]){
                return 1;
            }
        }
    }
return 0;
}

void makemove(Board* board, int player, int action){
    Position pos;
    pos = convert(action);
    peg(board,player,pos);
    if (board->move ==0){
        board->first = pos;
    }
        removelegal(board,red,pos);
        removelegal(board,blue,pos);
board->move++;
checkstate(board,player,pos);
}

void checkstate(Board* board, int player, Position positon){
    int opps;
    if (checkwin(board,player)){
        if (player==red){
            board->result = red_win;
        }
        else if (player==blue){
            board->result = blue_win;
        }
    return;
    }
    opps = 1-player;
    if (board->legal[opps].count == 0){
        board->result = draw;
    }
}

int showresult(Board* board){
    return board->result;
}

Action* legality(Board* board, int player){
    return &board->legal[player];
}

void removelegal(Board* board, int player, Position position){
    int act = reverse(position);
    removeaction(&board->legal[player],act);
}

void print(Board *board){
    Position position;
    Cell *cell;
    printf("\n     ");
    for(int i=0; i<24; i++){
        printf("\033[31m%c\033[0m ",'a'+i);
    }
    printf("\n    ");
    printf("-");
    for(int i=0; i<24; i++){
        printf("--");
    }
    printf("-     \n");
    for (int i=0; i<24; i++){
        printf(" \033[34m%2d\033[0m |",i+1);
        for (int j=0; j<24; j++){
            position.x = j;
            position.y = i;
            if (offboard(position)){
                printf("  ");
                continue;
            }
            cell = viewcell(board,position);

        if (cell->colour == redpeg){
            printf("\033[31mX\033[0m ");
        }
        else if (cell->colour == bluepeg){
            printf("\033[34mO\033[0m ");
            }
        else{
            printf(". ");
            }   
        }
    printf("| \033[34m%2d\033[0m\n",i+1);
    }
printf("    -");
for (int i=0; i<24; i++){
    printf("--");
    }  
printf("-\n");
printf("     ");
for (int i=0; i<24; i++){
    printf("\033[31m%c\033[0m ",'a'+i);
    }
printf("\n");
}

void actionlist(Action *list){
    list->count=0;
}

void addaction(Action *list, int action){
    if (list->count<572){
        list->list[list->count]=action;
        list->count++;
    }
}

void removeaction(Action *list, int action){
    for (int i=0; i<list->count;i++){
        if (list->list[i] == action){
            for(int j=i; j<(list->count)-1; j++){
                list->list[j] = list->list[j+1];
            }
        list->count--;
        return;
        }
    }
}

int checkaction(Action *list, int action){
    for (int i=0; i<list->count; i++){
        if (list->list[i] == action){
            return 1;
        }
    }
return 0;
}
