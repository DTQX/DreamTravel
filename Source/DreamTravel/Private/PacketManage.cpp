// Fill out your copyright notice in the Description page of Project Settings.

#include "PacketManage.h"
#include "SerialClass.h"


DEFINE_LOG_CATEGORY(PacketManage);

FPacketManage::FPacketManage()
{
    //FSerialClass = SerialClass();
    //ConnectWaitedTime = 0.0f;
	SerialClass =  new FSerialClass();
}

FPacketManage::~FPacketManage()
{
	delete SerialClass;
}

bool FPacketManage::IsConnected(){
    return SerialClass->IsOpened();
}

bool FPacketManage::Connect(float DeltaTime){
	// 如果已连接，则直接返回true
	if (IsConnected()) {
		return true;
	}

    ConnectWaitedTime += DeltaTime;
    if(ConnectWaitedTime > ExpectConnectWaitTime){
        bool result = SerialClass->Open(COM_PORT, 115200);
        if( result == false){
            UE_LOG(PacketManage, Warning, TEXT("连接失败，正在等待重连。。。"));
        }else{
            UE_LOG(PacketManage, Warning, TEXT("连接成功！"));
        }
        ConnectWaitedTime = 0.0f;
        return result;
    }
    return false;
}

int FPacketManage::UpdatePlayerPose(TArray<FQuat>* PlayerBonePoses, int BoneNums){
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
    //     int ReadBytes = SerialClass->ReadData(PacketBuff, ReadyBytesSize);
        
    //     if(ReadBytes != ReadyBytesSize){
    //         UE_LOG(PacketManage, Warning, TEXT("数据读取失败！"));
    //         return 0;
    //     }

    //     int LastEndCodeIndex = 0;
    //     for(int i = ReadyBytesSize-1; i >= 0 ; i--)
    //     {
    //         if (PacketBuff[i] == START_CODE_2) {
    //             LastEndCodeIndex = i;
    //         }
            
    //     }
        
    // }

	return 1;
    
}

int FPacketManage::ReadLastPacket_back() {
	ReadyBytesSize = SerialClass->GetReadySize();

	// 将数据流定向到数据包开始，不包括START_CODE_1, START_CODE_2
	if (IsLastReadPacketComplete) {
		// 如果上一次读取的数据包是完整的，那么上一次的数据就已经被读取了，我们这次可以重定位到START_CODE_1；
		// 否则，上一次的数据没被读取，我们要接着上一次的读，不能抛弃这些未读取的数据。
		SerialClass->ReadDataUtil(PacketBuff, START_CODE_1, START_CODE_2, ReadyBytesSize);
		ReadyBytesSize = SerialClass->GetReadySize();
	}

	// 如果剩下数据少于需要的数据（即剩下的数据不足PURE_PACKET_SIZE），则返回
	if (ReadyBytesSize < PURE_PACKET_SIZE) {
		UE_LOG(PacketManage, Warning, TEXT("数据不够一个数据包！ReadyBytesSize : %d, PURE_PACKET_SIZE : %d"), ReadyBytesSize, PURE_PACKET_SIZE);
		IsLastReadPacketComplete = false;
		return -1;
	}
	IsLastReadPacketComplete = true;

	if (ReadyBytesSize > PACKET_SIZE + PURE_PACKET_SIZE){
		if (ReadyBytesSize % PACKET_SIZE < PURE_PACKET_SIZE){
			BytesNeeded = ReadyBytesSize % PACKET_SIZE + PACKET_SIZE;
		}
		else {
			BytesNeeded = PURE_PACKET_SIZE;
		}

		BytesNotNeed = ReadyBytesSize - BytesNeeded;		// 不需要的数据大小
		//只需要后面一个完整的数据包，前面多余的数据清除，这里不用ReadDataUtil是为了优化性能
		while (BytesNotNeed > 0) {
			// 如果数据过多，超出缓存区，则考虑是开始时多余的数据，读取到最后一个buff。（只可能在最开始时发生，这些数据不做处理，直接清空）
			if (BytesNotNeed > PACKET_BUFF_SIZE) {
				BytesNotNeed -= SerialClass->ReadData(PacketBuff, PACKET_BUFF_SIZE);
			}
			else {
				// 清除不需要的数据
				BytesNotNeed -= SerialClass->ReadData(PacketBuff, BytesNotNeed);
			}
		}
	}

	

	// 开始读取数据
	readPacketSize = SerialClass->ReadData(PacketBuff, PURE_PACKET_SIZE);
	// 如果读取的数据大小不等于期望的数据包大小，则数据出错，丢弃读取到的数据，不进行任何处理， 返回-1
	if (readPacketSize != PURE_PACKET_SIZE) {
		UE_LOG(PacketManage, Warning, TEXT("数据包出错！ readPacketSize: %d , PacketSize-2 : %d"), readPacketSize, PURE_PACKET_SIZE);

		return -1;
	}

	return 1;
}

