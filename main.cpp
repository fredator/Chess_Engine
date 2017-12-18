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
/* Constants */
#define NUM_ROWS 8
#define NUM_COLS 8
#define NUM_STRINGS 13
#define EMPTY 0
#define WT_PWN 1
#define BK_PWN 2
#define WT_KNT 3
#define BK_KNT 4
#define WT_BSP 5
#define BK_BSP 6
#define WT_ROK 7
#define BK_ROK 8
#define WT_QEN 9
#define BK_QEN 10
#define WT_KNG 11
#define BK_KNG 12

/* Macros */
#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))

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
        bool game_over(int);
        bool check_move(int, int, int, int, int, int);
        bool check_blocked(int, int, int, int, int);
        bool turn;                       /* Black or White's turn (true = white, false = black)*/
        int turn_number;
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

/* Helper function templates */
int char_to_int(char);

int main()
{
    Board board_c;
    board_c.print_grid();
    board_c.turn = true;
    board_c.turn_number = 0;
    char source[2];
    char dest[2];
    int check;
    int mv;
    int quit = 0;

    while (!board_c.game_over(quit)) {
        
        printf("Select a piece to move and where to move it 'q' to quit:");
        check = scanf("%s", source);
        if(source[0] == 'q') {
            printf("Game Exiting \n");
            quit = 1;
            break;
        }

        if (check != 1)
            printf("Could not read source location input [Error: %d]\n", check);

        getchar();
        check = scanf("%s", dest);

        if (check != 1)
            printf("Could not read destination location input [Error: %d]\n", check);

        printf("\nx position = %c%c, y position = %c%c check = %d\n", source[0], source[1], dest[0], dest[1], check);
        mv = board_c.move(source[1] - '0', source[0], dest[1] - '0', dest[0]);
        if(mv == 1) {
            printf("Move Successful!\n");
            board_c.turn = !board_c.turn;
            board_c.turn_number ++;
            printf("Total moves: %d\n", board_c.turn_number);
        }
        else
            printf("Move Unsuccessful\n");

        board_c.print_grid();
    }

    return 0;
}

/* Function descriptions */
bool Board::game_over(int q)
{
    if(q == 0)
        return false;
    else
        return true;
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
    row = row - 1;
    new_row = new_row - 1;
    int piece = grid[row][col];
    int piece_dest = grid[new_row][new_col];
    printf("%s: Piece being moved: %d \t Piece at destination:%d\n", __func__, piece, piece_dest);
    bool move_ok, is_blocked;

    printf("%s: New Position found is %d %d\n", __func__, new_row, new_col);
    move_ok = check_move(piece, row, col, new_row, new_col, piece_dest);
    is_blocked = check_blocked(piece, row, col, new_row, new_col);

    if(move_ok && !is_blocked) {
        if (piece == WT_PWN &&  new_row == NUM_ROWS-1) /* Pawn Promotion */
            piece = WT_QEN;
        else if (piece == BK_PWN && new_row == 0)
            piece = BK_QEN;

        printf("Move is ok\n");
        grid[new_row][new_col] = piece; /* Populate square with moved piece */
        grid[row][col] = 0;             /* Empty Previous square */
        return 1;
    } else {
        printf("%s: Move not ok, block status = %d\n", __func__, is_blocked);
        return 0;
    }

}

/* Returns true if blocked */
bool Board::check_blocked(int piece, int row, int col, int new_row, int new_col)
{
    bool ret;
    switch (piece) {
        case WT_PWN:
            if((new_col == col && grid[new_row][new_col] != EMPTY) || (col != new_col && grid[new_row][new_col] == EMPTY))
                ret = true;
            else
                ret = false;
            break;
        case BK_PWN:
            if((new_col == col && grid[new_row][new_col] != EMPTY) || (col != new_col && grid[new_row][new_col] == EMPTY))
                ret = true;
            else
                ret = false;
            break;
        default:
            ret = false;
            printf("%s: Piece not found: %d\n", __func__, piece);
    }

    return ret;
}

