#include "power.h"
#include "../../Core/Inc/adc.h"

/***��ص����ɼ��ļ�***/

#define ADC_VAL_SIZE 10	 //�ɼ��˲�����
#define VOLTAGE_DIV_RATIO	20	//��ѹ�������


static uint16_t MedianFilter(uint16_t* data, uint8_t size);



static uint16_t power_adc_val[ADC_VAL_SIZE];
static float power_change;

float battery_voltage_get()//��ȡ��ص�ѹ����
{
	
	power_change = (float)VOLTAGE_DIV_RATIO*MedianFilter(power_adc_val,ADC_VAL_SIZE) * 3.3/4096; 

	return power_change;
}

//��ֵ�˲�����
static uint16_t MedianFilter(uint16_t* data, uint8_t size)
{
    // �򵥵�ð������
    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - i - 1; ++j) {
            if (data[j] > data[j + 1]) {
                // ��������
                uint16_t temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }
    // �����м�ֵ
    return data[size / 2];
}



void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)//�ص�����
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