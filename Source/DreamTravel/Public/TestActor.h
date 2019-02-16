// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestActor.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(TestActor, Log, All);

#define BONE_NUMS (3)		// mpu节点数

class FPacketManage;
class UStaticMeshComponent;

UCLASS()
class DREAMTRAVEL_API ATestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void BeginDestroy() override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	FPacketManage* PacketManage = nullptr;
	//UStaticMeshComponent* cube;
	TArray<UStaticMeshComponent*> cubes;

	//FQuat * PlayerBonePoses[BONE_NUMS];
	TArray<FQuat> * PlayerBonePoses;

	void InitPlayerBonePoses(int BoneNums);		// 初始化PlayerBonePoses


};
