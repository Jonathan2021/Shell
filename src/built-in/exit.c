#include "bultins.h"

int exit(void)
{
  int exit_val = 0; // a recuperer plus tard

  // tout free

  exit(exit_val);
  return 0;
}
