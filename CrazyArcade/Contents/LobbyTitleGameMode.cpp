#include "PreCompile.h"
#include "LobbyTitleGameMode.h"
#include "MainTitleGameMode.h"
#include <format>

ALobbyTitleGameMode::ALobbyTitleGameMode()
{
}

ALobbyTitleGameMode::~ALobbyTitleGameMode()
{
}

void ALobbyTitleGameMode::BeginPlay()
{
	Super::BeginPlay();

	{
		TextWidget = CreateWidget<UTextWidget>(GetWorld(), "LobbytText");
		TextWidget->SetFont("����");
		TextWidget->SetScale(15.0f);
		TextWidget->SetColor(Color8Bit::Black);
		TextWidget->SetPosition({ -340.0f ,100.0f });
		TextWidget->SetFlag(FW1_LEFT); //�·� ����
		TextWidget->AddToViewPort(4);
		TextWidget->SetText(PlayerName);
	}


	{
		UEngineSprite::CreateCutting("Button_GameStart_Hover.png", 1, 3);
		UEngineSprite::CreateCutting("Button_MapSelect_Hover.png", 1, 2);
	}
	{
		// BackGround
		{
			LobbyBackGround = CreateWidget<UImage>(GetWorld(), "LobbyBackGround");
			LobbyBackGround->SetSprite("Lobby.png");
			LobbyBackGround->AddToViewPort(1);
			LobbyBackGround->SetAutoSize(1.0f, true);
			LobbyBackGround->SetWidgetLocation({ 0.0f, 0.0f });
		}

		// Fade
		{
			Fade = CreateWidget<UImage>(GetWorld(), "Fade");
			Fade->SetSprite("FadeBlack.png");
			Fade->AddToViewPort(10);
			Fade->SetAutoSize(1.0f, true);
			Fade->SetWidgetLocation({ 0.0f, 0.0f });
			Fade->SetMulColor(float4(1.0f, 1.0f, 1.0f, 0.0f));
		}

		// GameStart
		{
			Btn_GameStart = CreateWidget<UImage>(GetWorld(), "Button_GameStart");
			Btn_GameStart->AddToViewPort(1);
			Btn_GameStart->SetAutoSize(1.0f, true);
			Btn_GameStart->SetWidgetLocation({ 231.0f, -222.0f });

			Btn_GameStart->CreateAnimation("UnHover", "Button_GameStart_UnHover.png", 0.1f, false, 0, 0);
			Btn_GameStart->CreateAnimation("Hover", "Button_GameStart_Hover.png", 0.1f, true, 0, 2);
			Btn_GameStart->CreateAnimation("Down", "Button_GameStart_Down.png", 0.1f, false, 0, 0);
			Btn_GameStart->ChangeAnimation("UnHover");

			Btn_GameStart->SetUnHover([=] {
				Btn_GameStart->ChangeAnimation("UnHover");
				});

			Btn_GameStart->SetHover([=] {
				if (Btn_GameStart->IsCurAnimationEnd() == true)
				{
					Btn_GameStart->ChangeAnimation("Hover");
				}
				});

			Btn_GameStart->SetDown([=] {
				Btn_GameStart->ChangeAnimation("Down");
				});

			Btn_GameStart->SetPress([=] {

				});

			Btn_GameStart->SetUp([=] {
				IsFadeOut = true;
				Fade->SetActive(true);
				});
		}

		// MapSelect
		{
			Btn_MapSelect = CreateWidget<UImage>(GetWorld(), "Button_MapSelect");
			Btn_MapSelect->AddToViewPort(1);
			Btn_MapSelect->SetAutoSize(1.0f, true);
			Btn_MapSelect->SetWidgetLocation({ 307.0f, -151.0f });

			Btn_MapSelect->CreateAnimation("UnHover", "Button_MapSelect_UnHover.png", 0.1f, false, 0, 0);
			Btn_MapSelect->CreateAnimation("Hover", "Button_MapSelect_Hover.png", 0.1f, true, 0, 1);
			Btn_MapSelect->CreateAnimation("Down", "Button_MapSelect_Down.png", 0.1f, false, 0, 0);
			Btn_MapSelect->ChangeAnimation("UnHover");

			Btn_MapSelect->SetUnHover([=] {
				Btn_MapSelect->ChangeAnimation("UnHover");
				});

			Btn_MapSelect->SetHover([=] {
				Btn_MapSelect->ChangeAnimation("Hover");
				});

			Btn_MapSelect->SetDown([=] {
				Btn_MapSelect->ChangeAnimation("Down");
				});

			Btn_MapSelect->SetPress([=] {

				});

			Btn_MapSelect->SetUp([=] {
				Btn_MapSelect->ChangeAnimation("Hover");
				});
		}

		// Space
		{
			for (int i = 0; i < 7; i++)
			{
				UImage* Btn_Space = CreateWidget<UImage>(GetWorld(), "Button_Space");
				Btn_Space->AddToViewPort(1);
				Btn_Space->SetAutoSize(1.0f, true);

				Btn_Space->CreateAnimation("Space_UnHover", "Button_Space_UnHover.png", 0.1f, false, 0, 0);
				Btn_Space->CreateAnimation("Space_Hover", "Button_Space_Hover.png", 0.1f, false, 0, 0);
				Btn_Space->CreateAnimation("Space_Down", "Button_Space_Down.png", 0.1f, false, 0, 0);
				Btn_Space->CreateAnimation("UnSpace_UnHover", "Button_UnSpace_UnHover.png", 0.1f, false, 0, 0);
				Btn_Space->CreateAnimation("UnSpace_Hover", "Button_UnSpace_Hover.png", 0.1f, false, 0, 0);
				Btn_Space->CreateAnimation("UnSpace_Down", "Button_UnSpace_Down.png", 0.1f, false, 0, 0);
				Btn_Space->ChangeAnimation("UnSpace_UnHover");

				if (i < 3)
				{
					Btn_Space->SetWidgetLocation({ -324.0f + 106.0f * (i + 1), 157.0f });
				}
				else
				{
					Btn_Space->SetWidgetLocation({ -324.0f + 106.0f * (i - 3), 12.0f });
				}

				Btns_Space.push_back(Btn_Space);
				Space_Available.push_back(false);
			}

			for (int i = 0; i < 8; i++)
			{
				UImage* Character_Space = CreateWidget<UImage>(GetWorld(), "Character_Space");
				Character_Space->AddToViewPort(2);
				Character_Space->SetAutoSize(1.0f, true);
				Character_Space->SetWidgetLocation({ -341.0f + 106.0f * (i % 4), 145.0f - 145.0f * (i / 4) });
				Character_Space->SetSprite("Charcater_Space_Random.png");

				UImage* Flag_Space = CreateWidget<UImage>(GetWorld(), "Flag_Space");
				Flag_Space->AddToViewPort(1);
				Flag_Space->SetAutoSize(1.0f, true);
				Flag_Space->SetWidgetLocation({ -298.0f + 106.0f * (i % 4), 138.0f - 145.0f * (i / 4) });
				Flag_Space->SetSprite("Flag_Space.png");

				UImage* Shadow_Space = CreateWidget<UImage>(GetWorld(), "Shadow_Space");
				Shadow_Space->AddToViewPort(1);
				Shadow_Space->SetAutoSize(1.0f, true);
				Shadow_Space->SetWidgetLocation({ -340.0f + 106.0f * (i % 4), 120.0f - 145.0f * (i / 4) });
				Shadow_Space->SetSprite("Shadow_Space.png");

				Characters_Space.push_back(Character_Space);
				Flags_Space.push_back(Flag_Space);
				Shadows_Space.push_back(Shadow_Space);
			}

			for (int i = 0; i < 7; i++)
			{
				Btns_Space[i]->SetUnHover([=] {
					if (Space_Available[i] == true)
					{
						Btns_Space[i]->ChangeAnimation("Space_UnHover");
					}
					else
					{
						Btns_Space[i]->ChangeAnimation("UnSpace_UnHover");
					}
					});

				Btns_Space[i]->SetHover([=] {
					if (Space_Available[i] == true)
					{
						Btns_Space[i]->ChangeAnimation("Space_Hover");
					}
					else
					{
						Btns_Space[i]->ChangeAnimation("UnSpace_Hover");
					}
					});

				Btns_Space[i]->SetDown([=] {
					if (Space_Available[i] == true)
					{
						Btns_Space[i]->ChangeAnimation("Space_Down");
						Space_Available[i] = false;
					}
					else
					{
						Btns_Space[i]->ChangeAnimation("UnSpace_Down");
						Space_Available[i] = true;
					}
					});

				Btns_Space[i]->SetPress([=] {

					});

				Btns_Space[i]->SetUp([=] {
					if (Space_Available[i] == true)
					{
						Btns_Space[i]->ChangeAnimation("Space_Hover");
						SpaceOn(i + 1);
					}
					else
					{
						Btns_Space[i]->ChangeAnimation("UnSpace_Hover");
						SpaceOff(i + 1);
					}
					});

				SpaceOff(i + 1);
			}
		}
		
		// CharacterSelect
		{
			{
				UpperPanel_CharacterSelect = CreateWidget<UImage>(GetWorld(), "UpperPanel_CharacterSelect");
				UpperPanel_CharacterSelect->SetSprite("UpperPanel_CharatorSelect_Random.png");
				UpperPanel_CharacterSelect->AddToViewPort(2);
				UpperPanel_CharacterSelect->SetAutoSize(1.0f, true);
				UpperPanel_CharacterSelect->SetWidgetLocation({ -21.0f, 230.0f });
				UpperPanel_CharacterSelect->SetActive(false);
			}
			{
				Panel_CharacterSelect = CreateWidget<UImage>(GetWorld(), "Panel_CharacterSelect");
				Panel_CharacterSelect->SetSprite("Panel_CharatorSelect.png");
				Panel_CharacterSelect->AddToViewPort(2);
				Panel_CharacterSelect->SetAutoSize(1.0f, true);
				Panel_CharacterSelect->SetWidgetLocation({ -21.0f, 185.0f });
				Panel_CharacterSelect->SetActive(false);
			}

			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					UImage* TraitBar_CharacterSelect = CreateWidget<UImage>(GetWorld(), "TraitBar_CharacterSelect");
					TraitBar_CharacterSelect->SetSprite("TraitBar_CharatorSelect_Min.png");
					TraitBar_CharacterSelect->AddToViewPort(3);
					TraitBar_CharacterSelect->SetAutoSize(1.0f, true);
					TraitBar_CharacterSelect->SetWidgetLocation({ -24.0f + (10.0f * j), 204.0f - (19.0f * i) });

					Traits_CharacterSelect[i].push_back(TraitBar_CharacterSelect);
				}
			}

			for (int i = 0; i < 12; i++)
			{
				UImage* Btn_CharacterSelect = CreateWidget<UImage>(GetWorld(), "Btn_CharacterSelect");
				Btn_CharacterSelect->AddToViewPort(1);
				Btn_CharacterSelect->SetAutoSize(1.0f, true);
				Btn_CharacterSelect->SetWidgetLocation({ 121.0f + (72.0f * (i % 4)), 185.0f - (55.0f * (i / 4)) });

				switch (i)
				{
				case 0:
				{
					Btn_CharacterSelect->CreateAnimation("UnHover", "Button_CharatorSelect_Random_UnHover.png", 0.1f, false, 0, 0);
					Btn_CharacterSelect->CreateAnimation("Hover", "Button_CharatorSelect_Random_Hover.png", 0.1f, false, 0, 0);
					Btn_CharacterSelect->CreateAnimation("Down", "Button_CharatorSelect_Random_Down.png", 0.1f, false, 0, 0);
					Btn_CharacterSelect->CreateAnimation("Pick", "Button_CharatorSelect_Random_Pick.png", 0.1f, false, 0, 0);
					CharacterType = ECharacterType::Random;
					break;
				}
				case 1:
				{
					Btn_CharacterSelect->CreateAnimation("UnHover", "Button_CharatorSelect_Dao_UnHover.png", 0.1f, false, 0, 0);
					Btn_CharacterSelect->CreateAnimation("Hover", "Button_CharatorSelect_Dao_Hover.png", 0.1f, false, 0, 0);
					Btn_CharacterSelect->CreateAnimation("Down", "Button_CharatorSelect_Dao_Down.png", 0.1f, false, 0, 0);
					Btn_CharacterSelect->CreateAnimation("Pick", "Button_CharatorSelect_Dao_Pick.png", 0.1f, false, 0, 0);
					CharacterType = ECharacterType::Dao;
					break;
				}
				case 2:
				{
					Btn_CharacterSelect->CreateAnimation("UnHover", "Button_CharatorSelect_Dizni_UnHover.png", 0.1f, false, 0, 0);
					Btn_CharacterSelect->CreateAnimation("Hover", "Button_CharatorSelect_Dizni_Hover.png", 0.1f, false, 0, 0);
					Btn_CharacterSelect->CreateAnimation("Down", "Button_CharatorSelect_Dizni_Down.png", 0.1f, false, 0, 0);
					Btn_CharacterSelect->CreateAnimation("Pick", "Button_CharatorSelect_Dizni_UnHover.png", 0.1f, false, 0, 0);
					//Btn_CharacterSelect->CreateAnimation("Pick", "Button_CharatorSelect_Dizni_Pick.png", 0.1f, false, 0, 0);
					CharacterType = ECharacterType::Dizni;
					break;
				}
				case 3:
				{
					Btn_CharacterSelect->CreateAnimation("UnHover", "Button_CharatorSelect_Mos_UnHover.png", 0.1f, false, 0, 0);
					Btn_CharacterSelect->CreateAnimation("Hover", "Button_CharatorSelect_Mos_Hover.png", 0.1f, false, 0, 0);
					Btn_CharacterSelect->CreateAnimation("Down", "Button_CharatorSelect_Mos_Down.png", 0.1f, false, 0, 0);
					Btn_CharacterSelect->CreateAnimation("Pick", "Button_CharatorSelect_Mos_UnHover.png", 0.1f, false, 0, 0);
					//Btn_CharacterSelect->CreateAnimation("Pick", "Button_CharatorSelect_Mos_Pick.png", 0.1f, false, 0, 0);
					CharacterType = ECharacterType::Mos;
					break;
				}
				case 4:
				{
					Btn_CharacterSelect->CreateAnimation("UnHover", "Button_CharatorSelect_Ethi_UnHover.png", 0.1f, false, 0, 0);
					Btn_CharacterSelect->CreateAnimation("Hover", "Button_CharatorSelect_Ethi_Hover.png", 0.1f, false, 0, 0);
					Btn_CharacterSelect->CreateAnimation("Down", "Button_CharatorSelect_Ethi_Down.png", 0.1f, false, 0, 0);
					Btn_CharacterSelect->CreateAnimation("Pick", "Button_CharatorSelect_Ethi_UnHover.png", 0.1f, false, 0, 0);
					//Btn_CharacterSelect->CreateAnimation("Pick", "Button_CharatorSelect_Ethi_Pick.png", 0.1f, false, 0, 0);
					CharacterType = ECharacterType::Ethi;
					break;
				}
				case 5:
				{
					Btn_CharacterSelect->CreateAnimation("UnHover", "Button_CharatorSelect_Marid_UnHover.png", 0.1f, false, 0, 0);
					Btn_CharacterSelect->CreateAnimation("Hover", "Button_CharatorSelect_Marid_Hover.png", 0.1f, false, 0, 0);
					Btn_CharacterSelect->CreateAnimation("Down", "Button_CharatorSelect_Marid_Down.png", 0.1f, false, 0, 0);
					Btn_CharacterSelect->CreateAnimation("Pick", "Button_CharatorSelect_Marid_Pick.png", 0.1f, false, 0, 0);
					CharacterType = ECharacterType::Marid;
					break;
				}
				case 6:
				{
					Btn_CharacterSelect->CreateAnimation("UnHover", "Button_CharatorSelect_Bazzi_UnHover.png", 0.1f, false, 0, 0);
					Btn_CharacterSelect->CreateAnimation("Hover", "Button_CharatorSelect_Bazzi_Hover.png", 0.1f, false, 0, 0);
					Btn_CharacterSelect->CreateAnimation("Down", "Button_CharatorSelect_Bazzi_Down.png", 0.1f, false, 0, 0);
					Btn_CharacterSelect->CreateAnimation("Pick", "Button_CharatorSelect_Bazzi_Pick.png", 0.1f, false, 0, 0);
					CharacterType = ECharacterType::Bazzi;
					break;
				}
				case 7:
				{
					Btn_CharacterSelect->CreateAnimation("UnHover", "Button_CharatorSelect_Uni_UnHover.png", 0.1f, false, 0, 0);
					Btn_CharacterSelect->CreateAnimation("Hover", "Button_CharatorSelect_Uni_Hover.png", 0.1f, false, 0, 0);
					Btn_CharacterSelect->CreateAnimation("Down", "Button_CharatorSelect_Uni_Down.png", 0.1f, false, 0, 0);
					Btn_CharacterSelect->CreateAnimation("Pick", "Button_CharatorSelect_Uni_UnHover.png", 0.1f, false, 0, 0);
					//Btn_CharacterSelect->CreateAnimation("Pick", "Button_CharatorSelect_Uni_Pick.png", 0.1f, false, 0, 0);
					CharacterType = ECharacterType::Uni;
					break;
				}
				case 8:
				{
					Btn_CharacterSelect->CreateAnimation("UnHover", "Button_CharatorSelect_Kephi_UnHover.png", 0.1f, false, 0, 0);
					Btn_CharacterSelect->CreateAnimation("Hover", "Button_CharatorSelect_Kephi_Hover.png", 0.1f, false, 0, 0);
					Btn_CharacterSelect->CreateAnimation("Down", "Button_CharatorSelect_Kephi_Down.png", 0.1f, false, 0, 0);
					Btn_CharacterSelect->CreateAnimation("Pick", "Button_CharatorSelect_Kephi_Pick.png", 0.1f, false, 0, 0);
					CharacterType = ECharacterType::Kephi;
					break;
				}
				case 9:
				{
					Btn_CharacterSelect->CreateAnimation("UnHover", "Button_CharatorSelect_Su_UnHover.png", 0.1f, false, 0, 0);
					Btn_CharacterSelect->CreateAnimation("Hover", "Button_CharatorSelect_Su_Hover.png", 0.1f, false, 0, 0);
					Btn_CharacterSelect->CreateAnimation("Down", "Button_CharatorSelect_Su_Down.png", 0.1f, false, 0, 0);
					Btn_CharacterSelect->CreateAnimation("Pick", "Button_CharatorSelect_Su_UnHover.png", 0.1f, false, 0, 0);
					//Btn_CharacterSelect->CreateAnimation("Pick", "Button_CharatorSelect_Su_Pick.png", 0.1f, false, 0, 0);
					CharacterType = ECharacterType::Su;
					break;
				}
				case 10:
				{
					Btn_CharacterSelect->CreateAnimation("UnHover", "Button_CharatorSelect_Hoou_UnHover.png", 0.1f, false, 0, 0);
					Btn_CharacterSelect->CreateAnimation("Hover", "Button_CharatorSelect_Hoou_Hover.png", 0.1f, false, 0, 0);
					Btn_CharacterSelect->CreateAnimation("Down", "Button_CharatorSelect_Hoou_Down.png", 0.1f, false, 0, 0);
					Btn_CharacterSelect->CreateAnimation("Pick", "Button_CharatorSelect_Hoou_UnHover.png", 0.1f, false, 0, 0);
					//Btn_CharacterSelect->CreateAnimation("Pick", "Button_CharatorSelect_Hoou_Pick.png", 0.1f, false, 0, 0);
					CharacterType = ECharacterType::HooU;
					break;
				}
				case 11:
				{
					Btn_CharacterSelect->CreateAnimation("UnHover", "Button_CharatorSelect_Ray_UnHover.png", 0.1f, false, 0, 0);
					Btn_CharacterSelect->CreateAnimation("Hover", "Button_CharatorSelect_Ray_Hover.png", 0.1f, false, 0, 0);
					Btn_CharacterSelect->CreateAnimation("Down", "Button_CharatorSelect_Ray_Down.png", 0.1f, false, 0, 0);
					Btn_CharacterSelect->CreateAnimation("Pick", "Button_CharatorSelect_Ray_UnHover.png", 0.1f, false, 0, 0);
					//Btn_CharacterSelect->CreateAnimation("Pick", "Button_CharatorSelect_Ray_Pick.png", 0.1f, false, 0, 0);
					CharacterType = ECharacterType::Ray;
					break;
				}
				default:
					break;
				}
				Btn_CharacterSelect->ChangeAnimation("UnHover");

				Btns_CharacterSelect.push_back(Btn_CharacterSelect);
				CharacterSelect_Pick.push_back(false);
			}

			for (int i = 0; i < 12; i++)
			{
				Btns_CharacterSelect[i]->SetUnHover([=] {
					if (CharacterSelect_Pick[i] == false)
					{
						Btns_CharacterSelect[i]->ChangeAnimation("UnHover");
					}
					else
					{
						Btns_CharacterSelect[i]->ChangeAnimation("Pick");
					}
					PanelOff();
					});

				Btns_CharacterSelect[i]->SetHover([=] {
					if (CharacterSelect_Pick[i] == false)
					{
						Btns_CharacterSelect[i]->ChangeAnimation("Hover");
					}
					else
					{
						Btns_CharacterSelect[i]->ChangeAnimation("Pick");
					}
					SettingPanel(ECharacterType(i));
					PanelOn();
					});

				Btns_CharacterSelect[i]->SetDown([=] {
					Btns_CharacterSelect[i]->ChangeAnimation("Down");
					});

				Btns_CharacterSelect[i]->SetPress([=] {

					});

				Btns_CharacterSelect[i]->SetUp([=] {
					ChangeCharacter(ECharacterType(i));
					});
			}

			{
				Outline_CharacterSelect = CreateWidget<UImage>(GetWorld(), "Outline_CharacterSelect");
				Outline_CharacterSelect->SetSprite("Outline_CharatorSelect_Random.png");
				Outline_CharacterSelect->AddToViewPort(1);
				Outline_CharacterSelect->SetAutoSize(1.0f, true);
				Outline_CharacterSelect->SetWidgetLocation({ 229.0f, 245.0f });
			}
			{
				Checker_CharacterSelect = CreateWidget<UImage>(GetWorld(), "Checker_CharacterSelect");
				Checker_CharacterSelect->SetSprite("Checker_CharacterSelect.png");
				Checker_CharacterSelect->AddToViewPort(2);
				Checker_CharacterSelect->SetAutoSize(1.0f, true);
				Checker_CharacterSelect->SetWidgetLocation({ 150.0f, 202.0f });
			}

			PanelOff();
			ChangeCharacter(ECharacterType::Random);
		}
	}
}

void ALobbyTitleGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	
	if (UEngineInput::IsDown('P'))
	{
		std::string str = PlayerName;
		TextWidget->SetText(PlayerName);
		//����� ���� 
	}
	// Debug
	{
		FVector CameraPos = GetWorld()->GetMainCamera()->GetActorLocation();
		FVector MousePos = GEngine->EngineWindow.GetScreenMousePos();
		FVector WindowScale = GEngine->EngineWindow.GetWindowScale();
		FVector TargetPos = FVector(CameraPos.X, CameraPos.Y, 0.0f) + FVector(MousePos.X - WindowScale.hX(), -(MousePos.Y - WindowScale.hY()), 0.0f);
		 
		{
			std::string Msg = std::format("MousePos : {}\n", TargetPos.ToString());
			UEngineDebugMsgWindow::PushMsg(Msg);
		}
	}

	// Fade & ChangeLevel
	{
		if (IsFadeIn == true)
		{
			if (FadeAlpha <= 0.0f)
			{
				IsFadeIn = false;
				Fade->SetActive(false);
				return;
			}

			FadeIn(_DeltaTime);
		}

		if (IsFadeOut == true)
		{
			if (FadeAlpha >= 1.0f)
			{
				IsFadeIn = true;
				IsFadeOut = false;
				GEngine->ChangeLevel("MainPlayLevel");
				return;
			}

			FadeOut(_DeltaTime);
		}
	}
}

