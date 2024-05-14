#pragma once
#include "BlockBase.h"

// ���� : MoveBox Block
class AMoveBox : public ABlockBase
{
	GENERATED_BODY(ABlockBase)
public:
	// constrcuter destructer
	AMoveBox();
	~AMoveBox();

	// delete Function
	AMoveBox(const AMoveBox& _Other) = delete;
	AMoveBox(AMoveBox&& _Other) noexcept = delete;
	AMoveBox& operator=(const AMoveBox& _Other) = delete;
	AMoveBox& operator=(AMoveBox&& _Other) noexcept = delete;

	bool MoveOneBlockCheck(const FVector& _Dir);

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	void MoveUpdate(float _DeltaTime);

private:
	bool IsMoveValue = false;
	float MoveTime = 0.0f;
	FVector StartPos = FVector::Zero;
	FVector TargetPos = FVector::Zero;

};

