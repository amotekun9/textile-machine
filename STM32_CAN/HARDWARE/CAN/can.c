#include "can.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//CAN驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/7
//版本：V1.1 
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved	
//********************************************************************************
//V1.1修改说明 20150528
//修正了CAN初始化函数的相关注释，更正了波特率计算公式
////////////////////////////////////////////////////////////////////////////////// 	 
 
//CAN初始化
//tsjw:重新同步跳跃时间单元.范围:CAN_SJW_1tq~ CAN_SJW_4tq
//tbs2:时间段2的时间单元.   范围:CAN_BS2_1tq~CAN_BS2_8tq;
//tbs1:时间段1的时间单元.   范围:CAN_BS1_1tq ~CAN_BS1_16tq
//brp :波特率分频器.范围:1~1024;  tq=(brp)*tpclk1
//波特率=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
//mode:CAN_Mode_Normal,普通模式;CAN_Mode_LoopBack,回环模式;
//Fpclk1的时钟在初始化的时候设置为36M,如果设置CAN_Mode_Init(CAN_SJW_2tq,CAN_BS1_2tq,4,CAN_BS2_3tq,CAN_Mode_LoopBack);
//则波特率为:36M /6 * （1+2+3） ） =  1MHZ
//返回值:0,初始化OK;
//    其他,初始化失败; 

void MyCAN_init(void)
{
	//第一步：打开相关外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
	
	//第二步：配置相关GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;		//can_rx
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin		=	GPIO_Pin_11;	//can_tx
	GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//第三步：can外设的初始化
	CAN_InitTypeDef CAN_InitStructure;
	
	//CAN_Init之后，CAN外设是正常模式，CAN进入正常模式后就自动开始工作了。那这下面又来初始化过滤器，会不会出现问题？其实不会有问题。
	CAN_InitStructure.CAN_Mode			=		CAN_Mode_Normal;
	CAN_InitStructure.CAN_Prescaler	=		6;						//	CAN速率：36M /6 * （1+2+3） ） =  1MHZ		；高速CAN速率范围：125K--1M
	CAN_InitStructure.CAN_BS1				=		CAN_BS1_2tq;
	CAN_InitStructure.CAN_BS2				=		CAN_BS1_3tq;
	CAN_InitStructure.CAN_SJW				=		CAN_SJW_2tq;
	CAN_InitStructure.CAN_NART			=		DISABLE;				//不开启自动重传功能
	CAN_InitStructure.CAN_TXFP			=		DISABLE;				//ENABLE：先请求先发送；		DISABLE：ID号小的先发送
	CAN_InitStructure.CAN_RFLM			=		DISABLE;				//ENABLE：FIFO溢出时，新报文丢弃；	DISABLE：FIFO溢出时，最后收到的报文被新报文覆盖
	CAN_InitStructure.CAN_AWUM			=		DISABLE;				//自动唤醒模式；ENABLE;自动唤醒						DISABLE:手动唤醒
	CAN_InitStructure.CAN_TTCM			=		DISABLE;				//时间触发通信模式；ENABLE：开启；		DISABLE：关闭
	CAN_InitStructure.CAN_ABOM			=		DISABLE;				//离线自动恢复；ENABLE：开启；		DISABLE：关闭
	CAN_Init(CAN1,&CAN_InitStructure);		
	
	//第四步：配置过滤器，如果是要配置成全通模式的话，Id和Mask参数全配置为0即可，前提是得选用32位屏蔽模式
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	CAN_FilterInitStructure.CAN_FilterNumber					=		0;											//指定将要初始化的滤波器，范围为0到13。
	CAN_FilterInitStructure.CAN_FilterIdHigh					=		0;											//指定过滤器识别编号（32位配置为高位字节，16位配置为第一个字节）。该参数的取值范围为 0x0000 至 0xFFFF。
	CAN_FilterInitStructure.CAN_FilterIdLow						=		0;											//指定过滤器识别编号（32位配置为低字节，16位配置为第二个）。该参数的值范围为 0x0000 至 0xFFFF。
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh			=		0;											//根据模式指定滤波掩码编号或识别编号（32位配置为MSB，16位配置为第一个）。该参数的取值范围为0x0000至0xFFFF。
	CAN_FilterInitStructure.CAN_FilterMaskIdLow				=		0;											//根据模式指定滤波掩码编号或识别编号（32位配置为LSB，16位配置为第二个）。该参数的取值范围为0x0000至0xFFFF。
	CAN_FilterInitStructure.CAN_FilterScale						=		CAN_FilterScale_32bit;	//指定过滤器的尺度
	CAN_FilterInitStructure.CAN_FilterMode						=		CAN_FilterMode_IdMask;	//指定要初始化的过滤模式
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment	=		CAN_Filter_FIFO0;				//指定将分配给过滤器的FIFO（0或1）。
	CAN_FilterInitStructure.CAN_FilterActivation			=		ENABLE;								//启用或禁用过滤器。
	
	CAN_FilterInit(&CAN_FilterInitStructure);			//因为CAN1和CAN2是共用一套的，所需不需要指定是CAN1还是CAN2
	
}

uint32_t Timeout = 0;				//超时等待，防止一直发送不成功程序卡在这儿
void MyCAN_Transmit(CanTxMsg *TxMessage)
{
	uint8_t TransmitMailbox = CAN_Transmit(CAN1, TxMessage);
	
//	uint32_t Timeout = 0;
	while (CAN_TransmitStatus(CAN1, TransmitMailbox) != CAN_TxStatus_Ok)
	{
		Timeout ++;
		if (Timeout > 100000)
		{
			break;
		}
	}
}

//判断FIIO里是否有报文
uint8_t MyCAN_ReceiveFlag(void)
{
	if(CAN_MessagePending(CAN1,CAN_FIFO0) > 0)	//CAN_MessagePending函数返回待处理消息的数量
	{
			return 1;	//表示接收队伍有报文
	};	
	return 0;		//表示接收队伍无报文
}

void MyCAN_Receive(CanRxMsg *RxMessage)
{
	CAN_Receive(CAN1, CAN_FIFO0, RxMessage);
}











