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
#include <stdlib.h>

#include "chess.h"

using namespace std;

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
        bool check_move(int, int, int, int, int, int);
        bool check_blocked(int, int, int, int, int);
        bool turn;                  /* Black or White's turn (true = white, false = black) */
        int turn_number;
        int quit;
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
    board_c.quit = 0;

    while (!board_c.game_over()) {
        
        printf("Select a piece to move and where to move it 'q' to quit:");
        check = scanf("%s", source);
        if (source[0] == 'q') {
            printf("Game Exiting \n");
            board_c.quit = 1;
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
        if (mv == 1) {
            printf("Move Successful!\n");
            board_c.turn = !board_c.turn;
            board_c.turn_number ++;
            printf("Total moves: %d\n", board_c.turn_number);
        } else {
            printf("Move Unsuccessful\n");
            printf("Total moves: %d\n", board_c.turn_number);
        }

        board_c.print_grid();
    }

    return 0;
}

/* Function descriptions */
bool Board::game_over(void)
{
    if(quit == 0)
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
    printf("%s: Piece being moved: %d \t Piece at destination: %d\n", __func__, piece, piece_dest);
    bool move_ok, is_blocked;

    printf("%s: New Position found is %d %d\n", __func__, new_row, new_col);
    move_ok = check_move(piece, row, col, new_row, new_col, piece_dest);
    is_blocked = check_blocked(piece, row, col, new_row, new_col);

    if (move_ok && !is_blocked) {
        if (piece == WT_PWN &&  new_row == NUM_ROWS-1) /* Pawn Promotion */
            piece = WT_QEN;
        else if (piece == BK_PWN && new_row == 0)
            piece = BK_QEN;

        printf("Move is ok\n");
        grid[new_row][new_col] = piece;     /* Populate square with moved piece */
        grid[row][col] = EMPTY;             /* Empty Previous square */
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
    int d_row, d_col;
    int s_row, s_col;
    int warn = 0;

    if (piece == WT_PWN || piece == BK_PWN) {
        if ((new_col == col && grid[new_row][new_col] != EMPTY) || (col != new_col && grid[new_row][new_col] == EMPTY))
            ret = true;
        else
            ret = false;
    } else if (piece == WT_KNT || piece == BK_KNT) { /* Knights cannot be blocked in this way */
        ret = false;
    } else if (piece == WT_BSP || piece == BK_BSP) {
        d_row = new_row - row;
        d_col = new_col - col;
        s_row = (d_row > 0) - (d_row < 0); /* Get sign of diff */
        s_col = (d_col > 0) - (d_col < 0);

        for (int x = row + s_row; x != new_row; x += s_row) {
            for (int y = col + s_col; y != new_col; y += s_col) {
                if (grid[x][y] != EMPTY) {
                    warn ++;
                    printf("%s: Checking grid[%d][%d] with a warn now = %d\n", __func__, x, y, warn);
                }
            }
        }

        if (warn != 0)
            ret = true;
        else
            ret = false;
    } else if (piece == WT_ROK || piece == BK_ROK) {
        d_row = new_row - row;
        d_col = new_col - col;

        if (d_row == 0) {
            s_col = (d_col > 0) - (d_col < 0); /* Get sign of diff */
            for (int x = col + s_col; x != new_col; x += s_col) {
                if (grid[row][x] != EMPTY) {
                    warn ++;
                    printf("%s: Checking grid[%d][%d] with a warn now = %d\n", __func__, row, x, warn);
                }
            }
        } else {
            s_row = (d_row > 0) - (d_row < 0);
            for (int x = row + s_row; x != new_row; x += s_row) {
                if(grid[x][col] != EMPTY) {
                    warn ++;
                    printf("%s: Checking grid[%d][%d] with a warn now = %d\n", __func__, x, col, warn);
                }
            }
        }

        if(warn != 0)
            ret = true;
        else
            ret = false;
    } else if (piece == WT_QEN || piece == BK_QEN) {
        d_row = new_row - row;
        d_col = new_col - col;
        s_row = (d_row > 0) - (d_row < 0); /* Get sign of diff */
        s_col = (d_col > 0) - (d_col < 0);


        /* Check Horizontal/Vertical */
        if (d_row == 0 || d_col == 0) {
            if (d_row == 0) {
                s_col = (d_col > 0) - (d_col < 0); /* Get sign of diff */
                for (int x = col + s_col; x != new_col; x += s_col) {
                    if (grid[row][x] != EMPTY) {
                        warn ++;
                        printf("%s: Checking grid[%d][%d] with a warn now = %d\n", __func__, row, x, warn);
                    }
                }
            } else {
                s_row = (d_row > 0) - (d_row < 0);
                for (int x = row + s_row; x != new_row; x += s_row) {
                    if(grid[x][col] != EMPTY) {
                        warn ++;
                        printf("%s: Checking grid[%d][%d] with a warn now = %d\n", __func__, x, col, warn);
                    }
                }
            }
        } else { /* Check Diagonal */
            for (int x = row + s_row; x != new_row; x += s_row) {
                for (int y = col + s_col; y != new_col; y += s_col) {
                    if (grid[x][y] != EMPTY) {
                        warn ++;
                        printf("%s: Checking grid[%d][%d] with a warn now = %d\n", __func__, x, y, warn);
                    }
                }
            }
        }

            if (warn != 0)
                ret = true;
            else
                ret = false;
        } else {
            ret = true;
            printf("%s: Piece not found: %d\n", __func__, piece);
    }

    return ret;
}

bool Board::check_move(int piece, int row, int col, int new_row, int new_col, int piece_dest)
{
    int bw = piece % 2;
    int bw_dest = piece_dest % 2;
    bool ret;
    int c_row;
    int c_col;

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
    } else if (bw == bw_dest && piece_dest != 0) {
        printf("%s: Move blocked by same-colored piece at destination %d %d\n", __func__, bw, bw_dest);
        return false; /* Another piece of the same color is on that square */
    }

    printf("%s: Piece at source location = %d\n", __func__, piece);
    if (turn == true) { /* White's turn */
        switch (piece) {
            case WT_PWN:
                if (new_row == row+1 || (new_row == row+2 && row == 1 && col == new_col))
                    ret = true;
                else
                    ret = false;
                break;
            case WT_KNT:
                if (new_row == row+1 || new_row == row-1) {
                    if(new_col == col+2 || new_col == col-2)
                        ret = true;
                    else
                        ret = false;
                } else if (new_row == row+2 || new_row == row-2) {
                    if(new_col == col+1 || new_col == col-1)
                        ret = true;
                    else
                        ret = false;
                }
                break;
            case WT_BSP:
                c_row = row - new_row;
                c_col = col-new_col;
                if (c_row != 0 && abs(c_row) == abs(c_col))
                    ret = true;
                else
                    ret = false;
                break;
            case WT_ROK:
                if ((new_row == row && new_col != col) || (new_row != row && new_col == col))
                    ret = true;
                else
                    ret = false;
                break;
            case WT_QEN:
                c_row = row - new_row;
                c_col = col-new_col;
                if ((c_row != 0 && abs(c_row) == abs(c_col)) || ((new_row == row && new_col != col) || (new_row != row && new_col == col)))
                    ret = true;
                else
                    ret = false;
                break;
            default:
                ret = false;
                printf("%s: That move is not valid for this game piece or it is not your turn\n", __func__);
        }
    } else { /* Black's turn */
        switch (piece) {
            case BK_PWN:
                if (new_row == row-1 || (new_row == row-2 && row == 6 && col == new_col))
                    ret = true;
                else
                    ret = false;
                break;
            case BK_KNT:
                if (new_row == row+1 || new_row == row-1) {
                    if(new_col == col+2 || new_col == col-2)
                        ret = true;
                    else
                        ret = false;
                } else if (new_row == row+2 || new_row == row-2) {
                    if(new_col == col+1 || new_col == col-1)
                        ret = true;
                    else
                        ret = false;
                }
                break;
            case BK_BSP:
                c_row = row - new_row;
                c_col = col-new_col;
                if (c_row != 0 && abs(c_row) == abs(c_col))
                    ret = true;
                else
                    ret = false;
                break;
            case BK_ROK:
                if ((new_row == row && new_col != col) || (new_row != row && new_col == col))
                    ret = true;
                else
                    ret = false;
                break;
            case BK_QEN:
                c_row = row - new_row;
                c_col = col-new_col;
                if ((c_row != 0 && abs(c_row) == abs(c_col)) || ((new_row == row && new_col != col) || (new_row != row && new_col == col)))
                    ret = true;
                else
                    ret = false;
                break;
            default:
                ret = false;
                printf("%s: That move is not valid for this game piece or it is not your turn\n", __func__);
        }
    }

    return ret;
}

void Board::print_grid(void)
{
    for(int row = NUM_ROWS-1; row >= 0; row--) {
        printf("%d\t", numbers[row]);

        for(int col = 0; col < NUM_COLS; col++)
        {
            int val = grid[row][col];
            if(val %2 == 0)
                printf(ANSI_COLOR_MAGENTA "%s\t" ANSI_COLOR_RESET, names[val]);
            else
                printf(ANSI_COLOR_YELLOW "%s\t" ANSI_COLOR_RESET, names[val]);
        }
        printf("\n");
        printf("\n");
    }

    printf("\t");
    for (int y = 0; y < NUM_COLS; y++) {
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
    grid[0][4] = WT_KNG;  /* White King   */
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
    grid[6][0] = BK_PWN;  /* Black Pawn    */
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
    grid[7][3] = BK_QEN;  /* Black Queen  */
    grid[7][4] = BK_KNG;  /* Black King   */
    grid[7][5] = BK_BSP;
    grid[7][6] = BK_KNT;
    grid[7][7] = BK_ROK;

    for(int row = 2; row < NUM_ROWS - 2; row++)
    {
        for(int col = 0; col < NUM_COLS; col++) {
            grid[row][col] = EMPTY;
        }
    }
}