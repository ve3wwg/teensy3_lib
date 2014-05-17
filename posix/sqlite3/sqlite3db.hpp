//////////////////////////////////////////////////////////////////////
// sqlite3db.hpp -- Sqlite3 C++ Interface
// Date: Sat May 17 07:59:59 2014   (C) Warren Gay ve3wwg
///////////////////////////////////////////////////////////////////////

#ifndef SQLITE3_HPP
#define SQLITE3_HPP

#include <sqlite3.h>

#include <string>
#include <vector>

class Sqlite3Db {
	struct sqlite3		*sqldb;		// Sqlite3 database 
	sqlite3_stmt		*stmt;		// Prepared statement
	bool			needs_reset;	// True when prepared statement needs a reset
	bool			trunc;		// True if any row result was truncated due to buffer size
	int			bindx;		// Bind index
	int			status;		// Last status returned
	std::string		errmsg;		// Error message for last error

	enum ResultType {
		RT_int,
		RT_int64,
		RT_double,
		RT_text,
		RT_blob
	};

	struct s_result {
		ResultType	type;		// Column's data type
		void		*loc;		// Receiving storage location
		unsigned	length;		// Receiving buffer max length
		unsigned	rbytes;		// Returned # of bytes
		bool		is_null;	// Returned null setting
	};

	std::vector<s_result>	rvec;		// Result vector (received columns)

public:	Sqlite3Db();

	bool open(const char *filename,bool readonly=false,bool create=false);
	void close();

	bool prepare(const char *sql);			// Perpare query (1st)

	bool qbind_null();				// Bind NULL to query (2nd)
	bool qbind(int qv);				// Bind int to query
	bool qbind(sqlite3_int64 qv);			// Bind int64 to query
	bool qbind(double qv);				// Bind double to query
	bool qbind(const char *qv);			// Bind text to query
	bool qbind(const char *qblob,int nbytes);	// Bind blob to query

	void rbind(int& rv);				// Bind int result (3rd)
	void rbind(sqlite3_int64& rv);			// Bind int64 result
	void rbind(double& rv);				// Bind double result
	void rbind(char *rv,unsigned maxbytes);		// Bind text result
	void rbind(char *rblob,int maxbytes);		// Bind blob result

	unsigned rlength(void *loc);			// Length of returned result

	int step();					// Execute statement/fetch (4th)

	bool is_rnull(void *loc);			// Returns true if the indicated value was NULL
	inline bool is_rtrunc() { return trunc; }	// Returns true if any result was truncated

	int rclear();					// Clear last prepared query & results (5th)
};


#endif // SQLITE3_HPP

// End sqlite3db.hpp
