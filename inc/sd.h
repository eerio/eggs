/* Prototypes for SD abstractions and initialization procedures
 *
 * author: Paweł Balawender
 * github.com@eerio
 */
#include"diskio.h" /* DSTATUS */

DSTATUS sd_initialize (void);
DRESULT sd_readp(BYTE*, DWORD, UINT, UINT);

