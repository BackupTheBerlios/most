/*
 * parser.c
 *
 *  Created on: 08.09.2013
 *      Author: maik
 */


#include <cli/parser.h>
#include <ace/string.h>

static enum CLI_token_id 
nexttoken(CLI_parser_t *p)
{
    p->buf += CLI_scan(&p->sc, p->buf);
    return p->sc.id; 
}

static ACE_err_t
match(CLI_parser_t *p, enum CLI_token_id t)
{
    if (p->lookahead == t){
        p->lookahead = nexttoken(p);
    } else {
        return CLI_ERR_TOKEN;
    }
    return ACE_OK;
}

static void
create_symbol (char *symbol, const char *token, int n)
{
    --n;
    ACE_strncpy(symbol, token, n);
    symbol[n] = '\0';
}

extern ACE_err_t
CLI_parse(CLI_parser_t *p, char *buf)
{
    p->buf = buf;
    p->lookahead = CLI_TOKEN_START;
    
    match(p, CLI_TOKEN_START);

    if (p->lookahead == CLI_TOKEN_END) {
        return CLI_ERR_LINEEMPTY;
    }
    
    if (p->lookahead == CLI_TOKEN_NAME) {
        create_symbol(p->cmd, p->sc.token, sizeof (p->cmd));
        match(p, CLI_TOKEN_NAME);
    } else {
        return CLI_ERR_SYNTAX;
    }
    
    if (p->lookahead == CLI_TOKEN_IN){
        create_symbol(p->in, p->cmd, sizeof (p->in));
        match(p, CLI_TOKEN_IN);
        create_symbol(p->cmd, p->sc.token, sizeof (p->cmd));
        if (match(p, CLI_TOKEN_NAME) != ACE_OK){
            return CLI_ERR_SYNTAX;
        }
    } else {
        create_symbol(p->in, NULL, 1);
    }
    
    if (p->lookahead == CLI_TOKEN_ARG){
        create_symbol(p->arg, p->sc.token, sizeof (p->arg));
        match(p, CLI_TOKEN_ARG);
    } else {
        create_symbol(p->arg, NULL, 1);
    }
    
    if (p->lookahead == CLI_TOKEN_OUT){
        match(p, CLI_TOKEN_OUT);
        create_symbol(p->out, p->sc.token, sizeof (p->out));
        if (match(p, CLI_TOKEN_NAME) != ACE_OK){
            return CLI_ERR_SYNTAX;
        }
    } else {
        create_symbol(p->out, NULL, 1);
    }
    
    if (p->lookahead == CLI_TOKEN_RUN){
        p->run = TRUE;
        match(p, CLI_TOKEN_RUN); 
    } else {
        p->run = FALSE;
    }
    
    if (p->lookahead != CLI_TOKEN_END){
        return CLI_ERR_SYNTAX;
    }

    return ACE_OK;
}


