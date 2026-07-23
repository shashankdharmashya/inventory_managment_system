#include<stdio.h>
int stack[100] , top =-1;
int push(int x)
{
    stack[++top] =x ;
     return 0;
} 
int pop()
{
 return stack[top--];
}