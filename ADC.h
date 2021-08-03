/* 
 * File:   ADC.h
 * Author: Lin
 *
 * Created on 2021?8?2?, ??11:30
 */

#ifndef ADC_H
#define	ADC_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    void init_ADC();

    void start_ADC();

    void stop_ADC();

    int read_ADC_val();

#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */

