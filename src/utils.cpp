#include "utils.hpp"

void traceEvent(int traceLevel, int eventTraceLevel,
		       const char* file, const int line, const char * format, ...) {
  va_list va_ap;
  struct tm result;
  const char *extra_msg = "";
  
  switch(eventTraceLevel)
  {
    // case TRACE_LEVEL_DISABLE:
    //     return;
    case TRACE_LEVEL_ERROR:
        extra_msg = "ERROR: ";
        break;
    case TRACE_LEVEL_WARNING:
        extra_msg = "WARNING: ";
        break;
    case TRACE_LEVEL_NORMAL:
        extra_msg = "NORMAL: ";
        break;
    case TRACE_LEVEL_DEBUG:
        extra_msg = "DEBUG: ";
        break;
    default:
        break;
  }
     
  if(eventTraceLevel <= traceLevel) {
    char buf[8192], out_buf[8229];
    char theDate[32];
    
    time_t theTime = time(NULL);
    //char *syslogMsg;

    va_start (va_ap, format);

    /* We have two paths - one if we're logging, one if we aren't
     *   Note that the no-log case is those systems which don't support it (WIN32),
     *                                those without the headers !defined(USE_SYSLOG)
     *                                those where it's parametrically off...
     */

    memset(buf, 0, sizeof(buf));
    strftime(theDate, 32, "%d/%b/%Y %H:%M:%S", localtime_r(&theTime, &result));

    vsnprintf(buf, sizeof(buf)-1, format, va_ap);


    while(buf[strlen(buf)-1] == '\n') buf[strlen(buf)-1] = '\0';

    // trace_mutex.lock();
    snprintf(out_buf, sizeof(out_buf), "%s [%s:%d] %s%s", theDate, file, line,
	     extra_msg, buf);

    if(log_file_path != NULL) {
      FILE *fd = fopen(log_file_path, "a");
      if(fd) {
      fprintf(fd, "%s\n", out_buf);
      fclose(fd);
          } else
      printf("%s\n", out_buf);
    } else
      printf("%s\n", out_buf);

    fflush(stdout);

    // trace_mutex.unlock();

    // syslogMsg = &out_buf[strlen(theDate)+1];
    // if(eventTraceLevel == 0 /* TRACE_ERROR */)
    //   syslog(LOG_ERR, "%s", syslogMsg);
    // else if(eventTraceLevel == 1 /* TRACE_WARNING */)
    //   syslog(LOG_WARNING, "%s", syslogMsg);

    va_end(va_ap);
  }
}
