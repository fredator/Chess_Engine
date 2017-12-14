/*
*
* @brief: This program is written as a one or two player chess emulator
*
*@author: Fredrik Treven
*@date: December 5th 2017
*
*
*/

#include <string>
#include <iostream>
#include <stdio.h>
/* TODO: move definitions etc. into board.h */
using namespace std;

#define NUM_ROWS 8
#define NUM_COLS 8
#define NUM_STRINGS 13
#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))

/* Errors */
#define ENO -1
#define EOB -2

/* Class Definition */
class Board {
    int grid[NUM_ROWS][NUM_COLS];   /* Raw Grid */
    const char* names[NUM_STRINGS]; /* Piece Grid */
    char letters[NUM_COLS];         /* Column Lettering */
    int numbers[NUM_ROWS];          /* Row Numbering */
    public:
        Board(void);
        void new_grid(void);
        void print_grid(void);
        int move(int, char, int, char);
        bool game_over(void);
};

/* Constructor */
Board::Board (void)
 {
    new_grid();

    /* Populate names[] array */
    names[0] = "--";
    names[1] = "pw";
    names[2] = "pb";
    names[3] = "NW";
    names[4] = "NB";
    names[5] = "BW";
    names[6] = "BB";
    names[7] = "RW";
    names[8] = "RB";
    names[9] = "QW";
    names[10] = "QB";
    names[11] = "KW";
    names[12] = "KB";

    /* Populate column letters */
    letters[0] = 'a';
    letters[1] = 'b';
    letters[2] = 'c';
    letters[3] = 'd';
    letters[4] = 'e';
    letters[5] = 'f';
    letters[6] = 'g';
    letters[7] = 'h';

    /* Populate row numbers */
    for (int x = 0; x < NUM_ROWS; x++) {
        numbers[x] = x+1;
    }
}

int char_to_int(char);
bool check_move(int, int, int, int, int, int);

int main()
{
    Board board_c;
    board_c.print_grid();
    char source[2];
    char dest[2];
    int check;
    int mv;
    while (!board_c.game_over()) {
        
        printf("Select a piece to move and where to move it 'q' to quit:");
        check = scanf("%s", source);

        if (check != 1)
            printf("Could not read source location input [Error: %d]\n", check);

        getchar();
        check = scanf("%s", dest);

        if (check != 1)
            printf("Could not read destination location input [Error: %d]\n", check);

        printf("\nx position = %c%c, y position = %c%c check = %d\n", source[0], source[1], dest[0], dest[1], check);
        mv = board_c.move(source[1], source[0], dest[1], dest[0]);
        return 0;

    }
}

bool Board::game_over(void)
{
    return false;
}

int char_to_int(char c)
{
    int ret;

    switch(c) {
    case 'a':
        ret = 0;
        break;
    case 'b':
        ret = 1;
        break;
    case 'c':
        ret = 2;
        break;
    case 'd':
        ret = 3;
        break;
    case 'e':
        ret = 4;
        break;
    case 'f':
        ret = 5;
        break;
    case 'g':
        ret = 6;
        break;
    case 'h':
        ret = 7;
        break;    
    default:
        ret = -1;
        printf("%s:Bad input\n", __func__);
        break;
    }

    return ret;
}
int Board::move(int row, char col_c, int new_row, char new_col_c)
{
    int col = char_to_int(col_c);
    int new_col = char_to_int(new_col_c);
    int piece = grid[row][col];
    int new_loc = grid[new_row][new_col];
    bool move_ok;

    move_ok = check_move(piece, row, col, new_row, new_col, new_loc);

    if(move_ok) {
        printf("Move is ok\n");
        return 1;
    } else {
        printf("Move not ok\n");
        return 0;
    }

}

bool check_move(int piece, int row, int col, int new_row, int new_col, int new_loc)
{
    bool bw = piece % 2;
    bool ans;
    /* Check if there is a piece at this location */
    if (row > NUM_ROWS-1 || row < 0 || col > NUM_COLS-1 || col < 0) {
        printf("New Location Out of Bounds\n");
        return false; /* Piece out of bounds */
    } else if (piece == 0) {
        printf("No Piece at Designated Location\n");
        return false; /* No Piece Found at Location */
    }

    if (new_loc %2 != bw || new_loc == 0) {
        switch(piece) {
            case 1: /* White Pawn */
                if(new_row != row+1 || new_row!=row+2)
                    ans = false;
                else
                    ans = true;
                break;
            default:
                ans = false;
                printf("No Piece Found\n");
        }
    }

    return ans;
}
void Board::print_grid(void)
{
    for(int row = NUM_ROWS-1; row >= 0; row--)
    {
        printf("%d\t", numbers[row]);

        for(int col = 0; col < NUM_COLS; col++)
        {
            int val = grid[row][col];
            printf("%s\t", names[val]);
        }
        printf("\n");
        printf("\n");
    }

    printf("\t");
    for (int y = 0; y < NUM_COLS; y++)
    {
        printf("%c\t", letters[y]);
    }
    printf("\n");
}

void Board::new_grid(void)
{
    grid[0][0] = 7;  /* White Rook   */
    grid[0][1] = 3;  /* White Knight */
    grid[0][2] = 5;  /* White Bishop */
    grid[0][3] = 9;  /* White Queen  */
    grid[0][4] = 11; /* White King   */
    grid[0][5] = 5;
    grid[0][6] = 3;
    grid[0][7] = 7;
    grid[1][0] = 1;  /* White Pawn   */
    grid[1][1] = 1;
    grid[1][2] = 1;
    grid[1][3] = 1;
    grid[1][4] = 1;
    grid[1][5] = 1;
    grid[1][6] = 1;
    grid[1][7] = 1;
    grid[6][0] = 2;  /*Black Pawn    */
    grid[6][1] = 2;
    grid[6][2] = 2;
    grid[6][3] = 2;
    grid[6][4] = 2;
    grid[6][5] = 2;
    grid[6][6] = 2;
    grid[6][7] = 2;
    grid[7][0] = 8;  /* Black Rook   */
    grid[7][1] = 4;  /* Black Knight */
    grid[7][2] = 6;  /* Black Bishop */
    grid[7][3] = 10; /* Black Queen  */
    grid[7][4] = 12; /* Black King   */
    grid[7][5] = 6;
    grid[7][6] = 4;
    grid[7][7] = 8;

    for(int row = 2; row < NUM_ROWS - 2; row++)
    {
        for(int col = 0; col < NUM_COLS; col++)
        {
            grid[row][col] = 0;
        }
    }
}