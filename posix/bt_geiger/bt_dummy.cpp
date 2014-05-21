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

#include "sqlite3db.hpp"
#include "bt_geiger.hpp"
#include "sliptty.hpp"

#include <string>
#include <sstream>

static std::string opt_serial_dev = "/dev/cu.usbserial-A703CYQ5";	// Must be provided as option
static int opt_speed = 38400;						// Serial baud rate

static int opt_help = false;
static int opt_errs = false;

SlipTty bluetooth;

u_packets *packet = 0;

static uint8_t
recv_packet() {
	unsigned pktlen = 0;

	for (;;) {
		packet = (u_packets *)bluetooth.read(pktlen);

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

	int rc = bluetooth.open(opt_serial_dev.c_str(),opt_speed,2048);
	if ( rc != 0 ) {
		fprintf(stderr,"%s: Serial device '%s'\n",
			strerror(rc),
			opt_serial_dev.c_str());
		exit(3);
	}

	printf("Serial device: %s, baud %d\n",opt_serial_dev.c_str(),opt_speed);

	for (;;) {
		uint8_t pkt_type = recv_packet();

		switch ( pkt_type ) {
		case PT_CurrentTime :
			{
				time_t t = static_cast<time_t>(packet->pkt_curtime.time);
				struct tm td;
				char buf[80];

				localtime_r(&t,&td);
				strftime(buf,sizeof buf,"%Y-%m=%d %H:%M:%D",&td);

				printf("CurrentTime : %s  (%u vs %lu)\n",buf,packet->pkt_curtime.time,(unsigned long)time(0));
			}
			break;
		default :
			assert(0);
		}
	}

	return 0;
}

// End bt_dummy.cpp
