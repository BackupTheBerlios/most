/*
 * parser.h
 *
 *  Created on: 08.09.2013
 *      Author: maik
 */

#ifndef CLI_PARSER_H_
#define CLI_PARSER_H_

#include <ace/stdio.h>
#include <cli/scanner.h>
#include <cli/err.h>
#include <mfs/descriptor.h>


/* Production rules
 *
 * CMDLINE := INopt name (arg)opt OUTopt &opt
 *
 * IN := name <
 * OUT := > name
 * 
 * 
 * name bezeichnet einen Descriptor Namen
 * arg bezeichnet einen String der alle Argumente beinhaltet
 */

struct CLI_parser
{
    CLI_scanner_t sc;
    enum CLI_token_id lookahead;
    char *buf;
    char in[MFS_NAME_SIZE];
    char cmd[MFS_NAME_SIZE]; 
    char arg[CLI_TOKEN_SIZE];
    char out[MFS_NAME_SIZE];
    ACE_bool_t run;
};


typedef struct CLI_parser CLI_parser_t;

extern ACE_err_t CLI_parse(CLI_parser_t *p, char *buf);

#endif /* PARSER_H_ */
