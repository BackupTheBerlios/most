#ifndef CLI_HISTORY_H
#define CLI_HISTORY_H

#include <ace/stddef.h>
#include <uso/list.h>

/** @defgroup history history.h
 *
 * Command line interpreter history.
 * @{
 */

#define CLI_TOKEN_SIZE 16
#define CLI_TOKEN_COUNTER 4

struct CLI_token
{
	char buffer[CLI_TOKEN_COUNTER][CLI_TOKEN_SIZE];
	int argc;	
};

typedef struct CLI_token CLI_token_t;

struct CLI_hisentry
{
	USO_node_t node;
	CLI_token_t token;
};

typedef struct CLI_hisentry CLI_hisentry_t;

struct CLI_history
{
	USO_list_t list;
	CLI_hisentry_t* current;
	CLI_hisentry_t* last;
	int size;
	int count;
};

typedef struct CLI_history CLI_history_t;

/**
 * Initialize history.
 * 
 * @param history : Pointer to history.
 * @param size : size of history.
 */
extern void	CLI_history_init (CLI_history_t* history, int size);

/**
 * Add token to history.
 * 
 * @param history : Pointer to history.
 * @param token : Pointer to token.
 */
extern void CLI_history_add (CLI_history_t* history, CLI_token_t* token);

/**
 * Get next history token.
 * 
 * @param history : Pointer to history.
 * @param token : Pointer where to store the token.
 */
extern void CLI_history_next (CLI_history_t* history, CLI_token_t* token);

/**
 * Get previous history token.
 * 
 * @param history : Pointer to history.
 * @param token : Pointer where to store the token.
 */
extern void CLI_history_prev (CLI_history_t* history, CLI_token_t* token);

/**
 * Show history.
 */
extern void CLI_history_show (CLI_history_t* history);

/** @}
 */

#endif /* CLI_HISTORY_H */
