/* 
 * @author Thomas Arnaud <thomas.arnaud@etu.univ-lyon1.fr>
 * @author Bruno Buiret <bruno.buiret@etu.univ-lyon1.fr>
 * @author Alexis Rabilloud <alexis.rabilloud@etu.univ-lyon1.fr>
 * @version 1.0
 */
#ifndef AUTOMATON_H
#define AUTOMATON_H

typedef struct {
    unsigned short zonesNumber;
    
    unsigned int *lampsOutput;
    
    int *lampsValues;
    
    unsigned int *valvesOutput;
    
    int *valvesValues;
    
    unsigned int waterVolumeInput;
    
    unsigned int waterLevelInput;
    
    unsigned int pumpOutput;
    
    int pumpValue;
} Automaton;

/**
 * Initializes an automaton by setting the inputs and outputs that are to be used.
 * The user needs to modify this function to property set them according to their
 * needs.
 * 
 * @param a The automaton to initialize.
 */
void automaton_init(Automaton *a, unsigned short zonesNumber);

/**
 * Frees any memory used by an automaton.
 * 
 * @param a The automaton to free.
 */
void automaton_free(Automaton *a);

/**
 * Creates a new automaton.
 * 
 * @return The newly created automaton.
 * @see automaton_init(Automaton*)
 */
Automaton* automaton_new(unsigned short zonesNumber);

/**
 * Destroys an automaton.
 * 
 * @param a The automaton to destroy.
 * @see automaton_free(Automaton*)
 */
void automaton_destroy(Automaton **a);

/**
 * Gets an automaton's number of zones.
 * 
 * @return The number of zones.
 */
unsigned short automaton_get_zones_number(const Automaton *a);

/**
 * 
 * @param a
 * @param zoneIndex
 */
unsigned int automaton_get_lamp_output(const Automaton *a, unsigned int zoneIndex);

/**
 * Turns a lamp on or off.
 * 
 * @param zoneIndex The zone's index.
 */
void automaton_set_lamp_output(Automaton *a, unsigned int zoneIndex, unsigned int pin);

/**
 * 
 * @param a
 * @param zoneIndex
 * @return 
 */
int automaton_get_lamp_value(const Automaton *a, unsigned int zoneIndex);

/**
 * 
 * @param a
 * @param zoneIndex
 * @param value
 */
void automaton_set_lamp_value(Automaton *a, unsigned int zoneIndex, int value);

/**
 * 
 * @param a
 * @param zoneIndex
 */
unsigned int automaton_get_valve_output(const Automaton *a, unsigned int zoneIndex);

/**
 * Sets a pin as a valve output.
 * 
 * @param zoneIndex The zone's index.
 */
void automaton_set_valve_output(Automaton *a, unsigned int zoneIndex, unsigned int pin);

/**
 * 
 * @param a
 * @param zoneIndex
 * @return 
 */
int automaton_get_valve_value(const Automaton *a, unsigned int zoneIndex);

/**
 * 
 * @param a
 * @param zoneIndex
 * @param value
 */
void automaton_set_valve_value(Automaton *a, unsigned int zoneIndex, int value);

/**
 * 
 * @param a
 * @return 
 */
unsigned int automaton_get_water_volume_input(const Automaton *a);

/**
 * 
 * @param a
 * @param pin
 */
void automaton_set_water_volume_input(Automaton *a, unsigned int pin);

/**
 * 
 * @param a
 * @return 
 */
int automaton_read_water_volume(const Automaton *a);

/**
 * 
 * @param a
 * @return 
 */
unsigned int automaton_get_water_level_input(const Automaton *a);

/**
 * 
 * @param a
 * @param pin
 */
void automaton_set_water_level_input(Automaton *a, unsigned int pin);

/**
 * 
 * @param a
 * @return 
 */
int automaton_read_water_level(const Automaton *a);

/**
 * 
 * @param a
 * @return 
 */
unsigned int automaton_get_pump_output(const Automaton *a);

/**
 * 
 * @param a
 * @param pin
 */
void automaton_set_pump_output(Automaton *a, unsigned int pin);

/**
 * 
 * @param a
 * @return 
 */
int automaton_get_pump_value(const Automaton *a);

/**
 * 
 * @param a
 * @param value
 */
void automaton_set_pump_value(Automaton *a, int value);

/**
 * 
 * @param a
 * @param path
 */
void automaton_dump_state(const Automaton *a, const char *path);

#endif
