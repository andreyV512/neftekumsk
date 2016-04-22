/*!
 * \copyright JSC "Rudnev-Shilyaev"
 * 
 * \file RshFunctions.h
 * \date 27.06.2014
 * \version 1.0 [SDK 2.00]
 * 
 * \~english
 * \brief
 * Global functions prototypes.
 * 
 * This file contains prototypes of common functions used within RSH SDK code.
 * 
 * \~russian
 * \brief
 * Прототипы глобальных функций.
 * 
 * В этом файле содержатся прототипы функций, часто используемых в RSH SDK.
 * 
 */
#ifndef __RSH_FUNCTIONS_H__
#define __RSH_FUNCTIONS_H__
#include "RshTypes.h"

/*!
 *
 * \~english
 * \brief
 * Tries to get a unique number
 *
 * This function generates unique
 * 64bit number using current clock,
 * time and process id.
 *
 * \returns
 * Generated unique number
 *
 * \~russian
 * \brief
 * Генерация уникального числа
 *
 * Данная функция генерирует 64битное число,
 * используя текущее время, идентификатор процесса
 * и данные функции clock().
 *
 * \returns
 * Сгенерированное уникальное число
 *
 */
U64 RshMix();

/*!
 *
 * \~english
 * \brief
 * Tries to get a unique number
 *
 * This function generates unique
 * 64bit number using given seed.
 * Generatied value depend on current time
 * and process id also.
 *
 * \param[in] seed
 * Source number
 *
 * \returns
 * Generated unique number
 *
 * \~russian
 * \brief
 * Генерация уникального числа
 *
 * Данная функция генерирует 64битное число,
 * используя заданное начальное значение (seed).
 * Генерация числа также зависит от 
 * текущего времени и идентификатора процесса
 *
 * \param[in] seed
 * Исходное число
 *
 * \returns
 * Сгенерированное уникальное число
 *
 */
U64 RshMix(U64 seed);

/*!
 *
 * \~english
 * \brief
 * CRC32 calculation
 *
 * This function calculates crc32 value of given string.
 *
 * \param[in] buf
 * Pointer to string buffer
 *
 * \param[in] len
 * How many symbols from buffer must be used in calculation
 *
 * \returns
 * Calculated crc32 value or zero if input parameters are invalid.
 *
 * \~russian
 * \brief
 * Вычисление контрольной суммы CRC32
 *
 * Данная функция вычисляет crc32 заданной строки.
 *
 * \param[in] buf
 * Указатель на буфер с данными (строку).
 *
 * \param[in] len
 * Количество символов в заданной строке, по которым будет
 * производиться вычисление контрольной суммы.
 *
 * \returns
 * Вычисленное значение crc32 или ноль, если входные параметры неверны.
 *
 */
U32 RshCRC32(U8 *buf, size_t len);

//double RshTimeCurrent();

/*!
 *
 * \~english
 * \brief
 * Fill structure with current time information
 *
 * This function fills the passed RshTime structure
 * with current time values.
 *
 * \param[in, out] currentTime
 * Structure to fill
 *
 * \~russian
 * \brief
 * Текущее время
 *
 * Данная функция заполняет переданную структуру
 * RshTime - поля структуры получают значения,
 * соответствующие системному времени.
 *
 * \param[in, out] currentTime
 * Структура, которая будет заполнена.
 *
 * \returns
 * Сгенерированное уникальное число
 *
 */
void RshTimeReal(RshTime& currentTime);

