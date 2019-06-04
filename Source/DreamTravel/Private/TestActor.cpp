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
	//Colculation();

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

	// 如果是测试则注释return，不然这里使用PacketManage->Connect()会与其他地方冲突（端口只能保持一个连接）
	// return;

	// 连接
	if (PacketManage->Connect(DeltaTime) == false) {
		return;
	}

	//UE_LOG(TestActor, Warning, TEXT("ATestActor TickComponent"));


	PacketManage->UpdatePlayerPose(PlayerBonePoses, BONE_NUMS);
	for (int i = 0; i < BONE_NUMS; i++)
	{
		UE_LOG(TestActor, Warning, TEXT("index: %d ,PlayerBonePoses: %s, PlayerBonePosesTransformation: %s, multi: %s"),i,
			*((*PlayerBonePoses)[i]).ToString(), *((*PlayerBonePosesTransformation)[i]).ToString(), 
			*((*PlayerBonePoses)[i] * (*PlayerBonePosesTransformation)[i]).ToString());
		//cubes[i]->SetAllPhysicsRotation((*PlayerBonePoses)[i] * (*PlayerBonePosesTransformation)[i]);
	}
    //UE_LOG(TestActor, Warning, TEXT("PlayerBonePoses size: %f, SizeSquared: %f"),(*PlayerBonePoses)[0].Size(), (*PlayerBonePoses)[0].SizeSquared());

	// 打印旋转轴和角度
	
	////(FQuat(0.034606934, 0.160156250, 0.979675293, 0.115783691) * FQuat(0.034606934, 0.160156250, 0.979675293, 0.115783691).Inverse()).ToAxisAndAngle(MyAxis, Angle);
 //   UE_LOG(TestActor, Warning, TEXT("Delta Euler : %s"), *((*PlayerBonePoses)[0] * (*LastPlayerBonePoses)[0].Inverse()).Euler().ToString());
    //UE_LOG(TestActor, Warning, TEXT("PlayerBonePoses Euler : %s"), *((*PlayerBonePoses)[0]).Euler().ToString());
 //   //UE_LOG(TestActor, Warning, TEXT("FQuat : %s, Angle: %f"),*MyAxis.ToString(), Angle);
	if (MyDeltaTime <0 )
	{
		FVector MyAxis;
		float Angle;
		((*PlayerBonePoses)[0] * (*LastPlayerBonePoses)[0].Inverse()).ToAxisAndAngle(MyAxis, Angle);
		UE_LOG(TestActor, Warning, TEXT("Delta Aixs : %s, Angle: %f"),*MyAxis.ToString(), Angle);
		MyDeltaTime = 0;
		(*LastPlayerBonePoses)[0] = (*PlayerBonePoses)[0];

	}
	else {
		MyDeltaTime += DeltaTime;
	}
	//UE_LOG(TestActor, Warning, TEXT("PlayerBonePoses %s"), *((*PlayerBonePoses)[0]).ToString());
	//UE_LOG(TestActor, Warning, TEXT("Delta Quat %s"), *((*PlayerBonePoses)[0] * (*LastPlayerBonePoses)[0].Inverse()).Euler().ToString());
	//UE_LOG(TestActor, Warning, TEXT("---- %s"), *((*PlayerBonePoses)[0] * (*PlayerBonePoses)[0].Inverse()).ToString());
	//UE_LOG(TestActor, Warning, TEXT("PlayerBonePosesTransformation %s"), *((*PlayerBonePosesTransformation)[0]).ToString());
	//UE_LOG(TestActor, Warning, TEXT("PlayerBonePosesTransformation %s"), *((*PlayerBonePosesTransformation)[0] * (*PlayerBonePoses)[0]).Euler().ToString());


	//cubes[0]->SetAllPhysicsRotation((*PlayerBonePoses)[0]);
	//if (PoseSynced){
	/*	SetActorRotation( (
			((*PlayerBonePosesTransformation)[0] * (*PlayerBonePoses)[0] )
			*
			(((*PlayerBonePosesTransformation)[0] * (*LastPlayerBonePoses)[0]).Inverse()) ) * GetActorQuat());*/

		/*SetActorRotation((
			
			(*PlayerBonePoses)[0]
			*
			((*LastPlayerBonePoses)[0].Inverse())) * (*PlayerBonePosesTransformation)[0]  * GetActorQuat());*/

		// 通过轴来转换
		/*FVector MyAxis;
		float Angle;
		((*PlayerBonePoses)[0] * ((*LastPlayerBonePoses)[0].Inverse())).ToAxisAndAngle(MyAxis, Angle);
		FVector Axis2 = (*LastPlayerBonePoses)[0] * ((*PlayerBonePosesTransformation)[0]) * MyAxis;
		SetActorRotation(FQuat(Axis2, Angle) * GetActorQuat());*/


		/*FVector tmp = ((*PlayerBonePoses)[0] * ((*LastPlayerBonePoses)[0].Inverse())).Euler();
		SetActorRotation( FQuat((*LastPlayerBonePoses)[0] * ((*PlayerBonePosesTransformation)[0]) * FVector(1, 0, 0), tmp.X / 180 * 3.14) *
			FQuat((*LastPlayerBonePoses)[0] * ((*PlayerBonePosesTransformation)[0]) * FVector(0, 1, 0), tmp.Y / 180 * 3.14) *
			FQuat((*LastPlayerBonePoses)[0] * ((*PlayerBonePosesTransformation)[0]) * FVector(0, 0, 1), tmp.Z / 180 * 3.14) * GetActorQuat());*/

		//SetActorRotation((*PlayerBonePoses)[0] );

		// 正式
		//FVector tmp = (*PlayerBonePoses)[0] * (*PlayerBonePosesTransformation)[0].Euler();
		
		// 重要！！！  可以让mpu初始状态在X轴向上的情况下完美驱动骨骼 只需同步PlayerBonePosesTransformation
	// 正确
	//UE_LOG(TestActor, Warning, TEXT("Fquat %s"), *(FQuat(FRotator(90, 0, 0)) * (*PlayerBonePosesTransformation)[0] * (*PlayerBonePoses)[0] * (-1)  * FQuat(FRotator(-90, 0, 0))).Euler().ToString());

		//SetActorRotation(FQuat(FRotator(90, 0, 0)) * (*PlayerBonePosesTransformation)[0] * (*PlayerBonePoses)[0] * (-1)  * FQuat(FRotator(-90, 0, 0)));
	// 正确
		//SetActorRotation(FQuat(FRotator(90, 0, 0)) * (*PlayerBonePosesTransformation)[0] * (*PlayerBonePoses)[0] * (-1)  * (FQuat(FRotator(90, 0, 0)).Inverse()));

	// 正确
		//SetActorRotation(FQuat(FRotator(45, 0, 0)) * (*PlayerBonePosesTransformation)[0] * (*PlayerBonePoses)[0]  * (-1) * FQuat(FRotator(-45, 0, 0)));
	// 重要！！！  可以让mpu初始状态在X轴向上的情况下完美驱动骨骼 只需同步PlayerBonePosesTransformation  正确	
	//UE_LOG(TestActor, Warning, TEXT("FQuat : %s"), *((*PlayerBonePosesTransformation)[0] * (PlayerBonePosesTransformation2.Inverse()) * (*PlayerBonePosesTransformation)[0] * (*PlayerBonePoses)[0] * (-1) * ((*PlayerBonePosesTransformation)[0] * PlayerBonePosesTransformation2).Inverse()).Euler().ToString());
		cubes[5]->SetAllPhysicsRotation((*PlayerBonePosesTransformation)[5] * PlayerBonePosesTransformation2 * (PlayerBonePosesTransformation2.Inverse()) * (*PlayerBonePoses)[5] * (-1) * ((*PlayerBonePosesTransformation)[5] * PlayerBonePosesTransformation2).Inverse());
		
		//UE_LOG(TestActor, Warning, TEXT("FQuat Euler : %s, %s"), *((*PlayerBonePosesTransformation)[0] * PlayerBonePosesTransformation2).Euler().ToString(), *(  PlayerBonePosesTransformation2 * (*PlayerBonePosesTransformation)[0]).Euler().ToString());

		// 正确
		//SetActorRotation((*PlayerBonePoses)[0] * (*PlayerBonePosesTransformation)[0] );
		//SetActorRotation(FQuat(FRotator(0, 90, 0)));
		/*FVector MyAxis;
		float Angle;
		FQuat(FRotator(0, 90, 0)).ToAxisAndAngle(MyAxis, Angle);*/
		//UE_LOG(TestActor, Warning, TEXT("FQuat Axis : %s, Angle: %f"),*MyAxis.ToString(), Angle);
	

		//UE_LOG(TestActor, Warning, TEXT("FQuat %s,  %s"), *(FQuat(FRotator(0, 90, 0)) * (*PlayerBonePosesTransformation)[0] * (*PlayerBonePoses)[0]).ToString(), * ((*PlayerBonePosesTransformation)[0] * (*PlayerBonePoses)[0]).ToString());
	// 正确
		//SetActorRotation( (*PlayerBonePosesTransformation)[0] * (*PlayerBonePoses)[0] );
	//}

		(*LastPlayerBonePoses)[0] = (*PlayerBonePoses)[0];


}

