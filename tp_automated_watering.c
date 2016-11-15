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

#define actions_path(path) "/var/www/html/data/" #path

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
  
  // Initialize vars
  bool isRunning = true;
  unsigned int i, j;

  // Initialize the automaton
  Automaton *a = automaton_new(2);
  
  automaton_set_pump_output(a, 24);
  automaton_set_water_volume_input(12);
  automaton_set_water_level_input(15);
  
  automaton_set_lamp_output(a, 0, 16);
  automaton_set_valve_output(a, 0, 18);
  
  automaton_set_lamp_output(a, 1, 22);
  automaton_set_valve_output(a, 1, 7);
  
  // 
  while(isRunning)
  {
      // Does the user want something to be done?
      for(i = 0, j = automaton_get_zones_number(a); i < j; i++)
      {
          // Turn off a lamp
          if(is_file(actions_path(lamp, i, off)))
          {
              // Turn down the pin output
              automaton_set_lamp_value(a, i, LOW);
              
              // Get rid of the file
              unlink(actions_path(lamp, i, off));
          }
          // Turn of a lamp
          else if(is_file(actions_path(lamp, i, on)))
          {
              // Turn up the pin output
              automaton_set_lamp_value(a, i, HIGH);
              
              // Get rid of the file
              unlink(actions_path(lamp, i, on));
          }
          // Close a valve
          else if(is_file(actions_path(valve, i, off)))
          {
              // Turn down the pin output
              automaton_set_valve_value(a, i, LOW);
              
              // Get rid of the file
              unlink(actions_path(valve, i, off));
          }
          // Open a valve
          else if(is_file(actions_path(valve, i, on)))
          {
              // Turn up the pin output
              automaton_set_valve_value(a, i, HIGH);
              
              // Get rid of the file
              unlink(actions_path(valve, i, on));
          }
      }
      
#ifdef HAVE_UNISTD_H
      sleep(1);
#endif
  }
  
  // Destroy the automaton so as to free the memory
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
