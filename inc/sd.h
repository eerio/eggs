/* Prototypes for SD abstractions and initialization procedures
 *
 * author: Paweł Balawender
 * github.com@eerio
 */
#include"diskio.h" /* DSTATUS */

DSTATUS sd_initialize (void);
DRESULT sd_readp(BYTE*, DWORD, UINT, UINT);

typedef struct {
    uint8_t status;
    uint8_t chs_addr_start[3];
    uint8_t type;
    uint8_t chs_addr_end[3];
    uint32_t lba_addr;
    uint32_t sectors_num;
} PartitionEntry;

typedef struct {
    uint8_t bootstrap_code[446];
    PartitionEntry PartitionTable[4];
    uint32_t boot_signature;
} MBR;


