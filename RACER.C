/*
   Title  : Racer, Racing car game version 1.3
   Author : Antonino Iannella, September 25th, 1996
   Description:
          A simple command-line car being driven down a
          fast, winding track.   The white lines are the
          boundaries in which to keep the 'Î' car.

          It is compilable under GCC (with no 
          optimisations) using

          gcc -Zomf -Zsys -o racer.exe racer.c racer.def

          If using a different compiler, the only command
          which may need changing is the _read_kbd() in
          function main().  It just gets any character
          without waiting for user input, and puts it in
          'let'.  This is not the best way to read keyboard
          input, so if anyone knows a better method, I'd
          like to know...

Revision:
          Added a delay period with every write to slow down
          the game. Uses _sleep2(), an EMX-specific command.
          Also reports the distance travelled; every line is
          LINE_DISTANCE kilometres.
          Added a prompt to race again after a crash.
          Added the RACER.DEF file/
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/kbdscan.h>

                          /* Defines the screen width (SCR_WIDTH characters) */
#define  SCR_WIDTH         80
                            /* Track width; the space between the boundaries */
#define  TRACK_WIDTH       20

/* Revised 25-9-96 */
#define  LINE_DISTANCE     0.01 
#define  SLOW_THE_PC_DOWN  10                     /* Milliseconds per delay */

int cpos,                       /* Car position from left side of the screen */
    lpost,
    rpost;                             /* Set left and right road boundaries */

unsigned long rnd;                                /* Random number generator */

double Travelled;                                  /* For distance travelled */

/* ------------------------ GRUNDIG - Made for you! ------------------------ */

void Oneline(void)
/* Draws the next line, representing the track
   and car, at the bottom of the screen.
*/
{
  int pt1,
      pt2,
      pt3;                                                       /* Counters */

  rpost=lpost+TRACK_WIDTH;

  for(pt1=0; pt1 < lpost; pt1++)                           /* Draw left edge */
   printf(" ");

  printf("Û");

  for(pt2=lpost+1; pt2 < cpos; pt2++)                        /* Draw the car */
   printf(" ");

  printf("Î");

  for(pt3=cpos+1; pt3 < rpost; pt3++)                     /* Draw right edge */
   printf(" ");

  printf("Û\n");

  /* Delay the process to make the game run at a more human speed.
     EMX-only command. */
  _sleep2(SLOW_THE_PC_DOWN);

  Travelled += LINE_DISTANCE;                 /* Increase distance travelled */
}

/* ------------------------ GRUNDIG - Made for you! ------------------------ */

void Crash()
/* Exits the game if 'q' was pressed, or when
   the car 'crashes' into a boundary.
*/
{
  char goagain;

  printf("\nSmack! Prang! You lasted %.3f km (%.4f miles).\n", Travelled, Travelled/1.6);

  fflush(stdin);
  printf("Would you like to drive again [Y/N]");
  goagain=getchar();

  if ((goagain == 'N') || (goagain == 'n'))
  {
    printf("\n\nThank you for trying Racer. ");
    printf("Tell me what you think - antonino@usa.net.\n");
    exit(0);
  }
  else                                       /* Reset car to starting position */
  {
    Travelled=0;
    lpost=25;                                        /* Set left road boundary */
    cpos=lpost+TRACK_WIDTH/2;                       /* Initialise car position */
  }
}

/* ------------------------ GRUNDIG - Made for you! ------------------------ */

void track(int kkey)
/* Moves the car left or right, and uses random
   numbers to decide which way to move the track.
*/
{
  switch (kkey)
  {
    case 75 : cpos--;                                          /* Move left  */
              Oneline();
              break;

    case 77 : cpos++;                                          /* Move right */
              Oneline();
              break;

    default : Oneline();

  }

/* RAND_MAX defined in <stdlib.h>, is the maximum random number.
   This part says that if the random number is less than 1 3rd of RAND_MAX,
   move left.  If between 1 and 2 3rds, don't move.  Else, move right. */

  if ((cpos==lpost) || (cpos==lpost+TRACK_WIDTH))
    Crash();

  rnd=rand();

  if ((rnd< (RAND_MAX/3)) && (lpost>0))
     lpost--;
  else
    if ((rnd> (2*(RAND_MAX/3))) && (rpost < SCR_WIDTH))
     lpost++;
}

/* ------------------------ GRUNDIG - Made for you! ------------------------ */

main()
{
  char let;

  lpost=25;                                        /* Set left road boundary */
  cpos=lpost+TRACK_WIDTH/2;                       /* Initialise car position */
  Travelled=0;

  printf("\nRacer version 1.3 by Antonino Iannella, September 1996.\n");

  while(1)
  {
    let=_read_kbd(0, 0, 0);        /* No echoing, no waiting, disable CTRL-C */
    fflush(stdin);

    if ((let=='Q') || (let=='q'))
      Crash();
    else
      track(let);                                        /* Print next track */
  }
}