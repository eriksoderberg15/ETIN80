/*****************************************************************************
 * ETIN80_group_7.c
 *****************************************************************************/
#include <processor_include.h>
#include <signal.h>
#include <string.h>
#include <filter.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "framework.h"
#include "ETIN80_group_7_h.h"
float audio_buffer[14400];
float echo_out[DSP_BLOCK_SIZE];
float overdrive_out[DSP_BLOCK_SIZE];
int delayPosition = 0;
int k = 0;
int delay_in_samples = 0;
float one_third = (1/3.0);
float two_thirds = (2/3.0);
//#include "framework.c"

void process(int sig)
{
    int n;
    float average_power_of_block = 0;
    float smooth_avg_power = 0;
    float dB_smooth_avg_power = 0;
    float AGC_result = 0;
    float power_of_block = 0;
    int choice = overdrive + 2*echo + 4*AGC;
    float echo_result;
   	//int currentPosition;
    //int delayPosition;
    //sample_t* u30 = dsp_get_audio_u30();    /* line 2 in without mic bias, no out */
    //sample_t* u31 = dsp_get_audio_u31();    /* line 1 in with mic bias, no out */
    sample_t* u32 = dsp_get_audio_u32(); 	  /* mic 1 and 2 in, headset out */
    //sample_t* u33 = dsp_get_audio_u33();    /* mic 3 and 4 in, no out */
	
   		
    sample_t* input = dsp_get_audio_u30();	//Read audio input without BIAS (Phantom power)
    
    
    
    //-------------- Start processsing ---------------------------
   	
    
    
    //Overdrive or distortion
    
    //WE SHOULD PUT AN AGC BEFORE THE OVERDRIVE TO REMOVE THE BACKGROUND NOISE.
    //THIS WILL PROBABLY CLEAN UP THE SIGNAL SIGNIFICANTLY
    
    if(overdrive)  {
    	overdrive_function(input);
    	
    /*	for (n=0; n<DSP_BLOCK_SIZE; n++) {
    	//	printf("%f\n", (float)input[n].right);
    	overdrive_out[n] = overdrive_function(input[n]);//10 is added
    	}
    	*/
    }else {
    	for (n=0; n<DSP_BLOCK_SIZE; n++) {
    	overdrive_out[n] = (float)input[n].right;//10 is added
    	}
    }
    	   
   	//Echo
   	
   	if(echo) {
   		//delayPosition = circindex(k,-delay_in_samples,bufferSize);
   		echo_function();
   		
   		
   		/*delayPosition = circindex(k,-delay_in_samples,bufferSize);
    	for(n=0; n<DSP_BLOCK_SIZE; n++) {
    		//delayPosition = k - delay_in_samples;
    		//if(delayPosition < 0) {
    		//	delayPosition = bufferSize + delayPosition;
    		//}
        	echo_out[n] = overdrive_out[n] + attenuation*audio_buffer[delayPosition+n]; //Mono input(?)!!! SOLVE MODULO??
        	audio_buffer[k] = echo_out[n]; //size of audio_buffer is 16k atm.
        	k = circindex(k,1,bufferSize);
    	}
    	*/		
   	}else{
    	for(n=0; n<DSP_BLOCK_SIZE; n++) {
       		echo_out[n] = overdrive_out[n]; //Mono input(?)!!! SOLVE MODULO??
   		}
    }
   
 /*  if(echo) { 
    	
    
    	for(n=0; n<DSP_BLOCK_SIZE; n++) {
    		echo_out[n] = echo_function((float)input[n].right, n, audio_buffer);
    		audio_buffer[k*32 + n] = echo_out[n];
        	//audio_buffer[k*32 + n] = echo_function((float)input[n].right, n);//size of audio_buffer is 16k atm
        	//echo_out[n] = audio_buffer[k*32 + n];
   		}
   		k++;
   		if(k==(bufferSize-32)/32) {
   			k=0;
   		}
    }
    */
    
    	//AGC
    if(AGC) {
    	for(n=0; n<DSP_BLOCK_SIZE; n++) { 
    		power_of_block = power_of_block + (echo_out[n] * echo_out[n]);
    	}
    	//AGC operations
    	average_power_of_block = power_of_block/DSP_BLOCK_SIZE;
    	smooth_avg_power = alpha * previous_avg_power + (1-alpha)*average_power_of_block;
    	previous_avg_power = smooth_avg_power;	//remebering current power for next block to use
    	if (smooth_avg_power==0) dB_smooth_avg_power = -100;
    	else dB_smooth_avg_power = 10*log10(smooth_avg_power);    	
    	AGC_result = gain_ctrl(dB_smooth_avg_power);
    	
    }else{
    	AGC_result = 1;
    }
      
    
    //------ Output ----------
    
    ///DOES IT NOT SUPPORT SWITCH_STATEMENTS??
    
    for(n=0; n<DSP_BLOCK_SIZE; n++) {
    	u32[n].right =  echo_out[n] * AGC_result;
    	u32[n].left = echo_out[n] * AGC_result;
    }  
}

