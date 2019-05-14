﻿// Fill out your copyright notice in the Description page of Project Settings.
// 管理连接
#pragma once

//#include "SerialClass.h"
#include "CoreMinimal.h"


DECLARE_LOG_CATEGORY_EXTERN(PacketManage, Log, All);

// 数据相关
#define PACKET_SIZE  (26)     // 一个packet包字节数， mpu数 * 一个mpu数据大小 + 2 ； 3*8+2
#define PURE_PACKET_SIZE (24)	// 一个packet包的内容字节数
#define PACKET_BUFF_SIZE (1024)		// 数据缓存区大小
#define UNIT_PACKET_SIZE (8)		// 一个mpu的数据大小

class FSerialClass;
/**
 * 
 */
class DREAMTRAVEL_API FPacketManage
{
public:
	FPacketManage();
	~FPacketManage();

    //是否已连接
    bool IsConnected();

    //连接
    bool Connect(float DeltaTime);

    //读取player最新姿态
    //int UpdatePlayerPose(FQuat * PlayerBonePoses[], int BoneNums);
    int UpdatePlayerPose(TArray<FQuat>* PlayerBonePoses, int BoneNums);

private:

	FSerialClass * SerialClass = nullptr;

	//FSerialClass * fSerialClass = nullptr;

  int ReadLastPacket();   // 读取数据包
  int ReadLastPacket_back();   // 读取数据包,旧版
  //int Packet2Quat(FQuat * PlayerBonePoses[], int BoneNums);  // 数据包转换成Quat
  int Packet2Quat(TArray<FQuat>* PlayerBonePoses, int BoneNums);  // 数据包转换成Quat
  uint8 dmpGetQuaternionL(int16 *data, const uint8* packet);   //packet 转int16
  uint8 dmpGetQuaternion(FQuat* q, const uint8* packet);   //  packet 转 quat

  float ConnectWaitedTime = 1.0f;      //  已等待的连接时间
  const float ExpectConnectWaitTime = 1.0f;     // 需要等待的连接时间

	// 数据包相关
	uint8 UnitPacket[UNIT_PACKET_SIZE] = {0};		// 一个mpu的数据
	uint8 PacketBuff[PACKET_BUFF_SIZE] = {0};     // packet缓存区
	uint8 LastIncompletePacket[PACKET_SIZE] = {0};     // 上一次不完整的packet
	const uint8 START_CODE_2 = 44;	// 数据包开始标志符2
	const uint8 START_CODE_1 = 88;	// 数据包开始标志符1
	bool IsLastReadPacketComplete = true;	// 记录上一次的读取到的数据包是否完整

	// 数据读取相关
	int ReadyBytesSize;
	int PacketBytesNeeded;
	int BytesNotNeed;
	int readPacketSize;

	// 端口相关
	const int COM_PORT = 4;		// 端口号
	const int COM_RATE = 115200;	// 串口速率

};