/*!
 *
 * \~english
 * \brief
 * Convert elapsed time
 *
 * Convert elapsed time in seconds (floating point) to
 * hours, minutes, seconds, milliseconds and microseconds elapsed.
 * For example, if one pass 65.3 value as timeSpent, result will be
 * hours = 0, minutes = 1, seconds = 5, milliseconds = 300, microseconds = 0
 *
 * \param[in] timeSpent
 * Time (in seconds) to be converted.
 *
 * \param[in, out] hours
 * Variable to store calculated hours result.
 *
 * \param[in, out] minutes
 * Variable to store calculated minutes result.
 *
 * \param[in, out] seconds
 * Variable to store calculated seconds result.
 *
 * \param[in, out] milliseconds
 * Variable to store calculated milliseconds result.
 *
 * \param[in, out] microseconds
 * Variable to store calculated microseconds result.
 *
 * \~russian
 * \brief
 * Преобразование интервала времени
 *
 * Преобразование времени (в секундах, число с плавающей точкой) в
 * полное представление - часы, минуты, секунды, миллисекунды и микросекунды.
 * Например, если передать в функцию значение timeSpent равное 65.3, результат
 * будет следующим: hours = 0, minutes = 1, seconds = 5, milliseconds = 300, microseconds = 0
 *
 * \param[in] timeSpent
 * Время (в секундах), которое нужно преобразовать.
 *
 * \param[in, out] hours
 * Переменная, в которую будет записан результат расчета - часы.
 *
 * \param[in, out] minutes
 * Переменная, в которую будет записан результат расчета - минуты.
 *
 * \param[in, out] seconds
 * Переменная, в которую будет записан результат расчета - секунды.
 *
 * \param[in, out] milliseconds
 * Переменная, в которую будет записан результат расчета - миллисекунды.
 *
 * \param[in, out] microseconds
 * Переменная, в которую будет записан результат расчета - микросекунды.
 *
 */
void RshTimeElapsed(double timeSpent, U32& hours, U32& minutes, U32& seconds, U32& milliseconds, U32& microseconds); 

/*!
 *
 * \~english
 * \brief
 * Convert elapsed time to string
 *
 * Convert elapsed time in seconds (floating point) to
 * hours, minutes, seconds, milliseconds and microseconds elapsed,
 * and store result in std::string
 * For example, if one pass 65.3 value as timeSpent, result string will be
 * "<b>h: 0; min: 1; sec: 5; msec: 300; usec: 0</b>"
 *
 * \param[in] timeSpent
 * Time (in seconds) to be converted. 
 *
 * \param[in, out] str
 * Reference to string where result of calculation will be placed.
 *
 * \~russian
 * \brief
 * Преобразование временного интервала в строку
 *
 * Преобразование времени (в секундах, число с плавающей точкой) в
 * полное представление - часы, минуты, секунды, миллисекунды и микросекунды; 
 * Результат преобразования выдается в виде строки std::string
 * Например, если передать в функцию значение timeSpent равное 65.3, результат
 * вычисления будет возвращен в виде следующей строки: 
 * "<b>h: 0; min: 1; sec: 5; msec: 300; usec: 0</b>"
 *
 * \param[in] timeSpent
 * Время (в секундах), которое нужно преобразовать.
 *
 * \param[in, out] str
 * Ссылка на строку, в которую будет помещен результат вычисления
 *
 */
void RshTimeElapsedString(double timeSpent, std::string& str); 

/*!
 *
 * \~english
 * \brief
 * Convert elapsed time to string (wchar)
 *
 * Convert elapsed time in seconds (floating point) to
 * hours, minutes, seconds, milliseconds and microseconds elapsed,
 * and store result in std::wstring
 * For example, if one pass 65.3 value as timeSpent, result string will be
 * "<b>h: 0; min: 1; sec: 5; msec: 300; usec: 0</b>"
 *
 * \param[in] timeSpent
 * Time (in seconds) to be converted. 
 *
 * \param[in, out] str
 * Reference to string where result of calculation will be placed.
 *
 * \~russian
 * \brief
 * Преобразование временного интервала в строку (wchar)
 *
 * Преобразование времени (в секундах, число с плавающей точкой) в
 * полное представление - часы, минуты, секунды, миллисекунды и микросекунды; 
 * Результат преобразования выдается в виде строки std::wstring
 * Например, если передать в функцию значение timeSpent равное 65.3, результат
 * вычисления будет возвращен в виде следующей строки: 
 * "<b>h: 0; min: 1; sec: 5; msec: 300; usec: 0</b>"
 *
 * \param[in] timeSpent
 * Время (в секундах), которое нужно преобразовать.
 *
 * \param[in, out] str
 * Ссылка на строку, в которую будет помещен результат вычисления
 *
 */
