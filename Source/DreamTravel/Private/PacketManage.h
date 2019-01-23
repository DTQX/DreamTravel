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

private:
    CSerialClass CSerialClass;


    float ConnectWaitedTime;      //  已等待的连接时间
    const float ExpectConnectWaitTime;     // 需要等待的连接时间
};
