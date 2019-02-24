﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "DTPoseableMeshComponent.h"
#include "PacketManage.h"

// #include "SerialPort.h" 

DEFINE_LOG_CATEGORY(DTPoseableMeshComponent);



void UDTPoseableMeshComponent::BeginPlay()
{
	Super::BeginPlay();
	PacketManage = new FPacketManage();

    // 初始化PlayerBonePoses
	//InitPoses(BONE_NUMS);


	//MyClass = MyClass();
	// PacketManage->Initialise();


	//连接DreamTravel，不断尝试连接，不连接成功，不进行下一步，或者直接退出
	//FSerialClass::Initialise();
	//while (FSerialClass::Open(3, 115200) == false)
	//{
	//	FPlatformProcess::Sleep(1000);
	//	UE_LOG(DTPoseableMeshComponent, Warning, TEXT("连接失败，正在重连。。。"));
	//}
	//（不是算法 TODO 新建DTClass，让SerialClass只提供读写服务。）

	//初始化DT，生成Avatar，提示玩家与avatar保存同一姿态，记录Avatar与Player的方位角差（注意坐标轴方向不同，考虑能否使用同一的方法），

	//获取每个关节和其所有子关节的质量，并保存。

	//开启Avatar物理模拟，开启碰撞模拟，开启模拟重力
}

void UDTPoseableMeshComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	 //UE_LOG(DTPoseableMeshComponent, Warning, TEXT("UDTPoseableMeshComponent TickComponent"));

	if (PacketManage == nullptr)
	{
		return;
	}

    // 连接
	if (PacketManage->Connect(DeltaTime) == false) {
		return;
	}

    // 更新玩家姿态（玩家即用户）
	PacketManage->UpdatePlayerPose(PlayerBonePoses, BONE_NUMS);

	// 如果没同步，则同步，然后返回
	if (PoseSynced == false){
		SyncPoses(DeltaTime);
		return;
	}

	// 更新Avatar姿态
	UpdateAvatarPoseNonPhysics();


	////初始化DT，并进行姿态校准
	//   Init();

	//// char buff[100];
	//
	////cSerialClass.ReadData(buff, 30);
	//FPlatformProcess::Sleep(1);
	
	// FString a = buff;
	// UE_LOG(DTPoseableMeshComponent, Warning, TEXT("%s"), *a);

	//float BoneMass = GetBoneMass(FName("spine_01"), false);
	//UE_LOG(DTPoseableMeshComponent, Warning, TEXT("BoneMass:  %f"), BoneMass);
	

	//在物理模拟前，读取最新play状态，获取avatar最新状态，计算当前方位角差，并施加力，通知DT去都下一组数据，以让在下一tick时可直接获取到player数据

	//在物理模拟结束后，计算当前avatar状态，并供下一tick开始时直接使用。
}

//初始化DT，并进行姿态校准
void UDTPoseableMeshComponent::Init(){
    // 更新姿态
    UpdatePose();
    
    

}

void UDTPoseableMeshComponent::UpdatePose(){

    //UpdateAvatarPose(AvatarBonePoses, BONE_NUMS);
    //PacketManage->UpdatePlayerPose(PlayerBonePoses, BONE_NUMS);

}

void UDTPoseableMeshComponent::UpdateAvatarPoseNonPhysics(){
    for(int i = 0; i < BONE_NUMS; i++){
        // 获取avatar bone姿态
        //AvatarBonePoses[i] = GetBoneQuaternion(BoneNames[i], ); 
		SetBoneRotationByName(BoneNames[i], ((*PlayerBonePosesTransformation)[i] * (*PlayerBonePoses)[i]* FQuat((*InitialAvatarBonePoses)[i])).Rotator(),EBoneSpaces::WorldSpace);
    }

}


