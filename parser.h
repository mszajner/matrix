
/*********************************************************************************
 * parser.h                                                                      *
 *                                                                               *
 * Opis:                                                                         *
 *       Plik   nag³ówkowy  biblioteki   parser.c.   Zawiera opis  i  deklaracjê *
 *    wszystkich funkcji biblioteki oraz zmiennych z niej wyeksportowanych.      *
 *                                                                               *
 * Autor:   Miroslaw Szajner                                                     *
 * E-mail:  noozy@tlen.pl                                                        *
 * License: GPL                                                                  *
 * Country: Poland                                                               *
 * Year:    2004                                                                 *
 *                                                                               *
 *********************************************************************************/

#ifndef __PARSER_H
#define __PARSER_H

extern int parser_error;

int parser_init(void);

const char* parser_error_str(void);

int parser_run(char *buffer);

void parser_free(void);

#endif
