// Fill out your copyright notice in the Description page of Project Settings.
// 管理连接
#pragma once

#include "CoreMinimal.h"
#include "SerialClass.h"
DECLARE_LOG_CATEGORY_EXTERN(CSerialClass, Log, All);
/**
 * 
 */
class DREAMTRAVEL_API CPacketManage
{
public:
	CPacketManage();
	~CPacketManage();

    //是否已连接
    BOOL IsConnected();

    //连接
    BOOL Connect(float DeltaTime);

    //读取player最新姿态
    int updatePlayerPose(FQuat * PlayerBonePoses, int BoneNums);

private:
    CSerialClass CSerialClass;


    float ConnectWaitedTime;      //  已等待的连接时间
    const float ExpectConnectWaitTime;     // 需要等待的连接时间

    const int PacketSize =  26;     // 一个packet包字节数， mpu数 * 一个mpu数据大小 + 2 ； 3*8+2
    const int PacketBuffSize = 1024;
    unsigned char PacketBuff[PacketBuffSize];     // packet缓存区
    unsigned char LastUncompletePacket[PacketSize];     // 上一次不完整的packet
    const char END_CODE = 44;
    const char START_CODE = 88;
};
