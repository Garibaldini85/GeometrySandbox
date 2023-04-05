// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGeometryActor.h"

#include "Engine/Engine.h"
#include "TimerManager.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Math/Color.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseGeometry, All, All)

// Sets default values
ABaseGeometryActor::ABaseGeometryActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject <UStaticMeshComponent>("BaseMash");
	SetRootComponent(BaseMesh);
}

// Called when the game starts or when spawned
void ABaseGeometryActor::BeginPlay() 
{
	Super::BeginPlay();
	ColorChangerData.BeginPlay();
	
	UE_LOG(LogBaseGeometry, Error, TEXT("Hello Unreal"));

	InitialLocation = GetActorLocation();

	uint32 iColor = 0x146152;
	FColor fColor(iColor);
	GeometryData.Color = FLinearColor(fColor);
	SetColor(GeometryData.Color);
	//GetWorldTimerManager().SetTimer(TimerHandle, this, &ABaseGeometryActor::OnTimerFired, GeometryData.TimerRate, true);
	GetWorldTimerManager().SetTimer(TimerColorChanger, this, &ABaseGeometryActor::OnTriggeredColorChanger, ColorChangerData.Interval / 1000000.0f, true);
	//printStringTypes();
	//printTypes();
	PrintTransform();
}

// Called every frame
void ABaseGeometryActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandlerMovement();
}

void ABaseGeometryActor::HandlerMovement()
{
	switch (GeometryData.MoveType) {
		case EMovementType::Sin: {
		//z = z0 + amplitude + sin(frec * t)
			FVector CurrentLocation = GetActorLocation();
			float Time = GetWorld()->GetTimeSeconds();
			CurrentLocation.Z = InitialLocation.Z + GeometryData.Amplitude + FMath::Sin(GeometryData.Frequency * Time);

			SetActorLocation(CurrentLocation);
			break;
		}
		case EMovementType::Static: break;
		default: break;
	}
}

void ABaseGeometryActor::OnTimerFired()
{
	GeometryData.Color = FLinearColor::MakeRandomColor();
	UE_LOG(LogBaseGeometry, Warning, TEXT("Color: %s"), *GeometryData.Color.ToString());
	SetColor(GeometryData.Color);
}

void ABaseGeometryActor::OnTriggeredColorChanger()
{
	ColorChangerData.CurrentStep = ColorChangerData.CurrentStep + ColorChangerData.Step;

	if (ColorChangerData.CurrentStep > 1.0f) {
		ColorChangerData.Index = !ColorChangerData.ReverseIndex ? ColorChangerData.Index + 1 : ColorChangerData.Index - 1;
		ColorChangerData.CurrentStep = 0.0f;
	}

	if (ColorChangerData.Index >= ColorChangerData.ArrayLinearColor.Num() - 1 && !ColorChangerData.ReverseIndex) {
		ColorChangerData.ReverseIndex = true;
		ColorChangerData.Index = ColorChangerData.ArrayLinearColor.Num() - 1;
		ColorChangerData.CurrentStep = 0.0f;
	} else if (ColorChangerData.Index <= 0 && ColorChangerData.ReverseIndex) {
		ColorChangerData.ReverseIndex = false;
		ColorChangerData.Index = 0;
		ColorChangerData.CurrentStep = 0.0f;
	}

	uint8 ind = !ColorChangerData.ReverseIndex ? ColorChangerData.Index + 1 : ColorChangerData.Index - 1;
	if (ColorChangerData.IsLogging)
		UE_LOG(LogBaseGeometry, Error, TEXT("%d %d %d %f %d"), ColorChangerData.Index, ind, ColorChangerData.ReverseIndex, ColorChangerData.CurrentStep, ColorChangerData.CurrentStep < 0.0f);
	GeometryData.Color = FLinearColor::LerpUsingHSV(ColorChangerData.ArrayLinearColor[ColorChangerData.Index], ColorChangerData.ArrayLinearColor[ind], ColorChangerData.CurrentStep);
	SetColor(GeometryData.Color);
}


void ABaseGeometryActor::PrintTypes()
{
	UE_LOG(LogBaseGeometry, Warning, TEXT("Actor name: %s"), *GetName());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Weapons num: %d, kills num: %i"), WeaponsNum, KillsNum);
	UE_LOG(LogBaseGeometry, Warning, TEXT("Health: %f"), Health);
	UE_LOG(LogBaseGeometry, Warning, TEXT("isDead: %d"), isDead);
	UE_LOG(LogBaseGeometry, Warning, TEXT("HasWeapon: %d"), static_cast<int>(HasWeapon));
}

void ABaseGeometryActor::PrintStringTypes()
{
	UE_LOG(LogBaseGeometry, Warning, TEXT("Name: %s"), *Name);

	FString WeaponsNumStr = "Weapons num = " + FString::FromInt(WeaponsNum);
	FString HealthStr = "Health = " + FString::SanitizeFloat(Health);
	FString IsDeadStr = "Is dead = " + FString(isDead ? "true" : "false");

	FString Stat =
	FString::Printf(TEXT(" == All Stat == \n\t%s\n\t%s\n\t%s\n"), *WeaponsNumStr, *HealthStr, *IsDeadStr);
	UE_LOG(LogBaseGeometry, Display, TEXT("%s"), *Stat);

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, Name);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, Stat, true, FVector2D(1.5f, 2.0f));

}

void ABaseGeometryActor::PrintTransform()
{
	FTransform transform = GetActorTransform();
	FVector location = transform.GetLocation();
	FRotator rotator = transform.Rotator();
	FVector scale    = transform.GetScale3D();

	UE_LOG(LogBaseGeometry, Warning, TEXT("Location: %s"), *location.ToString());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Rotator : %s"), *rotator.ToString());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Scale   : %s"), *scale.ToString());

	UE_LOG(LogBaseGeometry, Warning, TEXT("Transform: %s"), *transform.ToHumanReadableString());
}

void ABaseGeometryActor::SetColor(const FLinearColor& Color)
{
	if (ColorChangerData.IsLogging)
		UE_LOG(LogBaseGeometry, Warning, TEXT("New Color: %s"), *Color.ToString());

	UMaterialInstanceDynamic* DynMaterial = BaseMesh->CreateAndSetMaterialInstanceDynamic(0);
	if (DynMaterial) {
		DynMaterial->SetVectorParameterValue("Color", Color);
	}
}

