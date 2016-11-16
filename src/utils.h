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
 *
 */
bool is_file(const char *path);

/**
 *
 */
char* actions_path(const char *type, unsigned int i, const char *action);

/**
 *
 */
char* uint_to_string(unsigned int input);

#endif /* UTILS_H */

