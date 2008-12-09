/*
 *  Copyright (C) 2007  Michael Feiner
 *
 */

#include <ace/stdlib.h>
#include <ace/string.h>
#include <ace/stddef.h>

#include "uso/pipe.h"
#include "uso/arch/cpu.h"

/*------------- Implementation ------------------------------------------*/

extern void
USO_pipe_init (USO_pipe_t * pipe, char *buf, ACE_size_t size)
{
	pipe->state = USO_PIPE_EMPTY;
    pipe->start = buf;
    pipe->end = buf + size;
    pipe->read = pipe->start;
    pipe->write = pipe->start;
}

extern USO_pipe_t *
USO_pipe_new (ACE_size_t size)
{
    USO_pipe_t *pipe = ACE_malloc (sizeof (USO_pipe_t));
    if (pipe) {
        char *buf = ACE_malloc (size);
        if (buf) {
            USO_pipe_init (pipe, buf, size);
        } else {
            ACE_free (pipe);
            pipe = NULL;                
        }
    }
    return pipe;
}

extern void
USO_pipe_del (USO_pipe_t * pipe)
{
    ACE_free (pipe->start);
    ACE_free (pipe);
}


#define RW_STATE(rw) ((rw) ? USO_PIPE_EMPTY : USO_PIPE_FULL) 
#define RW_COPY(rw, a, buf, len) ((rw) ? memcpy(buf, a, len) : memcpy(a, buf, len))

ACE_INLINE_ static ACE_size_t
pipe_help (enum USO_pipe_state *state, ACE_bool_t rw,
		   char **a, char *b, char *s, char *e, char *buf, ACE_size_t len)
{
    ACE_size_t right = 0;
	ACE_size_t left = 0;
	
    if (*a >= b) {
		if (*state == RW_STATE(rw)) { return 0; }
    	right = e - *a;
    	right = ACE_MIN(right, len);
    	RW_COPY(rw, *a, buf, right);
    	*a += right;
	    if (*a >= e){
	    	*a = s; 
		    if (*a == b){
		    	*state = RW_STATE(rw);
				return right;
		    }
	    }
	    len -= right;
    	if (len == 0) {
	    	*state = USO_PIPE_DATA;
    		return right;
    	}
	    buf += right;
    }
    
    if (*a < b) {
		left = b - *a;
    	left = ACE_MIN(left, len);
    	RW_COPY(rw, *a, buf, left);
    	*a += left;
	    if (*a == b){
	    	*state = RW_STATE(rw);
	    } else {
	    	*state = USO_PIPE_DATA;
	    }
    }

	return right + left;
}

extern ACE_size_t
USO_pipe_read_ns (USO_pipe_t * pipe, char *buf, ACE_size_t len)
{
	return pipe_help (&pipe->state, TRUE, &pipe->read, pipe->write,
			   		  pipe->start, pipe->end, buf, len);
}

extern ACE_size_t
USO_pipe_write_ns (USO_pipe_t * pipe, const char *buf, ACE_size_t len)
{
	return pipe_help (&pipe->state, FALSE, &pipe->write, pipe->read,
			   		  pipe->start, pipe->end, (char*)buf, len);
}
