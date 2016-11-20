/* 
 * @author Thomas Arnaud <thomas.arnaud@etu.univ-lyon1.fr>
 * @author Bruno Buiret <bruno.buiret@etu.univ-lyon1.fr>
 * @author Alexis Rabilloud <alexis.rabilloud@etu.univ-lyon1.fr>
 * @version 1.0
 */
#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

/**
 * Tests if a file exists or not.
 * 
 * @param path The file to test's path.
 * @return {@code true} if the exists, {@code false} otherwise.
 */
bool is_file(const char *path);

/**
 * Creates a conditionnal action file's path.
 * 
 * @param type The action's type.
 * @param i A number to append to the type.
 * @param option An action to append to the path.
 * @return The generated path.
 */
char* actions_path(const char *type, unsigned int i, const char *option);

/**
 * Transforms an unsigned int into a string.
 * 
 * @param input An unsigned int.
 * @return A string containing the given unsigned int.
 */
char* uint_to_string(unsigned int input);

/**
 * Prints a log if the option was given to the program.
 * 
 * @param format The text to write.
 * @param ... Arguments to put into the text to write.
 */
void print_log(const char *format, ...);

#endif
