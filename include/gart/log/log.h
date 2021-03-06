#ifndef __GART_LOG_LOG_H__
#define __GART_LOG_LOG_H__

#include_next <log/log.h>

#ifdef __cplusplus
extern "C" {
#endif

// This prototype gets defined only when __linux__ is defined
clockid_t android_log_clockid(void);

#ifdef __cplusplus
}
#endif

#endif // !__GART_LOG_LOG_H__
