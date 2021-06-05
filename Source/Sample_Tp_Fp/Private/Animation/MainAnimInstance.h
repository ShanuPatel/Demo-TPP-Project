// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainAnimInstance.generated.h"

/**
 *
 */
UCLASS()
class UMainAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintReadonly, Category = "Movement stats", meta = (AllowPrivateAccess = "true"))
		float Direction;
	UPROPERTY(BlueprintReadonly, Category = "Movement stats", meta = (AllowPrivateAccess = "true"))
		float Speed;
	UPROPERTY(BlueprintReadonly, Category = "Movement stats", meta = (AllowPrivateAccess = "true"))
		bool bIsInAir;

	UPROPERTY(BlueprintReadonly, Category = "Movement stats", meta = (AllowPrivateAccess = "true"))
		AActor* MyActor;
	UPROPERTY(BlueprintReadonly, Category = "Movement stats", meta = (AllowPrivateAccess = "true"))

		class AMainCharacter* MyPawn;
	virtual void NativeInitializeAnimation() override;
	void UpdateAnimation();

public:

	UFUNCTION(BlueprintCallable, Category = "PlayerAnimation")
		void UpdateAnim() { UpdateAnimation(); }
};
