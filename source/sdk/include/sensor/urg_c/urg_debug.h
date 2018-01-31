#ifndef URG_DEBUG_H
#define URG_DEBUG_H


#ifdef __cplusplus
extern "C" {
#endif

#include "urg_c/urg_sensor.h"


   
    extern int urg_raw_write(urg_t *urg, const char *data, int data_size);


  
    extern int urg_raw_read(urg_t *urg, char *data, int max_data_size,
                            int timeout);

   
    extern int urg_raw_readline(urg_t *urg,char *data, int max_data_size,
                                int timeout);

#ifdef __cplusplus
}
#endif

#endif
