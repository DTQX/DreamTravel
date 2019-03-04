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

	// 测试类计算
	Colculation();

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

	// 如果是测试则注释return，不然这里使用PacketManage->Connect()会与其他地方冲突（端口只能保持一个连接）
	//return;

	// 连接
	if (PacketManage->Connect(DeltaTime) == false) {
		return;
	}

	//UE_LOG(TestActor, Warning, TEXT("ATestActor TickComponent"));


	PacketManage->UpdatePlayerPose(PlayerBonePoses, BONE_NUMS);
	for (int i = 0; i < BONE_NUMS; i++)
	{
		//UE_LOG(TestActor, Warning, TEXT("%s"), *((*PlayerBonePoses)[i]).ToString());
		//cubes[i]->SetAllPhysicsRotation((*PlayerBonePoses)[i]);

	}
    //UE_LOG(TestActor, Warning, TEXT("PlayerBonePoses size: %f, SizeSquared: %f"),(*PlayerBonePoses)[0].Size(), (*PlayerBonePoses)[0].SizeSquared());

	// 打印旋转轴和角度
	/*FVector MyAxis;
	float Angle;*/
	//((*PlayerBonePoses)[0] * (*LastPlayerBonePoses)[0].Inverse()).ToAxisAndAngle(MyAxis, Angle);
	////(FQuat(0.034606934, 0.160156250, 0.979675293, 0.115783691) * FQuat(0.034606934, 0.160156250, 0.979675293, 0.115783691).Inverse()).ToAxisAndAngle(MyAxis, Angle);
 //   UE_LOG(TestActor, Warning, TEXT("Delta Aixs : %s, Angle: %f"),*MyAxis.ToString(), Angle);
 //   UE_LOG(TestActor, Warning, TEXT("Delta Euler : %s"), *((*PlayerBonePoses)[0] * (*LastPlayerBonePoses)[0].Inverse()).Euler().ToString());
 //   UE_LOG(TestActor, Warning, TEXT("PlayerBonePoses Euler : %s"), *((*PlayerBonePoses)[0]).Euler().ToString());
 //   //UE_LOG(TestActor, Warning, TEXT("FQuat : %s, Angle: %f"),*MyAxis.ToString(), Angle);

	//UE_LOG(TestActor, Warning, TEXT("PlayerBonePoses %s"), *((*PlayerBonePoses)[0]).ToString());
	//UE_LOG(TestActor, Warning, TEXT("PlayerBonePosesTransformation %s"), *((*PlayerBonePosesTransformation)[0]).ToString());
	//UE_LOG(TestActor, Warning, TEXT("PlayerBonePosesTransformation %s"), *((*PlayerBonePosesTransformation)[0] * (*PlayerBonePoses)[0]).ToString());

	(*LastPlayerBonePoses)[0] = (*PlayerBonePoses)[0];

	//cubes[0]->SetAllPhysicsRotation((*PlayerBonePoses)[0]);
	//if (PoseSynced){
		SetActorRotation( (*PlayerBonePosesTransformation)[0] * (*PlayerBonePoses)[0] );
		//SetActorRotation((*PlayerBonePoses)[0] );

		// 正式
		//SetActorRotation(FQuat(FRotator(0, 45, 0)) * (*PlayerBonePosesTransformation)[0] * (*PlayerBonePoses)[0] );
	//}

}

void ATestActor::Colculation() {
	FQuat q1 = FQuat::MakeFromEuler(FVector(0, 0, 90));
	FQuat q2 = FQuat::MakeFromEuler(FVector(45, 0, 90));

	FVector MyAxis;
	float Angle;

	(q2 * (q1.Inverse())).ToAxisAndAngle(MyAxis, Angle);
	UE_LOG(TestActor, Warning, TEXT("Delta Aixs : %s, Angle: %f"),*MyAxis.ToString(), Angle);

	UE_LOG(TestActor, Warning, TEXT("Delta Euler : %s"), *((q2 * (q1.Inverse()))).Euler().ToString());

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
	LastPlayerBonePoses = new TArray<FQuat>;
	for (int i = 0; i < BoneNums; i++)
	{
		LastPlayerBonePoses->Push(FQuat(0, 0, 0, 0));
	}
	
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
