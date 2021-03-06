// FIXME: We mock this for now. However, this could be mapped e.g. to a <config> node on Genode.

#ifndef __GART_SYS__SYSTEM_PROPERTIES_H__
#define __GART_SYS__SYSTEM_PROPERTIES_H__

#include <stdio.h>
#include <sys/types.h>

#define PROP_VALUE_MAX 92

#if 0
#define LOG printf
#else
#define LOG(...)
#endif

typedef struct prop_info { } prop_info;

static uint32_t __system_property_serial(const prop_info* __pi) {
   LOG("%s() not implemented\n", __func__);
   return 0;
};

static const prop_info* __system_property_find(const char* __name) {
   LOG("%s(name=\"%s\") not implemented\n", __func__, __name);
   return NULL;
};

static int __system_property_read(const prop_info* __pi, char* __name, char* __value) {
   LOG("%s(name=\"%s\", value=\"%s\") not implemented\n", __func__, __name, __value);
   return -1;
};

static uint32_t __system_property_area_serial(void) {
   LOG("%s() not implemented\n", __func__);
   return -1;
};

static int __system_property_get(const char *__name, char *__value) {
   LOG("%s(name=\"%s\", value=\"%s\") not implemented\n", __func__, __name, __value);
   return -1;
};

static int __system_property_set(const char *__name, const char *__value) {
   LOG("%s(name=\"%s\", value=\"%s\") not implemented\n", __func__, __name, __value);
   return -1;
};

static void __system_property_read_callback
   (const prop_info* __pi,
    void (*__callback)(void* __cookie, const char* __name, const char* __value, uint32_t __serial),
    void* __cookie) {
   LOG("%s() not implemented\n", __func__);
}

static int __system_property_foreach(void (*__callback)(const prop_info* __pi, void* __cookie), void* __cookie) {
   LOG("%s() not implemented\n", __func__);
   return -1;
}

#endif // !__GART_SYS__SYSTEM_PROPERTIES_H__
