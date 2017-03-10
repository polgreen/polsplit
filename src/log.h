/*
 * log.h
 *
 *  Created on: 9 Mar 2017
 *      Author: elipol
 */

#ifndef SRC_LOG_H_
#define SRC_LOG_H_
// log.h; the header file which defines Log(); and LogErr();

#define LOGFILE "trace.log"     // all Log(); messages will be appended to this file

extern bool LogCreated;      // keeps track whether the log file is created or not

void Log (char *message);    // logs a message to LOGFILE

#endif /* SRC_LOG_H_ */
