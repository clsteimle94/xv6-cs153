#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "sysexit.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  int status;
  if(argint(0, &status) < 0)
    return -1;
  exit(status);
  return 0;  // not reached
}

int
sys_wait(void)
{
  int *status;
  if(argptr(0, (char**)&status, sizeof(int)) < 0)
    return -1;
  return wait(status);
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// ADDED sys_waitpid()
int 
sys_waitpid(void)
{
  int pid;
  int *status;
  int options;
  
  //The system call must return the process id of the child that was terminated or -1 if no child exists (or unexpected error occurred)  
  if(argint(0, &pid) < 0) return -1; //pid check pulled from sys_kill()
  if(argptr(1, (char**)&status, sizeof(int)) < 0) return -1;
  if(argint(2, &options) < 0) return -1;

  //From syscall.c line 48
  // Fetch the nth 32-bit system call argument.
  //int argint(int n, int *ip)

  //From syscall.c line 58
  // Fetch the nth word-sized system call argument as a pointer
  // to a block of memory of size bytes.  Check that the pointer
  // lies within the process address space.
  //int argptr(int n, char **pp, int size)

  //From sysfile.c line 76
  //if(argfd(0, 0, &f) < 0 || argint(2, &n) < 0 || argptr(1, &p, n) < 0)

  //Add a waitpid system call: int waitpid(int pid, int *status, int options).
  return waitpid(pid, status, options); //needs to return the call for waitpid
}