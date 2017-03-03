#include "framework.h"
#include "ETIN80_group_7_h.h"
#include "stdio.h"
#include "stdlib.h"
#include <string.h>

void abc()
{
  sample_t* input = dsp_get_audio_u31();
  int currentPosition;
  int delayPosition;
  int n;
  if(echo) {
  
    		for(n=0; n<DSP_BLOCK_SIZE; n++) {
    			currentPosition = k*32 + n;
    			delayPosition = currentPosition - delay_in_samples;
    			if(delayPosition < 0) {
    				delayPosition = bufferSize + delayPosition;
    			}
        		echo_out[n] = (float)input[n].right + attenuation*audio_buffer[delayPosition]; //Mono input(?)!!! SOLVE MODULO??
        		audio_buffer[k*32 + n] = echo_out[n]; //size of audio_buffer is 16k atm.
   			}
   			
   			k++;
   			
   			if(k==(bufferSize-32)/32) {
   				k=0;
   			}
    	}
   		//k = k%(bufferSize/32);
    
    
}


