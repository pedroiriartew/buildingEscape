// Fill out your copyright notice in the Description page of Project Settings.

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
	//Checking for Physics Handle Component and Input Component
	FindPhysicsHandle();
	SetUpInputComponent();
}

void UGrabber::SetUpInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Nice Input Component Bro"));
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("The Object %s has no Input Component attached to it. Weird, right?"), *GetOwner()->GetName());
	}
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("Nice PhysicsHandle Component Bro"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("The Object %s has no PhysicsHandle Component attached to it. Saved you from unemployment, thank me later."), *GetOwner()->GetName());
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector PlayerViewpointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewpointLocation,
		OUT PlayerViewPointRotation
	);

	//We add my location to the reach plus the rotation vector(It gives the direction)
	FVector LineTraceEnd = PlayerViewpointLocation + PlayerViewPointRotation.Vector() * Reach;

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Error, TEXT("Grabbbb"));

	FVector PlayerViewpointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewpointLocation,
		OUT PlayerViewPointRotation
	);

	//We add my location to the reach plus the rotation vector(It gives the direction)
	FVector LineTraceEnd = PlayerViewpointLocation + PlayerViewPointRotation.Vector() * Reach;

	//Try to reach any actor with physics body collision channel set.
	FHitResult ObjectGrabbed = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = ObjectGrabbed.GetComponent();

	if (ObjectGrabbed.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None,
			LineTraceEnd
		);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Error, TEXT("RILISSSSSS"));

	//Release what we grabbed

	FHitResult ObjectGrabbed = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = ObjectGrabbed.GetComponent();

	if (ObjectGrabbed.GetActor())
	{
		PhysicsHandle->ReleaseComponent();
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FVector PlayerViewpointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewpointLocation,
		OUT PlayerViewPointRotation
	);

	//We add my location to the reach plus the rotation vector(It gives the direction)
	FVector LineTraceEnd = PlayerViewpointLocation + PlayerViewPointRotation.Vector() * Reach;

	//Reaching out -> Raycast to a certain distance
	FHitResult Hit;

	//Our Trace parameters are its tag, if it recognize complex traces and what actors it ignores, in this case is our player's actor.
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewpointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), //Parametros de los objetos que quiero. En este caso el Object Type aka collision channel
		TraceParameters
	);

	AActor* ActorHit = Hit.GetActor();

	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("The actor's name is: %s"), *ActorHit->GetName());
	}

	return Hit;// We are going to do something with this actor hit.
}


/*
//Draw a Line from our Player showing the reach;

DrawDebugLine(
	GetWorld(),
	PlayerViewpointLocation,
	LineTraceEnd,
	FColor(0, 255, 0),
	false,
	0.f,
	0,
	5.f
);*/