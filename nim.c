#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DEFAULT_ROWS 4

struct nim_board {
    int rows;
	char board[DEFAULT_ROWS];
	
	int (*u1_callback)(struct nim_board *nb);
	int (*u2_callback)(struct nim_board *nb);
};

/******************************************************************************/
/* Data helper functions 						      */
/******************************************************************************/

int xor_total (char *data, int length)
{
	int xtot = 0;
	
	while (length--)
		xtot ^= data[length];

	return xtot;
}

int max_ind (char *data, int length)
{
    int ind = 0;
    
    while (length--)
        if (data[length] > data[ind])
            ind = length;
    
    return ind;
}

/******************************************************************************/
/* UI helper functions 						   	      */
/******************************************************************************/

void print_board (struct nim_board *nb)
{
	char string[40];
	int i, j, n;
	
	for (i = 0; i < sizeof(string); i++)
		string[i] = ' ';
	
	for (i = 0; i < 4; i++) {
		n = (i * 8) + ((7 - nb->board[i]) >> 1);
        
		for (j = 0; j < nb->board[i]; j++)
			string[n++] = '|';
        
		string[((i + 1) * 8) - 1] = '\n';
	}
    
    string[(i * 8) - 1] = 0;
    
	puts(string);
}

void print_move (int line, int no)
{
	line |= ((no << 8) | 0x3030);
    
	puts((char *)&line);
}



/******************************************************************************/
/* movers	 						   	      */
/******************************************************************************/

int ai_move (struct nim_board *nb, int *heap, int *no)
{
    unsigned int i, heaps_one = 0;
    unsigned int tot = xor_total(nb->board, nb->rows);
    
    if (tot == 0) {
        *heap = max_ind(nb->board, nb->rows);
        if (nb->board[*heap] == 0)
            return 0;
        *no = 1;
    } else {
        *heap = nb->rows - 1;
        
        do {
            i = (nb->board[*heap] & ~tot) | (tot & ~(nb->board[*heap] & tot));
        } while ((nb->board[*heap] <= i) && (*heap)--);

        (*no) = nb->board[*heap] - i;
        
        i = 0;
        while (i < nb->rows && (nb->board[i] - ((*heap == i) * (*no)) <= 1))
            i++;
   
        if (i == nb->rows) {
            for (i = 0; i < nb->rows; i++) {
                if (nb->board[i] == 1)
                    heaps_one++;
                if (nb->board[i] > nb->board[*heap])
                    *heap = i;
            }

            *no = nb->board[*heap] - !(heaps_one & 1);
        }
    }

	return 1;
}


/******************************************************************************/
/* callbacks	 						   	      */
/******************************************************************************/

int ai_callback (struct nim_board *nb)
{
	int heap, no;
	
    if (ai_move(nb, &heap, &no) == 0)
        return 0;
    
    nb->board[heap] -= no;
	print_move(heap + 1, no);
    
	return 1;
}

int user_callback (struct nim_board *nb)
{
	char in[4];
	
	do {
		*(unsigned int *)&in[0] = 0;
		fgets(in, sizeof(in), stdin);
		*(unsigned short *)&in[0] -= 0x3030;
	} while ((in[0] > nb->rows) ||
             (in[1] > nb->board[in[0] - 1]) || (in[1] < 1) ||
	         (in[2] != '\n'));
    
	nb->board[in[0] - 1] -= in[1];
	
	return !!(*(unsigned int *)&nb->board[0]);
}


int main (void)
{
	struct nim_board nb;
    
    nb.rows = DEFAULT_ROWS;
    
	nb.board[0] = 1;
	nb.board[1] = 3;
	nb.board[2] = 5;
	nb.board[3] = 7;
	
	nb.u1_callback = user_callback;
	nb.u2_callback = ai_callback;
    
	while (1) {
		print_board(&nb);
		if (!nb.u1_callback(&nb)) {
			puts("game over u2 wins");
			break;
		}
		
		print_board(&nb);
		if (!nb.u2_callback(&nb)) {
			puts("game over u1 wins");
			break;
		}
	}
    
	return 0;
}