bool Board::check_move(int piece, int row, int col, int new_row, int new_col, int piece_dest)
{
    int bw = piece % 2;
    int bw_dest = piece_dest % 2;
    bool ret;

    /* Check if there is a piece at this location */
    if (row > NUM_ROWS-1 || row < 0 || col > NUM_COLS-1 || col < 0) {
        printf("%s: Selected location Out of Bounds\n", __func__);
        return false; /* Piece out of bounds */
    } else if (piece == 0) {
        printf("%s: No Piece at Designated Location\n", __func__);
        return false; /* No Piece Found at Location */
    } else if (new_row > NUM_ROWS-1 || new_row < 0 || new_col > NUM_COLS-1 || new_col < 0) {
        printf("%s: Selected destination out of bounds\n", __func__);
        return false;
    } else if(bw == bw_dest && piece_dest != 0) {
        printf("%s: Move blocked by same-colored piece at destination %d %d\n", __func__, bw, bw_dest);
        return false; /* Another piece of the same color is on that square */
    }


    printf("%s: Piece at source location =%d\n", __func__, piece);
    if (piece_dest %2 != bw || piece_dest == 0) { /* Can move to this location */
        if (turn == true) {
            switch(piece) {
                case WT_PWN:
                    if(new_row == row+1 || (new_row == row+2 && row == 1 && col == new_col))
                        ret = true;
                    else
                        ret = false;
                    break;
                default:
                    ret = false;
                    printf("%s: That move is not valid for this game piece or it is not your turn\n", __func__);
            }
        } else {
            switch(piece) {
                case BK_PWN:
                    if(new_row == row-1 || (new_row == row-2 && row == 6 && col == new_col))
                        ret = true;
                    else
                        ret = false;
                    break;
                default:
                    ret = false;
                    printf("%s: That move is not valid for this game piece or it is not your turn\n", __func__);
            }
        }
    }
    return ret;
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
    grid[0][0] = WT_ROK;  /* White Rook   */
    grid[0][1] = WT_KNT;  /* White Knight */
    grid[0][2] = WT_BSP;  /* White Bishop */
    grid[0][3] = WT_QEN;  /* White Queen  */
    grid[0][4] = WT_KNG; /* White King   */
    grid[0][5] = WT_BSP;
    grid[0][6] = WT_KNT;
    grid[0][7] = WT_ROK;
    grid[1][0] = WT_PWN;  /* White Pawn   */
    grid[1][1] = WT_PWN;
    grid[1][2] = WT_PWN;
    grid[1][3] = WT_PWN;
    grid[1][4] = WT_PWN;
    grid[1][5] = WT_PWN;
    grid[1][6] = WT_PWN;
    grid[1][7] = WT_PWN;
    grid[6][0] = BK_PWN;  /*Black Pawn    */
    grid[6][1] = BK_PWN;
    grid[6][2] = BK_PWN;
    grid[6][3] = BK_PWN;
    grid[6][4] = BK_PWN;
    grid[6][5] = BK_PWN;
    grid[6][6] = BK_PWN;
    grid[6][7] = BK_PWN;
    grid[7][0] = BK_ROK;  /* Black Rook   */
    grid[7][1] = BK_KNT;  /* Black Knight */
    grid[7][2] = BK_BSP;  /* Black Bishop */
    grid[7][3] = BK_QEN; /* Black Queen  */
    grid[7][4] = BK_KNG; /* Black King   */
    grid[7][5] = BK_BSP;
    grid[7][6] = BK_KNT;
    grid[7][7] = BK_ROK;

    for(int row = 2; row < NUM_ROWS - 2; row++)
    {
        for(int col = 0; col < NUM_COLS; col++)
        {
            grid[row][col] = 0;
        }
    }
}