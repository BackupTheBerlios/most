/*
 * scanner.c
 *
 *  Created on: 08.09.2013
 *      Author: maik
 */

#include <ace/stdio.h>
#include <ace/stdlib.h>
#include <ace/ctype.h>
#include <cli/scanner.h>


extern int
CLI_scan(CLI_scanner_t *sc, char* buf)
{
    int i;
    sc->state = CLI_SC_STATE_START;
    sc->id = CLI_TOKEN_ERR;
    char *token = sc->token;
    for (i = 0; sc->state != CLI_SC_STATE_END && i < CLI_TOKEN_SIZE - 1; ++i){
        char c = *buf++;
        switch (sc->state){

            case CLI_SC_STATE_START:
                switch (c){
                    case '\0':
                        sc->id = CLI_TOKEN_END;
                        sc->state = CLI_SC_STATE_END;
                        break;
                    case '<':
                        *token++ = c;
                        sc->id = CLI_TOKEN_IN;
                        sc->state = CLI_SC_STATE_END;
                        break;
                    case '>':
                        *token++ = c;
                        sc->id = CLI_TOKEN_OUT;
                        sc->state = CLI_SC_STATE_END;
                        break;
                    case '&':
                        *token++ = c;
                        sc->id = CLI_TOKEN_RUN;
                        sc->state = CLI_SC_STATE_END;
                        break;
                    case '(':
                        sc->state = CLI_SC_STATE_ARG;
                        break;
                    default:
                        if (ACE_isspace(c)) {
                            ;
                        } else if (ACE_isalnum(c) || c == '_' || c == '.' || c == '~' || c == '-'){
                            *token++ = c;
                            sc->state = CLI_SC_STATE_NAME;
                        }else{
                            --i;
                            sc->id = CLI_TOKEN_ERR;
                            sc->state = CLI_SC_STATE_END;
                        }
                        break;
                }
                break;

            case CLI_SC_STATE_NAME:
                if (ACE_isalnum(c) || c == '_' || c == '.' || c == '~' || c == '-'){
                    *token++ = c;
                } else {
                    --i;
                    sc->id = CLI_TOKEN_NAME;
                    sc->state = CLI_SC_STATE_END;
                }
                break;

            case CLI_SC_STATE_ARG:
                if (c == ')'){
                    sc->id = CLI_TOKEN_ARG;
                    sc->state = CLI_SC_STATE_END;
                } else if (c == '('){
                    --i;
                    sc->id = CLI_TOKEN_ERR;
                    sc->state = CLI_SC_STATE_END;
                } else if (ACE_isprint(c)){
                    *token++ = c;
                } else {
                    --i;
                    sc->id = CLI_TOKEN_ERR;
                    sc->state = CLI_SC_STATE_END;
                }
                break;

            case CLI_SC_STATE_END:
                /* no break */
            default:
                ACE_ABORT("CLI: invalid scanner state!");
                break;
        }

    }

    *token = '\0';
    
    return i;
}
