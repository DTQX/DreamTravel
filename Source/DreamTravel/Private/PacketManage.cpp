// Fill out your copyright notice in the Description page of Project Settings.

#include "PacketManage.h"
#include "SerialClass.h"
#include "PlatformFilemanager.h"
#include "FileHelper.h"

DEFINE_LOG_CATEGORY(PacketManage);

FPacketManage::FPacketManage()
{
    SerialClass = new FSerialClass();
}

FPacketManage::~FPacketManage()
{
    delete SerialClass;
}
// 是否已连接，true：已连接，false：未连接
bool FPacketManage::IsConnected()
{
    return SerialClass->IsOpened();
}
// 进行连接，如果已连接则直接返回true，否则进行连接
bool FPacketManage::Connect(float DeltaTime)
{
    // 如果已连接，则直接返回true
    if (IsConnected())
    {
        return true;
    }

    ConnectWaitedTime += DeltaTime;
    if (ConnectWaitedTime > ExpectConnectWaitTime)
    {
        bool result = SerialClass->Open(COM_PORT, 115200);
        if (result == false)
        {
            UE_LOG(PacketManage, Warning, TEXT("连接失败，正在等待重连。。。"));
        }
        else
        {
            UE_LOG(PacketManage, Warning, TEXT("连接成功！"));
        }
        ConnectWaitedTime = 0.0f;
        return result;
    }
    return false;
}
// 更新playPose
int FPacketManage::UpdatePlayerPose(TArray<FQuat> *PlayerBonePoses, int BoneNums)
{
    for (int i = 0; i < BoneNums; i++)
    {
    }

    // 读取数据
    //int ReadStatus = ReadLastPacket_back();
    if (ReadLastPacket())
    {
        return -1;
    }

    // 处理数据
    Packet2Quat(PlayerBonePoses, BoneNums, PacketBuff);

    return 1;
}

// 读取最新的数据包， 数据流格式 S1 S1 24 S2 S2
int FPacketManage::ReadLastPacket()
{
    // 获取准备就绪的字节数
    ReadyBytesSize = SerialClass->GetReadySize();
    // 如果字节数不够一个数据包，则直接返回
    if (ReadyBytesSize < PACKET_SIZE)
    {
        UE_LOG(PacketManage, Warning, TEXT("FPacketManage::ReadLastPacket : not enough data!"));
        return -4;
    }

    if (IsLastReadError == true)
    {
        // 如果上次读取数据出错，则进行数据读取纠正
        if (!SerialClass->ReadDataUtil(PacketBuff, END_CODE, END_CODE, ReadyBytesSize))
        {
            UE_LOG(PacketManage, Warning, TEXT("FPacketManage::ReadLastPacket : data rectification complete!"));
            IsLastReadError = false;
        }else
        {
            UE_LOG(PacketManage, Warning, TEXT("FPacketManage::ReadLastPacket : data rectification incomplete!"));
            return -3;
        }
        
        ReadyBytesSize = SerialClass->GetReadySize();
    }

    // 如果准备就绪的字节数大于PACKET_SIZE，则循环获取一个包的数据，直到准备就绪的字节数不足一个数据包大小
    // 不需要考虑mpu产生数据量大于ue4处理的数据量，因为最终要保证ue4处理数据量>=mpu4产生数据量，所以循环读取就可以
    while (ReadyBytesSize >= PACKET_SIZE)
    {
        if (SerialClass->ReadData(PacketBuff, PACKET_SIZE) != PACKET_SIZE)
        {
            UE_LOG(PacketManage, Warning, TEXT("FPacketManage::ReadLastPacket : SerialClass->ReadData error!"));
            return -1;
        }
        if (PacketBuff[0] == START_CODE && PacketBuff[1] == START_CODE // 校验头
            && PacketBuff[PACKET_SIZE - 2] == END_CODE && PacketBuff[PACKET_SIZE - 1] == END_CODE){ // 校验尾
            ReadyBytesSize -= PACKET_SIZE;
            continue;
        }
        else
        {
            UE_LOG(PacketManage, Warning, TEXT("FPacketManage::ReadLastPacket : data error!"));
            IsLastReadError = true;
            //UE_LOG(PacketManage, Warning, TEXT("FPacketManage::ReadLastPacket : data error! %c %c"), PacketBuff[0], PacketBuff[0]);

            return -2;
        }
    }

    return 0;
}


// 数据包转quat
int FPacketManage::Packet2Quat(TArray<FQuat> *PlayerBonePoses, int BoneNums, uint8 *PacketBuff)
{
    // dmpGetQuaternion(PlayerBonePoses, )
    for (int i = 0; i < BoneNums; i++)
    {
        memcpy(UnitPacket, PacketBuff + 2 + i * UNIT_PACKET_SIZE * sizeof(uint8), UNIT_PACKET_SIZE * sizeof(uint8));
        dmpGetQuaternion(&((*PlayerBonePoses)[i]), UnitPacket);
        //PlayerBonePoses[i].X = i;
        //UE_LOG(PacketManage, Warning, TEXT("数据包转FQuat %s"), *((*PlayerBonePoses)[i].ToString()));
    }

    return 0;
}
// 一个mpu的数据转quat
uint8 FPacketManage::dmpGetQuaternionL(int16 *data, const uint8 *packet)
{
    // TODO: accommodate different arrangements of sent data (ONLY default supported now)
    data[0] = ((packet[0] << 8) | packet[1]);
    data[1] = ((packet[2] << 8) | packet[3]);
    data[2] = ((packet[4] << 8) | packet[5]);
    data[3] = ((packet[6] << 8) | packet[7]);
    return 0;
}
// 一个mpu的数据转quat
uint8 FPacketManage::dmpGetQuaternion(FQuat *q, const uint8 *packet)
{
    // TODO: accommodate different arrangements of sent data (ONLY default supported now)
    int16 qI[4];
    uint8 status = dmpGetQuaternionL(qI, packet);
    if (status == 0)
    {
        q->W = (float)qI[0] / 16384.0f;

        // 右手转左手 x,y不对调
        q->X = -(float)qI[1] / 16384.0f;
        q->Y = (float)qI[2] / 16384.0f;
        q->Z = -(float)qI[3] / 16384.0f;

        // 右手转左手 x,y对调
        /*q->X = (float)qI[2] / 16384.0f;
		q->Y = (float)qI[1] / 16384.0f;
		q->Z = - (float)qI[3] / 16384.0f;*/

        return 0;
    }
    return status; // int16 return value, indicates error if this line is reached
}

