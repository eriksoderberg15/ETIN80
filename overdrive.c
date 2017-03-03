#include "framework.h"
#include "ETIN80_group_7_h.h" 

   void overdrive_function(sample_t* input)
   {
   float overdrive_out_sample;
   float temp_drive;
   int n;
   for (n=0; n<DSP_BLOCK_SIZE; n++) {
    	
    	
   temp_drive = (float)input[n].right;//*0.01;
    		
    		if(temp_drive > two_thirds) overdrive_out_sample = 1;
    		else if(temp_drive <= two_thirds && temp_drive > one_third) overdrive_out_sample = one_third*(3-(2-3*temp_drive)*(2-3*temp_drive));	
    		
    		else if(temp_drive <= one_third && temp_drive > 0) overdrive_out_sample = 2*temp_drive;//[-0.0001,0,0001]
    		
    	//	else if(temp_drive <= 0.00007 && temp_drive > -0.00007) overdrive_out_sample = 0;
    		
    		else if(temp_drive >= -one_third && temp_drive <= 0) overdrive_out_sample = -2*temp_drive;
    		
    		else if(temp_drive >= -two_thirds && temp_drive < -one_third) overdrive_out_sample = one_third*((2+3*temp_drive)*(2+3*temp_drive) - 3);
    		
    		else overdrive_out_sample = -1;
   
   /*float q,p,z,y, max;	
    	max = maxi(input);
    	if(max!=0) {
    		
    	    	q =(float)input.right * gain/max;
    	   		p = sign(-q);
    	   	   	z = p*(1-1*exp(p*q));// 1-1*exp
    	   	   	y = (z*max)/(sign(z)*z);
    	   	   	overdrive_out_sample = (y*max)/(sign(y)*y);
    		
    	}
    
   
    
    		//printf("%f\n", (float)input.right);
            //if ((float)input.right<=0.0035 && (float)input.right>= -0.0035) temp_drive = 0;
          //  else  temp_drive = (float)input.right * 100;
    		
    		temp_drive = (float)input[n].right*0.01;
    		
    		if(temp_drive > 0.6666666666) overdrive_out_sample = 1;
    		else if(temp_drive <= 0.6666666666 && temp_drive > 0.3333333333) overdrive_out_sample = 0.3333333333*(3-(2-3*temp_drive)*(2-3*temp_drive));	
    		
    		else if(temp_drive <= 0.3333333333 && temp_drive > 0) overdrive_out_sample = 2*temp_drive;//[-0.0001,0,0001]
    		
    	//	else if(temp_drive <= 0.00007 && temp_drive > -0.00007) overdrive_out_sample = 0;
    		
    		else if(temp_drive >= -0.3333333333 && temp_drive <= 0) overdrive_out_sample = -2*temp_drive;
    		
    		else if(temp_drive >= -0.6666666666 && temp_drive < -0.3333333333) overdrive_out_sample = 0.3333333333*((2+3*temp_drive)*(2+3*temp_drive) - 3);
    		
    		else overdrive_out_sample = -1;
    	*/	
    		/*
    		
    		if(temp_drive > 0.888) overdrive_out_sample = 1;
    		else if(temp_drive <= 0.888 && temp_drive > 0.666) overdrive_out_sample = 0.333*(3-(2-3*temp_drive)*(2-3*temp_drive));	
    		
    		else if(temp_drive <= 0.666 && temp_drive > 0.6) overdrive_out_sample = 2*temp_drive;
    		
    		else if(temp_drive <= 0.6 && temp_drive > -0.6) overdrive_out_sample = 0;
    		
    		else if(temp_drive >= -0.666 && temp_drive <= -0.6) overdrive_out_sample = -2*temp_drive;
    		
    		else if(temp_drive >= -0.888 && temp_drive < -0.666) overdrive_out_sample = 0.333*((2+3*temp_drive)*(2+3*temp_drive) - 3);
    		
    		else overdrive_out_sample = -1;
    		
    	*/	
    		overdrive_out[n] = overdrive_out_sample;//*100;//100    	
    	
   }
    
      
    }
   