// Fill out your copyright notice in the Description page of Project Settings.

#include "DTPawn.h"
#include "Components/PoseableMeshComponent.h"
#include "PacketManage.h"

// #include "SerialPort.h" 

DEFINE_LOG_CATEGORY(DTPawn);


// Sets default values
ADTPawn::ADTPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
}

// Called when the game starts or when spawned
void ADTPawn::BeginPlay()
{
	Super::BeginPlay();

	PoseableMeshComponent = FindComponentByClass<UPoseableMeshComponent>();


	PoseableMeshComponent->SetBoneRotationByName(FName("upperarm_l"), FRotator(FQuat(FRotator(-90, 0, 0)) *FQuat(0.560380340, -0.564101338, -0.412479103, 0.444549859)), EBoneSpaces::WorldSpace);
	PoseableMeshComponent->SetBoneRotationByName(FName("lowerarm_l"), FRotator(FQuat(FRotator(-90, 0, 0)) *FQuat(-0.571011007, 0.613383830, 0.407904714, -0.362381667)), EBoneSpaces::WorldSpace);
	PoseableMeshComponent->SetBoneRotationByName(FName("hand_l"), FRotator(FQuat(FRotator(-90, 0, 0)) *FQuat(0.627503574, -0.760303736, 0.162374020, 0.042568613)), EBoneSpaces::WorldSpace);

	/*PoseableMeshComponent->SetBoneRotationByName(FName("upperarm_l"), FRotator(FQuat(0.560380340, -0.564101338, -0.412479103, 0.444549859)), EBoneSpaces::WorldSpace);
	PoseableMeshComponent->SetBoneRotationByName(FName("lowerarm_l"), FRotator(FQuat(-0.571011007, 0.613383830, 0.407904714, -0.362381667)), EBoneSpaces::WorldSpace);
	PoseableMeshComponent->SetBoneRotationByName(FName("hand_l"), FRotator(FQuat(0.627503574, -0.760303736, 0.162374020, 0.042568613)), EBoneSpaces::WorldSpace);
*/
	PacketManage = new FPacketManage();

	// 初始化PlayerBonePoses
	InitPoses(BONE_NUMS);


	//连接DreamTravel，不断尝试连接，不连接成功，不进行下一步，或者直接退出
	//FSerialClass::Initialise();
	//while (FSerialClass::Open(3, 115200) == false)
	//{
	//	FPlatformProcess::Sleep(1000);
	//	UE_LOG(DTPawn, Warning, TEXT("连接失败，正在重连。。。"));
	//}
	//（不是算法 TODO 新建DTClass，让SerialClass只提供读写服务。）

	//初始化DT，生成Avatar，提示玩家与avatar保存同一姿态，记录Avatar与Player的方位角差（注意坐标轴方向不同，考虑能否使用同一的方法），

	//获取每个关节和其所有子关节的质量，并保存。

	//开启Avatar物理模拟，开启碰撞模拟，开启模拟重力
	
}

// Called every frame
void ADTPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);




	// 测试时放开
	return;

	//UE_LOG(DTPawn, Warning, TEXT("ADTPawn TickComponent"));

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
	//FQuat tmp;
	//for (int i = 0; i< BONE_NUMS; i++)
	//{
	//	tmp = FQuat(FRotator(-90, 0, 0)) * (*PlayerBonePoses)[i];
	//	//(*PlayerBonePoses)[i] = tmp;

	//}

	// 如果没同步，则同步，然后返回
	/*if (PoseSynced == false) {
		SyncPoses(DeltaTime);
		return;
	}*/

	// 更新Avatar姿态
	if (PoseSynced)
	{
		UpdateAvatarPoseNonPhysics();
	}


	////初始化DT，并进行姿态校准
	//   Init();

	//// char buff[100];
	//
	////cSerialClass.ReadData(buff, 30);
	//FPlatformProcess::Sleep(1);

	// FString a = buff;
	// UE_LOG(DTPawn, Warning, TEXT("%s"), *a);

	//float BoneMass = GetBoneMass(FName("spine_01"), false);
	//UE_LOG(DTPawn, Warning, TEXT("BoneMass:  %f"), BoneMass);


	//在物理模拟前，读取最新play状态，获取avatar最新状态，计算当前方位角差，并施加力，通知DT去都下一组数据，以让在下一tick时可直接获取到player数据

	//在物理模拟结束后，计算当前avatar状态，并供下一tick开始时直接使用。

}


void ADTPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ADTPawn::ManualSyncPoses);
}



//初始化DT，并进行姿态校准
void ADTPawn::Init() {
	// 更新姿态
	UpdatePose();



}

void ADTPawn::UpdatePose() {

	//UpdateAvatarPose(AvatarBonePoses, BONE_NUMS);
	//PacketManage->UpdatePlayerPose(PlayerBonePoses, BONE_NUMS);

}

void ADTPawn::UpdateAvatarPoseNonPhysics() {
	for (int i = 0; i < BONE_NUMS; i++) {
		// 获取avatar bone姿态
		//AvatarBonePoses[i] = GetBoneQuaternion(BoneNames[i], ); 
		PoseableMeshComponent->SetBoneRotationByName(BoneNames[i], (FQuat(FRotator(90,0,0)) * (*PlayerBonePosesTransformation)[i] * (*PlayerBonePoses)[i] * FQuat((*InitialAvatarBonePoses)[i])).Rotator(), EBoneSpaces::WorldSpace);
	}

}


void ADTPawn::ManualSyncPoses() {
	PoseSynced = true;
	//PoseableMeshComponent->SetBoneRotationByName(FName("upperarm_l"), FRotator(FQuat(FRotator(-90, 0, 0)) *FQuat(0.560380340, -0.564101338, -0.412479103, 0.444549859)), EBoneSpaces::WorldSpace);
	//PoseableMeshComponent->SetBoneRotationByName(FName("lowerarm_l"), FRotator(FQuat(FRotator(-90, 0, 0)) *FQuat(-0.571011007, 0.613383830, 0.407904714, -0.362381667)), EBoneSpaces::WorldSpace);
	//PoseableMeshComponent->SetBoneRotationByName(FName("hand_l"), FRotator(FQuat(FRotator(-90, 0, 0)) *FQuat(0.627503574, -0.760303736, 0.162374020, 0.042568613)), EBoneSpaces::WorldSpace);

	for (int i = 0; i < BONE_NUMS; i++)
	{
		(*PlayerBonePosesTransformation)[i] = (*PlayerBonePoses)[i].Inverse();
		(*InitialAvatarBonePoses)[i] = PoseableMeshComponent->GetBoneRotationByName(BoneNames[i], EBoneSpaces::WorldSpace);
	}
}

