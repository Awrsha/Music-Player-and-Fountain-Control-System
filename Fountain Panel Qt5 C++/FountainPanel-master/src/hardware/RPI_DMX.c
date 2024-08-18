//note: the usart clock speed should be 8000000 or higher in config files (i forgot)

#include <asm/termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <string.h>
#include <pthread.h>
#include "RPI.h"

int DMX_GPU_Emulator ( void ) ;

extern struct BCM2835Peripheral gpio;

int DMX_GPU_Emulator ( void )
{
	struct termios2  config ;
	unsigned char buffer[ 3600 ] ;
	int fd, fd2, i ;
	//unsigned char *buffer = (unsigned char *)v_buffer ;
	unsigned long int flag = 0 ;
	int tlen = 0 ;


	if(mapPeripheral(&gpio) == -1)
	{
		printf("Failed to map the physical GPIO registers into the virtual memory space.\n");
		return 0;
	}

	//OUT_GPIO(4);

	fd = open ( "/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_SYNC ) ; // | O_NDELAY ) ;
	if ( fd == -1)
	{
		printf ( "failed to open port\n" ) ;
		return 0;
	} ;
	fd2 = open ( "/home/pi/Desktop/RPI_DMX_2/data.bin", O_RDWR | O_NOCTTY | O_SYNC ) ; // | O_NDELAY ) ;
	if ( fd2 == -1)
	{
		printf ( "failed to open file\n" ) ;
		return 0;
	} ;
	//close ( fd2 ) ;

	
	ioctl ( fd, TCGETS2, &config ) ;
	config.c_cflag &= ~CBAUD ;
	config.c_cflag |= CS8 ;
	config.c_cflag |= BOTHER ;
	config.c_cflag |= CSTOPB ;
	config.c_cflag &= ~PARENB ;
	config.c_ispeed = 250000 ;
	config.c_ospeed = 250000 ;
	config.c_oflag &= ~ONLCR ;
	config.c_oflag &= ~OCRNL ;
	ioctl ( fd, TCSETS2, &config ) ;

	//OUT_GPIO(4);
	SET_TX_FIFO ( 0 ) ;
	
	struct sched_param sched ;

	memset (&sched, 0, sizeof(sched)) ;
	
	if (99 > sched_get_priority_max (SCHED_RR))
		sched.sched_priority = sched_get_priority_max (SCHED_RR) ;
	else
		sched.sched_priority = 99 ;

	//printf ( "%d\n", sched_setscheduler (0, SCHED_RR, &sched) ) ;
	//printf ( "%d\n", nice ( -99 ) ) ;

	for ( ; ; )
	{
		for ( i=0 ; i < 513 ; i++ )
		{
			write ( fd, &buffer[i], 1 ) ;
			while ( tlen != 0 )
				ioctl ( fd, TIOCOUTQ, &tlen ) ;
			tlen = 1 ;
			//usleep ( 100 ) ;
		} ;

		// Define pin 8 as output
		OUT_GPIO(14);
		//for ( flag = 0 ; flag < 200 ;flag ++ )
		//	flag++ ;
		usleep ( 400 ) ;
		//GPIO_CLR |= 1 << 4;
		GPIO_CLR |= 1 << 14;
		usleep ( 500 ) ;
		//for ( flag = 0 ; flag < 1000000 ;flag ++ )
		//	flag++ ;
		//GPIO_SET |= 1 << 4;
		GPIO_SET |= 1 << 14;
		
		SET_GPIO_TX0 ;

		read( fd2, &buffer[0], 513 ) ;
		buffer[ 0 ] = 0 ;
		
		for ( flag = 0 ; flag < 5000 ;flag ++ )
			flag++ ;
	}
	
	close ( fd ) ;
	close ( fd2 ) ;

}
