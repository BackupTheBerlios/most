/*
 * @ Reinhard Feiner
 */
#include <ace/string.h>
#include <ace/stdlib.h>
#include <ace/stdio.h>
#include <uso/list.h>

#include "cli/history.h"

extern void CLI_history_init
(
	CLI_history_t*	history,
	int				size
)
{
	USO_list_init (&history->list);
	history->actual = NULL;
	history->last = NULL;
	history->size = size;
	history->count = 0;
}

extern void CLI_history_add
(
	CLI_history_t*	history,
	CLI_token_t*	token
)
{
	CLI_hisentry_t* entry = NULL;
	
	if (history->count < history->size)
	{
		entry = (CLI_hisentry_t*)malloc (sizeof(CLI_hisentry_t));
		history->count++;
	}
	else
	{
		CLI_hisentry_t* tmp = history->last;
		history->last = (CLI_hisentry_t*)USO_prev_element (&history->list, (USO_node_t*)tmp);
		if (history->actual == tmp)
			history->actual = (CLI_hisentry_t*)USO_next_element (&history->list, NULL);
		USO_remove (&history->list, (USO_node_t*)tmp);
		entry = tmp;
	}
	
	if (NULL != entry)
	{
		if (NULL == history->last) history->last = entry;
		history->actual = entry;
		memcpy (&entry->token, token, sizeof(CLI_token_t));
		USO_push (&history->list, (USO_node_t*)entry);
	}
}

extern void CLI_history_next
(
	CLI_history_t*	history,
	CLI_token_t*	token
)
{	
	if (0 == history->count)
		return;
		
	history->actual = (CLI_hisentry_t*)USO_next_element (&history->list, (USO_node_t*)history->actual);
	if (NULL == history->actual)
		history->actual = (CLI_hisentry_t*)USO_next_element (&history->list, NULL);
	
	memcpy (token, &history->actual->token, sizeof(CLI_token_t));
}

extern void CLI_history_prev
(
	CLI_history_t*	history,
	CLI_token_t*	token
)
{	
	if (0 == history->count)
		return;
		
	history->actual = (CLI_hisentry_t*)USO_prev_element (&history->list, (USO_node_t*)history->actual);
	if (NULL == history->actual)
		history->actual = (CLI_hisentry_t*)USO_prev_element (&history->list, NULL);
	
	memcpy (token, &history->actual->token, sizeof(CLI_token_t));
}

extern void CLI_history_show
(
	CLI_history_t*	history
)
{
	CLI_hisentry_t* entry = NULL;

	USO_LIST_FOR_EACH(&history->list, entry)
	{
		for (int i = 0; i < entry->token.argc; i++)
			printf ("%s ", entry->token.buffer[i]);
		putc ('\n');
	}
}
