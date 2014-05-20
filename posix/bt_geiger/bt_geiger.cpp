//////////////////////////////////////////////////////////////////////
// bt_gieger.cpp -- Bluetooth Gieger
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

#define UNKNOWN_BT 		"/dev/cu.Bluetooth"
#define KEY_BLUETOOTH_DEV	"bluetooth_dev"
#define KEY_BLUETOOTH_SPEED	"bluetooth_speed"

static std::string opt_database;			// Pathname of sqlite3 database
static std::string opt_bluetooth_dev = UNKNOWN_BT;	// Pathname of bluetooth device
static int opt_speed = 38400;				// Bluetooth baud rate

static int opt_help = false;
static int opt_errs = false;

static Sqlite3Db db;

SlipTty bluetooth;

u_packets *packet = 0;

static void
send_current_time() {
	time_t t = time(0);
	struct tm td;

	gmtime_r(&t,&td);

	s_current_time msg;

	msg.pkttype = PT_CurrentTime;
	msg.year = td.tm_year + 1900 - 2014;
	msg.month = td.tm_mon + 1;
	msg.mday = td.tm_mday;
	msg.hour = td.tm_hour;
	msg.min = td.tm_min;
	msg.sec = td.tm_sec;

	bluetooth.write(&msg,sizeof msg);
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

static void
put_config(const char *config_key,const char *config_value) {
	bool ok;

	ok = db.prepare(
		"insert into config(name,value) \n"
		"values (?,?) \n"
	);
	assert(ok);

	db.qbind(config_key);
	db.qbind(config_value);
	ok = db.step();
	assert(ok);
	db.rclear();
}

static void
put_config(const char *config_key,int config_value) {
	std::stringstream ss;

	ss << config_value;
	put_config(config_key,ss.str().c_str());
}

static std::string
get_config(const char *config_key,bool& nullind) {
	std::string value;
	bool ok;

	ok = db.prepare(
		"select value \n"
		"from config \n"
		"where name = ? \n"
	);
	assert(ok);

	ok = db.qbind(config_key);
	assert(ok);

	db.rbind(value);

	int s = db.step();
	assert(s == SQLITE_ROW);

	nullind = db.is_rnull(&value);
	return value;
}

static void
get_config(const char *config_key,bool& nullind,int& value) {
	bool ok;

	ok = db.prepare(
		"select value \n"
		"from config \n"
		"where name = ? \n"
	);
	assert(ok);

	ok = db.qbind(config_key);
	assert(ok);

	db.rbind(value);

	int s = db.step();
	assert(s == SQLITE_ROW);

	nullind = db.is_rnull(&value);
}

static uint8_t
recv_packet() {
	unsigned pktlen = 0;

	for (;;) {
		packet = (u_packets *)bluetooth.read(pktlen);

		switch ( packet->pkt_type ) {
		case PT_CurrentTime :
			if ( pktlen == SZ_CURRENT_TIME )
				return packet->pkt_type;
			fprintf(stderr,"Incorrect packet length: %u\n",pktlen);
			break;
		default :
			fprintf(stderr,"Unknown Packet Type: %d\n",packet->pkt_type);
		}
	}
}

int
main(int argc,char **argv) {
	int optch;
	const char *HOME = getenv("HOME");
	
	if ( HOME ) {
		opt_database = HOME;
		opt_database += "/";
	}

	opt_database += "bt_geiger.db";

	while ( (optch = getopt(argc,argv,":hb:s:d:")) != EOF ) {
		switch ( optch ) {
		case 'd' :
			opt_database = optarg;
			break;
		case 'b' :
			opt_bluetooth_dev = optarg;
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

	if ( !db.open(opt_database.c_str(),false,true) ) {
		fprintf(stderr,"%s: Opening/create database '%s'\n",
			db.errormsg().c_str(),
			opt_database.c_str());
	}
	
	bool ok;

	if ( !db.is_table("config") ) {
		//////////////////////////////////////////////////////
		// New database - initialize content
		//////////////////////////////////////////////////////
		fputs("Initializing new database content.\n",stderr);
		ok = db.execute(
			"create table config( \n"
			"  name  varchar(32) not null primary key, \n"
			"  value varchar(2048) \n"
			") \n"
		);

		if ( !ok ) {
			fprintf(stderr,"%s: create table config\n",
				db.errormsg().c_str());
			exit(2);
		}

		put_config(KEY_BLUETOOTH_DEV,opt_bluetooth_dev.c_str());
		put_config(KEY_BLUETOOTH_SPEED,opt_speed);

	} else	{
		//////////////////////////////////////////////////////
		// Load unspecified config values from database
		//////////////////////////////////////////////////////

		bool nullind = false;

		if ( opt_bluetooth_dev == UNKNOWN_BT ) {
			opt_bluetooth_dev = get_config(KEY_BLUETOOTH_DEV,nullind);
			assert(!nullind);
		}

		get_config(KEY_BLUETOOTH_SPEED,nullind,opt_speed);
		if ( nullind )
			opt_speed = 38400;

	}

	//////////////////////////////////////////////////////////////
	// Open the Bluetooth serial device
	//////////////////////////////////////////////////////////////

	int rc = bluetooth.open(opt_bluetooth_dev.c_str(),opt_speed,2048);
	if ( rc != 0 ) {
		fprintf(stderr,"%s: Bluetooth device '%s'\n",
			strerror(errno),
			opt_bluetooth_dev.c_str());
		db.close();
		exit(3);
	}

	db.close();

	printf("Bluetooth device: %s, baud %d\n",opt_bluetooth_dev.c_str(),opt_speed);

	for (;;) {
		uint8_t pkt_type = recv_packet();

		switch ( pkt_type ) {
		case PT_CurrentTime :
			printf("CurrentTime { year=%u, month = %u, day = %u, hour = %u, minute = %u, second = %u }\n",
				packet->pkt_curtime.year,
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

	send_current_time();
	bluetooth.close();

	return 0;
}

// End bt_gieger.cpp
