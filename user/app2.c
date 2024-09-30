/**
 * @file app2.c
 * @brief Game of life in C
 *
 * @author Ottavio Buonomo & Jean-Daniel Küenzi,
 * ottavio.buonomo@etu.hesge.ch & jean-daniel.kuenzi@etu.hesge.ch
 * @bug No known bugs.
 * @date February 7, 2021
 * @version 0.1
 */
#include "ulibc.h"
#include "../common/lib/utils.h"
#include "../common/lib/string.h"

#define ROWS 24
#define COLUMNS 80

void init(uint8_t grid[ROWS][COLUMNS])
{
    for (uint8_t i = 0; i < ROWS; i++)
    {
        for (uint8_t j = 0; j < COLUMNS; j++)
        {
            grid[i][j] = (uint8_t)(random(2));
        }
    }
}

uint8_t count_cells(uint8_t grid[ROWS][COLUMNS], int8_t y, int8_t x)
{
    uint8_t count = 0;
    count = (x - 1 >= 0 && y - 1 >= 0) ? count + grid[y - 1][x - 1] : count;
    count = (y - 1 >= 0) ? count + grid[y - 1][x] : count;
    count = (x + 1 < COLUMNS && y - 1 >= 0) ? count + grid[y - 1][x + 1] : count;

    count = (x - 1 >= 0) ? count + grid[y][x - 1] : count;
    count = (x + 1 < COLUMNS) ? count + grid[y][x + 1] : count;

    count = (x - 1 >= 0 && y + 1 < ROWS) ? count + grid[y + 1][x - 1] : count;
    count = (y + 1 < ROWS) ? count + grid[y + 1][x] : count;
    count = (x + 1 < COLUMNS && y + 1 < ROWS) ? count + grid[y + 1][x + 1] : count;

    return count;
}

void time_step(uint8_t grid[ROWS][COLUMNS], uint8_t temp_grid[ROWS][COLUMNS])
{
    for (uint8_t y = 0; y < ROWS; y++)
    {
        for (uint8_t x = 0; x < COLUMNS; x++)
        {
            uint8_t nb_cells = count_cells(grid, y, x);
            temp_grid[y][x] = (!grid[y][x] && (nb_cells == 3)) ||
                              (grid[y][x] && ((nb_cells == 2) || (nb_cells == 3)));
        }
    }
}

void print_grid(uint8_t grid[ROWS][COLUMNS])
{
    for (uint8_t y = 0; y < ROWS; y++)
    {
        for (uint8_t x = 0; x < COLUMNS; x++)
        {
            if (grid[y][x])
            {
                puts_x_y("o", x, y);
            }
            else
            {
                puts_x_y(" ", x, y);
            }
        }
    }
}

int main(char *argv[], int argc)
{
    int nb_iters;
    if (argc <= 1)
    {
        char buffer_nb_iters[XXS_BUFFER];
        puts("Please enter how many iterations you want : ");
        read_string(buffer_nb_iters, XXS_BUFFER);
        nb_iters = atoi(buffer_nb_iters);
    }
    else
    {
        nb_iters = atoi(argv[1]);
    }

    uint8_t grid[ROWS][COLUMNS];
    uint8_t temp_grid[ROWS][COLUMNS];
    init(grid);

    clean_vid_x_y(0, 0);
    disable_cursor();

    for (int i = 0; i < nb_iters; i++)
    {
        print_grid(grid);
        time_step(grid, temp_grid);
        memcpy(grid, temp_grid, ROWS * COLUMNS * sizeof(uint8_t));
        sleep(100);
    }

    setcursor(0, 24);
    enable_cursor();
    return 1;
}
