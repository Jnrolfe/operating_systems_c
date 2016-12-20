#include <stdio.h>     /* standard I/O functions                         */
#include <stdlib.h>    /* exit                                           */
#include <unistd.h>    /* standard unix functions, like getpid()         */
#include <string.h>    /* using ?memsec?                                 */
#include <signal.h>    /* signal name macros, and the signal() prototype */
#include <stdbool.h>

/* first, define the Ctrl-C counter, initialize it with zero. */
int ctrl_c_count = 0;
int got_response = 0;
#define CTRL_C_THRESHOLD  5 
bool valid_alrm;

/* the Ctrl-C signal handler */
void catch_int(int sig_num)
{
  /* increase count, and check if threshold was reached */
  ctrl_c_count++;
  if (ctrl_c_count >= CTRL_C_THRESHOLD) {
    char answer[30];

    /* prompt the user to tell us if to really
     * exit or not */
    printf("\nReally exit? [Y/n]: ");
    fflush(stdout);
    alarm(10);
    valid_alrm = true;    
    fgets(answer, sizeof(answer), stdin);
    if (answer[0] == 'n' || answer[0] == 'N') {
      printf("\nContinuing\n");
      valid_alrm = false;
      fflush(stdout);
      /* 
       * Reset Ctrl-C counter
       */
      ctrl_c_count = 0;
    }
    else {
      printf("\nExiting...\n");
      fflush(stdout);
      exit(0);
    }
  }
}

/* the Ctrl-Z signal handler */
void catch_tstp(int sig_num)
{
  /* print the current Ctrl-C counter */
  printf("\n\nSo far, '%d' Ctrl-C presses were counted\n\n", ctrl_c_count);
  fflush(stdout);
}

/* the Alarm signal handler */
void catch_alrm(int sig_num)
{
    if(valid_alrm)
    {
        printf("User taking too long to respond. Exiting ...\n");
        exit(0);
    }
    else
    {
        return;
    }
}

int main(int argc, char* argv[])
{
  /***** FOR CTRL-C *****/ 
  struct sigaction sa;
  sigset_t mask_set;  /* used to set a signal masking set. */

  // set bytes of size sigaction at &sa memory to 0
  memset(&sa, 0, sizeof(struct sigaction));

  /* setup mask_set */
  // fill mask_set i.e. block all signals except kill and stop
  sigfillset(&mask_set);
  // unblock alarm signal
  sigdelset(&mask_set, SIGALRM);
  // only blocks all signals inside of the sigaction struct
  sa.sa_mask = mask_set;

  /* set signal handlers */
  // set the handler to be the catch_int() function
  sa.sa_handler = catch_int;
  // set up the actual signal handler
  sigaction(SIGINT, &sa, NULL);

  /***** FOR CTRL-Z *****/
  struct sigaction sa_z;
  sigset_t mask_set_z;

  memset(&sa_z, 0, sizeof(struct sigaction));

  sigfillset(&mask_set_z);
  sa_z.sa_mask = mask_set_z;

  sa_z.sa_handler = catch_tstp;
  sigaction(SIGTSTP, &sa_z, NULL);

  /***** FOR Alarm *****/
  struct sigaction sa_a;
  sigset_t mask_set_a;

  memset(&sa_a, 0, sizeof(struct sigaction));

  sigfillset(&mask_set_a);
  sa_z.sa_mask = mask_set_a;

  sa_a.sa_handler = catch_alrm;
  sigaction(SIGALRM, &sa_a, NULL);

  while(true)
  {
    pause(); // run continuously
  } 
  return 0;
}

