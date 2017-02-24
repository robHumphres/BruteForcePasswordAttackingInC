#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include </usr/include/crypt.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h>


void pdie(const char *);
void die(const char *);
void bruteforce(char **bp,int total);
void encrypting(char * password, char e[], char ** bp);
//#define BUFFER_SIZE 1024   /* Size of the read/write buffer. */

int main(int argc, char* argv[])
{
   int total;
   int rfd;   /* Read file descriptor. */
   char buffer[35];   /* Read/Write buffer. */
   char **bp;   /* Pointer into write buffer. */
   int bufferChars;   /* Number of bytes remaining to be written. */ 
   /* Open file to be copied. */
   if ((rfd = open(argv[1], O_RDONLY, 0)) < 0)
      pdie("Open failed");
   bp = malloc(3 * sizeof(char*));//char array
   int i =0;// while loop reading in 
   while (1)
   {
      if ((bufferChars = read(rfd, buffer, sizeof(buffer))) > 0)
      {
         bp[i] = malloc(35 * sizeof *bp[i]);
         bp[i] = strdup(buffer);   /* Pointer to next byte to write. */
         i++;
         bufferChars -=1;
      }
      else if (bufferChars == 0)   /* EOF reached. */
         break;
      else   /* bufferChars < 0 --- read failure. */
         pdie("Read failed");
   }
   //printf("%s",bp[0]);      printf("%s",bp[1]);       printf("%s",bp[2]);//debug statement 
   close(rfd);//close read in
   total = i;
   bruteforce(bp,total);
   FILE *f = fopen(argv[2],"w");
   int x;
   for(x=0;x<total;x++)
   {
      fprintf(f,"%s",bp[x]);
   }
   fclose(f);//close write out
   for(x=0;x<total;x++)
   {
      free(bp[x]);
   }
   free(bp);
   return 0;
}//end of main

void bruteforce(char **bp,int total)
{ char * password;
   int a,b,c,d,x;
   for(a='a';a<'z';a++)
      for(b='a';b<'z';b++)
         for(c='a';c<'z';c++)
            for(d='a';d<'z';d++)
            {
               char e[4];
               e[0] = (char)a;
               e[1] = (char)b;
               e[2] = (char)c;
               e[3] = (char)d;
               encrypting(password,e,bp);
            }
            
}//end of bruteforce
void encrypting(char * password, char e[],char ** bp)
{  const char *const pass1 = "$1$6gMKIopE$I.zkP2EvrXHDmApzYoV.B.";
   const char *const pass2 = "$1$pkMKIcvE$WQfqzTNmcQr7fqsNq7K2p0";
   const char *const pass3 = "$1$0lMKIuvE$7mOnlu6RZ/cUFRBidK7PK.";
   unsigned long seed[2];
   char salt[] = "$1$........";
   const char *const seedchars =
    "./0123456789ABCDEFGHIJKLMNOPQRST"
    "UVWXYZabcdefghijklmnopqrstuvwxyz";
   //char *password;
   int i;

  /* Generate a (not very) random seed.
     You should do it better than this... */
   seed[0] = time(NULL);
   seed[1] = getpid() ^ (seed[0] >> 14 & 0x30000);

  /* Turn it into printable characters from Ã¢â‚¬ËœseedcharsÃ¢â‚¬â„¢. */
   for (i = 0; i < 8; i++)
      salt[3+i] = seedchars[(seed[i/5] >> (i%5)*6) & 0x3f];

  /* Read in the userÃ¢â‚¬â„¢s password and encrypt it. */
   password = ((char*)crypt(e, pass1));
   if(strcmp(password,pass1)==0)
   {bp[0] = strdup(e);}
   password = ((char*)crypt(e, pass2));
   if(strcmp(password,pass2)==0)
   {bp[1]= strdup(e);}
   password = ((char*)crypt(e, pass3));
   if(strcmp(password,pass3)==0)
   {bp[2] = strdup(e);}
}

void pdie(const char *mesg)
{
   perror(mesg);
   exit(1);
}

void die(const char *mesg)
{
   fputs(mesg,stderr);
   fputc('\n',stderr);
}

