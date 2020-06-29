#include "../../h/const.h"
#include "../../h/types.h"
#include "../../h/vpop.h"

#include "../../h/procq.e"
#include "../../h/asl.e"



intinit()
{
 
}

waitforpclock(old)
state_t *old;
{
}

waitforio(old)
state_t *old;
{
}

intdeadlock()
{
print("halt: end of program");
/*
asm("trap #4");
*/
  HALT();
}

intschedule()
{
}
