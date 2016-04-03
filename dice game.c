#include <stdio.h>
#include <stdlib.h>

#define DIE_MAX     6
#define WIN_VALUE   0x1ff
#define ARRAY_LEN   9

char get_numbers (short *result, short total)
{
    char input[5];
    short i, bitop, utot = 0;
    
    fgets(input, sizeof(input), stdin);
    *result = 0;
    
    for (i = 0; i < sizeof(input) && input[i] != '\n'; i++) {
        bitop = input[i] - '0';
        if (bitop <= 0 || bitop >= 10)
            return 0;
        utot += bitop;
        bitop = (0x1 << (bitop - 1));
        if (*result & bitop)
            return 0;
        *result |= bitop;
    }
    
    return total == utot;
}

char valid_moves (short data, short total)
{
    short *ptr;
    short operands[12][12] = {
                /* 1 */ /* 2 */                             /* 3 */                     /* 4 */
/* 2 */        { 0x002,                                                                                            0x000 },
/* 3 */        { 0x004, 0x003,                                                                                     0x000 },
/* 4 */        { 0x008, 0x005,                                                                                     0x000 },
/* 5 */        { 0x010, 0x009, 0x006,                                                                              0x000 },
/* 6 */        { 0x020, 0x011, 0x00A,                      0x007,                                                  0x000 },
/* 7 */        { 0x040, 0x021, 0x012, 0x00C,               0x00B,                                                  0x000 },
/* 8 */        { 0x080, 0x041, 0x022, 0x014,               0x013, 0x00D,                                           0x000 },
/* 9 */        { 0x100, 0x081, 0x042, 0x024, 0x018,        0x023, 0x015,               0x00E,                      0x000 },
/* 10 */       {        0x101, 0x082, 0x044, 0x028,        0x043, 0x025, 0x019,        0x016,        0x00F,        0x000 },
/* 11 */       {               0x102, 0x084, 0x048, 0x030, 0x083, 0x045, 0x029,        0x026, 0x01A, 0x017,        0x000 },
/* 12 */       {                      0x104, 0x088, 0x050, 0x103, 0x085, 0x049, 0x031, 0x046, 0x02A, 0x027, 0x01B, 0x000 }
    };
    
    for (ptr = operands[total - 2]; *ptr; ptr++)
        if ((data & *ptr) == 0)
            return 1;    
    return 0;
}

int main (void)
{
    short result, data = 0;
    short die1, die2, i;

    do {
        die1 = (arc4random() % DIE_MAX) + 1;
        die2 = (arc4random() % DIE_MAX) + 1;
        printf("rolled a %d and %d\n", die1, die2);
        if (!valid_moves(data, die1 + die2)) {
            puts("you lose");
            data = 0;
        } else {        
            while (!get_numbers (&result, die1 + die2) || (data & result))
                puts("invalid input");
            data |= result;
        }
        puts("---------\n123456789");
        for (i = 0; i < ARRAY_LEN; i++)
            putchar((data & (0x1 << i)) ? '*' : '-');
        puts("\n---------\n");
    } while (data != WIN_VALUE);
    
    puts("you win");
    
    return 0;
}