int FPacketManage::ReadLastPacket(){
    ReadyBytesSize =  SerialClass->GetReadySize();

    // 将数据流定向到数据包开始，不包括START_CODE_1, START_CODE_2
	if (IsLastReadPacketComplete) {
		// 如果上一次读取的数据包是完整的，那么上一次的数据就已经被读取了，我们这次可以重定位到START_CODE_1；
		// 否则，上一次的数据没被读取，我们要接着上一次的读，不能抛弃这些未读取的数据。
		SerialClass->ReadDataUtil(PacketBuff, START_CODE_1, START_CODE_2, ReadyBytesSize);
		ReadyBytesSize = SerialClass->GetReadySize();
	}
    
	// 需要读取的数据大小，eg： 24 E S 24 E，则需要 24 E； 24 E S 24 ，则需要 24 E S 24
    BytesNeeded = ReadyBytesSize % PACKET_SIZE + PACKET_SIZE;     
    // 如果剩下数据少于需要的数据（即剩下的数据不足一个数据包的大小），则返回
    if(ReadyBytesSize < BytesNeeded){
        UE_LOG(PacketManage, Warning, TEXT("数据不够一个数据包！ReadyBytesSize : %d, BytesNeeded : %d"), ReadyBytesSize, BytesNeeded);
		IsLastReadPacketComplete = false;
        return -1;
    }
	IsLastReadPacketComplete = true;

    BytesNotNeed = ReadyBytesSize - BytesNeeded;       // 不需要的数据大小
    //只需要后面一个完整的数据包，前面多余的数据清除，这里不用ReadDataUtil是为了优化性能
    while(BytesNotNeed > 0) {
        // 如果数据过多，超出缓存区，则考虑是开始时多余的数据，读取到最后一个buff。（只可能在最开始时发生，这些数据不做处理，直接清空）
        if(BytesNotNeed > PACKET_BUFF_SIZE){
            BytesNotNeed -= SerialClass->ReadData(PacketBuff, PACKET_BUFF_SIZE);
        }else{
            // 清除不需要的数据
            BytesNotNeed -= SerialClass->ReadData(PacketBuff, BytesNotNeed);
        }
    }
   
    // 开始读取数据
	readPacketSize = SerialClass->ReadData(PacketBuff, PURE_PACKET_SIZE);
    // 如果读取的数据大小不等于期望的数据包大小，则数据出错，丢弃读取到的数据，不进行任何处理， 返回-1
    if(readPacketSize != PURE_PACKET_SIZE){
        UE_LOG(PacketManage, Warning, TEXT("数据包出错！ readPacketSize: %d , PacketSize-2 : %d"), readPacketSize, PURE_PACKET_SIZE);

        return -1;
    }

    return 1;
}

int FPacketManage::Packet2Quat(TArray<FQuat>* PlayerBonePoses, int BoneNums){
    // dmpGetQuaternion(PlayerBonePoses, )
    for(int i = 0; i < BoneNums; i++){
		memcpy(UnitPacket, PacketBuff + i * UNIT_PACKET_SIZE * sizeof(uint8), UNIT_PACKET_SIZE * sizeof(uint8));
		dmpGetQuaternion(&((*PlayerBonePoses)[i]), UnitPacket);
		//PlayerBonePoses[i].X = i;
		UE_LOG(PacketManage, Warning, TEXT("数据包转FQuat %s"), *((*PlayerBonePoses)[i].ToString()));


    }

	return 1;
}

uint8 FPacketManage::dmpGetQuaternionL(int16 *data, const uint8* packet) {
    // TODO: accommodate different arrangements of sent data (ONLY default supported now)
    data[0] = ((packet[0] << 8) | packet[1]);
    data[1] = ((packet[2] << 8) | packet[3]);
    data[2] = ((packet[4] << 8) | packet[5]);
    data[3] = ((packet[6] << 8) | packet[7]);
    return 0;
}

uint8 FPacketManage::dmpGetQuaternion(FQuat* q, const uint8* packet) {
    // TODO: accommodate different arrangements of sent data (ONLY default supported now)
    int16 qI[4];
    uint8 status = dmpGetQuaternionL(qI, packet);
    if (status == 0) {
        q->W = (float)qI[0] / 16384.0f;
        q->X = (float)qI[1] / 16384.0f;
        q->Y = (float)qI[2] / 16384.0f;
        q->Z = (float)qI[3] / 16384.0f;
        return 0;
    }
    return status; // int16 return value, indicates error if this line is reached
}
