#include "types.h"
#include "stat.h"
#include "user.h"
#include "sysexit.h"

int
main(int argc, char **argv)
{
  int i;

  if(argc < 2){
    printf(2, "usage: kill pid...\n");
    exit(EX_fail);
  }
  for(i=1; i<argc; i++)
    kill(atoi(argv[i]));
  exit(EX_succ);
}
