/* 
 * @author Thomas Arnaud <thomas.arnaud@etu.univ-lyon1.fr>
 * @author Bruno Buiret <bruno.buiret@etu.univ-lyon1.fr>
 * @author Alexis Rabilloud <alexis.rabilloud@etu.univ-lyon1.fr>
 * @version 1.0
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <wiringPi.h>
#include "automaton.h"
#include "xmalloc.h"
#include "system.h"

void automaton_init(Automaton *a, unsigned short zonesNumber)
{
    // Initialize properties
    a->zonesNumber = zonesNumber;
    a->lampsOutput = (unsigned int*) xmalloc(sizeof(unsigned int) * a->zonesNumber);
    a->lampsValues = (int*) xmalloc(sizeof(int) * a->zonesNumber);
    a->valvesOutput = (unsigned int*) xmalloc(sizeof(unsigned int) * a->zonesNumber);
    a->valvesValues = (int*) xmalloc(sizeof(int) * a->zonesNumber);
    
    memset(a->lampsOutput, 0, sizeof(unsigned int) * a->zonesNumber);
    memset(a->lampsValues, 0, sizeof(unsigned int) * a->zonesNumber);
    memset(a->valvesOutput, 0, sizeof(unsigned int) * a->zonesNumber);
    memset(a->valvesValues, 0, sizeof(unsigned int) * a->zonesNumber);
    a->waterVolumeInput = 0;
    a->waterLevelInput = 0;
    a->pumpOutput = 0;
    a->pumpValue = 0;
}

void automaton_free(Automaton *a)
{
    // Free memory
    free(a->lampsOutput);
    free(a->lampsValues);
    free(a->valvesOutput);
    free(a->valvesValues);
    
    // Clear fields
    a->zonesNumber = 0;
    a->lampsOutput = NULL;
    a->valvesOutput = NULL;
    a->waterVolumeInput = 0;
    a->waterLevelInput = 0;
    a->pumpOutput = 0;
    a->pumpValue = 0;
}

Automaton* automaton_new(unsigned short zonesNumber)
{
    Automaton *a = (Automaton*) xmalloc(sizeof(Automaton));
    
    automaton_init(a, zonesNumber);
    
    return a;
}

void automaton_destroy(Automaton **a)
{
    automaton_free(*a);
    free(*a);
    *a = NULL;
}

unsigned short automaton_get_zones_number(const Automaton *a)
{
    return a->zonesNumber;
}

unsigned int automaton_get_lamp_output(const Automaton *a, unsigned int zoneIndex)
{
    assert(zoneIndex < a->zonesNumber);
    
    return a->lampsOutput[zoneIndex];
}

void automaton_set_lamp_output(Automaton *a, unsigned int zoneIndex, unsigned int pin)
{
    assert(zoneIndex < a->zonesNumber);
    
    a->lampsOutput[zoneIndex] = pin;
    pinMode(pin, OUTPUT);
}

int automaton_get_lamp_value(const Automaton *a, unsigned int zoneIndex)
{
    assert(zoneIndex < a->zonesNumber);
    
    return a->lampsValues[zoneIndex];
}

void automaton_set_lamp_value(Automaton *a, unsigned int zoneIndex, int value)
{
    assert(zoneIndex < a->zonesNumber);
    assert(a->lampsOutput[zoneIndex] > 0);
    assert(value == HIGH || value == LOW);
    
    digitalWrite(a->lampsOutput[zoneIndex], value);
    a->lampsValues[zoneIndex] = value;
}

unsigned int automaton_get_valve_output(const Automaton *a, unsigned int zoneIndex)
{
    assert(zoneIndex < a->zonesNumber);
    
    return a->valvesOutput[zoneIndex];
}

void automaton_set_valve_output(Automaton *a, unsigned int zoneIndex, unsigned int pin)
{
    assert(zoneIndex < a->zonesNumber);
    
    a->valvesOutput[zoneIndex] = pin;
    pinMode(pin, OUTPUT);
}

int automaton_get_valve_value(const Automaton *a, unsigned int zoneIndex)
{
    assert(zoneIndex < a->zonesNumber);
    
    return a->valvesValues[zoneIndex];
}

void automaton_set_valve_value(Automaton *a, unsigned int zoneIndex, int value)
{
    assert(zoneIndex < a->zonesNumber);
    assert(a->valvesOutput[zoneIndex] > 0);
    assert(value == HIGH || value == LOW);
    
    digitalWrite(a->valvesOutput[zoneIndex], value);
    a->valvesValues[zoneIndex] = value;
}

unsigned int automaton_get_water_volume_input(const Automaton *a)
{
    return a->waterVolumeInput;
}

void automaton_set_water_volume_input(Automaton *a, unsigned int pin)
{
    a->waterVolumeInput = pin;
    pinMode(pin, INPUT);
}

int automaton_read_water_volume(const Automaton *a)
{
    assert(a->waterVolumeInput > 0);
    
    return digitalRead(a->waterVolumeInput);
}

unsigned int automaton_get_water_level_input(const Automaton *a)
{
    return a->waterLevelInput;
}

void automaton_set_water_level_input(Automaton *a, unsigned int pin)
{
    a->waterLevelInput = pin;
    pinMode(pin, INPUT);
}

int automaton_read_water_level(const Automaton *a)
{
    assert(a->waterLevelInput > 0);
    
    return digitalRead(a->waterLevelInput);
}

unsigned int automaton_get_pump_output(const Automaton *a)
{
    return a->pumpOutput;
}

void automaton_set_pump_output(Automaton *a, unsigned int pin)
{
    a->pumpOutput = pin;
    pinMode(pin, OUTPUT);
}

int automaton_get_pump_value(const Automaton *a)
{
    return a->pumpValue;
}

void automaton_set_pump_value(Automaton *a, int value)
{
    assert(a->pumpOutput > 0);
    assert(value == HIGH || value == LOW);
    
    digitalWrite(a->pumpOutput, value);
    a->pumpValue = value;
}

void automaton_dump_state(const Automaton *a, const char *path)
{
    // Try opening the file
    FILE *file = fopen(path, "w");
    unsigned short i;
    
    if(file == NULL)
    {
        fprintf(stderr, "Can't open file \"%s\".", path);
        exit(EXIT_FAILURE);
    }
    
    // Dump the automaton's state
    fprintf(file, "[settings]\n");
    fprintf(file, "zones_number = %u\n", a->zonesNumber);
    
    fprintf(file, "[lighting]\n");
    
    for(i = 0; i < a->zonesNumber; i++)
    {
        fprintf(file, "lamp_%u = %s\n", i + 1, automaton_get_lamp_value(a, i) == HIGH ? "true" : "false");
    }
    
    fprintf(file, "[valves]\n");
    
    for(i = 0; i < a->zonesNumber; i++)
    {
        fprintf(file, "valve_%u = %s\n", i + 1, automaton_get_valve_value(a, i) == HIGH ? "true" : "false");
    }
    
    fprintf(file, "[sensors]\n");
    fprintf(file, "not_enough_water = %s\n", automaton_read_water_level(a) == HIGH ? "true" : "false");
    fprintf(file, "flow = %s\n", "?");
    
    fprintf(file, "[pump]\n");
    fprintf(file, "pump = %s\n", automaton_get_pump_value(a) == HIGH ? "true" : "false");
    
    // Close file
    fclose(file);
}
