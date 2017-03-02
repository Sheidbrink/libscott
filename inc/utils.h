#ifndef _UTILS_H_
#define _UTILS_H_
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif
size_t copy_to_buffer(void **buf, size_t buf_size, size_t buf_index, void *toAdd, size_t toCopy);

#ifdef __cplusplus
}
#endif

#endif
