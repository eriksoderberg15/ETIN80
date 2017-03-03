#include "framework.h"
#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include "ETIN80_group_7_h.h" 
#include <processor_include.h>

void echo_function() {
	    delayPosition = circindex(k,-delay_in_samples,bufferSize);
		int n;
    	for(n=0; n<DSP_BLOCK_SIZE; n++) {
    		//delayPosition = k - delay_in_samples;
    		//if(delayPosition < 0) {
    		//	delayPosition = bufferSize + delayPosition;
    		//}
        	echo_out[n] = overdrive_out[n] + attenuation*audio_buffer[delayPosition+n]; //Mono input(?)!!! SOLVE MODULO??
        	audio_buffer[k] = echo_out[n]; //size of audio_buffer is 16k atm.
        	k = circindex(k,1,bufferSize);
    	}
}