void ATestActor::Colculation() {
	FQuat q1 = FQuat::MakeFromEuler(FVector(45, 0, 0));
	FQuat q2 = FQuat::MakeFromEuler(FVector(45, 45, 0));

	FVector MyAxis;
	float Angle;

	(q2 * (q1.Inverse())).ToAxisAndAngle(MyAxis, Angle);
	//(q2 ).ToAxisAndAngle(MyAxis, Angle);                
	UE_LOG(TestActor, Warning, TEXT("Delta Aixs : %s, Angle: %f"),*MyAxis.ToString(), Angle);

	UE_LOG(TestActor, Warning, TEXT("Delta Euler : %s"), *((q2 * (q1.Inverse()))).Euler().ToString());

}

void ATestActor::SyncPoses() {
	UE_LOG(TestActor, Warning, TEXT("SyncPoses!"));

	for (int i = 0; i < BONE_NUMS; i++)
	{
		(*PlayerBonePosesTransformation)[i] = (*PlayerBonePoses)[i].Inverse();
	}
	//SetActorRotation(FQuat(FRotator(0, 0, 0)));
	PoseSynced = true;
}

void ATestActor::SyncPoses2() {
	UE_LOG(TestActor, Warning, TEXT("SyncPoses2!"));

	
	
	PlayerBonePosesTransformation2= (*PlayerBonePoses)[5] ;
	//PlayerBonePosesTransformation2= (*PlayerBonePoses)[0] * (*PlayerBonePosesTransformation)[0];
	
	
}

void ATestActor::InitPlayerBonePoses(int BoneNums)
{
	PlayerBonePoses = new TArray<FQuat>;
	for (int i = 0; i < BoneNums; i++)
	{
		PlayerBonePoses->Push(FQuat(0 , 0, 0, 1));
	}

	// 初始化转换Quat
	LastPlayerBonePoses = new TArray<FQuat>;
	for (int i = 0; i < BoneNums; i++)
	{
		LastPlayerBonePoses->Push(FQuat(0, 0, 0, 1));
	}
	
	PlayerBonePosesTransformation = new TArray<FQuat>;
	for (int i = 0; i < BoneNums; i++)
	{
		PlayerBonePosesTransformation->Push(FQuat(0, 0, 0, 1));
	}
}

void ATestActor::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ATestActor::SyncPoses);
	InputComponent->BindAction("Fire", IE_Pressed, this, &ATestActor::SyncPoses2);
}


void ATestActor::BeginDestroy()
{
	Super::BeginDestroy();

	delete PacketManage;
}
