/*
 * scanner.h
 *
 *  Created on: 08.09.2013
 *      Author: maik
 */

#ifndef CLI_SCANNER_H_
#define CLI_SCANNER_H_

#define CLI_TOKEN_SIZE     128

enum CLI_sc_state
{
    CLI_SC_STATE_START,
    CLI_SC_STATE_END,
    CLI_SC_STATE_NAME,
    CLI_SC_STATE_ARG
};


/* regular expressions for tokens:
 * 
 * name :=  (digit | letter | [_.-+~])+
 * arg :=   '(' ([^()])* ')'
 * in :=    '<'
 * out :=   '>'
 * run :=   '&'
 * 
 */

enum CLI_token_id
{
    CLI_TOKEN_START,
    CLI_TOKEN_ERR,
    CLI_TOKEN_NAME,
    CLI_TOKEN_ARG,
    CLI_TOKEN_IN,
    CLI_TOKEN_OUT,
    CLI_TOKEN_RUN,
    CLI_TOKEN_END
};

struct CLI_scanner
{
    char token[CLI_TOKEN_SIZE];
    enum CLI_token_id id;
    enum CLI_sc_state state;
};

typedef struct CLI_scanner CLI_scanner_t;

extern int CLI_scan(CLI_scanner_t *sc, char* buf);

#endif /* SCANNER_H_ */
