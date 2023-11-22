#pragma once
#include "fmod.hpp"
#include "MemLib\ML_Vector.hpp"

#define CHANNEL_LIMIT 2

enum Channel {Channel_Base, Channel_Extra};
enum UISFX {Button_Hover, Button_Press, Button_Start};
enum Music {Music_Title, Music_Hot, Music_Shop, Music_Cold, Music_Boss};
enum Ambience {Ambience_Cave = 5, Ambience_Lava, Ambience_Blizzard};
enum Player { Player_Attack1, Player_Attack2, Player_Attack3, Player_AttackHeavyCharge, Player_HeavyAttack, Player_Dash, Player_Hurt, Player_Death };
enum PlayerVoice {Player_IntroSkeleton1 = 8, Player_IntroSkeleton2, Player_IntroDog1, Player_IntroDog2, Player_IntroDog3, Player_IntroDog4,
				Player_IntroEye1, Player_IntroEye2, Player_IntroEye3, Player_IntroEye4, Player_IntroImp1, Player_IntroImp2, Player_IntroImp3,
				Player_IntroMinotaur1, Player_IntroMinotaur2, Player_IntroSplitboss1, Player_IntroSplitboss2, Player_Hot, Player_Chills1, Player_Chills2,
				Player_BetterLuck, Player_AnotherSoul, Player_BringItOn, Player_ThisWillBeFun, Player_DontMind, Player_FindersKeepers, Player_WatchStep,
				Player_PlaceSucks, Player_HotInHere, Player_BurningUp, Player_IceInHell, Player_HotChocolate, Player_WasntSoBad, Player_BetterWork,
				Player_SomethingPositive, Player_LikeTheLook, Player_WillDoDamage, Player_SuckOnThat, Player_WinnerIs, Player_HellYeah, Player_NotAgain,
				Player_GetRevenge, Player_HellSucks, Player_TimeToGo, Player_GetGoing, Player_ConvenientGates};
enum Eye {Eye_Attack, Eye_Shoot, Eye_Hurt, Eye_Death};
enum Hellhound {Hellhound_Attack, Hellhound_Hurt, Hellhound_Inhale, Hellhound_Flame, Hellhound_Death};
enum Skeleton {Skeleton_Attack, Skeleton_Hurt, Skeleton_Death};
enum Boss {Boss_Attack_Hammer, Boss_Attack_Jump, Boss_Attack_Charge, Boss_Attack_Slam, Boss_Hurt};
enum BossVoice {}; //Add all text later
enum Imp {Imp_AttackCharge, Imp_AttackThrow, Imp_Teleport, Imp_Hurt, Imp_Death};
enum Minotaur {Minotaur_Attack, Minotaur_Jump, Minotaur_Slam, Minotaur_Hurt, Minotaur_Death};
enum Shop {Shop_Hover, Shop_Press, Shop_Buy, Shop_Heal, Shop_Dibs, Shop_Reroll, Shop_Upgrade};
enum ShopVoice { Shop_FirstMeet = 7, Shop_BeforeSplitBoss, Shop_BuyingNothing, Shop_FirstPurchase, Shop_PurchaseBeforeBoss, Shop_LowHealth, Shop_RelicPurchase,
	Shop_RelicPurchase2, Shop_UpgradeWeapon, Shop_UpgradeWeapon2, Shop_BeforeLava, Shop_BeforeIce, Shop_BeforeLastBoss};

enum AllSounds{MENU1, MENU2, MENU3, MUSIC1, MUSIC2, MUSIC3, MUSIC4, MUSIC5, AMBIENCE1, AMBIENCE2, AMBIENCE3, PLAYER1, PLAYER2, PLAYER3, PLAYER4,
				PLAYER5, PLAYER6, PLAYER7, PLAYER8, PLAYER9, PLAYER10, PLAYER11, PLAYER12, PLAYER13, PLAYER14, PLAYER15, PLAYER16, PLAYER17,
				PLAYER18, PLAYER19, PLAYER20, PLAYER21, PLAYER22, PLAYER23, PLAYER24, PLAYER25, PLAYER26, PLAYER27, PLAYER28, PLAYER29, PLAYER30,
				PLAYER31, PLAYER32, PLAYER33, PLAYER34, PLAYER35, PLAYER36, PLAYER37, PLAYER38, PLAYER39, PLAYER40, PLAYER41, PLAYER42, PLAYER43,
				PLAYER44, PLAYER45, PLAYER46, PLAYER47, PLAYER48, PLAYER49, PLAYER50, PLAYER51, PLAYER52, PLAYER53, PLAYER54, EYE1, EYE2, EYE3, EYE4,
				HELLHOUND1, HELLHOUND2, HELLHOUND3, HELLHOUND4, HELLHOUND5, SKELETON1, SKELETON2, SKELETON3, IMP1, IMP2, IMP3, IMP4, IMP5,
				MINOTAUR1, MINOTAUR2, MINOTAUR3, MINOTAUR4, MINOTAUR5, MINIBOSS1, MINIBOSS2, MINIBOSS3, MINIBOSS4, SHOP1, SHOP2, SHOP3, SHOP4,
				SHOP5, SHOP6, SHOP7, SHOP8, SHOP9, SHOP10, SHOP11, SHOP12, SHOP13, SHOP14, SHOP15, SHOP16, SHOP17, SHOP18, SHOP19};
enum Type {MENU, MUSIC, PLAYER, EYE, HELLHOUND, SKELETON, IMP, MINOTAUR, MINIBOSS, BOSS, SHOP};

enum ChannelGroup {MASTER_GROUP, SFX_GROUP, MUSIC_GROUP, VOICE_GROUP, AMBIENCE_GROUP};

struct SoundComponent
{
	bool playSound[CHANNEL_LIMIT] = { false, false };
	bool stopSound[CHANNEL_LIMIT] = { false, false };
	int soundIndex[CHANNEL_LIMIT] = { 0, 0 };
	int channelIndex[CHANNEL_LIMIT] = { 0, 0 };
	//ML_Vector<FMOD::Sound*> sounds; If loading all sounds once doesn't work
	ML_Vector<int> soundIndices;
	void Load(const int EntityType = 0);
	void Play(const int SoundIndex = 0, const int SelectedChannel = 0);
	void Stop(const int SelectedChannel = 0);
	void Unload();
};

struct Volume
{
	float volume = 1.0f;
	int group = MASTER_GROUP;

	Volume(float vol = 1.0f, int gr = MASTER_GROUP)
	{
		this->volume = vol;
		this->group = gr;
	}
};

struct AudioEngineComponent
{
	FMOD_RESULT result = FMOD_OK;
	FMOD::System* system = nullptr;
	ML_Vector<FMOD::Sound*> sounds;
	ML_Vector<FMOD::Channel*> channels;
	ML_Vector<FMOD::ChannelGroup*> groups;
	ML_Vector<int> freeChannels;
	ML_Vector<Volume> volumes;
	ML_Vector<int> occasionalVoiceLinesPlayed;
	void Setup(int& ID);
	void AddChannel();
	void StopAllSounds(bool stopMusic = false);
	void HandleSound();
	void HandleSpecificSound(bool& playSound, bool& stopSound, int& channelIndex, ML_Vector<int>& soundIndices, int& soundIndex);
	void Destroy();
};