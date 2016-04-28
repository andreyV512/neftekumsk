#pragma once
#pragma message("ПОДКЛЮЧЕНИЕ ПЛАТ РАСШИРЕНИЯ!!!")
//undef DEBUG_ITEMS - ПОДключение платы: 1730
//define DEBUG_ITEMS - ОТключение платы: 1730
#undef DEBUG_ITEMS

#ifndef DEBUG_ITEMS
#pragma message("подключена плата 1730")
#endif

#define THREAD_PRIORITY

#define DISABLE_TCP

