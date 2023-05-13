#pragma once

#define USE_PTHREADS
#define USE_SPEEX
#define USE_KISSFFT
#define MALLOC_IS_ALIGNED

#ifdef _WIN32
#undef USE_PTHREADS
#undef MALLOC_IS_ALIGNED
#endif