/*
 * @ Reinhard Feiner
 */
#include <ace/string.h>
#include <ace/stdlib.h>
#include <ace/stdio.h>
#include <uso/list.h>

#include "cli/history.h"

static void entry_info
(
	USO_node_t*	entry
)
{
	for (int i = 0; i < ((CLI_hisentry_t*)entry)->token.argc; i++)
		ACE_printf ("%s ", ((CLI_hisentry_t*)entry)->token.buffer[i]);
	ACE_putc ('\n');
}

extern void CLI_history_init
(
	CLI_history_t*	history,
	int				size
)
{
	USO_list_init (&history->list);
	history->current = NULL;
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
		entry = (CLI_hisentry_t*)ACE_malloc (sizeof(CLI_hisentry_t));
		history->count++;
	}
	else
	{
		CLI_hisentry_t* tmp = history->last;
		history->last = (CLI_hisentry_t*)USO_prev_element (&history->list, (USO_node_t*)tmp);
		if (history->current == tmp)
			history->current = (CLI_hisentry_t*)USO_next_element (&history->list, NULL);
		USO_remove (&history->list, (USO_node_t*)tmp);
		entry = tmp;
	}
	
	if (NULL != entry)
	{
		if (NULL == history->last) history->last = entry;
		history->current = entry;
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
		
	history->current = (CLI_hisentry_t*)USO_next_element (&history->list, (USO_node_t*)history->current);
	if (NULL == history->current)
		history->current = (CLI_hisentry_t*)USO_next_element (&history->list, NULL);
	
	memcpy (token, &history->current->token, sizeof(CLI_token_t));
}

extern void CLI_history_prev
(
	CLI_history_t*	history,
	CLI_token_t*	token
)
{	
	if (0 == history->count)
		return;
	
	history->current = (CLI_hisentry_t*)USO_prev_element (&history->list, (USO_node_t*)history->current);
	if (NULL == history->current)
		history->current = (CLI_hisentry_t*)USO_prev_element (&history->list, NULL);
		
	memcpy (token, &history->current->token, sizeof(CLI_token_t));
}

extern void CLI_history_show
(
	CLI_history_t*	history
)
{
	USO_map (&history->list, entry_info);
}
