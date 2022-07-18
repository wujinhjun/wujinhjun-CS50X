#include<stdio.h>
// #include<stdin.h>
#include<cs50.h>

int main(void)
{
    string name = get_string("What's your name?\n");
    printf("hello, %s\n", name);
}