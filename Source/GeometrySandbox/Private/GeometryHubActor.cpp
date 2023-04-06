// Fill out your copyright notice in the Description page of Project Settings.


#include "GeometryHubActor.h"
#include "Engine/World.h"

// Sets default values
AGeometryHubActor::AGeometryHubActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGeometryHubActor::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = AActor::GetWorld();
	if (World)
	{
		for(uint8 i = 0; i < 10; i++)
		{
			GeometryPayload.SetGeometryData(
				50, 2.0f + i,
				EMovementType::Sin, FLinearColor::Black, NULL);

			GeometryPayload.SetColorChangerData(
				10000 + 100 * i, NULL, NULL,
				{}, false);

			GeometryPayload.SetTransform(FRotator::ZeroRotator, FVector(0.0f, 300.0f + i * 100, 300.0f));

			ABaseGeometryActor* Geometry = World->SpawnActor<ABaseGeometryActor>(GeometryPayload.GeometryClass, GeometryPayload.Transform);
			Geometry->SetColorChangerData(GeometryPayload.ColorChangerData);
			Geometry->SetGeometryData(GeometryPayload.GeometryData);
			
			Geometry->FinishSpawning(GeometryPayload.Transform);
		}
	}

	for (const auto elem: GeometryPayloads)
	{
		ABaseGeometryActor* Geometry = World->SpawnActor<ABaseGeometryActor>(elem.GeometryClass, elem.Transform);
		Geometry->SetGeometryData(elem.GeometryData);
		Geometry->SetColorChangerData(elem.ColorChangerData);
			
		Geometry->FinishSpawning(elem.Transform);
	}
}

// Called every frame
void AGeometryHubActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

