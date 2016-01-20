void Analyzer_Init(void);

#ifdef debug_mode
	// Set
	#define Anal_CH1_Set();	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET);
	#define Anal_CH2_Set();	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
	#define Anal_CH3_Set();	HAL_GPIO_WritePin(GPIOI,GPIO_PIN_1,GPIO_PIN_SET);
	#define Anal_CH4_Set();	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);
	#define Anal_CH5_Set();	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);
	// Reset
	#define Anal_CH1_Reset();	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);
	#define Anal_CH2_Reset();	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
	#define Anal_CH3_Reset();	HAL_GPIO_WritePin(GPIOI,GPIO_PIN_1,GPIO_PIN_RESET);
	#define Anal_CH4_Reset();	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);
	#define Anal_CH5_Reset();	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET);
#else
	// Set
	#define Anal_CH1_Set();
	#define Anal_CH2_Set();
	#define Anal_CH3_Set();
	#define Anal_CH4_Set();
	#define Anal_CH5_Set();
	// Reset
	#define Anal_CH1_Reset();
	#define Anal_CH2_Reset();
	#define Anal_CH3_Reset();
	#define Anal_CH4_Reset();
	#define Anal_CH5_Reset();
#endif
