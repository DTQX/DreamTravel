// Fill out your copyright notice in the Description page of Project Settings.

#include "DTSkeletalMeshComponent.h"



void UDTSkeletalMeshComponent::InitializeComponent()
{
	Super::InitializeComponent();

	InitAnim(false);
}

void UDTSkeletalMeshComponent::BeginPlay()
{
	Super::BeginPlay();

	//连接DreamTravel，不断尝试连接，不连接成功，不进行下一步，或者直接退出
	//（不是算法 TODO 新建DTClass，让SerialClass只提供读写服务。）

	//初始化DT，生成Avatar，提示玩家与avatar保存同一姿态，记录Avatar与Player的方位角差（注意坐标轴方向不同，考虑能否使用同一的方法），

	//获取每个关节和其所有子关节的质量，并保存。

	//开启Avatar物理模拟，开启碰撞模拟，开启模拟重力
}

void UDTSkeletalMeshComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//float BoneMass = GetBoneMass(FName("spine_01"), false);
	//UE_LOG(LogTemp, Warning, TEXT("BoneMass:  %f"), BoneMass);
	/*

	//在物理模拟前，读取最新play状态，获取avatar最新状态，计算当前方位角差，并施加力，通知DT去都下一组数据，以让在下一tick时可直接获取到player数据

	//在物理模拟结束后，计算当前avatar状态，并供下一tick开始时直接使用。

}







