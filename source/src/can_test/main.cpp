/******************************************************************************
	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    fieldrobot  Copyright (C) 2013  ZhouPeng CAAMS	<zp_caams@163.com>
    @file can-app.c
    Xilinx Zynq CAN PS test app for ZED board.
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/sockios.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>

#include <aio.h>
#include <signal.h>


#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#include "Comm.h"

void aio(int s,struct can_frame &frame){


  int  ret;
  struct aiocb my_aiocb;

  int buffer_len = sizeof(struct can_frame);

  /* Zero out the aiocb structure (recommended) */
  bzero( (char *)&my_aiocb, sizeof(struct aiocb) );

  /* Allocate a data buffer for the aiocb request */
  my_aiocb.aio_buf = malloc(buffer_len + 1);
  if (!my_aiocb.aio_buf) perror("malloc");

  /* Initialize the necessary fields in the aiocb */
  my_aiocb.aio_fildes = s;
  my_aiocb.aio_nbytes = buffer_len;
  my_aiocb.aio_offset = 0;

  while(1){

	  ret = aio_read( &my_aiocb );
	  if (ret < 0) perror("aio_read");

	  while ( aio_error( &my_aiocb ) == EINPROGRESS ) ;

	  if ((ret = aio_return( &my_aiocb )) > 0) {
		  std::string ss = cComm::ByteToHexString((U8*)(my_aiocb.aio_buf),ret);
		  std::cout<<"res len:"<<ret<<" "<<ss<<std::endl;
		  //printf("%s",my_aiocb.aio_buf);
		/* got ret bytes on the read */
	  } else {
		/* read failed, consult errno */
	  }

  }
}
void aio_completion_handler( sigval_t sigval )
{
  int  ret;
  struct aiocb *req;

  req = (struct aiocb *)sigval.sival_ptr;

  /* Did the request complete? */
  if (aio_error( req ) == 0) {

    /* Request completed successfully, get the return status */

	if ((ret = aio_return( req )) > 0) {
		std::string ss = cComm::ByteToHexString((U8*)(req->aio_buf),ret);
		std::cout<<"res len:"<<ret<<" "<<ss<<std::endl;
		//printf("%s",my_aiocb.aio_buf);
		/* got ret bytes on the read */
	} else {
		/* read failed, consult errno */
	}
  }

  return;
}
void aio2(int s,struct can_frame &frame){
	struct aiocb my_aiocb;
	int  ret;

	int buffer_len = sizeof(struct can_frame);
	/* Set up the AIO request */
	bzero( (char *)&my_aiocb, sizeof(struct aiocb) );
	my_aiocb.aio_fildes = s;
	my_aiocb.aio_buf = &frame;
	my_aiocb.aio_nbytes = buffer_len;
	my_aiocb.aio_offset = 0;

	/* Link the AIO request with a thread callback */
	my_aiocb.aio_sigevent.sigev_notify = SIGEV_THREAD;
	my_aiocb.aio_sigevent.sigev_notify_function = aio_completion_handler;
	my_aiocb.aio_sigevent.sigev_notify_attributes  = NULL;
	my_aiocb.aio_sigevent.sigev_value.sival_ptr = &my_aiocb;

	std::cout<<"begin read"<<std::endl;
	ret = aio_read( &my_aiocb );
	std::cout<<"after read"<<std::endl;
	while(1){}
}
int main(void)
{
	int s;
	struct sockaddr_can addr;
	struct can_frame frame;
	int nbytes;
	int i;
    struct ifreq ifr;

printf("1\n");

	s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	if(s==-1){
		fprintf(stderr, "Socket error:%s\n\a", strerror(errno));
		exit(1);
	}
printf("2\n");
    strcpy(ifr.ifr_name, "can0" );
    ioctl(s, SIOCGIFINDEX, &ifr);

	bzero(&addr, sizeof(struct sockaddr_can));
    addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	bind(s, (struct sockaddr *)&addr, sizeof(addr));


#if 0
	frame.can_id = 0x01;
	frame.can_dlc = 8;
	for (i=0;i<frame.can_dlc;i++){
		frame.data[i] = i;
	}

	for(i=0;i<1;i++){
		nbytes = write(s, &frame, sizeof(struct can_frame));
		if (nbytes < 0){
			fprintf(stderr, "raw write error:%s\n\a", strerror(errno));
			exit(1);
		}

		if(nbytes < sizeof(struct can_frame)){
			fprintf(stderr, "write incomplete CAN frame\n\a");
			exit(1);
		}
		sleep(1);
		printf("can frame sent %d\n", i);
	}

	sleep(1);
	nbytes = read(s, &frame, sizeof(struct can_frame));
	if (nbytes < 0){
		fprintf(stderr, "raw read error:%s\n\a", strerror(errno));
		exit(1);
	}

	if(nbytes < sizeof(struct can_frame)){
		fprintf(stderr, "read incomplete CAN frame\n\a");
		exit(1);
	}
	printf("Frame can_id:%d, can_dlc:%d\n", frame.can_id, frame.can_dlc);

	for (i=0;i<frame.can_dlc;i++){
		printf("Frame can_data%d: %d\n", i, frame.data[i]);
	}
#else
	//aio(s,frame);
	aio2(s,frame);
#endif
	close(s);

	//puts("Hello Can World!"); /* prints !!!Hello World!!! */
	return EXIT_SUCCESS;
}
