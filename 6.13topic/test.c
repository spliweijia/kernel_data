#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
 #include <unistd.h>
 #include <string.h>
int main(int argc, const char *argv[])
{
   int fd=-1;

   char buf[32]="hello world";
   fd=open("/dev/mydev",O_RDWR);
   if(fd==-1)
   {
        perror("fd open error\n");
   }


   write(fd,buf,sizeof(buf));
   memset(buf,0,sizeof(buf));
   read(fd,buf,sizeof(buf));
   printf("buf=%s\n",buf);

     close(fd);

    return 0;
}