// Fill out your copyright notice in the Description page of Project Settings.


#include "Card.h"

// Sets default values
ACard::ACard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ACard::Clicked()
{
	UE_LOG(LogTemp, Log, TEXT("Clicked"));
	if (!bController)
		return;

	if (!bSelected)
	{
		bSelected = true;
		Destination.X += 300;
	}

	else
	{
		bSelected = false;
		Destination.X -= 300;
	}
}

// Called when the game starts or when spawned
void ACard::BeginPlay()
{
	Super::BeginPlay();
	TArray<UPaperSpriteComponent*> SpriteComponents;
	GetComponents<UPaperSpriteComponent>(SpriteComponents);
	for (UPaperSpriteComponent* SpriteComponent : SpriteComponents)
	{
		if (!BackSprite)
		{
			BackSprite = SpriteComponent;
		}
		else
		{
			FrontSprite = SpriteComponent;
			break;
		}
	}
}
/*
void ACard::Flip(float Duration)
{

	// 회전 애니메이션 시작
	bFliping = true;
	float RotationSpeed = 180.f / Duration;
	float RotationAngle = 180.f;
	float ElapsedTime = 0.f;

	GetWorldTimerManager().SetTimer(FlipTimerHandle, [this, RotationSpeed, RotationAngle, Duration, &ElapsedTime]()
		{
			ElapsedTime += GetWorld()->GetDeltaSeconds();

			if (ElapsedTime >= Duration)
			{
				// 회전 애니메이션 종료
				SetActorRotation(FRotator(0.f, RotationAngle, 0.f));
				bFliping = false;
				return;
			}

			FRotator NewRotation = GetActorRotation();
			float DeltaRotation = GetWorld()->GetDeltaSeconds() * RotationSpeed;
			NewRotation.Roll += DeltaRotation;

			if (NewRotation.Roll > RotationAngle)
			{
				NewRotation.Roll = RotationAngle;
			}

			SetActorRotation(NewRotation);
		}, 0.01f, true);
}
*/
// Called every frame
void ACard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 목표 위치로 lerp

	// bFliping이 true면, 카드를 뒤집고 bFliping을 false로 바꿈
	if (bFliping)
	{
		FRotator NewRotation = GetActorRotation();
		float RotationSpeed = 180.f / rotate_duration;

		NewRotation.Roll += DeltaTime * RotationSpeed;
		flip_sum += DeltaTime * RotationSpeed;

		if (NewRotation.Roll > -89.f && NewRotation.Roll < 90.f)
		{
			BackSprite->SetVisibility(false);
			FrontSprite->SetVisibility(true);
		}

		else if ((NewRotation.Roll > 90.f || NewRotation.Roll < -90.f))
		{
			BackSprite->SetVisibility(true);
			FrontSprite->SetVisibility(false);
		}

		if (flip_sum > 175.f && (NewRotation.Roll > 359.f || NewRotation.Roll < 1.f))
		{
			NewRotation.Roll = 0.f;
			bFliping = false;
			flip_sum = 0.f;
		}

		else if (flip_sum > 175.f && (NewRotation.Roll > 179.f || NewRotation.Roll < 181.f))
		{
			NewRotation.Roll = 180.f;
			bFliping = false;
			flip_sum = 0.f;
		}

		SetActorRotation(NewRotation);

	}

	// 목표 위치로 이동. fliping 중에는 이동하지 않음
	else
	{
		FVector CurrentLocation = GetActorLocation();
		FVector NewLocation = FMath::Lerp(CurrentLocation, Destination, DeltaTime * move_speed);
		SetActorLocation(NewLocation);

		FRotator NewRotation = GetActorRotation();

		if (NewRotation.Yaw < HeadingDirection.Yaw - 1.f)
		{
			NewRotation.Yaw += 1.f;
		}

		else if (NewRotation.Yaw > HeadingDirection.Yaw + 1.f)
		{
			NewRotation.Yaw -= 1.f;
		}
		
		else
		{
			NewRotation.Yaw = HeadingDirection.Yaw;
		}
		SetActorRotation(NewRotation);
	}




	//TopSprite->SetTranslucentSortPriority(static_cast<int>(round(GetActorLocation().Z)));
	//BottomSprite->SetTranslucentSortPriority(static_cast<int>(round(GetActorLocation().Z)));
	/*
	FRotator CurrentTopRot = TopSprite->GetComponentRotation();
	FRotator CurrentBottomRot = BottomSprite->GetComponentRotation();
	FRotator AddRot(-1.f, 0.f, 0.f); // X축 기준으로 30도 회전

	if (!bLeft)
	{
		AddRot = FRotator(1.f, 0.f, 0.f);
	}




	TopSprite->SetWorldRotation(CurrentTopRot + AddRot);
	BottomSprite->SetWorldRotation(CurrentBottomRot + AddRot);
	*/
	//AddActorLocalOffset(NewLocation);
	
	/*
	float x = 0, y = 0;

	if (current_moved_x < offset_x)
	{
		x = FMath::Lerp(current_moved_x, offset_x, DeltaTime * speed);
	}

	if (current_moved_y < offset_y)
	{
		y = FMath::Lerp(current_moved_y, offset_y, DeltaTime * speed);
	}
		
	FVector MoveVector(x, y, 0.f);
	AddActorLocalOffset(MoveVector);
	current_moved_y += y;
	current_moved_x += x;

	*/

	// z오더 세팅
}

