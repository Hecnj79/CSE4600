#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"

int main(int argc,char *argv[])
{
  if(argc<2)
  {
    printf(1,"touch expects args\n");
    exit();
  }
  int i, fdd;
  for(i = 1; i < argc; i++)
  {
    if((fdd = open(argv[i], O_CREATE|O_RDWR)) < 0)
    {
      printf(1,"touch cannot open %s\n", argv[i]);
      exit();
    }
    close(fdd);
  }
  exit();
}
