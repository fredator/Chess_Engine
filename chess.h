/*
*
* @brief: Definitions for main.cpp
*
*@author: Fredrik Treven
*@date: April 5th 2018
*
*
*/

#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

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

/* Colors */

#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_RESET "\x1b[0m"

/* Macros */
#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))