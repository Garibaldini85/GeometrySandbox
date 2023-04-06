// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseGeometryActor.h"

#include "GeometryHubActor.generated.h"

USTRUCT(BlueprintType)
struct FGeometryPayload
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseGeometryActor> GeometryClass;

	UPROPERTY(EditAnywhere)
	FGeometryData GeometryData;

	UPROPERTY(EditAnywhere)
	FColorChangerData ColorChangerData;

	UPROPERTY(EditAnywhere)
	FTransform Transform;

	void SetGeometryData(const float& Amplitude = NULL, const float& Frequency = NULL,
	    EMovementType MoveType = EMovementType::Undefined, const FLinearColor& Color = FLinearColor::Black,
		const float& TimerRate = NULL)
	{
		if (Amplitude != NULL) 	GeometryData.Amplitude = Amplitude;
		if (Frequency != NULL)	GeometryData.Frequency = Frequency;
		if (TimerRate != NULL)	GeometryData.TimerRate = TimerRate;
		if (Color     != FLinearColor::Black)		GeometryData.Color     = Color;
		if (MoveType  != EMovementType::Undefined)	GeometryData.MoveType  = MoveType;
	}

	void SetColorChangerData(const int& Interval = NULL, const float& Step = NULL,
		const float& CurrentStep = NULL, const TArray<uint32>& ArraIntColor= {},
		const bool& IsLogging = false)
	{
		if (Interval != NULL) 		 ColorChangerData.Interval    = Interval;
		if (Step != NULL)			 ColorChangerData.Step        = Step;
		if (CurrentStep != NULL)	 ColorChangerData.CurrentStep = CurrentStep;
		if (IsLogging  != false)	 ColorChangerData.IsLogging     = IsLogging;
		if (ArraIntColor.Num() != 0) ColorChangerData.ArrayIntColor = ArraIntColor;
	}

	void SetTransform(const FRotator& Rotator, const FVector& Vector)
	{
		Transform.SetRotation(Rotator.Quaternion());
		Transform.SetLocation(Vector);
	}
};

UCLASS()
class GEOMETRYSANDBOX_API AGeometryHubActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGeometryHubActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "Geometry Payload Data")
	FGeometryPayload GeometryPayload;
	
	UPROPERTY(EditAnywhere, Category = "Geometry Payload Data")
	TArray<FGeometryPayload> GeometryPayloads;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
