#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

bool onlyDigits(string str);
string ciphere(string str, int k);

int main(int argc, string argv[])
{
    if (argc == 2 && onlyDigits(argv[1]) && atoi(argv[1]) >= 0)
    {
        int k = atoi(argv[1]) % 26;
        string plainText = get_string("plaintext: ");

        int length = strlen(plainText);
        char ciphereText[length + 1];
        strcpy(ciphereText, plainText);

        for (int i = 0; i < length; i++)
        {
            char temp = plainText[i];
            if (temp <= 'z' && temp >= 'a')
            {
                ciphereText[i] = (((temp - 'a') + k) % 26) + 'a';
            }
            else if (temp <= 'Z' && temp >= 'A')
            {
                ciphereText[i] = (((temp - 'A') + k) % 26) + 'A';
            }
        }
        // string ciphereString = ciphere(plainText, k);
        printf("ciphertext: %s\n", ciphereText);
        return 0;
    }
    else
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
}

bool onlyDigits(string str)
{
    for (int i = 0, length = strlen(str); i < length; i++)
    {
        if (!isdigit(str[i]))
        {
            return false;
        }
    }
    return true;
}