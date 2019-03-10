// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
//#include "GameFramework/Actor.h"
#include "TestActor.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(TestActor, Log, All);

#define BONE_NUMS (3)		// mpu节点数

class FPacketManage;
class UStaticMeshComponent;
//class UInputComponent;

UCLASS()
class DREAMTRAVEL_API ATestActor : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void BeginDestroy() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	FPacketManage* PacketManage = nullptr;
	//UStaticMeshComponent* cube;
	TArray<UStaticMeshComponent*> cubes;

	//FQuat * PlayerBonePoses[BONE_NUMS];
	TArray<FQuat> * PlayerBonePoses;

	TArray<FQuat> * LastPlayerBonePoses;

	TArray<FQuat> * PlayerBonePosesTransformation;		// 初始化转换Quat
	FQuat PlayerBonePosesTransformation2;		// 初始化转换Quat2

	float MyDeltaTime = 0;

	bool PoseSynced = false;

	// 输入绑定
	//UInputComponent* InputComponent;

	void InitPlayerBonePoses(int BoneNums);		// 初始化PlayerBonePoses

	void SyncPoses();
	void SyncPoses2();

	void Colculation();
};
