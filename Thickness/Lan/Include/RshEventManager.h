/*!
 * \copyright JSC "Rudnev-Shilyaev"
 *
 * \file RshEventManager.h
 * \date 27.06.2014
 * \version 1.0 [SDK 2.00]
 *
 * \~english
 * \brief
 * Cross platform realisation of event manager class.
 *
 * \~russian
 * \brief
 * Кроссплатформенная реализация класса для синхронизации потоков и событий.
 *
 */

#ifndef __RSH_EVENT_MANAGER_H__
#define	__RSH_EVENT_MANAGER_H__

#include "RshTypes.h"

//#define __BOOST_EVENT_MODE__ // для Windows есть возможность работать через буст
//#define __BOOST_CRITSECTION_MODE__

#if defined(RSH_MSWINDOWS)
    #include "Windows.h"
#elif defined(RSH_LINUX)
    #include "pevents.h"
    #include "pthread.h"
    #define __PEVENT_MODE__      // POSIX реализация работы с эвентами для LINUX
#else   
    #define __BOOST_EVENT_MODE__ // для Windows есть возможность работать через буст
    #define __BOOST_CRITSECTION_MODE__
#endif


#if defined(__BOOST_EVENT_MODE__) || defined(__BOOST_CRITSECTION_MODE__)
	#include "boost/thread/thread.hpp"
	#include "boost/thread/mutex.hpp"
	#include "boost/thread/condition.hpp"
    #include "boost/version.hpp"
	#include <boost/lambda/lambda.hpp>

enum RSHEventStates // допустимые состояния событий
{
    RSH_EVENT_STATE_NONSIGNALED,
    RSH_EVENT_STATE_SIGNALED
};

#endif

/*!
 *
 * \~english
 * \brief
 * Event object.
 *
 * Platform independent event object. Event can be in two state:
 * signaled or non signaled. Usually one thread waits some event to occur,
 * while second thread (data processing, for example) change state of event
 * object.\n
 * Usually objects of this class are not used directly, but from RshEventManager class.
 *
 * \see
 * RshEventManager
 *
 * \~russian
 * \brief
 * Событийный объект
 *
 * Кроссплатформенная реализация событийного объекта. Объект может принимать два
 * состояния: сигнальное или несигнальное. Обычно один или несколько потоков находятся
 * в режиме ожидания, в то время как другой поток (поток обработки данных, например) меняет
 * состояние объекта. Как только состояние объекта изменяется, все ожидающие потоки будут
 * уведомлены об этом.\n
 * Объекты данного класса, как правило, используются не напрямую, а через вызовы класса RshEventManager.
 *
 * \see
 * RshEventManager
 *
 */
struct RshEventObject
{

#if defined(__BOOST_EVENT_MODE__)
    volatile RSHEventStates eventState; // состояние, которое принимает событие
    boost::xtime time; // структура используется для задания времени
    U32 timeToWait; // время ожидания наступления события при выполнении функции
                    // WaitForEvent до наступления таймаута в миллисекундах
                    // конвертируется в поле time
    bool autoReset; // автоматический сбор события в начальное состояние после срабатывания функции WaitForEvent

    boost::condition condition; // условие сигнализации события
    boost::mutex mutex;

    volatile bool waitState; // захвачен ли mutex в WaitForEvent
#else
    
    #if defined(RSH_MSWINDOWS)
        HANDLE hEvent;
        BOOL  autoReset;
        BOOL  timeToWait;
    #elif defined(RSH_LINUX)
        neosmart::neosmart_event_t hEvent;
        U32 timeToWait;
        bool autoReset;
    #else
        #error undefinedMode
    #endif

#endif
	/*!
	 *
	 * \~english
	 * \brief
	 * Set event object state to signaled.
	 *
	 * Signal event. All waiting threads will be informed on
	 * signal state change.
	 *
	 * \~russian
	 * \brief
	 * Установить состояние объекта "сигнальное"
	 *
	 * Вызов данного метода устанавливает объект в сигнальное состояние -
	 * все потоки, ожидающие события, будут разблокированы.
	 *
	 */
    void SetSignaled();
	/*!
	 *
	 * \~english
	 * \brief
	 * Set event object state to nonsignaled.
	 *
	 * Set event object to nonsignaled state.
	 *
	 * \~russian
	 * \brief
	 * Установить состояние объекта "несигнальное"
	 *
	 * Вызов данного метода устанавливает объект в несигнальное состояние.
	 *
	 */
    void SetNonSignaled();

