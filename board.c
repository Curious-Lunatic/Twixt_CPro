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
    return (a.x == b.x && a.y == b.y);
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
                check.x = j;
                check.y = i;

                if (offboard(check)){
                    continue;
                    }
                else{
                    ccheck = viewcell(board, check);
                    for (int k=0; k<8; k++){
                        if (ccheck->links & (1<<k)){
                            if (crosslinkage(here,direction,check,k)){
                                return 0;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
return 1;
}

int crosslinkage(Position P1, int D1, Position P2, int D2){
    Position E1,E2;
    int mx1,Mx1,my1,My1;
    int mx2,Mx2,my2,My2;
    long long dx1,dx2,dy1,dy2;
    long long dx12,dy12;
    long long c1,c2;

    E1.x = P1.x + diff[D1][0];
    E1.y = P1.y + diff[D1][1];  
    E2.x = P2.x + diff[D2][0];
    E2.y = P2.y + diff[D2][1];
    
    if ((samepos(P1,P2) && D1==D2) || (samepos(P1,E2) && samepos(E1,P2))|| samepos(P1,P2) || samepos(P1,E2) || samepos(E1,P2) || samepos(E1,E2)){
        return 0;
    }

    if (P1.x<E1.x){
        mx1=P1.x;
        Mx1=E1.x;
    }
    else{
        mx1=E1.x;
        Mx1=P1.x;
    }
    if (P1.y<E1.y){
        my1=P1.y;
        My1=E1.y;
    }
    else{
        my1=E1.y;
        My1=P1.y;
    }
    if (P2.x<E2.x){
        mx2=P2.x;
        Mx2=E2.x;
    }
    else{
        mx2=E2.x;
        Mx2=P2.x;
    }
    if (P2.y<E2.y){
        my2=P2.y;   
        My2=E2.y;
    }
    else{
        my2=E2.y;
        My2=P2.y;
    }

if (Mx1<mx2 || Mx2<mx1 || My1<my2 || My2<my1){
    return 0;
    }

dx1=E1.x-P1.x;
dy1=E1.y-P1.y;
dx2=E2.x-P2.x;
dy2=E2.y-P2.y;
dx12=P2.x-P1.x;
dy12=P2.y-P1.y;
c1 = (dx1*dy12)-(dy1*dx12);
c2 = (dx1*(E2.y-P1.y))-(dy1*(E2.x-P1.x));
if((c1>0 && c2>0) || (c1<0 && c2<0)){
    return 0;
}
dx12=P1.x-P2.x;
dy12=P1.y-P2.y;
c1 = (dx2*dy12)-(dy2*dx12);
c2 = (dx2*(E1.y-P2.y))-(dy2*(E1.x-P2.x));
if((c1>0 && c2>0) || (c1<0 && c2<0)){
    return 0;
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
update(board,player);
}

void update(Board *board, int player){
    Cell* cell;
    for(int i=0; i<24; i++){
        for(int j=0; j<24; j++){
            board->cells[i][j].start[player]=0;
            board->cells[i][j].end[player]=0;
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
            else if (player == blue && j==23 && i>0 && i<23){
            cell->end[player]=1;
                }
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

void printlinks(Board* board){
    Position position;
    Cell *cell;
    int rc=0,bc=0;

    printf("\n");
    printf("=============\n");
    printf("Current LInks\n");
    printf("=============\n");

    printf("\n");
    printf("RED LINKS:\n");
    for (int i=0; i<24; i++){
        for (int j=0; j<24; j++){
            position.x = j;
            position.y = i;
            if (offboard(position)){
                continue;
            }
            cell = viewcell(board,position);
            if (cell->colour == redpeg){
                for (int k=0; k<8; k++){
                    if (cell->links & (1<<k)){
                        Position target = cell->neigbours[k];
                        if (target.x >= 0 && target.y >=0){
                            if (position.y < target.y || (position.y == target.y && position.x <target.x)){
                                printf("  %c%d <==> %c%d\n",'a'+position.x,position.y+1,'a'+target.x,target.y+1);
                                rc++;
                            }
                        }
                    }
                }
            }
        }
    }
    if (rc==0){
        printf(" No links yet :( \n");
    }
    else{
        printf(" Total: %d links\n",rc);
    }
    printf("\n");
    printf("BLUE LINKS:\n");
    for (int i=0; i<24; i++){
        for (int j=0; j<24; j++){
            position.x = j;
            position.y = i;
            if (offboard(position)){
                continue;
            }
            cell = viewcell(board,position);
            if (cell->colour == bluepeg){
                for (int k=0; k<8; k++){
                    if (cell->links & (1<<k)){
                        Position target = cell->neigbours[k];
                        if (target.x >= 0 && target.y >=0){
                            if (position.y < target.y || (position.y == target.y && position.x <target.x)){
                                printf("  %c%d <==> %c%d\n",'a'+position.x,position.y+1,'a'+target.x,target.y+1);
                                bc++;
                            }
                        }
                    }
                }
            }
        }
    }
    if (bc==0){
        printf(" No links yet :( \n");
    }
    else{
        printf(" Total: %d links\n",bc);
    }
printf("\n");
}
