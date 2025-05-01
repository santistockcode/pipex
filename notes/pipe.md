#include <unistd.h>
#include <stdio.h>

int main()
{
 int  pfd[2];
 char str[20];

 pipe(pfd);
 write(pfd[1], "hello", 5);
 read(pfd[0], str, 2);
 str[2] = '\0';
 printf("%s\n",str);
}

ACHTUNG: 
https://jan.newmarch.name/OS/l9_1.html
https://people.cs.rutgers.edu/~pxk/416/notes/c-tutorials/pipe.html?source=post_page-----5b4afa4a37b7---------------------------------------