    /*!
	 *
	 * \~english
	 * \brief
	 * Constructor
	 *
	 * \param[in] timeToWait
	 * Maximum wait time in milliseconds. ::RSH_INFINITE_WAIT_TIME constant
	 * is used as default value.
	 *
	 * \param[in] autoReset
	 * If true, event object will be set in nonsignaled
	 * state automatically, after the first waitng thread was
	 * successfully received that object was signaled.\n
	 * Default value is set to false.
	 *
	 * Constructor with two parameters.
	 *
	 * \~russian
	 * \brief
	 * Конструктор
	 *
	 * \param[in] timeToWait
	 * Максимальное время ожидания в миллисекундах. В качестве
	 * дефолтного значения используется константа ::RSH_INFINITE_WAIT_TIME.
	 *
	 * \param[in] autoReset
	 * Автоматический сброс события. Если включено, и какой-либо поток ожидает
	 * прихода события, сразу же после его получения состояние объекта будет
	 * сброшено в несигнальное.\n
	 * По умолчанию данный флаг выключен.
	 *
	 * Конструктор с двумя параметрами.
	 *
	 */
    RshEventObject(U32 timeToWait = RSH_INFINITE_WAIT_TIME, bool autoReset = false);

	/*!
	 *
	 * \~english
	 * \brief
	 * Wait until event is signaled
	 *
	 * \returns
	 * ::RSH_API_SUCCESS or error code.
	 *
	 * This method blocks exectuion of current thread
	 * until event object is set to signaled state or time
	 * set as timeout value is expired.
	 *
	 * \~russian
	 * \brief
	 * Ожидание перехода эвента в сигнальное состояние
	 *
	 * \returns
	 * ::RSH_API_SUCCESS или код ошибки.
	 *
	 * Вызов данного метода блокирует выполнение текущего потока
	 * до тех пор, пока объект не будет просигнален, или не
	 * истечет время, заданное в качестве таймаута.
	 *
	 */

    U32 WaitForEvent();
    void WaitForMutex();
};


/*!
 *
 * \~english
 * \brief
 * Critical section object
 *
 * Platform independent critical section (mutex) objects.
 * This object is used to thead synchronization - one thread can
 * lock object, and all other threads that need this object too will
 * be blocked until first thread unlock object. Usually this type oeventf
 * object is used to restrict access two resource that cannot be accessed
 * simultaneously from two or more threads.\n
 * This object is usually not used directly, but from RshEventManager class.
 *
 * \see
 * RshEventManager
 *
 * \~russian
 * \brief
 * Объект "Критическая секция"
 *
 * Кроссплатформенная реализация критической секции (мьютекса).
 * Данный объект используется в тех случаях, когда нужно ограничить
 * одновременный доступ к одному ресурсу из нескольких потоков. Конкурирующие
 * потоки пытаются получить доступ к ресурсу, и поток, который сделает это
 * первым, блокирует доступ к ресурсу для остальных потоков - они будут ждать
 * до тех пор, пока объект не перейдет в незаблокированное состояние.
 *
 * \see
 * RshEventManager
 *
 */
struct RshCriticalSectionObject
{
#if defined(__BOOST_CRITSECTION_MODE__)
    boost::mutex::scoped_try_lock critSection;
#else
    #if defined(RSH_MSWINDOWS)
        CRITICAL_SECTION critSection;
    #elif defined(RSH_LINUX)
        pthread_mutex_t critSection;
    #else
        #error undefinedMode
    #endif
#endif    
    

	RshCriticalSectionObject();
	~RshCriticalSectionObject();

	U32 EnterCriticalSection();
	U32 TryEnterCriticalSection();
	U32 LeaveCriticalSection();

};

