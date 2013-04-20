#ifdef _WIN32
#ifndef WIN_THREAD_CPP
#define WIN_THREAD_CPP

#include "webserver/winthread.h"

int pthread_mutex_init(pthread_mutex_t *mutex, void *unused)
{
	(void) unused;
	*mutex = CreateMutex(NULL, FALSE, NULL);
	return *mutex == NULL ? -1 : 0;
}

int pthread_mutex_lock(pthread_mutex_t *mutex)
{
	return WaitForSingleObject(*mutex, INFINITE) == WAIT_OBJECT_0? 0 : -1;
}

int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
	return ReleaseMutex(*mutex) == 0 ? -1 : 0;
}

int pthread_cond_init(pthread_cond_t *cv, const void *unused)
{
	(void) unused;
	cv->signal = CreateEvent(NULL, FALSE, FALSE, NULL);
	cv->broadcast = CreateEvent(NULL, TRUE, FALSE, NULL);
	return cv->signal != NULL && cv->broadcast != NULL ? 0 : -1;
}

int pthread_cond_wait(pthread_cond_t *cv, pthread_mutex_t *mutex)
{
	HANDLE handles[] = {cv->signal, cv->broadcast};
	ReleaseMutex(*mutex);
	WaitForMultipleObjects(2, handles, FALSE, INFINITE);
	return WaitForSingleObject(*mutex, INFINITE) == WAIT_OBJECT_0? 0 : -1;
}

int pthread_cond_signal(pthread_cond_t *cv)
{
	return SetEvent(cv->signal) == 0 ? -1 : 0;
}

int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg)
{
	return (long)_beginthread((void (__cdecl *)(void *))start_routine, 0, arg) == -1L ? -1 : 0;
}

#endif
#endif