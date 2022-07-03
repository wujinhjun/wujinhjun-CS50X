#include <cs50.h>
#include <stdio.h>

int caluNumDigits(long n);
long powNum(int a, int b);

int main(void)
{
    long credit, copy;
    int sum, numDigit;
    string type;
    do
    {
       credit = get_long("Number: ");
    }
    while(credit < 0);
    copy = credit;
    sum = 0;
    while(copy > 0)
    {
        int oddDigi = copy % 10;
        // printf("oddDigi: %i\t", oddDigi);
        sum += oddDigi;
        copy /= 10;
        int evenDigi = 2 * (copy % 10);
        // printf("evenDigi: %i\t", evenDigi);
        int evenDigiDis = (evenDigi % 10) + ((evenDigi / 10) % 10);
        // printf("evenDigiDis: %i\t", evenDigiDis);
        sum += evenDigiDis;
        // printf("sum: %i\n", sum);
        copy /= 10;
    }
    // printf("copy: %li\n", copy);
    // printf("sum: %i\n", sum);

    numDigit = caluNumDigits(credit);
    // printf("Numdigit: %i\n", numDigit);
    int title = credit / powNum(10, numDigit - 2);
    // printf("title: %i\n", title);
    if ((numDigit == 13 || numDigit == 16) && title / 10 == 4)
    {
        type = "VISA";
    }
    else if (numDigit == 15 && (title == 34 || title == 37))
    {
        type = "AMEX";
    }
    else if (numDigit == 16 && title >= 51 && title <= 55)
    {
        type = "MASTERCARD";
    }
    else
    {
        type = "INVALID";
    }

    if (sum % 10 == 0)
    {
        printf("%s\n", type);
    }
    else
    {
        printf("INVALID\n");
    }

}

// calculate the digit
int caluNumDigits(long n)
{
    int count;
    for (count = 0; n > 0; count ++)
    {
        n = n / 10;
    }
    return count;
}

// pow for the a
long powNum(int a, int b)
{
    long sum = a;
    for (int i = 1; i < b; i++)
    {
        sum *= a;
    }
    return sum;
}