/*!
 *
 * \~english
 * \brief
 * Thread synchronization manager
 *
 * Platform independent class used to manage events, mutexes and
 * thread synchronization.
  *
 * \~russian
 * \brief
 * Управление событиями и синхронизацией потоков
 *
 * Кроссплатформенный класс для управления событиями, мьютексами
 * и синхронизацией потоков.
 *
 */
class RshEventManager
{
private:
    RshEventObject** m_userEvents;
	RshCriticalSectionObject** m_userCriticalSections;

	U32 m_objSize;
	U32 m_csSize;
	U32 m_objCount;
	U32 m_csCount;

public:

	/*!
	 *
	 * \~english
	 * \brief
	 * Create event manager instance.
	 *
	 * \param[in] objSize
	 * Maximum number of RshEventObject objects in internal storage.
	 * Actual objects are created using  InitializeEvent() method.
	 * At construction time object storage is empty.
	 *
	 * \param[in] csSize
	 * Maximum number of RshCriticalSectionObject objects in internal storage.
	 * Actual objects are created using  InitializeCriticalSection() method.
	 * At construction time object storage is empty.
	 *
	 * \~russian
	 * \brief
	 * Создание экземпляра класса.
	 *
	 * \param[in] objSize
	 * Максимальное количество объектов RshEventObject во внутреннем хранилище.
	 * Сами объекты создаютя и добавляются в хранилище с помощью метода InitializeEvent().
	 * На момент создания класса хранилище не содержит ни одного объекта.
	 *
	 * \param[in] csSize
	 * Максимальное количество объектов RshCriticalSectionObject во внутреннем хранилище.
	 * Сами объекты создаютя и добавляются в хранилище с помощью метода InitializeCriticalSection().
	 * На момент создания класса хранилище не содержит ни одного объекта.
	 *
	 */

    RshEventManager(U32 objSize, U32 csSize=0);
    ~RshEventManager();
    void DeInitializeResources();

	/*!
	 *
	 * \~english
	 * \brief
	 * Create and initialize event object
	 *
	 * \param[in] eventCode
	 * Event object code reference. This code is used to access event object methods.
	 * Each time when this method is called new RshEventObject is created.
	 * Id of new created object is stored in eventCode variable passed by reference. 
	 * If internal storage limit is exceeded, error code will be returned and passed
	 * variable will stay unchaned.
	 *
	 * \param[in] autoReset
	 * Set RshEventObject auto reset property. See RshEventObject::RshEventObject() for details.
	 *
	 * \returns
	 * ::RSH_API_SUCCESS or error code.
	 *
	 * This method is used to create and initialize event object. Access to event object and
	 * critical section objects is performed using unique ID. 
	 *
	 * \~russian
	 * \brief
	 * Создание и инициализация событийного объекта
	 *
	 * \param[in] eventCode
	 * Переменная, в которую будет помещен уникальный идентификатор объекта.
	 * Данный идентификатор используется для доступа к объекту.
	 * При вызове данного метода создается новый объект, и в данной переменной, переданной по ссылке,
	 * сохраняется его идентификационный номер, который в дальнейшем должен использоваться для доступа
	 * к объекту.
	 *
	 * \param[in] autoReset
	 * Установка свойства автоматического сброса для объекта RshEventObject.
	 * См. RshEventObject::RshEventObject().
	 *
	 * \warning
	 * В случае, если не удалось создать объект, значение переменной eventCode не изменится!
	 * Проверяейте возвращаемое значение, чтобы быть уверенным в том, что объект успешно создан.
	 *
	 * \returns
	 * ::RSH_API_SUCCESS или код ошибки.
	 *
	 * Данный метод используется для создания и иницализации событийного объекта.
	 * Доступ к таким объектам (а также к объектам типа "критическая секция")
	 * осуществляется с использованием уникального идентификатора. 
	 *
	 */
    U32 InitializeEvent(U32 &eventCode, bool autoReset = false);

