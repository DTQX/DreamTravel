
#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ObjectMacros.h"
#include "BoneContainer.h"
#include "Components/SkinnedMeshComponent.h"
#include "SkeletalAndPoseableMeshComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(SkeletalAndPoseableMeshComponent, Log, All);

class UPoseableMeshComponent;

/**
 *
 */
UCLASS(ClassGroup = ("DreamTravel"), meta = (BlueprintSpawnableComponent))
class DREAMTRAVEL_API USkeletalAndPoseableMeshComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	virtual void BeginPlay() override;

	/** Temporary array of local-space (ie relative to parent bone) rotation/translation/scale for each bone. */
	TArray<FTransform> BoneSpaceTransforms;

	FBoneContainer RequiredBones;
private:


	UFUNCTION(BlueprintCallable, Category = "Components|PoseableMesh")
		void SetBoneTransformByName(FName BoneName, const FTransform& InTransform, EBoneSpaces::Type BoneSpace);

	UFUNCTION(BlueprintCallable, Category = "Components|PoseableMesh")
		void SetBoneLocationByName(FName BoneName, FVector InLocation, EBoneSpaces::Type BoneSpace);

	UFUNCTION(BlueprintCallable, Category = "Components|PoseableMesh")
		void SetBoneRotationByName(FName BoneName, FRotator InRotation, EBoneSpaces::Type BoneSpace);

	UFUNCTION(BlueprintCallable, Category = "Components|PoseableMesh")
		void SetBoneScaleByName(FName BoneName, FVector InScale3D, EBoneSpaces::Type BoneSpace);

	UFUNCTION(BlueprintCallable, Category = "Components|PoseableMesh")
		FTransform GetBoneTransformByName(FName BoneName, EBoneSpaces::Type BoneSpace);

	UFUNCTION(BlueprintCallable, Category = "Components|PoseableMesh")
		FVector GetBoneLocationByName(FName BoneName, EBoneSpaces::Type BoneSpace);

	UFUNCTION(BlueprintCallable, Category = "Components|PoseableMesh")
		FRotator GetBoneRotationByName(FName BoneName, EBoneSpaces::Type BoneSpace);

	UFUNCTION(BlueprintCallable, Category = "Components|PoseableMesh")
		FVector GetBoneScaleByName(FName BoneName, EBoneSpaces::Type BoneSpace);

	UFUNCTION(BlueprintCallable, Category = "Components|PoseableMesh")
		void ResetBoneTransformByName(FName BoneName);

	UFUNCTION(BlueprintCallable, Category = "Components|PoseableMesh")
		void CopyPoseFromSkeletalComponent(const USkeletalMeshComponent* InComponentToCopy);

	//~ Begin USkinnedMeshComponent Interface
	virtual void RefreshBoneTransforms(FActorComponentTickFunction* TickFunction = NULL) override;
	virtual bool AllocateTransformData() override;
	//~ End USkinnedMeshComponent Interface

	bool IsRunningParallelEvaluation() const { return false; }
	/**
	 * Take the BoneSpaceTransforms array (translation vector, rotation quaternion and scale vector) and update the array of component-space bone transformation matrices (SpaceBases).
	 * It will work down hierarchy multiplying the component-space transform of the parent by the relative transform of the child.
	 * This code also applies any per-bone rotators etc. as part of the composition process
	 */
	void FillComponentSpaceTransforms();

};