void RshTimeElapsedWString(double timeSpent, std::wstring& str); 

/*!
 *
 * \~english
 * \brief
 * Calculate [LSB/volts] conversion coefficient
 *
 * This function calculates coefficient
 * that can be used to convert data from 
 * voltage to LSB codes (or vice versa).\n
 * For example, if one need  send signal buffer
 * to GSPF, conversion coefficient can be calculated
 * and then every item in data array (in volts) must be
 * multipyed on that coefficient to produce valid buffer,
 * that can be send to device.
 *
 * \param[in] gain
 * Gain coefficient used by the device or channel we need coefficient for.
 *
 * \param[in] range
 * Maximum range in volts (for gain equal to 1). You can get this value using
 * IRshDevice::Get() method with the ::RSH_GET_DEVICE_INPUT_RANGE_VOLTS to get max
 * positive voltage and then multiplying it by 2 (to get full range).
 *
 * \param[in] bitSize
 * Bit number. This value determines LSB range. (range = 1<<bitSize)
 * RshApi IRshDevice::GetData() method use data shifted to MSB, so
 * one need pass 8, 16, or 32 as bitSize value, not ADC or DAC bits.
 *
 * \returns
 * Coefficient that can be used to convert data from Volt to LSB (or vice versa)
 *
 * \remarks
 * "LSB to volt" in function name means that calculated coefficient
 * will have [LSB/Volt] unit, so you can actualy use it two ways - to
 * convert from volts to LSB (need to multiply) or to convert from LSB to
 * volts (need to devide).
 *
 * \~russian
 * \brief
 * Вычисление коэффициента преобразования [МЗР/Вольт]
 *
 * Данная функция вычисляет коэффициент, который может быть использован
 * для преобразования данных из Вольт в МЗР (и наоборот).\n
 * Например, если нужно загрузить буфер с данными в ГСПФ, можно подготовить
 * буфер с отсчетами в вольтах (результат вычисления некой функции, к примеру),
 * и затем преобразовать данный буфер в формат МЗР, который уже можно загрузить
 * в устройство. Для этого каждый отсчет нужно домножить на коэффициент, который
 * и рассчитывает данная функция.
 *
 * \param[in] gain
 * Коэффициент усиления, используемый устройством или каналом, данные для которого
 * нужно преобразовать.
 *
 * \param[in] range
 * Полный входной диапазон устройства (при коэффициенте усиления равном 1).
 * Можно получить максимальное положительное значение напряжения используя
 * метод IRshDevice::Get() с параметром ::RSH_GET_DEVICE_INPUT_RANGE_VOLTS,
 * и затем умножить полученное значение на 2, чтобы получить полный входной диапазон.
 *
 * \param[in] bitSize
 * Количество бит. Данное значение определяет максимальный диапазон в МЗР (диапазон = 1<<bitSize)
 * Метод RshApi IRshDevice::GetData() сдвигает данные влево, чтобы был заполнен
 * максимальный значащий разряд, поэтому при работе с этими данными размер bitSize
 * будет равен 8, 16, или 32 (а не разрядности АЦП/ЦАП).
 *
 * \returns
 * Рассчитанный коэффициент, который может быть использован для преобразования данных
 *
 * \remarks
 * "LSB to volt" в названии функции означает, что расчитанный коэффициент будет иметь
 * размерность [МЗР/Вольт] и можно его испольовать как для преобразования данных из
 * вольт в МЗР (для этого данные нужно на коэффициент домножить), так и для преобразования
 * из МЗР в вольты (для этого данные нужно на коэффициент поделить).
 *
 */
double RshLsbToVoltCoef(U32 gain, double range, U8 bitSize);

