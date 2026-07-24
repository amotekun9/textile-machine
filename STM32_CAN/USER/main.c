#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "can.h" 
#include "Timer.h"
#include <string.h>

uint8_t KeyValue;

uint8_t TimingFlag;
uint8_t TriggerFlag;
uint8_t RequestFlag;

CanRxMsg RxMsg;

CanTxMsg TxMsg_Timing = {
	.StdId = 0x100,
	.ExtId = 0x00000000,
	.IDE   = CAN_Id_Standard,
	.RTR   = CAN_RTR_DATA,
	.DLC	 = 4,
	.Data  = {11,22,33,44}
};

CanTxMsg TxMsg_Trigger = {
	.StdId = 0x200,
	.ExtId = 0x00000000,
	.IDE   = CAN_Id_Standard,
	.RTR   = CAN_RTR_DATA,
	.DLC	 = 4,
	.Data  = {111,166,222,254}
};

CanTxMsg TxMsg_Ruqestger = {
	.StdId = 0x300,
	.ExtId = 0x00000000,
	.IDE   = CAN_Id_Standard,
	.RTR   = CAN_RTR_DATA,
	.DLC	 = 4,
	.Data  = {156,214,55,123}
};

int main(void)
{	 

	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	LCD_Init();			   	//��ʼ��LCD	
	KEY_Init();				//������ʼ��		 	
  MyCAN_init();			//哈哈哈，你好哈哈哈哈�?������������
//	Timer_Init(); 
	
	LCD_ShowString(60,60,200,24,24,"Tx:");	
	LCD_ShowString(60,90,200,24,24,"Tim:");	
	LCD_ShowString(60,120,200,24,24,"Tri:");		

	LCD_ShowString(60,300,200,24,24,"Rx:");	
	LCD_ShowString(60,330,200,24,24,"ID:");	
	LCD_ShowString(60,360,200,24,24,"DLC:");	
	LCD_ShowString(60,390,200,24,24,"Data[0]:");	
	LCD_ShowString(60,420,200,24,24,"Data[1]:");	
	LCD_ShowString(60,450,200,24,24,"Data[2]:");	
	LCD_ShowString(60,480,200,24,24,"Data[3]:");	
	LCD_ShowString(60,510,200,24,24,"Data[4]:");	
	LCD_ShowString(60,540,200,24,24,"Data[5]:");	
	LCD_ShowString(60,570,200,24,24,"Data[6]:");	
	LCD_ShowString(60,600,200,24,24,"Data[7]:");	
	
 	while(1)
	{
		/*		��ʱ����		 */
//		if (TimingFlag == 1)
//		{
//				TimingFlag = 0;
//				TxMsg_Timing.Data[0]++;
//				TxMsg_Timing.Data[1]++;
//				TxMsg_Timing.Data[2]++;
//				TxMsg_Timing.Data[3]++;
//				
//				MyCAN_Transmit(&TxMsg_Timing);
//				LCD_ShowNum(150,90,TxMsg_Timing.Data[0],3,24);
//				LCD_ShowNum(200,90,TxMsg_Timing.Data[1],3,24);
//				LCD_ShowNum(250,90,TxMsg_Timing.Data[2],3,24);
//				LCD_ShowNum(300,90,TxMsg_Timing.Data[3],3,24);
//		}
		
		/*		��������		*/
//		KeyValue = KEY_Scan(0);
//		if (KeyValue == 1)
//		{
//				TriggerFlag = 1;
//		}
//		
//		if (TriggerFlag == 1)
//		{
//				TriggerFlag = 0;
//				TxMsg_Trigger.Data[0]++;
//				TxMsg_Trigger.Data[1]++;
//				TxMsg_Trigger.Data[2]++;
//				TxMsg_Trigger.Data[3]++;
//				
//				MyCAN_Transmit(&TxMsg_Trigger);
//				LCD_ShowNum(150,120,TxMsg_Trigger.Data[0],3,24);
//				LCD_ShowNum(200,120,TxMsg_Trigger.Data[1],3,24);
//				LCD_ShowNum(250,120,TxMsg_Trigger.Data[2],3,24);
//				LCD_ShowNum(300,120,TxMsg_Trigger.Data[3],3,24);
//		}
		
		if (MyCAN_ReceiveFlag())
		{
				MyCAN_Receive(&RxMsg);
			
			//��ʾ���ݳ���
			LCD_ShowHexNum(200,360,RxMsg.DLC,3,24);
			
			if (RxMsg.IDE == CAN_Id_Standard)
			{
				LCD_ShowHexNum(200,330,RxMsg.StdId,4,24);
			}
			
			else if (RxMsg.IDE == CAN_Id_Extended)
			{
				LCD_ShowHexNum(200,330,RxMsg.ExtId,9,24);
			}
			
			if (RxMsg.RTR == CAN_RTR_Data)
			{
				LCD_ShowHexNum(200,390,RxMsg.Data[0],3,24);
				LCD_ShowHexNum(200,420,RxMsg.Data[1],3,24);
				LCD_ShowHexNum(200,450,RxMsg.Data[2],3,24);
				LCD_ShowHexNum(200,480,RxMsg.Data[3],3,24);
				LCD_ShowHexNum(200,510,RxMsg.Data[4],3,24);
				LCD_ShowHexNum(200,540,RxMsg.Data[5],3,24);
				LCD_ShowHexNum(200,570,RxMsg.Data[6],3,24);
				LCD_ShowHexNum(200,600,RxMsg.Data[7],3,24);
			}
			
			else if (RxMsg.RTR == CAN_RTR_Remote)
			{
				LCD_ShowHexNum(200,390,0,3,24);
				LCD_ShowHexNum(200,420,0,3,24);
				LCD_ShowHexNum(200,450,0,3,24);
				LCD_ShowHexNum(200,480,0,3,24);
				LCD_ShowHexNum(200,510,0,3,24);
				LCD_ShowHexNum(200,540,0,3,24);
				LCD_ShowHexNum(200,570,0,3,24);
				LCD_ShowHexNum(200,600,0,3,24);
			}
		}
	}
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		TimingFlag = 1;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

