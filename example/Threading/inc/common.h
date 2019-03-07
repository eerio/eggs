/* Collection of miscellaneous and commonly-used functions and macros
 *
 * author: Pawel Balawender
 * https://github.com/eerio/eggs.git
 */

/* Simplest delay routine possible, it counts down n times */
void delay(unsigned int);

/* Handlers. They periodically toggle the User LED with different freq. */
void handler_blinking_fast(void);
void handler_blinking_medium(void);
void handler_blinking_slow(void);

