#include "utils.h"
size_t copy_to_buffer(void **buf, size_t buf_size, size_t buf_index, void *toAdd, size_t toCopy) {
	size_t required_size = toCopy + buf_index;
	if(*buf == NULL) {
		*buf = malloc(required_size);
	}
	else if(buf_size < required_size) {
		*buf = realloc(*buf, required_size);
	}
	memcpy(*buf + buf_index, toAdd, toCopy);
	return required_size;
}