/*
Function for the distortion operations. 
Returns 1 if input larger than 0.
Returns 0 if input equals 0.
Returns -1 if input less than 0.

float sign(float f) {
	if(f > 0) return 1;
	else if (f == 0) return 0;
	else return -1;
}


Function for the distortion operations. 
Finds the maximum value in the array

float maxi(sample_t* in) 
{
	int i = 0;
	float largest = 0;
	for(i=0; i<DSP_BLOCK_SIZE;i++) {
		float temp = sign((float)in[i].right)*(float)in[i].right;
		if(temp > largest) {
			largest = temp;
		}
	}
	return largest;
}
*/

/*
Function for the distortion operations. 
Describes the curve of the gain-controller. Returns the cut off amplitude value.
*/
float gain_ctrl(float p_in)
{	
	float beta = 0;
	float temp = 0;
	
	if(p_in < -50) return beta;
	/*
	else if (p_in >= -70 && (p_in < -40)) 
	{
		//temp = 3*p_in + 120; //for p_in >= 60
 		temp = 2*p_in + 80;
		temp = temp/10;
		beta = pow(10, temp);
		return beta;*/
	 else if((p_in >= -50) && (p_in < -20)) 
	{
		beta = 1;
		return beta;
	} else 
	{
		temp = -p_in - 20;
		temp = temp/10;
		beta = pow(10, temp);
		return beta;
	}
	
		
}


static void keyboard(int sig)
{
	timer_on();
	
	
	//REMEMBER TO CHANGE bufferSize WHEN DELAY CHANGES!!
    unsigned int keys = dsp_get_keys();	//Creates some unique number based on   
    unsigned int leds = 0;
	
    //printf("%d", keys);
    
    
    
    if(keys & DSP_SW1) {
    	//delay();
        leds = 0;
        if(echo == 1) {
        	echo = !echo;
        	delay_in_seconds = 0.3; //So that you know that echo is on when you start it.
        							//Even if exited with delay_in seconds = 0.
        	delay_in_samples = delay_in_seconds * DSP_SAMPLE_RATE;
        }
        else echo = !echo;
    } else if(keys & DSP_SW2) {
        //delay();
    	leds = DSP_D1;
    	AGC = !AGC;
    } else if(keys & DSP_SW3) {
        //delay();
    	leds = DSP_D2;
    	overdrive = !overdrive;
    } else if(keys & DSP_SW4) {
        if (delay_in_seconds == 0.3) {
        	delay_in_seconds = 0.1;
        	delay_in_samples = delay_in_seconds * DSP_SAMPLE_RATE;
        }else if (delay_in_seconds == 0.1) {
        	delay_in_seconds = 0.2;
        	delay_in_samples = delay_in_seconds * DSP_SAMPLE_RATE;
        }else {
        	delay_in_seconds = 0.3;
        	delay_in_samples = delay_in_seconds * DSP_SAMPLE_RATE;
        }
    	leds = DSP_D1|DSP_D2;
    }
    
    dsp_set_leds(leds);
    
    
}

static void timer(int sig)
{
    timer_off();
    
    
    timer_set(DSP_FREQUENCY/10, DSP_FREQUENCY/10);

}

void main()
{   

    /*
    Setup the DSP framework.
    */
    dsp_init();
    
    dsp_set_leds(DSP_D1 | DSP_D2);
    delay_in_samples = delay_in_seconds * DSP_SAMPLE_RATE;	
    
 	/*
    Register interrupt handlers:
    SIG_SP1: the audio callback
    SIG_USR0: the keyboard callback
    SIG_TMZ: the timer callback
    */
    interrupt(SIG_SP1, process);
    interrupt(SIG_USR0, keyboard);
    interrupt(SIG_TMZ, timer);
    timer_set(DSP_FREQUENCY/10, DSP_FREQUENCY/10);	//What are these?
    /* timer_on(); */	//Only for synchronous actions?

    /*
    
    Start the framework.
    */
    dsp_start();
    
    /*
    Everything is handled by the interrupt handlers, so just put an empty
    idle-loop here. If not, the program falls back to an equivalent idle-loop
    in the run-time library when main() returns.
    */
    for(;;) {
    
    	idle();
    }
}


