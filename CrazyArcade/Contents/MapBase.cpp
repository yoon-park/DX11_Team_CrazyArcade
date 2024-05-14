#include "PreCompile.h"
#include "MapBase.h"
#include "MapConstant.h"
#include "BlockBase.h"

float AMapBase::BlockSize = 40.0f;

AMapBase::AMapBase()
{
	UDefaultSceneComponent* Root = CreateDefaultSubObject<UDefaultSceneComponent>("Root");

	BackGround = CreateDefaultSubObject<USpriteRenderer>("BackGround");
	BackGround->SetPosition({ -80.0f, 0.0f, 0.0f });
	BackGround->SetAutoSize(1.0f, true);
	BackGround->SetupAttachment(Root);
	BackGround->SetOrder(ERenderOrder::Map);

	PlayUI_BackGround = CreateDefaultSubObject<USpriteRenderer>("PlayUI_BackGround");
	PlayUI_BackGround->SetSprite(MapImgRes::play_ui_background);
	PlayUI_BackGround->SetSamplering(ETextureSampling::LINEAR);
	PlayUI_BackGround->SetAutoSize(1.0f, true);
	PlayUI_BackGround->SetupAttachment(Root);
	PlayUI_BackGround->SetOrder(ERenderOrder::Map);

	SetRoot(Root);
}

AMapBase::~AMapBase()
{
}

void AMapBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetMainCamera()->SetActorLocation({ 400.0f, 300.0f, -100.0f });
	SetActorLocation({ 400.0f, 300.0f, 0.0f });
}

void AMapBase::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

void AMapBase::SetMapInfoSize(int _X, int _Y)
{
	SizeY = _Y;
	SizeX = _X;
	MapInfo.resize(_Y);
	for (size_t Y = 0; Y < MapInfo.size(); Y++)
	{
		MapInfo[Y].resize(_X);
	}
}

bool AMapBase::CanMovePos(const FVector& _NextPos)
{
	// MapInfo
	FVector NextPos = _NextPos - StartPos;
	
	float NextPlayerFX = NextPos.X / BlockSize;
	float NextPlayerFY = NextPos.Y / BlockSize;

	int NextPlayerX = static_cast<int>(NextPos.X / BlockSize);
	int NextPlayerY = static_cast<int>(NextPos.Y / BlockSize);
	
	// 맵 범위 밖일때
	if (SizeX <= NextPlayerX || 0 > NextPlayerFX || SizeY <= NextPlayerY || 0 > NextPlayerFY)
	{
		return false;
	}
	
	// 빈 공간일때
	if (nullptr == MapInfo[NextPlayerY][NextPlayerX])
	{
		return true;
	}

	// 오브젝트 존재할때
	EBlockType BlockType = MapInfo[NextPlayerY][NextPlayerX]->GetBlockType();
	if (EBlockType::Wall == BlockType || EBlockType::Box == BlockType)
	{
		return false;
	}

	return true;
}