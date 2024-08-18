#include <unistd.h>
#include <stdio.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>


#define BCM2708_PERI_BASE       0x3f000000//0x20000000
#define GPIO_BASE               (BCM2708_PERI_BASE + 0x200000)	// GPIO controller
#define UART_BASE		(BCM2708_PERI_BASE + 0x200100)
//uart in datasheet: 0x7e20100
//gpio in datasheet: 0x7e20000

#define BLOCK_SIZE 		(4*1024)

// IO Acces
struct BCM2835Peripheral {
  unsigned long addr_p;
  int mem_fd;
  void *map;
  volatile unsigned int *addr;
};

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x)
#define INP_GPIO(g)   *(gpio.addr + ((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g)   INP_GPIO(g) ; *(gpio.addr + ((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio.addr + (((g)/10))) |= (((a)<=3?(a) + 4:(a)==4?3:2)<<(((g)%10)*3))
#define SET_GPIO_TX0   INP_GPIO(14) ; *(gpio.addr + (((14)/10))) |= (4<<(((14)%10)*3))

#define GPIO_SET  *(gpio.addr + 7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR  *(gpio.addr + 10) // clears bits which are 1 ignores bits which are 0

#define GPIO_READ(g)  *(gpio.addr + 13) &= (1<<(g))

#define SET_TX_FIFO(g)	*(gpio.addr + 0x100 + 0x34) &= 0xFFFFFFF8 ; *(gpio.addr + 0x100 + 0x34) |= g<<0 ;

int mapPeripheral(struct BCM2835Peripheral *p) ;
void unmapPeripheral(struct BCM2835Peripheral *p) ;


