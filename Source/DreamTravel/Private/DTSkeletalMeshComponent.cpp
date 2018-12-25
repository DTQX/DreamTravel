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

}

void UDTSkeletalMeshComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//float BoneMass = GetBoneMass(FName("spine_01"), false);
	//UE_LOG(LogTemp, Warning, TEXT("BoneMass:  %f"), BoneMass);
/*
	float Mass = GetMass();
	UE_LOG(LogTemp, Warning, TEXT("BoneMass:  %f"), Mass);*/

	//TArray < FTransform > BonesLocalSpace = BoneSpaceTransforms;
	/*for (size_t i = 0; i < BoneSpaceTransforms.GetTypeSize(); i++)
	

		UE_LOG(LogTemp, Warning, TEXT("BoneSpaceTransforms:  %s"), *BoneSpaceTransforms[i].ToString());
	}*/
	//UE_LOG(LogTemp, Warning, TEXT("BoneSpaceTransforms:  %s"), *BoneSpaceTransforms[GetBoneIndex(FName("lowerarm_r"))].ToString());

	//GetBoneAxis(FName("lowerarm_r"),EAxis::X);
	//UE_LOG(LogTemp, Warning, TEXT("BoneAxis x:  %s"), *GetBoneAxis(FName("lowerarm_r"), EAxis::X).ToString());
	//UE_LOG(LogTemp, Warning, TEXT("BoneAxis y:  %s"), *GetBoneAxis(FName("lowerarm_r"), EAxis::Y).ToString());
	//UE_LOG(LogTemp, Warning, TEXT("BoneAxis z:  %s"), *GetBoneAxis(FName("lowerarm_r"), EAxis::Z).ToString());

	//GetBodyInstance(FName("lowerarm_r"))->AddTorqueInRadians(FVector(0, 0, 1000000));
	//AddTorqueInRadians(FVector(0, 0, 1000000), FName("lowerarm_r"), false);

}







