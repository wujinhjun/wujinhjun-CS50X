#include <cs50.h>
#include <stdio.h>

void draw(int n);

int main(void)
{
    int n;
    do
    {
        n = get_int("Height: ");
    }
    while(n < 1 || n >= 9);

    draw(n);
}

void draw(int n)
{
    for(int i = 1; i <= n; i++)
    {
        for(int k = n - i; k > 0; k--)
        {
            printf(" ");
        }
        for(int k = i; k > 0; k--)
        {
            printf("#");
        }
        printf("  ");
        for(int k = i; k > 0; k--)
        {
            printf("#");
        }
        printf("\n");
    }
}