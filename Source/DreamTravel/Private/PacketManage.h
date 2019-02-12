// Fill out your copyright notice in the Description page of Project Settings.
// 管理连接
#pragma once

#include "CoreMinimal.h"
#include "SerialClass.h"
DECLARE_LOG_CATEGORY_EXTERN(CSerialClass, Log, All);

typedef short int16_t;
typedef unsigned char uint8_t;

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
    int UpdatePlayerPose(FQuat * PlayerBonePoses, int BoneNums);

private:
    int ReadLastPacket();   // 读取数据包
    int Packet2Quat(FQuat * PlayerBonePoses, int BoneNums);  // 数据包转换成Quat
    uint8_t dmpGetQuaternion(int16_t *data, const uint8_t* packet);
    uint8_t dmpGetQuaternion(Quaternion *q, const uint8_t* packet)

    CSerialClass CSerialClass;

    float ConnectWaitedTime;      //  已等待的连接时间
    const float ExpectConnectWaitTime;     // 需要等待的连接时间

    const int PacketSize =  26;     // 一个packet包字节数， mpu数 * 一个mpu数据大小 + 2 ； 3*8+2
    const int PacketBuffSize = 1024;
    unsigned char PacketBuff[PacketBuffSize];     // packet缓存区
    unsigned char LastIncompletePacket[PacketSize];     // 上一次不完整的packet
    const char END_CODE = 44;
    const char START_CODE = 88;
};
