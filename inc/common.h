/* Collection of miscellaneous and commonly-used functions and macros
 *
 * author: Paweł Balawender
 * github.com@eerio
 */
#include<stm32f0xx.h>

/* Simplest delay routine possible, it counts down n times */
void delay(unsigned int);

/* Handler for a thread. It periodically toggles the User LED */
void handler_blinking_fast(void);