/*!
 *
 * \~english
 * \brief
 * Calculate [volts/LSB] conversion coefficient
 *
 * This function is analog ot RshLsbToVoltCoef() function, the only
 * differenct is that coefficient is reversed.\n
 * RshVoltToLsbCoef() = 1/RshLsbToVoltCoef();
 * 
 * \see
 * RshLsbToVoltCoef()
 *
 * \~russian
 * \brief
 * Вычисление коэффициента преобразования [Вольт/МЗР]
 *
 * Данная функция - аналог функции RshLsbToVoltCoef() function, 
 * но вычисляет обратный коэффициент.\n
 * RshVoltToLsbCoef() = 1/RshLsbToVoltCoef();\n
 * Добавлена для удобства.
 * 
 * \see
 * RshLsbToVoltCoef()

 */
double RshVoltToLsbCoef(U32 gain, double range, U8 bitSize);

/*!
 *
 * \~english
 * \brief
 * Convert voltage value to LSB value
 *
 * This function convert input value in volts
 * to corresponding value in LSB.
 *
 * \param[in] voltage
 * Voltage value we need to convert
 *
 * \param[in] gain
 * Gain coefficient used by the device or channel we need coefficient for.
 *
 * \param[in] range
 * Maximum range in volts (for gain equal to 1). You can get this value using
 * IRshDevice::Get() method with the ::RSH_GET_DEVICE_INPUT_RANGE_VOLTS to get max
 * positive voltage and then multiplying it by 2 (to get full range).
 *
 * \param[in] bitSize
 * Bit number. This value determines LSB range. (range = 1<<bitSize)
 * RshApi IRshDevice::GetData() method use data shifted to MSB, so
 * one need pass 8, 16, or 32 as bitSize value, not ADC or DAC bits.
 *
 * \returns
 * Corresponding value in LSB
 *
 * \remarks
 * This function is convinient when one or two calculation need be
 * performed. If you need convert big array of data, better use one 
 * of RshVoltToLsbCoef() or RshLsbToVoltCoef() functions and then
 * multiply all array values on coefficient obtained.
 *
 * \~russian
 * \brief
 * Преобразование данных из вольт в МЗР
 *
 * Данная функция преобразует значение напряжения в вольтах
 * соответствующий ему код АЦП в МЗР.
 *
 * \param[in] voltage
 * Значение напряжения, которое необходимо преобразовать
 *
 * \param[in] gain
 * Коэффициент усиления, используемый устройством или каналом, данные для которого
 * нужно преобразовать.
 *
 * \param[in] range
 * Полный входной диапазон устройства (при коэффициенте усиления равном 1).
 * Можно получить максимальное положительное значение напряжения используя
 * метод IRshDevice::Get() с параметром ::RSH_GET_DEVICE_INPUT_RANGE_VOLTS,
 * и затем умножить полученное значение на 2, чтобы получить полный входной диапазон.
 *
 * \param[in] bitSize
 * Количество бит. Данное значение определяет максимальный диапазон в МЗР (диапазон = 1<<bitSize)
 * Метод RshApi IRshDevice::GetData() сдвигает данные влево, чтобы был заполнен
 * максимальный значащий разряд, поэтому при работе с этими данными размер bitSize
 * будет равен 8, 16, или 32 (а не разрядности АЦП/ЦАП).
 *
 * \returns
 * Код АЦП в МЗР
 *
 * \remarks
 * Данная функция удобна, когда нужно сделать небольшое количество преобразований.
 * Если предполагается многократный вызов данной функции (при обработке массива, например),
 * лучше воспользоваться одной из функций RshVoltToLsbCoef() или RshLsbToVoltCoef(), и 
 * использовать полученный коэффициент для преобразования данных в массиве.
 *
 */
double RshVoltToLsb(double voltage, U32 gain, double range, U8 bitSize);

/*!
 *
 * \~english
 * \brief
 * Convert LSB value to voltage value
 *
 * This function do reverse operation of RshVoltToLsb() function - 
 * convert LSB value to volts.
 *
 * \see RshVoltToLsb()
 *
 * \~russian
 * \brief
 * Преобразование данных из МЗР в вольты
 *
 * Данная функция выполняет действие, обратное вызову RshVoltToLsb() - 
 * преобразовывает данные из МЗР в вольты.
 *
 * \see RshVoltToLsb()
 *
 */
double RshLsbToVolt(double lsb, U32 gain, double range, U8 bitSize);

