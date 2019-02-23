﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "DTSkeletalMeshComponent.h"
#include "PacketManage.h"

// #include "SerialPort.h" 

DEFINE_LOG_CATEGORY(DTSkeletalMeshComponent);


void UDTSkeletalMeshComponent::InitializeComponent()
{
	Super::InitializeComponent();

	InitAnim(false);
}

void UDTSkeletalMeshComponent::BeginPlay()
{
	Super::BeginPlay();
	PacketManage = new FPacketManage();

    // 初始化PlayerBonePoses
	InitPoses(BONE_NUMS);


	//MyClass = MyClass();
	// PacketManage->Initialise();


	//连接DreamTravel，不断尝试连接，不连接成功，不进行下一步，或者直接退出
	//FSerialClass::Initialise();
	//while (FSerialClass::Open(3, 115200) == false)
	//{
	//	FPlatformProcess::Sleep(1000);
	//	UE_LOG(DTSkeletalMeshComponent, Warning, TEXT("连接失败，正在重连。。。"));
	//}
	//（不是算法 TODO 新建DTClass，让SerialClass只提供读写服务。）

	//初始化DT，生成Avatar，提示玩家与avatar保存同一姿态，记录Avatar与Player的方位角差（注意坐标轴方向不同，考虑能否使用同一的方法），

	//获取每个关节和其所有子关节的质量，并保存。

	//开启Avatar物理模拟，开启碰撞模拟，开启模拟重力
}

void UDTSkeletalMeshComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // 连接
	if (PacketManage->Connect(DeltaTime) == false) {
		return;
	}

    // 更新玩家姿态（玩家即用户）
	//PacketManage->UpdatePlayerPose(PlayerBonePoses, BONE_NUMS);

	////初始化DT，并进行姿态校准
 //   Init();

	//// char buff[100];
	//
	////cSerialClass.ReadData(buff, 30);
	//FPlatformProcess::Sleep(1);
	
	// FString a = buff;
	// UE_LOG(DTSkeletalMeshComponent, Warning, TEXT("%s"), *a);

	//float BoneMass = GetBoneMass(FName("spine_01"), false);
	//UE_LOG(DTSkeletalMeshComponent, Warning, TEXT("BoneMass:  %f"), BoneMass);
	

	//在物理模拟前，读取最新play状态，获取avatar最新状态，计算当前方位角差，并施加力，通知DT去都下一组数据，以让在下一tick时可直接获取到player数据

	//在物理模拟结束后，计算当前avatar状态，并供下一tick开始时直接使用。
}

//初始化DT，并进行姿态校准
void UDTSkeletalMeshComponent::Init(){
    // 更新姿态
    UpdatePose();
    
    

}

void UDTSkeletalMeshComponent::UpdatePose(){

    //UpdateAvatarPose(AvatarBonePoses, BONE_NUMS);
    //PacketManage->UpdatePlayerPose(PlayerBonePoses, BONE_NUMS);

}

void UDTSkeletalMeshComponent::UpdateAvatarPose(FQuat * AvatarBonePoses, int BoneNums){
    for(int i = 0; i < BONE_NUMS; i++){
        // 获取avatar bone姿态
        //AvatarBonePoses[i] = GetBoneQuaternion(BoneNames[i], EBoneSpaces::WorldSpace);
    }

}


void UDTSkeletalMeshComponent::SyncPoses(float DeltaTime) {


  //  bool IsSame = false;
  //  for(int i = 0; i< BONE_NUMS; i++){
  //      //UE_LOG(DTSkeletalMeshComponent, Warning, TEXT("quat size: %f, SizeSquared: %f"),(*PlayerBonePoses)[0].Size(), (*PlayerBonePoses)[0].SizeSquared());
  //      
		//// 用上次记录的静止的pose的模减去最新的pose的模，如果小于DeltaSize则认为用户静止，（因为mpu有误差，所以不用直接判断相等）
		//// 如果所有节点都小于，则相等
		//float LastPoseSize = (*PlayerBonePoses)[i].Size();
		//if(PlayerBonePosesSyncSize[i] - LastPoseSize <= DeltaSize){
		//	IsSame = true;
		//}else{
		//	// 如果有一个不小于，则认为不相等， 则重新等待静止，并将不同的pose的模进行更新
		//	StaticStayTime = 0.0;
		//	PlayerBonePosesSyncSize[i] = LastPoseSize;
		//}
  //  }
  //  
  //  StaticStayTime += DeltaTime;
  //  if(StaticStayTime >= StaticNeeded){
  //      for (int i = 0; i < BONE_NUMS; i++){
  //          (*PlayerBonePosesTransformation)[0] = (*PlayerBonePoses)[0].Inverse();
  //      }
  //      UE_LOG(DTSkeletalMeshComponent, Warning, TEXT("SyncPoses success!"));
  //  }


	

	PoseSynced = true;
}

void UDTSkeletalMeshComponent::InitPoses(int BoneNums)
{
	PlayerBonePoses = new TArray<FQuat>;
	PlayerBonePosesTransformation = new TArray<FQuat>;

	for (int i = 0; i < BoneNums; i++)
	{
        // 初始化玩家pose
		PlayerBonePoses->Push(FQuat(0, 0, 0, 0));

	    // 初始化转换Quat
		PlayerBonePosesTransformation->Push(FQuat(0, 0, 0, 0));
	}
}

void UDTSkeletalMeshComponent::BeginDestroy()
{
	Super::BeginDestroy();

	delete PacketManage;
}





void UDTSkeletalMeshComponent::Test() {
	
	//while (cSerialClass.Open(4, 115200) == false) {
	//	FPlatformProcess::Sleep(1);
	//	UE_LOG(DTSkeletalMeshComponent, Warning, TEXT("连接失败，正在重连。。。"));
	//}
	UE_LOG(DTSkeletalMeshComponent, Warning, TEXT("连接成功"));

	char buff[100];
	//while (true)
	//{
		//cSerialClass.ReadData(buff, 30);
		UE_LOG(DTSkeletalMeshComponent, Warning, TEXT("%s"), buff);
		FPlatformProcess::Sleep(0.1);
	//}
	

	//CSerialPort mySerialPort;

	//if (!mySerialPort.InitPort(2))
	//{
	//	UE_LOG(DTSkeletalMeshComponent, Warning, TEXT("initPort fail !"));
	//}
	//else
	//{
	//	UE_LOG(DTSkeletalMeshComponent, Warning, TEXT("initPort success !"));
	//}

	//if (!mySerialPort.OpenListenThread())
	//{
	//	UE_LOG(DTSkeletalMeshComponent, Warning, TEXT("OpenListenThread fail !"));
	//}
	//else
	//{
	//	UE_LOG(DTSkeletalMeshComponent, Warning, TEXT("OpenListenThread fail !"));
	//}


	return;

}
