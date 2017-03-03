

//Variables
extern int k;
static int bufferSize = 14400;
extern float audio_buffer[14400];//static float audio_buffer[14400];  //If larger tham 8k, the registers overflow!
static bool AGC = 0;
static bool echo = 0;
static bool overdrive = 0;
static float delay_in_seconds = 0.3;	//Doesnt seem to be 100% correct? Hardware?
extern int delay_in_samples;
static float attenuation = 0.3;
extern float echo_out[DSP_BLOCK_SIZE];	//32 float values
//static float echo_overdrive_out[DSP_BLOCK_SIZE];
//static float dist_out[DSP_BLOCK_SIZE]; //for dist
extern float overdrive_out[DSP_BLOCK_SIZE];
static float previous_avg_power = 0;
static float alpha = 0.9;
extern int delayPosition; //Starting position of the echo at 0.3s (delayPosition = bufferSize - delay_in_samples - 1)
extern float one_third ;
extern float two_thirds ;
 
//static int timer_test = 0;	//False as standard timer set it to true when OK.

//Functions
float gain_ctrl(float);
void overdrive_function(sample_t*);
void echo_function(void);