	/*!
	 *
	 * \~english
	 * \brief
	 * Set event object to signaled state
	 *
	 * \param[in] eventCode
	 * ID of event object that need to be signaled
	 *
	 * \returns
	 * ::RSH_API_SUCCESS or error code.
	 *
	 * This method is used to signal event object referenced
	 * via \b eventCode ID.
	 *
	 * \~russian
	 * \brief
	 * Установка событийного объекта в сигнальное состояние
	 *
	 * \param[in] eventCode
	 * Идентификационный номер объекта, который нужно перевести
	 * в сигнальное состояние.
	 *
	 * \returns
	 * ::RSH_API_SUCCESS или код ошибки.
	 *
	 * Данный метод используется для перевода событийного объекта
	 * с идентификатором \b eventCode в сигнальное состояние.
	 *
	 */
    U32 SetEvent(U32 eventCode);

	/*!
	 *
	 * \~english
	 * \brief
	 * Set event object to nonsignaled state
	 *
	 * \param[in] eventCode
	 * ID of event object that need to be reset
	 *
	 * \returns
	 * ::RSH_API_SUCCESS or error code.
	 *
	 * This method is used to set event object referenced
	 * via \b eventCode ID to nonsignaled state.
	 *
	 * \~russian
	 * \brief
	 * Установка событийного объекта в несигнальное состояние
	 *
	 * \param[in] eventCode
	 * Идентификационный номер объекта, который нужно перевести
	 * в несигнальное состояние.
	 *
	 * \returns
	 * ::RSH_API_SUCCESS или код ошибки.
	 *
	 * Данный метод используется для перевода событийного объекта
	 * с идентификатором \b eventCode в несигнальное состояние.
	 *
	 */
    U32 ResetEvent(U32 eventCode);

	/*!
	 *
	 * \~english
	 * \brief
	 * Wait event to occure
	 *
	 * \param[in] eventCode
	 * ID of event object to wait.
	 *
	 * \param[in] timeToWait
	 * Maximum wait time (int milliseconds), or ::RSH_INFINITE_WAIT_TIME
	 * to wait infinitely.
	 *
	 * \returns
	 * ::RSH_API_SUCCESS or error code.
	 *
	 * Calling this method will block current thread until
	 * event specified via \b eventCode ID will be set to signaled state or
	 * timeout specified in \b timeToWait parameter is expired.
	 *
	 * \remarks
	 * When ::RSH_INFINITE_WAIT_TIME is used, setting event to signaled state is the only
	 * way to unblock waiting thred exectution.
	 *
	 * \~russian
	 * \brief
	 * Ожидание события
	 *
	 * \param[in] eventCode
	 * Идентификатор события
	 *
	 * \param[in] timeToWait
	 * Максимальное время ожидания в миллисекундах, или ::RSH_INFINITE_WAIT_TIME
	 * для "бесконечного" ожидания.
	 *
	 * \returns
	 * ::RSH_API_SUCCESS или код ошибки.
	 *
	 * Вызов данного метода блокирует текущий поток до тех пор, пока событие
	 * с идентификатором \b eventCode не перейдет в сигнальное состояние, или
	 * пока не истечет время ожидания, заданное в качестве параметра \b timeToWait.
	 *
	 * \remarks
	 * Если в качестве времени ожидания используется ::RSH_INFINITE_WAIT_TIME,
	 * единственный способ разблокировать ожидающий поток - перевод эвента
	 * в сигнальное состояние.
	 *
	 */
    U32 WaitForEvent(U32 eventCode, U32 timeToWait = RSH_INFINITE_WAIT_TIME);

	/*!
	 *
	 * \~english
	 * \brief
	 * Create and initialize critical section object
	 *
	 * \param[in] sectionCode
	 * Critical section object code. This code is used to access critical section methods.
	 * This method will create new critical section object, its ID will be stored in 
	 * sectionCode variable passed by reference.
	 *
	 * \returns
	 * ::RSH_API_SUCCESS or error code.
	 *
	 * This method is used to create and initialize critical section. Access to
	 * critical section objects is performed using unique ID.
	 *
	 * \~russian
	 * \brief
	 * Создание и инициализация критической секции
	 *
	 * \param[in] sectionCode
	 * Переменная, в которую будет помещен уникальный идентификатор объекта.
	 * Данный идентификатор используется для доступа к объекту.
	 * Вызов этого метода создает новый объект критической секции и присваивает ему
	 * уникальный идентификатор, который возвращается в переменной sectionCode,
	 * переданной по ссылке.
	 *
	 * \warning
	 * В случае, если не удалось создать объект, значение переменной sectionCode не изменится!
	 * Проверяейте возвращаемое значение, чтобы быть уверенным в том, что объект успешно создан.
	 *
	 * \returns
	 * ::RSH_API_SUCCESS или код ошибки.
	 *
	 * Данный метод используется для создания и иницализации критической секции.
	 * Доступ к таким объектам осуществляется с использованием уникального идентификатора.
	 *
	 */
	U32 InitializeCriticalSection(U32 &sectionCode);

