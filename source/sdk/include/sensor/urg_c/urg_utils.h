#ifndef URG_UTILS_H
#define URG_UTILS_H



#ifdef __cplusplus
extern "C" {
#endif

#include "urg_c/urg_sensor.h"


    
    extern const char *urg_error(const urg_t *urg);


    
    extern void urg_distance_min_max(const urg_t *urg,
                                     long *min_distance, long *max_distance);


    
    extern void urg_step_min_max(const urg_t *urg, int *min_step, int *max_step);


   
    extern long urg_scan_usec(const urg_t *urg);


   
    extern int urg_max_data_size(const urg_t *urg);


   
    extern double urg_index2rad(const urg_t *urg, int index);


   
    extern double urg_index2deg(const urg_t *urg, int index);


    
    extern int urg_rad2index(const urg_t *urg, double radian);


    
    extern int urg_deg2index(const urg_t *urg, double degree);


   
    extern int urg_rad2step(const urg_t *urg, double radian);


    
    extern int urg_deg2step(const urg_t *urg, double degree);


   
    extern double urg_step2rad(const urg_t *urg, int step);


  
    extern double urg_step2deg(const urg_t *urg, int step);

    
    extern int urg_step2index(const urg_t *urg, int step);

#ifdef __cplusplus
}
#endif

#endif /* !URG_UTILS_H */
