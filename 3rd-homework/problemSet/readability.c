#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int countLetter(string str);
int countWords(string str);
int countSents(string str);

int main(void)
{
    string text = get_string("Text: ");
    int letters = countLetter(text);
    int words = countWords(text);
    int sents = countSents(text);
    float l = (1.0 * letters / words) * 100;
    float s = (1.0 * sents / words) * 100;
    float index = 0.0588 * l - 0.296 * s - 15.8;
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", (int)(index + 0.5));
    }
}

int countLetter(string str)
{
    int count = 0;
    for (int i = 0, length = strlen(str); i < length; i++)
    {
        char temp = str[i];
        if ((temp >= 'a' && temp <= 'z') || (temp >= 'A' && temp <= 'Z'))
        {
            count ++;
        }
    }
    return count;
}

int countWords(string str)
{
    int count = 0;
    for (int i = 0, length = strlen(str); i < length; i++)
    {
        char temp = str[i];
        if (temp == ' ')
        {
            count ++;
        }
    }
    return count + 1;
}

int countSents(string str)
{
    int count = 0;
    for (int i = 0, length = strlen(str); i < length; i++)
    {
        char temp = str[i];
        if ((temp == '?') || (temp == '.') || (temp == '!'))
        {
            count ++;
        }
    }
    return count;
}