void ALobbyTitleGameMode::SpaceOn(int _Index)
{
	Characters_Space[_Index]->SetActive(true);
	Flags_Space[_Index]->SetActive(true);
	Shadows_Space[_Index]->SetActive(true);
}

void ALobbyTitleGameMode::SpaceOff(int _Index)
{
	Characters_Space[_Index]->SetActive(false);
	Flags_Space[_Index]->SetActive(false);
	Shadows_Space[_Index]->SetActive(false);
}

void ALobbyTitleGameMode::PanelOn()
{
	UpperPanel_CharacterSelect->SetActive(true);
	Panel_CharacterSelect->SetActive(true);

	for (int i = 0; i < BombMax; i++)
	{
		Traits_CharacterSelect[0][i]->SetActive(true);
	}
	for (int i = 0; i < BombWaterMax; i++)
	{
		Traits_CharacterSelect[1][i]->SetActive(true);
	}
	for (int i = 0; i < SpeedMax; i++)
	{
		Traits_CharacterSelect[2][i]->SetActive(true);
	}
}

void ALobbyTitleGameMode::PanelOff()
{
	UpperPanel_CharacterSelect->SetActive(false);
	Panel_CharacterSelect->SetActive(false);

	for (int i = 0; i < 10; i++)
	{
		Traits_CharacterSelect[0][i]->SetActive(false);
		Traits_CharacterSelect[1][i]->SetActive(false);
		Traits_CharacterSelect[2][i]->SetActive(false);
	}
}

