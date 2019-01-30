// Fill out your copyright notice in the Description page of Project Settings.

#include "PacketManage.h"

DEFINE_LOG_CATEGORY(CSerialClass);

CPacketManage::CPacketManage()
{
    CSerialClass = CSerialClass();
    IsConnected = false;
    ConnectWaitedTime = 0.0f;
}

CPacketManage::~CPacketManage()
{

}

BOOL CPacketManage::IsConnected(){
    return CSerialClass.IsOpened();
}

BOOL CPacketManage::Connect(float DeltaTime){
    ConnectWaitedTime += DeltaTime;
    if(ConnectWaitedTime > ExpectConnectWaitTime){
        Bool result = CSerialClass.Open(4, 115200);
        if( result == false){
            UE_LOG(LogTemp, Warning, TEXT("连接失败，正在等待重连。。。"));
        }else{
            UE_LOG(LogTemp, Warning, TEXT("连接成功！"));
        }
        ConnectWaitedTime = 0.0f;
        return result;
    }
    return false;
}

int CPacketManage::updatePlayerPose(FQuat * PlayerBonePoses, int BoneNums){
    for(int i = 0; i < BoneNums; i++){
        
    }
}