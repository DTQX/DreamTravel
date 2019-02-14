// Fill out your copyright notice in the Description page of Project Settings.

#include "PacketManage.h"

//DEFINE_LOG_CATEGORY(CPacketManage);

CPacketManage::CPacketManage()
{
    //CSerialClass = CSerialClass();
    //ConnectWaitedTime = 0.0f;
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
        BOOL result = CSerialClass.Open(4, 115200);
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

int CPacketManage::UpdatePlayerPose(FQuat * PlayerBonePoses, int BoneNums){
    for(int i = 0; i < BoneNums; i++){
        
    }
    
    // 读取数据
    int ReadStatus = ReadLastPacket();
    if(ReadStatus != 1){
        return -1;
    }

    // 处理数据
    Packet2Quat(PlayerBonePoses, BoneNums);




    // // 如果剩下的bytes数大于PACKET_SIZE则读取，否则不读取
    // if(ReadyBytesSize > PACKET_SIZE){
    //     int ReadBytes = CSerialClass.ReadData(PacketBuff, ReadyBytesSize);
        
    //     if(ReadBytes != ReadyBytesSize){
    //         UE_LOG(LogTemp, Warning, TEXT("数据读取失败！"));
    //         return 0;
    //     }

    //     int LastEndCodeIndex = 0;
    //     for(int i = ReadyBytesSize-1; i >= 0 ; i--)
    //     {
    //         if (PacketBuff[i] == END_CODE) {
    //             LastEndCodeIndex = i;
    //         }
            
    //     }
        
    // }

	return 0;
    
}

int CPacketManage::ReadLastPacket(){
    int ReadyBytesSize =  CSerialClass.GetReadySize();
    // 将数据流定向到数据包开始，不包括START_CODE
    CSerialClass.ReadDataUtil(PacketBuff, START_CODE, ReadyBytesSize);
    ReadyBytesSize = CSerialClass.GetReadySize();
    
    int bytesNeeded = ReadyBytesSize % PACKET_SIZE + PACKET_SIZE;     // 需要读取的数据大小
    // 如果剩下数据少于需要的数据（即剩下的数据不足一个数据包的大小），则返回
    if(ReadyBytesSize < bytesNeeded){
        UE_LOG(LogTemp, Warning, TEXT("数据不够一个数据包！"));
        return -1;
    }
    int bytesNotNeed = ReadyBytesSize - bytesNeeded;       // 不需要的数据大小
    //只需要后面一个完整的数据包，前面多余的数据清除
    while(bytesNotNeed > 0) {
        // 如果数据过多，超出缓存区，则考虑是开始时多余的数据，读取到最后一个buff。（只可能在最开始时发生，这些数据不做处理，直接清空）
        if(bytesNotNeed > PACKET_BUFF_SIZE){
            bytesNotNeed -= CSerialClass.ReadData(PacketBuff, PACKET_BUFF_SIZE);
        }else{
            // 清除不需要的数据
            bytesNotNeed -= CSerialClass.ReadData(PacketBuff, bytesNotNeed);
        }
    }
   
    // 开始读取数据
    int readPacketSize = CSerialClass.ReadDataUtil(PacketBuff, END_CODE, bytesNeeded);
    // 如果读取的数据大小不等于期望的数据包大小，则数据出错，丢弃读取到的数据，不进行任何处理， 返回-1
    if(readPacketSize != PACKET_SIZE - 1){
        UE_LOG(LogTemp, Warning, TEXT("数据包出错！"));
        return -1;
    }

    return 1;
}

int CPacketManage::Packet2Quat(FQuat * PlayerBonePoses, int BoneNums){
    // dmpGetQuaternion(PlayerBonePoses, )
    for(int i = 0; i < BoneNums; i++){
		memcpy(UnitPacket, PacketBuff + i * UNIT_PACKET_SIZE * sizeof(uint8), UNIT_PACKET_SIZE * sizeof(uint8));
		dmpGetQuaternion(PlayerBonePoses[i], UnitPacket);
    }

	return 0;
}

uint8 CPacketManage::dmpGetQuaternion(int16 *data, const uint8* packet) {
    // TODO: accommodate different arrangements of sent data (ONLY default supported now)
    data[0] = ((packet[0] << 8) | packet[1]);
    data[1] = ((packet[2] << 8) | packet[3]);
    data[2] = ((packet[4] << 8) | packet[5]);
    data[3] = ((packet[6] << 8) | packet[7]);
    return 0;
}

uint8 CPacketManage::dmpGetQuaternion(FQuat q, const uint8* packet) {
    // TODO: accommodate different arrangements of sent data (ONLY default supported now)
    int16 qI[4];
    uint8 status = dmpGetQuaternion(qI, packet);
    if (status == 0) {
        q.W = (float)qI[0] / 16384.0f;
        q.X = (float)qI[1] / 16384.0f;
        q.Y = (float)qI[2] / 16384.0f;
        q.Z = (float)qI[3] / 16384.0f;
        return 0;
    }
    return status; // int16 return value, indicates error if this line is reached
}
