#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void left (char array[10][10], int mx, int my, int ox, int oy, int *ch);
void right (char array[10][10], int mx, int my, int ox, int oy, int *ch);
void up (char array[10][10], int mx, int my, int ox, int oy, int *ch);
void down (char array[10][10], int mx, int my, int ox, int oy, int *ch);

void hide (char array[10][10], int no)
{
    int x, y;
    
    while (no--) {
        do {
            x = (arc4random() % 8) + 1;
            y = (arc4random() % 8) + 1;
        } while (array[x][y] == 1);
        
        array[x][y] = 1;
    }
}

void print_board (char array[10][10])
{
    int x, y;
    
    for (y = 0; y < 10; y++) {
        for (x = 0; x < 10; x++)
            putchar(array[x][y] == 1 ? '*' :
                    array[x][y] != 0 ? array[x][y] : '-');
        putchar('\n');
    }
}

void right (char array[10][10], int mx, int my, int ox, int oy, int *ch)
{
    if ((++mx) > 8) {
        if ((ox == mx) && (oy == my))
            array[ox][oy] = 'R';
        else
            array[ox][oy] = array[mx][my] = (*ch)++;
        return;
    }
    
    if (array[mx][my] == 1) {
        array[ox][oy] = 'H';
        return;
    }
    
    if ((mx == 1) && (array[mx][my + 1] == 1 || array[mx][my - 1] == 1)) {
        array[ox][oy] = 'R';
        return;
    }
    
    if (array[mx][my - 1] == 1)
        down(array, mx - 1, my, ox, oy, ch);
    else if (array[mx][my + 1] == 1)
        up(array, mx - 1, my, ox, oy, ch);
    else
        right(array, mx, my, ox, oy, ch);
}

void left (char array[10][10], int mx, int my, int ox, int oy, int *ch)
{
    if ((--mx) < 1) {
        if ((ox == mx) && (oy == my))
            array[ox][oy] = 'R';
        else
            array[ox][oy] = array[mx][my] = (*ch)++;
        return;
    }
    
    if (array[mx][my] == 1) {
        array[ox][oy] = 'H';
        return;
    }
    
    if ((mx == 9) && (array[mx][my + 1] == 1 || array[mx][my - 1] == 1)) {
        array[ox][oy] = 'R';
        return;
    }
    
    if (array[mx][my - 1] == 1)
        down(array, mx - 1, my, ox, oy, ch);
    else if (array[mx][my + 1] == 1)
        up(array, mx - 1, my, ox, oy, ch);
    else
        left(array, mx, my, ox, oy, ch);
}

void down (char array[10][10], int mx, int my, int ox, int oy, int *ch)
{
    if ((++my) > 8) {
        if ((ox == mx) && (oy == my))
            array[ox][oy] = 'R';
        else
            array[ox][oy] = array[mx][my] = (*ch)++;
        return;
    }
    
    if (array[mx][my] == 1) {
        array[ox][oy] = 'H';
        return;
    }
    
    if ((my == 1) && (array[mx - 1][my] == 1 || array[mx + 1][my] == 1)) {
        array[ox][oy] = 'R';
        return;
    }
    
    if (array[mx - 1][my] == 1)
        right(array, mx, my - 1, ox, oy, ch);
    else if (array[mx + 1][my] == 1)
        left(array, mx, my - 1, ox, oy, ch);
    else
        down(array, mx, my, ox, oy, ch);
}

void up (char array[10][10], int mx, int my, int ox, int oy, int *ch)
{
    if ((--my) < 1) {
        if ((ox == mx) && (oy == my))
            array[ox][oy] = 'R';
        else
            array[ox][oy] = array[mx][my] = (*ch)++;
        return;
    }
    
    if (array[mx][my] == 1) {
        array[ox][oy] = 'H';
        return;
    }
    
    if ((my == 9) && (array[mx - 1][my] == 1 || array[mx + 1][my] == 1)) {
        array[ox][oy] = 'R';
        return;
    }
    
    if (array[mx - 1][my] == 1)
        right(array, mx, my + 1, ox, oy, ch);
    else if (array[mx + 1][my] == 1)
        left(array, mx, my + 1, ox, oy, ch);
    else
        up(array, mx, my, ox, oy, ch);
}

void move (char array[10][10], int mx, int my, int *ch)
{
    if (mx == 0)
        right(array, mx, my, mx, my, ch);
    if (mx == 9)
        left(array, mx, my, mx, my, ch);
    if (my == 0)
        down(array, mx, my, mx, my, ch);
    if (my == 9)
        up(array, mx, my, mx, my, ch);
}

int main (int argc, const char * argv[])
{
    char array[10][10];
    int i;
    int ch = 'a';
    
    memset(array, 0, sizeof(array));
    
    hide(array, 5z);
    
    for (i = 1; i < 9; i++)
        if (!array[i][0])
            move(array, i, 0, &ch);
    for (i = 1; i < 9; i++)
        if (!array[i][9])
            move(array, i, 9, &ch);
    for (i = 1; i < 9; i++)
        if (!array[0][i])
            move(array, 0, i, &ch);
    for (i = 1; i < 9; i++)
        if (!array[9][i])
            move(array, 9, i, &ch);

    print_board(array);
    
    return 0;
}

