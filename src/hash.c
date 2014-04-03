
#include"hash.h"

unsigned int sax_hash(const char *key)
{
	unsigned int h=0;

	while(*key) h^=(h<<5)+(h>>2)+(unsigned char)*key++;

	return h;
}

unsigned int sdbm_hash(const char *key)
{
	unsigned int h=0;
	while(*key) h=(unsigned char)*key++ + (h<<6) + (h<<16) - h;
	return h;
}

unsigned int fnv_hash(const char *key)
{
	int p = 16777619;
	int h = (int)2166136261L;
	while(*key) h=(h ^ (unsigned char)*key++) * p;
	h += h<<13;
	h ^= h>>7;
	h += h<<3;
	h ^= h>>17;
	h += h<<5;
	return h;
}
