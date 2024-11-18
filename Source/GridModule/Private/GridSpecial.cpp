#include "GridSpecial.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"

// Constructor
AGridSpecial::AGridSpecial()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create the Box Component
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    RootComponent = CollisionBox;

    // Set the size of the box
    CollisionBox->SetBoxExtent(FVector(99.0f, 99.0f, 99.0f)); // make box size slightly smaller than the bounds, so that the surrounding traces will not hit it!

    // Enable overlap events
    CollisionBox->SetCollisionProfileName(TEXT("OverlapAll"));
}

// Called when the game starts or when spawned
void AGridSpecial::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AGridSpecial::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
