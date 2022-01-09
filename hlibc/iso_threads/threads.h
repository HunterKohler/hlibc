/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */

#ifndef ISO_THREADS_THREADS_H_
#define ISO_THREADS_THREADS_H_

#include <time.h>
#include <pthread.h>
#include "threads.h"

#define thread_local _Thread_local

#define ONCE_FLAG_INIT ({ (pthread_once_t)PTHREAD_ONCE_INIT })
#define TSS_DTOR_ITERATIONS PTHREAD_DESTRUCTOR_ITERATIONS

typedef int (*thrd_start_t)(void *);
typedef void (*tss_dtor_t)(void *);

typedef struct {
    pthread_t native_handle;
} thrd_t;

typedef struct {
    pthread_mutex_t native_handle;
} mtx_t;

typedef struct {
    pthread_cond_t native_handle;
} cnd_t;

typedef struct {
    pthread_key_t native_handle;
} tss_t;

typedef struct {
    pthread_once_t native_handle;
} once_flag;

enum {
    thrd_success,
    thrd_nomem,
    thrd_timedout,
    thrd_busy,
    thrd_error,
};

enum {
    mtx_plain = 1 << 0,
    mtx_recursive = 1 << 1,
    mtx_timed = 1 << 2,
};

int thrd_create(thrd_t *thr, thrd_start_t func, void *arg);
int thrd_equal(thrd_t lhs, thrd_t rhs);
thrd_t thrd_current(void);
int thrd_sleep(const struct timespec *duration, struct timespec *remaining);
void thrd_yield(void);
_Noreturn void thrd_exit(int res);
int thrd_detach(thrd_t thr);
int thrd_join(thrd_t thr, int *res);

int mtx_init(mtx_t *mutex, int type);
int mtx_lock(mtx_t *mutex);
int mtx_timedlock(mtx_t *restrict mutex,
                  const struct timespec *restrict time_point);
int mtx_trylock(mtx_t *mutex);
int mtx_unlock(mtx_t *mutex);
void mtx_destroy(mtx_t *mutex);

void call_once(once_flag *flag, void (*func)(void));

int cnd_init(cnd_t *cond);
int cnd_signal(cnd_t *cond);
int cnd_broadcast(cnd_t *cond);
int cnd_wait(cnd_t *cond, mtx_t *mutex);
int cnd_timedwait(cnd_t *restrict cond, mtx_t *restrict mutex,
                  const struct timespec *restrict time_point);
void cnd_destroy(cnd_t *cond);

int tss_create(tss_t *tss_key, tss_dtor_t destructor);
void *tss_get(tss_t tss_key);
int tss_set(tss_t tss_id, void *val);
void tss_delete(tss_t tss_id);

#endif