void UDTPoseableMeshComponent::SyncPoses(float DeltaTime) {

	// 如果不满IntervalTime，则直接返回
	StayedTime += DeltaTime;
	if (StayedTime < IntervalTime){
		return;
	}else{
		StayedTime = 0.0f;
	}

	// 进行校验， 转换成欧拉角进行比较
    bool IsSame = false;
    for(int i = 0; i< BONE_NUMS; i++){
        
		// 用最初的欧拉角与最新的欧拉角进行比较，如果差值小于，（因为mpu有误差，所以不用直接判断相等）
		// 如果所有节点都小于，则相等；如果有一个不小于，则认为不相等，并更新相应的PlayerBonePosesSync
		FVector LastPose = (*PlayerBonePoses)[i].Euler();
		//LastPose -= (*PlayerBonePosesSync)[i];
		
		if ((LastPose- (*PlayerBonePosesSync)[i]).Size() < DeltaSize){
			IsSame = IsSame && true;
		}else{
			// 如果大于DeltaSize，则将新的姿态赋给PlayerBonePosesSync
			IsSame = IsSame && false;
			(*PlayerBonePosesSync)[i] = LastPose;
		}

		UE_LOG(DTPoseableMeshComponent, Warning, TEXT("PlayerBonePoses Eular : %s, PlayerBonePosesSync Eular: %s"), *LastPose.ToString(), *(*PlayerBonePosesSync)[i].ToString());
		UE_LOG(DTPoseableMeshComponent, Warning, TEXT("Delta Eular: %s, Size: %f,  SizeSquared: %f"), *(LastPose - (*PlayerBonePosesSync)[i]).ToString(), (LastPose - (*PlayerBonePosesSync)[i]).Size(), (LastPose - (*PlayerBonePosesSync)[i]).SizeSquared());

    }

	if (IsSame)
	{
		SameTimes += 1;
		if (SameTimes >= SameNeededTimes)
		{
			PoseSynced = true;
			for (int i = 0; i < BONE_NUMS; i++)
			{
				(*PlayerBonePosesTransformation)[i] = (*PlayerBonePoses)[i].Inverse();
				(*InitialAvatarBonePoses)[i] = GetBoneRotationByName(BoneNames[i], EBoneSpaces::WorldSpace);
			}

			UE_LOG(DTPoseableMeshComponent, Warning, TEXT("Synced! ------"));
		}
	}
	else {
		SameTimes = 0;
	}
}

void UDTPoseableMeshComponent::InitPoses(int BoneNums)
{
	PlayerBonePoses = new TArray<FQuat>;
	PlayerBonePosesTransformation = new TArray<FQuat>; 
	PlayerBonePosesSync = new TArray<FVector>;
	InitialAvatarBonePoses = new TArray<FRotator>;

	for (int i = 0; i < BoneNums; i++)
	{
        // 初始化玩家pose
		PlayerBonePoses->Push(FQuat(0, 0, 0, 0));

	    // 初始化转换Quat
		PlayerBonePosesTransformation->Push(FQuat(0, 0, 0, 0));

		// 初始化同步欧拉
		PlayerBonePosesSync->Push(FVector(0, 0, 0));

		// 初始化InitialAvatarBonePoses
		InitialAvatarBonePoses->Push(FRotator(0, 0, 0));
	}
}

void UDTPoseableMeshComponent::BeginDestroy()
{
	Super::BeginDestroy();

	delete PacketManage;
}





void UDTPoseableMeshComponent::Test() {
	
	//while (cSerialClass.Open(4, 115200) == false) {
	//	FPlatformProcess::Sleep(1);
	//	UE_LOG(DTPoseableMeshComponent, Warning, TEXT("连接失败，正在重连。。。"));
	//}
	UE_LOG(DTPoseableMeshComponent, Warning, TEXT("连接成功"));

	char buff[100];
	//while (true)
	//{
		//cSerialClass.ReadData(buff, 30);
		UE_LOG(DTPoseableMeshComponent, Warning, TEXT("%s"), buff);
		FPlatformProcess::Sleep(0.1);
	//}
	

	//CSerialPort mySerialPort;

	//if (!mySerialPort.InitPort(2))
	//{
	//	UE_LOG(DTPoseableMeshComponent, Warning, TEXT("initPort fail !"));
	//}
	//else
	//{
	//	UE_LOG(DTPoseableMeshComponent, Warning, TEXT("initPort success !"));
	//}

	//if (!mySerialPort.OpenListenThread())
	//{
	//	UE_LOG(DTPoseableMeshComponent, Warning, TEXT("OpenListenThread fail !"));
	//}
	//else
	//{
	//	UE_LOG(DTPoseableMeshComponent, Warning, TEXT("OpenListenThread fail !"));
	//}


	return;

}