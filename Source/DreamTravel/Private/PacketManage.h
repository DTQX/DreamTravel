// Fill out your copyright notice in the Description page of Project Settings.
// 管理连接
#pragma once

#include "CoreMinimal.h"
#include "SerialClass.h"

DECLARE_LOG_CATEGORY_EXTERN(CPacketManage, Log, All);

#define PACKET_SIZE  (26)     // 一个packet包字节数， mpu数 * 一个mpu数据大小 + 2 ； 3*8+2
#define PACKET_BUFF_SIZE (1024)
#define UNIT_PACKET_SIZE (8)

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
    uint8 dmpGetQuaternion(int16 *data, const uint8* packet);   //packet 转int16
	uint8 dmpGetQuaternion(FQuat q, const uint8* packet);   //  packet 转 quat

    CSerialClass CSerialClass;

    float ConnectWaitedTime;      //  已等待的连接时间
    const float ExpectConnectWaitTime = 1.0f;     // 需要等待的连接时间

	uint8 UnitPacket[UNIT_PACKET_SIZE];		// 一个mpu的数据
	uint8 PacketBuff[PACKET_BUFF_SIZE];     // packet缓存区
	uint8 LastIncompletePacket[PACKET_SIZE];     // 上一次不完整的packet
	const uint8 END_CODE = 44;
	const uint8 START_CODE = 88;
};
