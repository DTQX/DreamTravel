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
    int ReadyBytesSize =  CSerialClass.GetReadySize();
    // 如果数据过多，超出缓存区，则考虑是开始时多余的数据，读取到最后一个buff。（只可能在最开始时发生，这些数据不做处理，直接清空）
    while(ReadyBytesSize / PacketBuffSize) {
        CSerialClass.ReadData(PacketBuff, PacketBuffSize);
        ReadyBytesSize -= PacketBuffSize;
    }
    // 如果剩下的bytes数大于PacketSize则读取，否则不读取
    if(ReadyBytesSize > PacketSize){
        int ReadBytes = CSerialClass.ReadData(PacketBuff, ReadyBytesSize);
        
        if(ReadBytes != ReadyBytesSize){
            UE_LOG(LogTemp, Warning, TEXT("数据读取失败！"));
            return 0;
        }

        int LastEndCodeIndex = 0;
        for(int i = ReadyBytesSize-1; i >= 0 ; i--)
        {
            if (PacketBuff[i] == END_CODE) {
                LastEndCodeIndex = i;
            }
            
        }
        
    }
    
}