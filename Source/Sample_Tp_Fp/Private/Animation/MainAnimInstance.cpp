// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/MainAnimInstance.h"
#include "Character/MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


void UMainAnimInstance::NativeInitializeAnimation()
{
	if (!MyActor)
	{
		MyActor = TryGetPawnOwner();
		if (MyActor)
		{
			MyPawn = Cast<AMainCharacter>(MyActor);
		}
	}
}

void UMainAnimInstance::UpdateAnimation()
{
	if (!MyActor)
	{
		MyActor = TryGetPawnOwner();
	}
	if (MyActor)
	{
		FVector TempSpeed = MyActor->GetVelocity();
		FVector LateralSpeed = FVector(TempSpeed.X, TempSpeed.Y, 0.0f);
		Speed = LateralSpeed.Size();
		Direction = CalculateDirection(MyActor->GetVelocity(), MyActor->GetActorRotation());
		bIsInAir = MyPawn->GetMovementComponent()->IsFalling();
	}
}
