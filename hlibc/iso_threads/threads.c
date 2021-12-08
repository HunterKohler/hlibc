/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */
#include <stdlib.h>
#include <stdatomic.h>
#include <errno.h>
#include "threads.h"

union thrd_routine_info {
    int ret;
    struct {
        thrd_start_t func;
        void *arg;
    };
};

int module_init_err;

pthread_mutexattr_t mtx_attr_plain;
pthread_mutexattr_t mtx_attr_timed;
pthread_mutexattr_t mtx_attr_plain_recursive;
pthread_mutexattr_t mtx_attr_timed_recursive;

thread_local union thrd_routine_info *routine_info;

static void module_init_routine()
{
    if ((module_init_err = pthread_mutexattr_init(&mtx_attr_plain)) ||
        (module_init_err = pthread_mutexattr_init(&mtx_attr_timed)) ||
        (module_init_err = pthread_mutexattr_init(&mtx_attr_plain_recursive)) ||
        (module_init_err = pthread_mutexattr_init(&mtx_attr_timed_recursive)) ||
        (module_init_err = pthread_mutexattr_settype(&mtx_attr_plain,
                                                     PTHREAD_MUTEX_NORMAL)) ||
        (module_init_err = pthread_mutexattr_settype(&mtx_attr_timed,
                                                     PTHREAD_MUTEX_NORMAL)) ||
        (module_init_err = pthread_mutexattr_settype(
             &mtx_attr_plain_recursive, PTHREAD_MUTEX_RECURSIVE)) ||
        (module_init_err = pthread_mutexattr_settype(
             &mtx_attr_timed_recursive, PTHREAD_MUTEX_RECURSIVE))) {
        pthread_mutexattr_destroy(&mtx_attr_plain);
        pthread_mutexattr_destroy(&mtx_attr_timed);
        pthread_mutexattr_destroy(&mtx_attr_plain_recursive);
        pthread_mutexattr_destroy(&mtx_attr_timed_recursive);
    }
}

static int module_init()
{
    static pthread_once_t once = PTHREAD_ONCE_INIT;
    pthread_once(&once, module_init_routine);
    return module_init_err;
}

/*
 * Get ISO thread error (or success) value from POSIX `errno` codes.
 */
static int thrd_return_code(int n)
{
    switch (n) {
    case 0:
        return thrd_success;
    case ENOMEM:
        return thrd_nomem;
    case EBUSY:
        return thrd_busy;
    default:
        return thrd_error;
    }
}

static void *thrd_routine(void *thrd_arg)
{
    routine_info = thrd_arg;
    routine_info->ret = routine_info->func(routine_info->arg);
    pthread_exit(routine_info);
}

int thrd_create(thrd_t *thr, thrd_start_t func, void *arg)
{
    union thrd_routine_info *info = malloc(sizeof(*info));
    if (!info)
        return thrd_nomem;

    info->func = func;
    info->arg = arg;

    int err = pthread_create(&thr->native_handle, NULL, thrd_routine, info);
    if (err)
        free(info);

    return thrd_return_code(err);
}

int thrd_equal(thrd_t lhs, thrd_t rhs)
{
    return pthread_equal(lhs.native_handle, rhs.native_handle);
}

thrd_t thrd_current(void)
{
    return (thrd_t){ pthread_self() };
}

// int thrd_sleep(const struct timespec *duration, struct timespec *remaining);

void thrd_yield(void)
{
    sched_yield();
}

_Noreturn void thrd_exit(int res)
{
    routine_info->ret = res;
    pthread_exit(routine_info);
}

int thrd_detach(thrd_t thr)
{
    return thrd_return_code(pthread_detach(thr.native_handle));
}

int thrd_join(thrd_t thr, int *res)
{
    union thrd_routine_info *info;
    int err = pthread_join(thr.native_handle, (void **)&info);

    *res = info->ret;
    free(info);

    return thrd_return_code(err);
}

int mtx_init(mtx_t *mutex, int type)
{
    int err;
    pthread_mutexattr_t *attr;

    if ((err = module_init()))
        return thrd_return_code(err);

    switch (type) {
    case mtx_plain:
        attr = &mtx_attr_plain;
        break;
    case mtx_timed:
        attr = &mtx_attr_timed;
        break;
    case mtx_plain | mtx_recursive:
        attr = &mtx_attr_plain_recursive;
        break;
    case mtx_timed | mtx_recursive:
        attr = &mtx_attr_timed_recursive;
        break;
    default:
        return thrd_error;
    }

    return thrd_return_code(pthread_mutex_init(&mutex->native_handle, attr));
}

int mtx_lock(mtx_t *mutex)
{
    return thrd_return_code(pthread_mutex_lock(&mutex->native_handle));
}

// int mtx_timedlock(mtx_t *restrict mutex,
//                   const struct timespec *restrict time_point);

int mtx_trylock(mtx_t *mutex)
{
    return thrd_return_code(pthread_mutex_trylock(&mutex->native_handle));
}

int mtx_unlock(mtx_t *mutex)
{
    return thrd_return_code(pthread_mutex_unlock(&mutex->native_handle));
}

void mtx_destroy(mtx_t *mutex)
{
    pthread_mutex_destroy(&mutex->native_handle);
}

void call_once(once_flag *flag, void (*func)(void))
{
    pthread_once(&flag->native_handle, func);
}

int cnd_init(cnd_t *cond)
{
    return thrd_return_code(pthread_cond_init(&cond->native_handle, NULL));
}

int cnd_signal(cnd_t *cond)
{
    return thrd_return_code(pthread_cond_signal(&cond->native_handle));
}

int cnd_broadcast(cnd_t *cond)
{
    return thrd_return_code(pthread_cond_broadcast(&cond->native_handle));
}

int cnd_wait(cnd_t *cond, mtx_t *mutex)
{
    return thrd_return_code(
        pthread_cond_wait(&cond->native_handle, &mutex->native_handle));
}

// int cnd_timedwait(cnd_t *restrict cond, mtx_t *restrict mutex,
//                   const struct timespec *restrict time_point);

void cnd_destroy(cnd_t *cond)
{
    pthread_cond_destroy(&cond->native_handle);
}

int tss_create(tss_t *tss_key, tss_dtor_t destructor)
{
    return thrd_return_code(
        pthread_key_create(&tss_key->native_handle, destructor));
}

void *tss_get(tss_t tss_key)
{
    return pthread_getspecific(tss_key.native_handle);
}

int tss_set(tss_t tss_id, void *val)
{
    return thrd_return_code(pthread_setspecific(tss_id.native_handle, val));
}

void tss_delete(tss_t tss_id)
{
    pthread_key_delete(tss_id.native_handle);
}
