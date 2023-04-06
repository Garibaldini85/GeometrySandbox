// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseGeometryActor.generated.h"

UENUM(BlueprintType)
enum class EMovementType : uint8
{
	Sin,
	Static,
	Undefined
};

USTRUCT(BlueprintType)
struct FGeometryData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	float Amplitude = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Frequency = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	EMovementType MoveType = EMovementType::Static;

	UPROPERTY(EditAnywhere, Category = "Design")
	FLinearColor Color = FLinearColor::Black;

	UPROPERTY(EditAnywhere, Category = "Design")
	float TimerRate = 3.0f;
};

USTRUCT(BlueprintType)
struct FColorChangerData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Timer")
	uint32 Interval = 10000; //mcsec
	
	UPROPERTY(EditAnywhere, Category = "Timer")
	float Step = 0.01f;

	UPROPERTY(BlueprintReadOnly, Category = "Timer")
	float CurrentStep = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Color")
	TArray<uint32> ArrayIntColor = { 0x000000, 0xffffff };
	//TArray<uint32> ArrayIntColor = {};

	UPROPERTY(BlueprintReadOnly)
	TArray<FLinearColor> ArrayLinearColor;

	UPROPERTY(EditAnywhere, Category = "Color")
	uint8 Index = 0;

	UPROPERTY(EditAnywhere, Category = "Color")
	bool ReverseIndex = false;

	UPROPERTY(EditAnywhere, Category = "Color")
	bool IsLogging = false;

	FColorChangerData()
	{
		for (auto elem: ArrayIntColor) {
			ArrayLinearColor.Add(FLinearColor(FColor(elem)));
		}
	}
	
	void BeginPlay() {
		if (ArrayIntColor.Num() <= 2)
		{ return; }
		TArray<FLinearColor> util;
		
		int begin = ArrayIntColor.Num() > 2 ? 2 : 0,
			end = ArrayIntColor.Num();
		
		for (int i = begin; i < end; i++) {
			util.Add(FLinearColor(FColor(ArrayIntColor[i])));
		}

		ArrayLinearColor = util;
	}
};

UCLASS()
class GEOMETRYSANDBOX_API ABaseGeometryActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseGeometryActor();

	void SetColorChangerData (const FColorChangerData& Data)
	{
		ColorChangerData = Data;
	}

	void SetGeometryData (const FGeometryData& Data)
	{
		GeometryData = Data;
	}

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

protected:
	UPROPERTY(EditAnywhere, Category = "Geometry Data")
	FGeometryData GeometryData;

	UPROPERTY(EditAnywhere, Category = "Color Changer")
	FColorChangerData ColorChangerData;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	int32 WeaponsNum = 7;

	UPROPERTY(EditDefaultsOnly, Category = "Stat")
	int32 KillsNum = 0;

	UPROPERTY(EditInstanceOnly, Category = "Health")
	float Health = 34.43445f;

	UPROPERTY(EditAnywhere, Category = "Health")
	bool isDead = false;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	bool HasWeapon = true;

	FString Name = "John Connor";

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void PrintTypes();
	void PrintStringTypes();
	void PrintTransform();

	void SetColor(const FLinearColor& Color);

	void HandlerMovement();

	void OnTimerFired();
	void OnTriggeredColorChanger();

	FVector InitialLocation;
	FTimerHandle TimerHandle, TimerColorChanger;
};
