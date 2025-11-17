#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include "board.h"
#include "defination.h"

int read(char *input, int *column, int *row){
    char c,d;
    int r;
    if (sscanf(input," %c%d%c",&c,&r,&d) != 2){
        return 0;
    }
        if (c<'a' || c>'x'){
            return 0;
        }
        if (r<1 || r>24){
            return 0;
        }
*column = c-'a';
*row = r-1;
return 1;
}

void help(void){
    printf("\n");
    printf("================================================\n");
    printf("               TWIXT GAME RULES                 \n");
    printf("================================================\n");

    printf("OBJECTIVE:\n");
    printf("\n");
    printf("Red (X): Connect from Top to Bottom of the board\n");
    printf("Blue (O): Connect from Left to Right of the board\n");

    printf("\n");
    printf("HOW TO PLAY:\n");
    printf("\n");
    printf("1. Take turns by placing pegs by inputing their place\n");
    printf("2. Links will form automatically if they are at\n");
    printf("   a knight's distance to each other !!\n");
    printf("3. Links cannot cross each other.\n");
    printf("4. First to connect their ends wins the game :D\n");

    printf("\n");
    printf("COMMANDS:\n");
    printf("\n");
    printf(" help    - Show the help menu\n");
    printf(" show    - Show the board\n");
    printf(" links   - Show the current links on the board\n");
    printf(" exit    - Exit the game\n");
    printf("\n");
}

void welcome(void) {
    printf("\n");
    printf("\033[1;39m\n");
    printf("  █████████  ███      ████      ███  █████████  ███    ███  █████████\n");
    printf("     ███     ███     ██████     ███     ███      ███  ███      ███\n");
    printf("     ███      ███   ███  ███   ███      ███        ████        ███\n"); // cool lag raha :) //
    printf("     ███       ███ ███    ███ ███       ███      ███  ███      ███\n");
    printf("     ███         ████      ████      █████████  ███    ███     ███\n");
    printf("\033[0m");
    printf("\n");
    printf("Commands: a) help      b) show      c) exit   \n");
    printf("\n");
    printf("Press Enter to start playing :D\n");
    printf("\n");
}

void play(State *game){
    char input[101];
    int column,row;
    Position buffer;
    Position positon;
    int action;
    Action* legal;
    int result;

    while(showresult(&game->board) == open){
        print(&game->board);
        printf("\n");
        printf("Move #%d\n",game->board.move+1);

    if (game->current == red){
        printf("Red's turn: ");
    }
    else if (game->current ==blue){
        printf("Blue's turn: ");
        }
    
    if (fgets(input,sizeof(input),stdin)==NULL){
        break;
        }
    if (input[0] == '\n'){
        continue;
        }
    input[strcspn(input, "\n")] = 0;

    if (strcmp(input,"exit")==0){
        printf("\n");
        printf("Exiting the game........\n");
        printf("\n");
        printf("Thanks for playing :D\n");
        return;
        }
    if (strcmp(input,"help")==0){
        help();
        continue;
        }
    if (strcmp(input,"show")==0){
        continue;
        }
    if (!read(input,&column,&row)){
        printf("Invalid move! Try to put a different move!\n");
        printf("The letter should be in a-x\n");
        printf("The number should be from 1-24\n");
        continue;
        }
    positon.x = column;
    positon.y = row;
    action = reverse(positon);
    legal = legality(&game->board,game->current);

    if (!checkaction(legal, action)){
        printf(" Sorry :(! This Position is not available! Try another one\n");
        printf("\n");
        continue;
        }
    makemove(&game->board,game->current,action);
    if (showresult(&game->board)==open){
        game->current = 1-game->current;
    }
}
    
printf("\n");
print(&game->board);
printf("\n");
printf("===========================================================\n");
printf("||                  GAME OVER :O !!                      ||\n");
printf("===========================================================\n");

result = showresult(&game->board);
if (result == red_win){
    printf(" RED WINS !!! \n");
    printf("Won in %d moves !!!\n", game->board.move);
    }
if (result == blue_win){
    printf(" BLUE WINS !!! \n");
    printf("Won in %d moves !!!\n", game->board.move);
    }   
if (result == draw){
    printf(" DRAW !!! \n");
    printf("Well played by both teams !!!\n");
    }   
}

int main(){
    State game;
    welcome();
    getchar();
    printf("Setting up the board ....\n");
    printf("\n");
    game.current=red;
    game.gameover=0;
    createboard(&game.board);
    play(&game);
return 0;
}
