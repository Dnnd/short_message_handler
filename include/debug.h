#ifndef _DEBUG_H
#define _DEBUG_H

#ifdef ENABLE_DEBUG
#include <stdio.h>
#define DEBUGF printf
#else
#define DEBUGF(...)
#endif

#endif //_DEBUG_H
