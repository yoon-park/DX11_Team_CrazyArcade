#include "PreCompile.h"
#include "ServerGameMode.h"
#include "Player.h"
#include <EngineCore/Image.h>
#include <EngineCore/Camera.h>
#include <EngineBase/EngineSerializer.h>
#include <EngineCore/BlurEffect.h>
#include <EngineCore/EngineEditorGUI.h>
#include "PlayGameMode.h"
#include "CrazyArcadeCore.h"
#include "Packets.h"
#include "ServerTestPlayer.h"
#include "ServerTestOtherPlayer.h"
#include <EngineBase/NetObject.h>
#include "ServerHelper.h"
#include "CrazyArcadeEnum.h"
#include "BombBase.h"
#include "MapBase.h"

AServerGameMode::AServerGameMode()
	:AMainPlayLevel()
{
}

AServerGameMode::~AServerGameMode()
{
	if (nullptr != UCrazyArcadeCore::Net)
	{
		UCrazyArcadeCore::Net->End();
		UCrazyArcadeCore::Net = nullptr;
	}
}

void AServerGameMode::BeginPlay()
{
	Super::BeginPlay();

}

void AServerGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	UNetObject::AllNetObject;
	int a = 0;
}

void AServerGameMode::LevelStart(ULevel* _DeltaTime)
{
	if (nullptr == NetWindow)
	{
		NetWindow = UEngineEditorGUI::CreateEditorWindow<UEngineNetWindow>("NetWindow");

		NetWindow->SetServerOpenFunction([&]()
			{
				UCrazyArcadeCore::Net = std::make_shared<UEngineServer>();
				UCrazyArcadeCore::Net->ServerOpen(30000, 512);

				// 여기에서 메인 플레이어한테 번호를 하나 줄겁니다.

				Player->SetObjectToken(UNetObject::GetNewObjectToken());

				ServerPacketInit(UCrazyArcadeCore::Net->Dispatcher);
			});

		NetWindow->SetClientConnectFunction([&](std::string IP, short PORT)
			{
				UCrazyArcadeCore::Net = std::make_shared<UEngineClient>();
				UCrazyArcadeCore::Net->Connect(IP, PORT);

				UCrazyArcadeCore::Net->SetTokenPacketFunction([=](USessionTokenPacket* _Token)
					{
						Player->SetObjectToken(UCrazyArcadeCore::Net->GetSessionToken() * 1000 + UNetObject::GetNewObjectToken());
					});

				// 어떤 패키싱 왔을때 어떻게 처리할건지를 정하는 걸 해야한다.
				ClientPacketInit(UCrazyArcadeCore::Net->Dispatcher);
			});
	}
	NetWindow->On();
}

void AServerGameMode::ServerPacketInit(UEngineDispatcher& Dis)
{
	Dis.AddHandler<UActorUpdatePacket>([=](std::shared_ptr<UActorUpdatePacket> _Packet)
		{
			if (nullptr == UCrazyArcadeCore::Net)
			{
				return;
			}

			// 다른 사람들한테 이 오브젝트에 대해서 알리고
			UCrazyArcadeCore::Net->Send(_Packet);

			GetWorld()->PushFunction([=]()
				{
					// 여긴 주쓰레드니까.
					ServerTestOtherPlayer* OtherPlayer = UNetObject::GetNetObject<ServerTestOtherPlayer>(_Packet->GetObjectToken());
					if (nullptr == OtherPlayer)
					{
						OtherPlayer = this->GetWorld()->SpawnActor<ServerTestOtherPlayer>("OtherPlayer", 0).get();
						OtherPlayer->SetObjectToken(_Packet->GetObjectToken());
					}
					OtherPlayer->PushProtocol(_Packet);
					//OtherPlayer
				});


		});

	Dis.AddHandler<USpawnUpdatePacket>([=](std::shared_ptr<USpawnUpdatePacket> _Packet)  //엑터 스폰 테스트용
		{
			GetWorld()->PushFunction([=]()
				{
					ABombBase* MyBomb;
					MyBomb = ServerHelper::EnumSpawn<ABombBase>(GetWorld(), _Packet->SpawnSelect).get();
					MyBomb->SetObjectToken(_Packet->GetObjectToken());
					MyBomb->PushProtocol(_Packet);
					MyBomb->SetActorLocation(_Packet->Pos);

					FPoint Point = AMapBase::ConvertLocationToPoint(_Packet->Pos);
					MyBomb->SetCurPoint(Point);

					FEngineTimeStamp Stamp = UEngineTime::GetCurTime();
					float FloatResult = Stamp.TimeToFloat();
					MyBomb->ReduceCurExplosionTime(FloatResult - _Packet->SpawnTime);
				});
		});
}

void AServerGameMode::ClientPacketInit(UEngineDispatcher& Dis)
{
	Dis.AddHandler<UActorUpdatePacket>([=](std::shared_ptr<UActorUpdatePacket> _Packet)
		{
			GetWorld()->PushFunction([=]()
				{
					ServerTestOtherPlayer* OtherPlayer = UNetObject::GetNetObject<ServerTestOtherPlayer>(_Packet->GetObjectToken());
					if (nullptr == OtherPlayer)
					{
						OtherPlayer = this->GetWorld()->SpawnActor<ServerTestOtherPlayer>("OtherPlayer", 0).get();
						OtherPlayer->SetObjectToken(_Packet->GetObjectToken());
					}
					OtherPlayer->PushProtocol(_Packet);
				});
		});

	Dis.AddHandler<USpawnUpdatePacket>([=](std::shared_ptr<USpawnUpdatePacket> _Packet)  //엑터 스폰 테스트용
		{
			GetWorld()->PushFunction([=]()
				{
					ABombBase* MyBomb;
					MyBomb = ServerHelper::EnumSpawn<ABombBase>(GetWorld(), _Packet->SpawnSelect).get();
					MyBomb->SetObjectToken(_Packet->GetObjectToken());
					MyBomb->PushProtocol(_Packet);
					MyBomb->SetActorLocation(_Packet->Pos);

					FPoint Point = GetMap()->ConvertLocationToPoint(_Packet->Pos);
					MyBomb->SetCurPoint(Point);

					FEngineTimeStamp Stamp = UEngineTime::GetCurTime();
					float FloatResult = Stamp.TimeToFloat();
					MyBomb->ReduceCurExplosionTime(FloatResult - _Packet->SpawnTime);
				});
		});
}

void AServerGameMode::LevelEnd(ULevel* _DeltaTime)
{
	NetWindow->Off();
}