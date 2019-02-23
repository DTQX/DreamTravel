// Fill out your copyright notice in the Description page of Project Settings.

#include "TestActor.h"
#include "PacketManage.h"
#include "Components/StaticMeshComponent.h"
#

DEFINE_LOG_CATEGORY(TestActor);


// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// InputComponent = CreateDefaultSubobject<UInputComponent>(FName("Input Component"));
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

	// InputComponent->BindAction("Jump", IE_Released, this, &ATestActor::SyncPoses);
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
    //UE_LOG(TestActor, Warning, TEXT("PlayerBonePoses size: %f, SizeSquared: %f"),(*PlayerBonePoses)[0].Size(), (*PlayerBonePoses)[0].SizeSquared());

	UE_LOG(TestActor, Warning, TEXT("PlayerBonePoses %s"), *((*PlayerBonePoses)[0]).ToString());
	UE_LOG(TestActor, Warning, TEXT("PlayerBonePosesTransformation %s"), *((*PlayerBonePosesTransformation)[0]).ToString());
	UE_LOG(TestActor, Warning, TEXT("PlayerBonePosesTransformation %s"), *((*PlayerBonePosesTransformation)[0] * (*PlayerBonePoses)[0]).ToString());

	//cubes[0]->SetAllPhysicsRotation((*PlayerBonePoses)[0]);
	//if (PoseSynced){
		//SetActorRotation( (*PlayerBonePosesTransformation)[0] * (*PlayerBonePoses)[0] * FQuat(FRotator(45, 0, 0)));
		SetActorRotation(FQuat(FRotator(0, 45, 0)) * (*PlayerBonePosesTransformation)[0] * (*PlayerBonePoses)[0] );
		//SetActorRotation(FQuat(FRotator(45,45,45)));
	//}
	



}

void ATestActor::SyncPoses() {
	UE_LOG(TestActor, Warning, TEXT("SyncPoses!"));

	for (int i = 0; i < BONE_NUMS; i++)
	{
		(*PlayerBonePosesTransformation)[0] = (*PlayerBonePoses)[0].Inverse();
	}

	PoseSynced = true;
}

void ATestActor::InitPlayerBonePoses(int BoneNums)
{
	PlayerBonePoses = new TArray<FQuat>;
	for (int i = 0; i < BoneNums; i++)
	{
		PlayerBonePoses->Push(FQuat(0, 0, 0, 0));
	}

	// 初始化转换Quat
	PlayerBonePosesTransformation = new TArray<FQuat>;
	for (int i = 0; i < BoneNums; i++)
	{
		PlayerBonePosesTransformation->Push(FQuat(0, 0, 0, 0));
	}
}

void ATestActor::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ATestActor::SyncPoses);
}


void ATestActor::BeginDestroy()
{
	Super::BeginDestroy();

	delete PacketManage;
}
