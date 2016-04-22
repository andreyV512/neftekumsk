#pragma once
#pragma message("ПОДКЛЮЧЕНИЕ ПЛАТ РАСШИРЕНИЯ!!!")
//undef DEBUG_ITEMS - ПОДключение платы: 1730
//define DEBUG_ITEMS - ОТключение платы: 1730
#undef DEBUG_ITEMS

//undef L502OFF - ПОДключение платы: 502
//define L502OFF - ОТключение платы: 502
#define L502OFF

//undef L791OFF - ПОДключение платы: 791
//define L791OFF - ОТключение платы: 791
#undef L791OFF

//define NO_EXPRESS - ОТключение
#undef NO_EXPRESS

#ifndef DEBUG_ITEMS
#pragma message("подключена плата 1730")
#endif

#ifndef L502OFF
#pragma message("подключена плата 502")
#endif

#ifndef L791OFF
#pragma message("подключена плата 791")
#endif


#define THREAD_PRIORITY

#define DISABLE_TCP