/*!
 *
 * \~english
 * \brief
 * Round a double value with specified precision
 *
 * This function rounds given value to presicion.\n
 * For example, if one pass 3.14159 with precision=2,
 * result will be 3.14
 *
 * \param[in] value
 * Value to round
 *
 * \param[in] precision
 * Desired precision (digits after point)
 *
 * \returns
 * Rounded doulbe value
 *
 * \~russian
 * \brief
 * Округление числа с заданной точностью
 *
 * Данная функция округляет число с плавающей точкой
 * до заданного количества знаков после запятой.\n
 * Например, если передать в функцию значение 3.14159 и
 * указать precision=2, результат будет равено 3.14
 *
 * \param[in] value
 * Число, которое нужно округлить
 *
 * \param[in] precision
 * Желаемая точность (количество знаков после запятой)
 *
 * \returns
 * Результат округления
 *
 */
double RshRoundD(double value, double precision);

/*!
 *
 * \~english
 * \brief
 * Construct UTF-16 string from UTF-8 constant string
 *
 * Convinience function used to initialize std::wstring 
 * values with string constants. 
 * When source file is coded to UTF-8 there some issues
 * with cyrilic string constants that must be UTF-16
 * converted (at least in Windows).
 * This function work over this issue
 *
 * \param[in] pStr
 * UTF-8 encoded string
 *
 * \returns
 * UTF-16 constant string 
 *
 * \warning
 * Current realisaion is a bit clumsy, use wchar_t buffer
 * with size 1024 and replace data in it with every function call.
 * So, result of the function can be used only to initialize other
 * variables and not directly.
 *
 * \~russian
 * \brief
 * Преобразование строки в кодировку UTF-16 из UTF-8
 *
 * Данная функция используется в основном для инициализации строк
 * std::wstring значениями строковых констант.
 * Дело в том, что если исходник имеет кодировку UTF-8, возникают
 * проблемы с кирилическими символами (по крайней мере в Windows) 
 * при преобразовании их в формат UTF-16.
 * Данная функция позволяет обойти эту проблему.
 *
 * \param[in] pStr
 * Строка в кодировке UTF-8
 *
 * \returns
 * Строка в кодировке UTF-16
 *
 * \warning
 * Текущая реализации функции не очень хорошая, используется
 * статический буфер размером 1024 символа (отсюда максимальная
 * длина строки), плюс этот буфер перезаписывается каждый раз
 * при вызове функции, т.е. результат преобразования может использоваться
 * только для инициализации других переменных - иначе данные будут
 * потеряны при следующем вызове функции.
 *
 */
const wchar_t* RshConvertToUTF16(const char* pStr);

/*!
 *
 * \~english
 * \brief
 * Copy string to ::RSH_BUFFER_U8 or ::RSH_BUFFER_S8 buffer
 *
 * This function can be used to set buffer values from
 * string. If buffer type differs from rshBufferTypeU8 or rshBufferTypeS8,
 * error code will be returned
 *
 * \param[in] str
 * String that will be copied to buffer.
 * Must be NULL-terminated (strlen is used to determine size).
 *
 * \param[in,out] userBuffer
 * Buffer where data will be copied
 *
 * \returns 
 * RSH_API_SUCCESS or error code.
 *
  * \~russian
 * \brief
 * Копирование данных из строки в буфер ::RSH_BUFFER_U8 или ::RSH_BUFFER_S8
 *
 * Данная функция может быть использована для того, чтобы скопировать данные из
 * строки в буфер. Тип буфера должен быть rshBufferTypeU8 или rshBufferTypeS8,
 * иначе будет возвращен код ошибки.
 *
 * \param[in] str
 * Строка, которая будет скопирована в буфер.
 * Должна завершаться нулем, т.к. для определения длины строки используется вызов strlen()
 *
 * \param[in,out] userBuffer
 * Буфер, в который будут скопированы данные строки
 *
 * \returns 
 * RSH_API_SUCCESS или код ошибки.
 */
U32 RshCopyStringTo(const char* str, RshBaseType* userBuffer); 

#endif //__RSH_FUNCTIONS_H__