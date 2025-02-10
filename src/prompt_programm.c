#include <stdio.h>

int main()
{
char name[50];
int age;

printf("Hello, This is prompt program!\n");

printf("Please enter Your Name: ");
scanf("%s", name);

printf("Please enter Your age: ");
if (scanf("%d", &age) != 1) {
    printf("Invalid input. Please enter a valid number.\n");
    return 1;
}
if ((age < 0) || (age > 200))
{
    printf("ERROR: The age %d shall be in range from 0 to 200.\n", age);
    return 2;
}
if (age == 10) printf("Hello, %s, glad to meet You, I am 10 Yers old, You are the same age as me!\n", name);
else if (age < 10) printf("Hello, %s, glad to meet You, I am 10 Yers old, You younger than me!\n", name);
else printf("Hello, %s, glad to meet You, I am 10 Yers old, You older than me!\n", name);

return 0;
}