	/*!
	 *
	 * \~english
	 * \brief
	 * Enter critical section
	 *
	 * \param[in] sectionCode
	 * Critical section ID.
	 *
	 * \returns
	 * ::RSH_API_SUCCESS or error code.
	 *
	 * If critical section specified via \b sectionCode
	 * is not locked yet, calling this method will lock section
	 * and proceed to execute section code as usual.\n
	 * If section is already locked, thread will be blocked until
	 * section owner will release section using LeaveCriticalSection().
	 *
	 *
	 * \~russian
	 * \brief
	 * Вход в критическую секцию
	 *
	 * \param[in] sectionCode
	 * Идентификатор критической секции
	 *
	 * \returns
	 * ::RSH_API_SUCCESS или код ошибки.
	 *
	 * Если критическая секция с идентификатором \b sectionCode
	 * не была захвачена на момент вызова данного метода, вызывающий
	 * поток захватывает ее и продолжает выполнение.\n
	 * Если же секция уже была захвачена другим потоком, текущий поток
	 * будет блокирован до тех пор, пока секция не будет освобождена
	 * текущим владельцем путем вызова метода LeaveCriticalSection().
	 *
	 */
    U32 EnterCriticalSection(U32 sectionCode);

	/*!
	 *
	 * \~english
	 * \brief
	 * Try enter critical section
	 *
	 * \param[in] sectionCode
	 * Critical section ID.
	 *
	 * \returns
	 * ::RSH_API_SUCCESS or error code.
	 *
	 * If critical section specified via \b sectionCode
	 * is not locked yet, calling this method will lock section
	 * and proceed to execute section code as usual.\n
	 * If section is already locked, error code will be returned,
	 * but calling thread will not be blocked.
	 *
	 *
	 * \~russian
	 * \brief
	 * Попытка входа в критическую секцию
	 *
	 * \param[in] sectionCode
	 * Идентификатор критической секции
	 *
	 * \returns
	 * ::RSH_API_SUCCESS или код ошибки.
	 *
	 * Если критическая секция с идентификатором \b sectionCode
	 * не была захвачена на момент вызова данного метода, вызывающий
	 * поток захватывает ее и продолжает выполнение.\n
	 * Если же секция уже была захвачена другим потоком,
	 * будет возвращен код ошибки, но вызывающий поток не будет
	 * заблокирован.
	 *
	 */
    U32 TryEnterCriticalSection(U32 sectionCode);

	/*!
	 *
	 * \~english
	 * \brief
	 * Unlock critical section
	 *
	 * \param[in] sectionCode
	 * Critical section ID.
	 *
	 * \returns
	 * ::RSH_API_SUCCESS or error code.
	 *
	 * Leave (unlock) critical section specified via \b sectionCode.\n
	 * If there are other threads that wait this critical section,
	 * they will be informed that it availiable now.
	 *
	 *
	 * \~russian
	 * \brief
	 * Особождение критической секции
	 *
	 * \param[in] sectionCode
	 * Идентификатор критической секции
	 *
	 * \returns
	 * ::RSH_API_SUCCESS или код ошибки.
	 *
	 * Особождение критической секции с идентификатором \b sectionCode.\n
	 * Если есть другие потоки, ожидающие вход в данную секцию, они будут
	 * уведомлены о том, что секция доступна.
	 *
	 */
	U32 LeaveCriticalSection(U32 sectionCode);
};


#endif	//__RSH_EVENT_MANAGER_H__

