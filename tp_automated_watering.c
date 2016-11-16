/* 
   tp_automated_watering - 

   Copyright (C) 2016 Bruno Buiret

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  

#include <termios.h>
#include <grp.h>
#include <pwd.h>
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdbool.h>
#include <time.h>
#include <argp.h>
#include <wiringPi.h>
#include "system.h"
#include "src/automaton.h"
#include "src/utils.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#if ENABLE_NLS
# include <libintl.h>
# define _(Text) gettext (Text)
#else
# define textdomain(Domain)
# define _(Text) Text
#endif
#define N_(Text) Text

static error_t parse_opt (int key, char *arg, struct argp_state *state);
static void show_version (FILE *stream, struct argp_state *state);

/* argp option keys */
enum {DUMMY_KEY=129
};

/* Option flags and variables.  These are initialized in parse_opt.  */


static struct argp_option options[] =
{
  { NULL, 0, NULL, 0, NULL, 0 }
};

/* The argp functions examine these global variables.  */
const char *argp_program_bug_address = "<bruno.buiret@etu.univ-lyon1.fr>";
void (*argp_program_version_hook) (FILE *, struct argp_state *) = show_version;

static struct argp argp =
{
  options, parse_opt, N_("[FILE...]"),
  N_(""),
  NULL, NULL, NULL
};

int
main (int argc, char **argv)
{
  textdomain(PACKAGE);
  argp_parse(&argp, argc, argv, 0, NULL, NULL);
  // wiringPiSetup();
  
  // Initialize vars
  bool isRunning = true;
  unsigned int i, j;

  // Initialize the automaton
  printf("Initializing automaton... ");
  Automaton *a = automaton_new(2);
  
  automaton_set_pump_output(a, 24);
  automaton_set_water_volume_input(a, 12);
  automaton_set_water_level_input(a, 15);
  
  automaton_set_lamp_output(a, 0, 16);
  automaton_set_valve_output(a, 0, 18);
  
  automaton_set_lamp_output(a, 1, 22);
  automaton_set_valve_output(a, 1, 7);
  
  printf("Initialization complete.\n");
  
  // 
  while(isRunning)
  {
      // Does the user want something to be done?
      for(i = 0, j = automaton_get_zones_number(a); i < j; i++)
      {
          // Initialize paths
          char *lampOffPath = actions_path("lamp", i + 1, "off");
          char *lampOnPath = actions_path("lamp", i + 1, "on");
          // char *valveOffPath = actions_path("valve", i + 1, "off");
          char *valveOnPath = actions_path("valve", i + 1, "on");
          
          // Turn off a lamp
          if(is_file(lampOffPath))
          {
              // Log action
              printf("Turning off lamp #%u.\n", i + 1);
                
              // Turn down the pin output
              automaton_set_lamp_value(a, i, LOW);
              
              // Get rid of the file
              unlink(lampOffPath);
          }
          // Turn of a lamp
          else if(is_file(lampOnPath))
          {
              // Log action
              printf("Turning on lamp #%u.\n", i + 1);
              
              // Turn up the pin output
              automaton_set_lamp_value(a, i, HIGH);
              
              // Get rid of the file
              unlink(lampOnPath);
          }
          // Close a valve
          /*
          else if(is_file(valveOffPath))
          {
              // Log action
              printf("Closing valve #%u.\n", i + 1);
              
              // Turn down the pin output
              automaton_set_valve_value(a, i, LOW);
              
              // Get rid of the file
              unlink(valveOffPath);
          }
          */
          
          // Open a valve
          if(is_file(valveOnPath))
          {
              // Log action
              printf("Trying to open valve #%u.\n", i + 1);
              
              /*
              if(automaton_read_water_level(a) == LOW)
              {
                  // Open valve file
                  FILE *handle = fopen(valveOnPath, "r");
                  
                  if(handle != NULL)
                  {
                      // Read wanted water volume
                      unsigned int wantedWaterVolume = 0, currentWaterVolume = 0;
                      int previousOutput = 0, currentOutput;
                      
                      fscanf(handle, "%u", &wantedWaterVolume);
                      fclose(handle);
                      
                      // Turn up the valve's pin output
                      automaton_set_valve_value(a, i, HIGH);
                      
                      // Turn up the pump's pin output
                      automaton_set_pump_value(a, HIGH);
                      
                      do
                      {
                          printf("Water volume: %u / %u\n", currentWaterVolume, wantedWaterVolume);
                          
                          currentOutput = automaton_read_water_volume(a);
                          
                          if(previousOutput != currentOutput)
                          {
                              previousOutput = currentOutput;
                              
                              if(currentOutput == HIGH)
                              {
                                  currentWaterVolume++;
                              }
                          }
                          
                          // Sleep so as not to overload the processor
                          struct timespec ts;
                          ts.tv_sec = 0.5;
                          ts.tv_nsec = 0;
                          
                          nanosleep(&ts, NULL);
                      }
                      while(currentWaterVolume < wantedWaterVolume);
                      
                      automaton_set_pump_value(a, LOW);
                      automaton_set_valve_value(a, i, LOW);
                  }
                  else
                  {
                      fprintf(stderr, "Can't open file \"%s\".\n", valveOnPath);
                  }
              }
              else
              {
                fprintf(stderr, "Not enough water to hydrate zone #%u.\n", i + 1);
              }
              */
              
              // Get rid of the file
              unlink(valveOnPath);
          }
          
          // Free paths memory
          free(lampOffPath);
          free(lampOnPath);
          // free(valveOffPath);
          free(valveOnPath);
      }
      
      // Dump current automaton's state
      // automaton_dump_state(a, "/var/www/html/data/automaton.ini");
      
#ifdef HAVE_UNISTD_H
      sleep(1);
#endif
  }
  
  // Destroy the automaton so as to free the memory
  printf("Destroying automaton.\n");
  automaton_destroy(&a);

  return EXIT_SUCCESS;
}

/* Parse a single option.  */
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  switch (key)
    {
    case ARGP_KEY_INIT:
      /* Set up default values.  */
      break;


    case ARGP_KEY_ARG:		/* [FILE]... */
      /* TODO: Do something with ARG, or remove this case and make
         main give argp_parse a non-NULL fifth argument.  */
      break;

    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}

/* Show the version number and copyright information.  */
static void
show_version (FILE *stream, struct argp_state *state)
{
  (void) state;
  /* Print in small parts whose localizations can hopefully be copied
     from other programs.  */
  fputs(PACKAGE" "VERSION"\n", stream);
  fprintf(stream, _("Written by %s.\n\n"), "Bruno Buiret");
  fprintf(stream, _("Copyright (C) %s %s\n"), "2016", "Bruno Buiret");
  fputs(_("\
This program is free software; you may redistribute it under the terms of\n\
the GNU General Public License.  This program has absolutely no warranty.\n"),
	stream);
}