// 获取最新的数据包，备份
int FPacketManage::ReadLastPacket_back()
{
    ReadyBytesSize = SerialClass->GetReadySize();

    // 将数据流定向到数据包开始，不包括START_CODE_1, END_CODE
    if (IsLastReadPacketComplete)
    {
        // 如果上一次读取的数据包是完整的，那么上一次的数据就已经被读取了，我们这次可以重定位到START_CODE_1；
        // 否则，上一次的数据没被读取，我们要接着上一次的读，不能抛弃这些未读取的数据。
        SerialClass->ReadDataUtil(PacketBuff, START_CODE, END_CODE, ReadyBytesSize);
        ReadyBytesSize = SerialClass->GetReadySize();
    }
    //FPlatformProcess::Sleep(0.001);

    // 如果剩下数据少于需要的数据（即剩下的数据不足PURE_PACKET_SIZE），则返回
    if (ReadyBytesSize < PURE_PACKET_SIZE)
    {
        UE_LOG(PacketManage, Warning, TEXT("数据不够一个数据包！ReadyBytesSize : %d, PURE_PACKET_SIZE : %d"), ReadyBytesSize, PURE_PACKET_SIZE);
        IsLastReadPacketComplete = false;
        return -1;
    }
    IsLastReadPacketComplete = true;

    if (ReadyBytesSize > PACKET_SIZE + PURE_PACKET_SIZE)
    {
        if (ReadyBytesSize % PACKET_SIZE < PURE_PACKET_SIZE)
        {
            PacketBytesNeeded = ReadyBytesSize % PACKET_SIZE + PACKET_SIZE;
        }
        else
        {
            PacketBytesNeeded = PURE_PACKET_SIZE;
        }
        //PacketBytesNeeded = ReadyBytesSize % PACKET_SIZE + PACKET_SIZE;

        BytesNotNeed = ReadyBytesSize - PacketBytesNeeded; // 不需要的数据大小
        //只需要后面一个完整的数据包，前面多余的数据清除，这里不用ReadDataUtil是为了优化性能
        while (BytesNotNeed > 0)
        {
            // 如果数据过多，超出缓存区，则考虑是开始时多余的数据，读取到最后一个buff。（只可能在最开始时发生，这些数据不做处理，直接清空）
            if (BytesNotNeed > PACKET_BUFF_SIZE)
            {
                BytesNotNeed -= SerialClass->ReadData(PacketBuff, PACKET_BUFF_SIZE);
            }
            else
            {
                // 清除不需要的数据
                BytesNotNeed -= SerialClass->ReadData(PacketBuff, BytesNotNeed);
            }
        }
    }

    // 开始读取数据
    readPacketSize = SerialClass->ReadData(PacketBuff, PURE_PACKET_SIZE);
    // 如果读取的数据大小不等于期望的数据包大小，则数据出错，丢弃读取到的数据，不进行任何处理， 返回-1
    if (readPacketSize != PURE_PACKET_SIZE)
    {
        UE_LOG(PacketManage, Warning, TEXT("数据包出错！ readPacketSize: %d , PacketSize-2 : %d"), readPacketSize, PURE_PACKET_SIZE);

        return -1;
    }

    return 1;
}

// 获取mpu的初始偏移量
int FPacketManage::getMPUOffset(TArray<FQuat> *MpuOffsetPoses, int BoneNums)
{

    TArray<uint8> buffer;
    if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*MPU_OFFSET_FILE_PATH))
    {
        // 如果本地有配置，则从本地获取
        FFileHelper::LoadFileToArray(
            buffer,
            //PacketBuff,
            *MPU_OFFSET_FILE_PATH,
            EFileWrite::FILEWRITE_None);
    }
    else
    {
        // 本地没有则从远程服务器获取， TODO

        // 再保存到本地  TODO
    }

    for (int i = 0; i < 3; i++)
    {
        UE_LOG(LogTemp, Warning, TEXT("PacketBuff to read: %d"), buffer[i]);
    }

    Packet2Quat(MpuOffsetPoses, BoneNums, buffer.GetData());

    return 0;
}

// 设置mpu的初始偏移量
int FPacketManage::setMPUOffset(bool SyncToRemote)
{

    FFileHelper::SaveArrayToFile(
        *new TArrayView<uint8>(PacketBuff),
        //PacketBuff,
        *MPU_OFFSET_FILE_PATH,
        &IFileManager::Get(),
        EFileWrite::FILEWRITE_None);

    for (int i = 0; i < 3; i++)
    {
        UE_LOG(LogTemp, Warning, TEXT("PacketBuff to write: %d"), PacketBuff[i]);
    }

    if (SyncToRemote)
    {
        // TODO 同步到远程
    }

    return 0;
}