#pragma once

class ATileMapBase;

// 설명 : Map 테스트 레벨
class AMapTestLevel : public AGameMode
{
	GENERATED_BODY(AGameMode)
public:
	// constrcuter destructer
	AMapTestLevel();
	~AMapTestLevel();

	// delete Function
	AMapTestLevel(const AMapTestLevel& _Other) = delete;
	AMapTestLevel(AMapTestLevel&& _Other) noexcept = delete;
	AMapTestLevel& operator=(const AMapTestLevel& _Other) = delete;
	AMapTestLevel& operator=(AMapTestLevel&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	std::shared_ptr<ATileMapBase> TileMap = nullptr;

};

