inline void Anal_CH1_Set()
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);
}
inline void Anal_CH2_Set()
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);
}
inline void Anal_CH3_Set()
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);
}
inline void Anal_CH4_Set()
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
}
inline void Anal_CH5_Set()
{
	HAL_GPIO_WritePin(GPIOI,GPIO_PIN_15,GPIO_PIN_SET);
}

inline void Anal_CH1_Reset()
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);
}
inline void Anal_CH2_Reset()
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET);
}
inline void Anal_CH3_Reset()
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);
}
inline void Anal_CH4_Reset()
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
}
inline void Anal_CH5_Reset()
{
	HAL_GPIO_WritePin(GPIOI,GPIO_PIN_15,GPIO_PIN_RESET);
}