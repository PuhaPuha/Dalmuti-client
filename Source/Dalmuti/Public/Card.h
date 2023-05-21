// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperSpriteComponent.h"
#include "Card.generated.h"

UCLASS()
class DALMUTI_API ACard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACard();

	bool bController = false;
	bool bFront = false;	// true면 카드가 앞면
	//AActor* Owner;	// 카드를 소유한 액터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Info)
	int32 number = 0;	// 카드의 번호
	bool bSelected = false;
	//bool bLeft = true;
	float move_speed = 1;
	bool bFliping = false;
	float flip_sum = 0.f;

	// 목표위치 FVector 선언, z오더 포함
	FVector Destination = FVector(0, 0, 0);
	FRotator HeadingDirection = FRotator(0, 0, 0);

	//float offset_y = 0.f;
	//float offset_x = 0.f;
	//float current_moved_y = 0.f;
	//float current_moved_x = 0.f;
	float rotate_duration = 2.f;

	UPaperSpriteComponent* BackSprite = nullptr;
	UPaperSpriteComponent* FrontSprite = nullptr;

	UFUNCTION(BlueprintCallable)
	void Clicked();	// 카드 클릭시 할 동작. 목표위치 변경, bSelected 변경
	//void SetFront(int32 number);
	//UFUNCTION(BlueprintCallable)
	//	void Flip(float Duration);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