void ALobbyTitleGameMode::SettingPanel(ECharacterType _CharacterType)
{
	switch (_CharacterType)
	{
	case ECharacterType::Random:
	{
		UpperPanel_CharacterSelect->SetSprite("UpperPanel_CharatorSelect_Random.png");
		BombMin = 0;
		BombMax = 0;
		BombWaterMin = 0;
		BombWaterMax = 0;
		SpeedMin = 0;
		SpeedMax = 0;
		break;
	}
	case ECharacterType::Dao:
	{
		UpperPanel_CharacterSelect->SetSprite("UpperPanel_CharatorSelect_Dao.png");
		BombMin = 1;
		BombMax = 10;
		BombWaterMin = 1;
		BombWaterMax = 7;
		SpeedMin = 5;
		SpeedMax = 7;
		break;
	}
	case ECharacterType::Dizni:
	{
		UpperPanel_CharacterSelect->SetSprite("UpperPanel_CharatorSelect_Dizni.png");
		BombMin = 2;
		BombMax = 7;
		BombWaterMin = 1;
		BombWaterMax = 9;
		SpeedMin = 4;
		SpeedMax = 8;
		break;
	}
	case ECharacterType::Mos:
	{
		UpperPanel_CharacterSelect->SetSprite("UpperPanel_CharatorSelect_Mos.png");
		BombMin = 1;
		BombMax = 8;
		BombWaterMin = 1;
		BombWaterMax = 5;
		SpeedMin = 5;
		SpeedMax = 8;
		break;
	}
	case ECharacterType::Ethi:
	{
		UpperPanel_CharacterSelect->SetSprite("UpperPanel_CharatorSelect_Ethi.png");
		BombMin = 1;
		BombMax = 10;
		BombWaterMin = 1;
		BombWaterMax = 8;
		SpeedMin = 4;
		SpeedMax = 8;
		break;
	}
	case ECharacterType::Marid:
	{
		UpperPanel_CharacterSelect->SetSprite("UpperPanel_CharatorSelect_Marid.png");
		BombMin = 2;
		BombMax = 9;
		BombWaterMin = 1;
		BombWaterMax = 6;
		SpeedMin = 4;
		SpeedMax = 8;
		break;
	}
	case ECharacterType::Bazzi:
	{
		UpperPanel_CharacterSelect->SetSprite("UpperPanel_CharatorSelect_Bazzi.png");
		BombMin = 1;
		BombMax = 6;
		BombWaterMin = 1;
		BombWaterMax = 7;
		SpeedMin = 5;
		SpeedMax = 9;
		break;
	}
	case ECharacterType::Uni:
	{
		UpperPanel_CharacterSelect->SetSprite("UpperPanel_CharatorSelect_Uni.png");
		BombMin = 1;
		BombMax = 6;
		BombWaterMin = 2;
		BombWaterMax = 7;
		SpeedMin = 5;
		SpeedMax = 8;
		break;
	}
	case ECharacterType::Kephi:
	{
		UpperPanel_CharacterSelect->SetSprite("UpperPanel_CharatorSelect_Kephi.png");
		BombMin = 1;
		BombMax = 9;
		BombWaterMin = 2;
		BombWaterMax = 8;
		SpeedMin = 4;
		SpeedMax = 8;
		break;
	}
	case ECharacterType::Su:
	{
		UpperPanel_CharacterSelect->SetSprite("UpperPanel_CharatorSelect_Su.png");
		BombMin = 2;
		BombMax = 9;
		BombWaterMin = 1;
		BombWaterMax = 7;
		SpeedMin = 6;
		SpeedMax = 10;
		break;
	}
	case ECharacterType::HooU:
	{
		UpperPanel_CharacterSelect->SetSprite("UpperPanel_CharatorSelect_HooU.png");
		BombMin = 3;
		BombMax = 9;
		BombWaterMin = 1;
		BombWaterMax = 7;
		SpeedMin = 5;
		SpeedMax = 10;
		break;
	}
	case ECharacterType::Ray:
	{
		UpperPanel_CharacterSelect->SetSprite("UpperPanel_CharatorSelect_Ray.png");
		BombMin = 2;
		BombMax = 9;
		BombWaterMin = 1;
		BombWaterMax = 7;
		SpeedMin = 6;
		SpeedMax = 10;
		break;
	}
	default:
		break;
	}

	for (int i = 0; i < BombMin; i++)
	{
		Traits_CharacterSelect[0][i]->SetSprite("TraitBar_CharatorSelect_Min.png");
	}
	for (int i = BombMin; i < BombMax; i++)
	{
		Traits_CharacterSelect[0][i]->SetSprite("TraitBar_CharatorSelect_Max.png");
	}

	for (int i = 0; i < BombWaterMin; i++)
	{
		Traits_CharacterSelect[1][i]->SetSprite("TraitBar_CharatorSelect_Min.png");
	}
	for (int i = BombWaterMin; i < BombWaterMax; i++)
	{
		Traits_CharacterSelect[1][i]->SetSprite("TraitBar_CharatorSelect_Max.png");
	}

	for (int i = 0; i < SpeedMin; i++)
	{
		Traits_CharacterSelect[2][i]->SetSprite("TraitBar_CharatorSelect_Min.png");
	}
	for (int i = SpeedMin; i < SpeedMax; i++)
	{
		Traits_CharacterSelect[2][i]->SetSprite("TraitBar_CharatorSelect_Max.png");
	}
}

