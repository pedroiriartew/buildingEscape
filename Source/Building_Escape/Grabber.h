// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float Reach = 100.f;
	UPROPERTY()//Following the rules in the unreal documents. It has an explanation, its long.
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UPROPERTY()
	UInputComponent* InputComponent = nullptr;

	void Grab();
	void Release();
	void FindPhysicsHandle();
	void SetUpInputComponent();

	//Returns the distance of our grabbing hand
	FVector GetPlayersReach() const;

	//Returns the first actor in reach with a Physics body
	FHitResult GetFirstPhysicsBodyInReach() const;

	FVector GetPlayerWorldPosition() const;
};
