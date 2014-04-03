#ifndef __HASH_H__
#define __HASH_H__

unsigned int sdbm_hash(const char *);
unsigned int sax_hash(const char *);
unsigned int fnv_hash(const char *);

#endif