void ADTPawn::SyncPoses(float DeltaTime) {
	//int Strength = 100;
	//// 左手添加力
	//PoseableMeshComponent->AddForce(FVector(0, -1, 0) * Strength, FName("index_03_l"), true);
	//PoseableMeshComponent->AddForce(FVector(0, -1, 0) * Strength, FName("middle_03_l"), true);
	//PoseableMeshComponent->AddForce(FVector(0, -1, 0) * Strength, FName("pinky_03_l"), true);
	//PoseableMeshComponent->AddForce(FVector(0, -1, 0) * Strength, FName("ring_03_l"), true);
	//PoseableMeshComponent->AddForce(FVector(0, -1, 0) * Strength, FName("thumb_03_l"), true);
	//// 对手掌上部加向上的力，下部加向下的力。使手掌掌心朝前
	//PoseableMeshComponent->AddForce(FVector(0, 0, 1) * Strength, FName("index_01_l"), true);
	//PoseableMeshComponent->AddForce(FVector(0, 0, -1) * Strength, FName("pinky_01_l"), true);

	//// 右手添加力
	//PoseableMeshComponent->AddForce(FVector(0, 1, 0) * Strength, FName("index_03_r"), true);
	//PoseableMeshComponent->AddForce(FVector(0, 1, 0) * Strength, FName("middle_03_r"), true);
	//PoseableMeshComponent->AddForce(FVector(0, 1, 0) * Strength, FName("pinky_03_r"), true);
	//PoseableMeshComponent->AddForce(FVector(0, 1, 0) * Strength, FName("ring_03_r"), true);
	//PoseableMeshComponent->AddForce(FVector(0, 1, 0) * Strength, FName("thumb_03_r"), true);
	//// 对手掌上部加向上的力，下部加向下的力。使手掌掌心朝前
	//PoseableMeshComponent->AddForce(FVector(0, 0, 1) * Strength, FName("index_01_r"), true);
	//PoseableMeshComponent->AddForce(FVector(0, 0, -1) * Strength, FName("pinky_01_r"), true);

	// 如果不满IntervalTime，则直接返回
	StayedTime += DeltaTime;
	if (StayedTime < IntervalTime) {
		return;
	}
	else {
		StayedTime = 0.0f;
	}

	// 进行校验， 转换成欧拉角进行比较
	bool IsSame = true;
	FVector LastPose;
	for (int i = 0; i < BONE_NUMS; i++) {

		// 用最初的欧拉角与最新的欧拉角进行比较，如果差值小于，（因为mpu有误差，所以不用直接判断相等）
		// 如果所有节点都小于，则相等；如果有一个不小于，则认为不相等，并更新相应的PlayerBonePosesSync
		LastPose = (*PlayerBonePoses)[i].Euler();
		//LastPose -= (*PlayerBonePosesSync)[i];

		if ((LastPose - (*PlayerBonePosesSync)[i]).Size() < DeltaSize) {
			IsSame = IsSame && true;
		}
		else {
			// 如果大于DeltaSize，则将新的姿态赋给PlayerBonePosesSync
			IsSame = IsSame && false;
			(*PlayerBonePosesSync)[i] = LastPose;
		}

		UE_LOG(DTPawn, Warning, TEXT("PlayerBonePoses Eular : %s, PlayerBonePosesSync Eular: %s"), *LastPose.ToString(), *(*PlayerBonePosesSync)[i].ToString());
		UE_LOG(DTPawn, Warning, TEXT("Delta Eular: %s, Size: %f,  SizeSquared: %f"), *(LastPose - (*PlayerBonePosesSync)[i]).ToString(), (LastPose - (*PlayerBonePosesSync)[i]).Size(), (LastPose - (*PlayerBonePosesSync)[i]).SizeSquared());

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
				(*InitialAvatarBonePoses)[i] = PoseableMeshComponent->GetBoneRotationByName(BoneNames[i], EBoneSpaces::WorldSpace);
			}

			UE_LOG(DTPawn, Warning, TEXT("Synced! ------"));
		}
	}
	else {
		SameTimes = 0;
	}
}

void ADTPawn::InitPoses(int BoneNums)
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

void ADTPawn::BeginDestroy()
{
	Super::BeginDestroy();

	delete PacketManage;
}





void ADTPawn::Test() {

	//while (cSerialClass.Open(4, 115200) == false) {
	//	FPlatformProcess::Sleep(1);
	//	UE_LOG(DTPawn, Warning, TEXT("连接失败，正在重连。。。"));
	//}
	UE_LOG(DTPawn, Warning, TEXT("连接成功"));

	char buff[100];
	//while (true)
	//{
		//cSerialClass.ReadData(buff, 30);
	UE_LOG(DTPawn, Warning, TEXT("%s"), buff);
	FPlatformProcess::Sleep(0.1);
	//}


	//CSerialPort mySerialPort;

	//if (!mySerialPort.InitPort(2))
	//{
	//	UE_LOG(DTPawn, Warning, TEXT("initPort fail !"));
	//}
	//else
	//{
	//	UE_LOG(DTPawn, Warning, TEXT("initPort success !"));
	//}

	//if (!mySerialPort.OpenListenThread())
	//{
	//	UE_LOG(DTPawn, Warning, TEXT("OpenListenThread fail !"));
	//}
	//else
	//{
	//	UE_LOG(DTPawn, Warning, TEXT("OpenListenThread fail !"));
	//}


	return;

}