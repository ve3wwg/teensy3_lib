//////////////////////////////////////////////////////////////////////
// bt_dummy.cpp -- Bluetooth Gieger Test Dummy Program
// Date: Sun May 18 16:05:40 2014
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <termios.h>
#include <assert.h>

#include "slip.hpp"
#include "sqlite3db.hpp"
#include "bt_geiger.hpp"

#include <string>
#include <sstream>

static std::string opt_serial_dev = "/dev/cu.usbserial-A703CYQ5";	// Must be provided as option
static int opt_speed = 38400;						// Serial baud rate
static int btdev = -1;							// Open fd of serial dev

static int opt_help = false;
static int opt_errs = false;

static uint8_t
bt_read() {
	int rc;
	uint8_t b;

	do	{
		rc = read(btdev,&b,1);
	} while ( rc == -1 );			// EINTR may be one of the expected errors here

	return b;
}

static void
bt_write(uint8_t b) {
	write(btdev,&b,1);
}

SLIP bluetooth(bt_read,bt_write);			// Bluetooth SLIP protocol I/O object

static char rxbuf[2048];
static unsigned pktlen;

static u_packets *packet = (u_packets *)rxbuf;

static uint8_t
recv_packet() {
	SLIP::Status s;

	for (;;) {
		s = bluetooth.read(rxbuf,sizeof rxbuf,pktlen);
		if ( s != SLIP::ES_Ok ) {
			fprintf(stderr,"SLIP::Status: %d\n",s);
			continue;
		}

		switch ( packet->pkt_type ) {
		case PT_CurrentTime :
			if ( pktlen == sizeof(s_current_time) )
				return packet->pkt_type;
			fprintf(stderr,"Incorrect packet length: %u\n",pktlen);
			break;
		default :
			fprintf(stderr,"Unknown Packet Type: %d\n",packet->pkt_type);
		}
	}
}

static void
usage(const char *cmd) {
	const char *cp = strrchr(cmd,'/');

	if ( cp )
		++cp;
	fprintf(stderr,"Usage: %s [-options]\n",cp);
	fputs(	"where:\n"
		"\t-h\t\tThis help.\n",
		stderr);
}

int
main(int argc,char **argv) {
	int optch;

	while ( (optch = getopt(argc,argv,":hb:s:d:")) != EOF ) {
		switch ( optch ) {
		case 'b' :
			opt_serial_dev = optarg;
			break;
		case 's' :
			opt_speed = atoi(optarg);
			break;
		case 'h' :
			opt_help = true;
			break;
		case '?':
			fprintf(stderr,"Unsupported option -%c\n",optopt);
			opt_errs = true;
			break;
		case ':':
			fprintf(stderr,"Option -%c requires an argument.\n",optopt);
			opt_errs = true;
			break;
		default:
			fprintf(stderr,"Unsupported option: -%c\n",optch);
			opt_errs = true;
		}
	}

	if ( opt_help ) {
		usage(argv[0]);
		exit(opt_errs ? 1 : 0);
	}

	if ( opt_errs )
		exit(1);

	//////////////////////////////////////////////////////////////
	// Open the serial device
	//////////////////////////////////////////////////////////////

	btdev = open(opt_serial_dev.c_str(),O_RDWR);
	if ( btdev < 0 ) {
		fprintf(stderr,"%s: Serial device '%s'\n",
			strerror(errno),
			opt_serial_dev.c_str());
		exit(3);
	}

	//////////////////////////////////////////////////////////////
	// Set the serial parameters for raw I/O
	//////////////////////////////////////////////////////////////

	{
		struct termios tattr;
		int rc;

		rc = tcgetattr(btdev,&tattr);
		if ( rc == -1 ) {
			fprintf(stderr,"%s: tcgetattr(bluetooth_dev=%d)\n",
				strerror(errno),
				btdev);
			exit(4);
		}

		cfmakeraw(&tattr);
		rc = cfsetspeed(&tattr,opt_speed);
		rc = tcsetattr(btdev,TCSANOW,&tattr);
		if ( rc == -1 ) {
			fprintf(stderr,"%s: tcsetattr(bluetooth_dev=%d,TCSANOW)\n",
				strerror(errno),
				btdev);
			exit(4);
		}
	}

	printf("Serial device: %s on fd %d, baud %d\n",opt_serial_dev.c_str(),btdev,opt_speed);

	bluetooth.enable_crc8(true);

	for (;;) {
		uint8_t pkt_type = recv_packet();

		switch ( pkt_type ) {
		case PT_CurrentTime :
			printf("CurrentTime { year=%u, month = %u, day = %u, hour = %u, minute = %u, second = %u }\n",
				packet->pkt_curtime.year + 2014,
				packet->pkt_curtime.month,
				packet->pkt_curtime.mday,
				packet->pkt_curtime.hour,
				packet->pkt_curtime.min,
				packet->pkt_curtime.sec);
			break;
		default :
			assert(0);
		}
	}

	return 0;
}

// End bt_dummy.cpp
