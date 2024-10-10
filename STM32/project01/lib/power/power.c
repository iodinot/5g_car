#include "power.h"
#include "../../Core/Inc/adc.h"

/***电池电量采集文件***/

#define ADC_VAL_SIZE 10	 //采集滤波次数
#define VOLTAGE_DIV_RATIO	20	//分压电阻比率


static uint16_t MedianFilter(uint16_t* data, uint8_t size);



static uint16_t power_adc_val[ADC_VAL_SIZE];
static float power_change;

float battery_voltage_get()//获取电池电压函数
{
	
	power_change = (float)VOLTAGE_DIV_RATIO*MedianFilter(power_adc_val,ADC_VAL_SIZE) * 3.3/4096; 

	return power_change;
}

//中值滤波函数
static uint16_t MedianFilter(uint16_t* data, uint8_t size)
{
    // 简单的冒泡排序
    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - i - 1; ++j) {
            if (data[j] > data[j + 1]) {
                // 交换数据
                uint16_t temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }
    // 返回中间值
    return data[size / 2];
}



void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)//回调函数
{
	if(hadc==&hadc1)
	{
		static uint8_t size;		
		
		power_adc_val[size++] = HAL_ADC_GetValue(&hadc1);
		
		if(size>ADC_VAL_SIZE)
		{
			size = 0;
		}	
	}
}