void ALobbyTitleGameMode::ChangeCharacter(ECharacterType _CharacterType)
{
	if (
		_CharacterType == ECharacterType::Dizni ||
		_CharacterType == ECharacterType::Mos ||
		_CharacterType == ECharacterType::Ethi ||
		_CharacterType == ECharacterType::Uni ||
		_CharacterType == ECharacterType::Su ||
		_CharacterType == ECharacterType::HooU ||
		_CharacterType == ECharacterType::Ray
		)
	{
		return;
	}

	CharacterType = _CharacterType;
	int Index = int(_CharacterType);

	CharacterSelect_Pick[Index] = true;
	Btns_CharacterSelect[Index]->ChangeAnimation("Pick");

	for (int i = 0; i < 12; i++)
	{
		if (i != Index)
		{
			CharacterSelect_Pick[i] = false;
			Btns_CharacterSelect[i]->ChangeAnimation("UnHover");
		}
	}

	switch (_CharacterType)
	{
	case ECharacterType::Random:
	{
		Outline_CharacterSelect->SetSprite("Outline_CharatorSelect_Random.png");
		break;
	}
	case ECharacterType::Dao:
	{
		Outline_CharacterSelect->SetSprite("Outline_CharatorSelect_Dao.png");
		break;
	}
	case ECharacterType::Marid:
	{
		Outline_CharacterSelect->SetSprite("Outline_CharatorSelect_Marid.png");
		break;
	}
	case ECharacterType::Bazzi:
	{
		Outline_CharacterSelect->SetSprite("Outline_CharatorSelect_Bazzi.png");
		break;
	}
	case ECharacterType::Kephi:
	{
		Outline_CharacterSelect->SetSprite("Outline_CharatorSelect_Kephi.png");
		break;
	}
	default:
		break;
	}

	Checker_CharacterSelect->SetWidgetLocation({ 150.0f + (72.0f * (Index % 4)), 202.0f - (55.0f * (Index / 4)) });
}

void ALobbyTitleGameMode::FadeIn(float _DeltaTime)
{
	FadeAlpha -= _DeltaTime * 3.0f;
	Fade->SetMulColor(float4(1.0f, 1.0f, 1.0f, FadeAlpha));
}

void ALobbyTitleGameMode::FadeOut(float _DeltaTime)
{
	FadeAlpha += _DeltaTime * 3.0f;
	Fade->SetMulColor(float4(1.0f, 1.0f, 1.0f, FadeAlpha));
}