// Fill out your copyright notice in the Description page of Project Settings.

#include "TestActor.h"
#include "PacketManage.h"
#include "Components/StaticMeshComponent.h"

DEFINE_LOG_CATEGORY(TestActor);


// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();
	PacketManage = new FPacketManage();
	
	// 初始化PlayerBonePoses
	InitPlayerBonePoses(BONE_NUMS);
	
	//cube = GetComponentByClass(UStaticMeshComponent::UStaticMeshComponent());
	GetComponents(cubes);

	for (int32 b = 0; b < cubes.Num(); b++)
	{
		UStaticMeshComponent*a = cubes[b];

		UE_LOG(TestActor, Warning, TEXT("%s"), *(a->GetName()));
	}
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 连接
	if (PacketManage->Connect(DeltaTime) == false) {
		return;
	}

	PacketManage->UpdatePlayerPose(PlayerBonePoses, BONE_NUMS);
	for (int i = 0; i < BONE_NUMS; i++)
	{
		//UE_LOG(TestActor, Warning, TEXT("%s"), *((*PlayerBonePoses)[i]).ToString());
		//cubes[i]->SetAllPhysicsRotation((*PlayerBonePoses)[i]);
	}
	UE_LOG(TestActor, Warning, TEXT("%s"), *((*PlayerBonePoses)[0]).ToString());

	cubes[0]->SetAllPhysicsRotation((*PlayerBonePoses)[0]);

}

void ATestActor::InitPlayerBonePoses(int BoneNums)
{
	//TArray<FQuat> a;
	//TArray<FQuat> a;
	PlayerBonePoses = new TArray<FQuat>;
	for (int i = 0; i < BoneNums; i++)
	{
		//*PlayerBonePoses[i] = FQuat(0, 0, 0, 0);
		PlayerBonePoses->Push(FQuat(0, 0, 0, 0));
	}
}

void ATestActor::BeginDestroy()
{
	Super::BeginDestroy();

	//cSerialClass.Close();
	delete PacketManage;
}
