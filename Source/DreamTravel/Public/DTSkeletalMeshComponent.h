// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "DTSkeletalMeshComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(DTSkeletalMeshComponent, Log, All);

// 骨头（mpu）节点数量
#define BONE_NUMS (3)

class FPacketManage;

/**
 * 
 */
UCLASS(ClassGroup = ("DreamTravel"), meta = (BlueprintSpawnableComponent))
class DREAMTRAVEL_API UDTSkeletalMeshComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()
	
public:
	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	virtual void BeginPlay() override;
	void Test();
	virtual void BeginDestroy() override;
private:

	void UpdatePose();  // 更新姿态
	void UpdateAvatarPose(FQuat * PlayerBonePoses, int BoneNums);    // 更新avatar姿态

	// BoneNames = spine_01, spine_02, spine_03
	// 	左手 :左手 upperarm_l, lowerarm_l, hand_l,
	// 		手掌，middle_01_l
	// 		手指头
	// 			(0)thumb_02_l, thumb_03_l,
	// 			(1)index_02_l, index_03_l, 
	// 			(2)middle_02_l, middle_03_l,
	// 			(3)ring_02_l, ring_03_l,
	// 			(4)pinky_02_l, pinky_03_l

	//TMap<FString, >

	//GetBoneAxis 获取骨骼的方向向量

    
	// 记录需要操作的骨骼名
	FName BoneNames[BONE_NUMS] = {
		//FName("spine_01"),FName("spine_02"), FName("spine_03"), FName(""),FName(""),    //脊椎
		//FName("upperarm_l"),FName("lowerarm_l"),FName("hand_l"),FName("middle_01_l"),FName(""),FName(""),
		FName("upperarm_l"),FName("lowerarm_l"),FName("hand_l")
	};



    void Init();



    FPacketManage* PacketManage = nullptr;      // 数据包管理中心

	TArray<FQuat> * PlayerBonePoses;        // 存放用户最新的用户姿态

	TArray<FQuat> * PlayerBonePosesTransformation;		// 用于姿态初始化的Quat，值为playerBone的初始姿态的逆


	void InitPoses(int BoneNums);		// 初始化PlayerBonePoses和PlayerBonePosesTransformation

	bool PoseSynced = false;        // 是否已同步
    float StaticStayTime = 0;     // 用户静止的时长
    const float StaticNeeded = 3.0;       // 用户需要静止的时长
	const float DeltaSize = 1.0;	// 最初的静止pose与最新的pose的模差如果小于DeltaSize，则说明玩家是静止的。
	float PlayerBonePosesSyncSize[BONE_NUMS] = {0.0};        // 存放玩家上次静止的pose的模

	void SyncPoses(float DeltaTime);       // 同步player、avatar
};