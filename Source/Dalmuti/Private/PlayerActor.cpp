// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerActor.h"

// Sets default values
APlayerActor::APlayerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Hand.Init(0, 13);
	UE_LOG(LogTemp, Log, TEXT("IM HERE"));
	
}

// Called when the game starts or when spawned
void APlayerActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APlayerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 미사용중. 추후 수정하여 컨트롤러UI에 사용
void APlayerActor::UpdatePlayerWidget()
{
	if (PlayerWidget != nullptr)
	{
		for (int i = 1; i <= 13; i++)
		{
			if (PlayerWidget->GetHandText(i) && Hand.IsValidIndex(i - 1))
			{
				PlayerWidget->GetHandText(i)->SetText(FText::AsNumber(Hand[i - 1]));
			}
		}
	}
}

// 미사용중. 추후 수정하여 컨트롤러UI에 사용
void APlayerActor::TurnOnPlayerWidget()
{
	if (WidgetClass != nullptr)
	{
		PlayerWidget = Cast<UPlayerWidget>(CreateWidget(GetWorld(), WidgetClass));
		if (PlayerWidget != nullptr)
		{
			UpdatePlayerWidget();
			PlayerWidget->AddToViewport();
		}
	}
}
