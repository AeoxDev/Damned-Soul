#include "Audio.h"
#include "Registry.h"
#include <assert.h>

void AudioEngineComponent::Setup(int& ID)
{
	this->result = FMOD::System_Create(&this->system);      // Create the main system object.
	assert(this->result == FMOD_OK);

	this->result = this->system->init(256, FMOD_INIT_NORMAL, 0);    // Initialize FMOD (The value 256 is the max amount of channels. If we have problems, increase this value)
	assert(this->result == FMOD_OK);

	//Herman: I suspect uninitialized ML_[Container] structs might cause infrequent crashes
	this->sounds.Initialize();
	this->channels.Initialize();
	this->groups.Initialize();
	this->freeChannels.Initialize();
	this->volumes.Initialize();

	FMOD::ChannelGroup* add = nullptr;
	this->groups.clear();
	this->groups.push_back(add); //Master
	this->groups.push_back(add); //SFX
	this->groups.push_back(add); //Music
	this->groups.push_back(add); //Voice
	this->groups.push_back(add); //Ambient

	system->getMasterChannelGroup(&this->groups[MASTER_GROUP]);
	system->createChannelGroup("SFX Group", &this->groups[SFX_GROUP]);
	this->groups[MASTER_GROUP]->addGroup(this->groups[SFX_GROUP]);
	system->createChannelGroup("Music Group", &this->groups[MUSIC_GROUP]);
	this->groups[MASTER_GROUP]->addGroup(this->groups[MUSIC_GROUP]);
	system->createChannelGroup("Voice Group", &this->groups[VOICE_GROUP]);
	this->groups[MASTER_GROUP]->addGroup(this->groups[VOICE_GROUP]);
	system->createChannelGroup("Ambient Group", &this->groups[AMBIENCE_GROUP]);
	this->groups[MASTER_GROUP]->addGroup(this->groups[AMBIENCE_GROUP]);

	//Load all sounds to use in the game
	FMOD::Sound* toAdd = nullptr;
	this->sounds.clear();
	for (int i = 0; i < 132; i++) //Change x in (i < x) to however many sounds you want to load into the game.
	{
		this->sounds.push_back(toAdd);
	}
	int i = 0;

	//Menu (3)
	this->system->createSound("SFX/Menu/MouseHoverButton.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(1.0f, SFX_GROUP));
	this->system->createSound("SFX/Menu/MenuButtonPress.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(1.0f, SFX_GROUP));
	this->system->createSound("SFX/Menu/StartGameClick.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(1.0f, SFX_GROUP));

	//Music (8)
	this->system->createSound("SFX/Music/TitleTheme.mp3", FMOD_LOOP_NORMAL, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, MUSIC_GROUP));
	this->system->createSound("SFX/Music/LavaCave.mp3", FMOD_LOOP_NORMAL, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, MUSIC_GROUP));
	this->system->createSound("SFX/Music/Shop.mp3", FMOD_LOOP_NORMAL, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.1f, MUSIC_GROUP));
	this->system->createSound("SFX/Music/Ice.mp3", FMOD_LOOP_NORMAL, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, MUSIC_GROUP));
	this->system->createSound("SFX/Music/Boss.mp3", FMOD_LOOP_NORMAL, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, MUSIC_GROUP));
	this->system->createSound("SFX/Music/CaveAmbience.mp3", FMOD_LOOP_NORMAL, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(1.0f, AMBIENCE_GROUP));
	this->system->createSound("SFX/Music/LavaAmbience.mp3", FMOD_LOOP_NORMAL, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.75f, AMBIENCE_GROUP));
	this->system->createSound("SFX/Music/BlizzardAmbience.mp3", FMOD_LOOP_NORMAL, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.15f, AMBIENCE_GROUP));

	//Player (54)
		//Constants (8/54)
	this->system->createSound("SFX/Player/Constant/Combo1.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Player/Constant/Combo2.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Player/Constant/Combo3.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Player/Constant/HeavyCharge.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.75f, SFX_GROUP));
	this->system->createSound("SFX/Player/Constant/HeavyAttack.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Player/Constant/Dash.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Player/Constant/Hurt.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Player/Constant/Death.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
		//Intros (17/54)
	this->system->createSound("SFX/Player/Intro/IntroSkeleton1.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Intro/IntroSkeleton2.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Intro/IntroDog1.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Intro/IntroDog2.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Intro/IntroDog3.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Intro/IntroDog4.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Intro/IntroEye1.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Intro/IntroEye2.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Intro/IntroEye3.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Intro/IntroEye4.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Intro/IntroImp1.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Intro/IntroImp2.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Intro/IntroImp3.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Intro/IntroMinotaur1.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Intro/IntroMinotaur2.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Intro/IntroSplitBoss1.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Intro/IntroSplitBoss2.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
		//Voice (29/54)
	this->system->createSound("SFX/Player/Voice/Hot.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Voice/Chills1.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Voice/Chills2.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Voice/BetterLuck.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Voice/AnotherSoul.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Voice/BringItOn.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Voice/ThisWillBeFun.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Voice/DontMind.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Voice/FindersKeepers.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Voice/WatchStep.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Voice/PlaceSucks.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Voice/HotInHere.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Voice/BurningUp.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Voice/IceInHell.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Voice/HotChocolate.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Voice/WasntSoBad.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Voice/BetterWork.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Voice/SomethingPositive.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Voice/LikeTheLook.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Voice/WillDoDamage.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Voice/SuckOnThat.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Voice/WinnerIs.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Voice/HellYeah.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Voice/NotAgain.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Voice/GetRevenge.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Voice/HellSucks.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Voice/TimeToGo.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Voice/GetGoing.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));
	this->system->createSound("SFX/Player/Voice/ConvenientGates.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, VOICE_GROUP));

	//Eye (4)
	this->system->createSound("SFX/Enemy/Eye/Attack.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Eye/Shoot.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Eye/Hurt.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Eye/Death.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));

	//Hellhound (5)
	this->system->createSound("SFX/Enemy/Hellhound/Attack.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Hellhound/Hurt.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Hellhound/Inhale.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Hellhound/Flamethrower.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Hellhound/Death.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));

	//Skeleton (3)
	this->system->createSound("SFX/Enemy/Skeleton/Attack.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Skeleton/Hurt.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Skeleton/Death.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));

	//Imp (5)
	this->system->createSound("SFX/Enemy/Imp/AttackCharge.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.20f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Imp/AttackThrow.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.20f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Imp/Teleport.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.4f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Imp/Hurt.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Imp/Death.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, SFX_GROUP));

	//Minotaur (6)
	this->system->createSound("SFX/Enemy/Minotaur/Attack.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Minotaur/Charge.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Minotaur/Jump.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.75f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Minotaur/Slam.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Minotaur/Hurt.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Minotaur/Death.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));

	//Miniboss (5)
	this->system->createSound("SFX/Enemy/Miniboss/Attack.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Miniboss/Slam.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.5f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Miniboss/Hurt.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Miniboss/Reassembling.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.75f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Miniboss/Death.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));

	//Shop (18)
		//Sound Effects (5/18)
	this->system->createSound("SFX/Shop/Sound/BuyRelic.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Shop/Sound/Heal.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Shop/Sound/Dibs.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Shop/Sound/Reroll.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Shop/Sound/UpgradeWeapon.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
		//Voice (13/18)
	this->system->createSound("SFX/Shop/Voice/FirstMeet.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.75f, VOICE_GROUP));
	this->system->createSound("SFX/Shop/Voice/BeforeSplitBoss.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.75f, VOICE_GROUP));
	this->system->createSound("SFX/Shop/Voice/BuyingNothing.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.75f, VOICE_GROUP));
	this->system->createSound("SFX/Shop/Voice/FirstPurchase.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.75f, VOICE_GROUP));
	this->system->createSound("SFX/Shop/Voice/PurchaseBeforeBoss.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.75f, VOICE_GROUP));
	this->system->createSound("SFX/Shop/Voice/LowHealth.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.75f, VOICE_GROUP));
	this->system->createSound("SFX/Shop/Voice/RelicPurchase.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.75f, VOICE_GROUP));
	this->system->createSound("SFX/Shop/Voice/RelicPurchase2.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.75f, VOICE_GROUP));
	this->system->createSound("SFX/Shop/Voice/UpgradeWeapon.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.75f, VOICE_GROUP));
	this->system->createSound("SFX/Shop/Voice/UpgradeWeapon2.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.75f, VOICE_GROUP));
	this->system->createSound("SFX/Shop/Voice/BeforeLava.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.75f, VOICE_GROUP));
	this->system->createSound("SFX/Shop/Voice/BeforeIce.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.75f, VOICE_GROUP));
	this->system->createSound("SFX/Shop/Voice/BeforeLastBoss.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.75f, VOICE_GROUP));

	//Boss (21)
		//Sound Effects (11/21)
	this->system->createSound("SFX/Enemy/Boss/Constant/Attack.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Boss/Constant/Charge.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Boss/Constant/Jump.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Boss/Constant/Slam.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Boss/Constant/Channeling.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Boss/Constant/Hurt1.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Boss/Constant/Hurt2.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Boss/Constant/Hurt3.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Boss/Constant/Hurt4.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Boss/Constant/Hurt5.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
	this->system->createSound("SFX/Enemy/Boss/Constant/DeathNut.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, SFX_GROUP));
		//Voice (10/21)
	this->system->createSound("SFX/Enemy/Boss/Voice/AngryIntroGrunt.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, VOICE_GROUP));
	this->system->createSound("SFX/Enemy/Boss/Voice/APunyMortal.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, VOICE_GROUP));
	this->system->createSound("SFX/Enemy/Boss/Voice/CrushYou.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, VOICE_GROUP));
	this->system->createSound("SFX/Enemy/Boss/Voice/DodgeThis.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, VOICE_GROUP));
	this->system->createSound("SFX/Enemy/Boss/Voice/FlattenYou.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, VOICE_GROUP));
	this->system->createSound("SFX/Enemy/Boss/Voice/Heathen.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, VOICE_GROUP));
	this->system->createSound("SFX/Enemy/Boss/Voice/JudgementIsUpon.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, VOICE_GROUP));
	this->system->createSound("SFX/Enemy/Boss/Voice/MustNotDie.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, VOICE_GROUP));
	this->system->createSound("SFX/Enemy/Boss/Voice/YourCrimes.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, VOICE_GROUP));
	this->system->createSound("SFX/Enemy/Boss/Voice/YouveBeenJudged.mp3", FMOD_DEFAULT, 0, &this->sounds[i++]);
	this->volumes.push_back(Volume(0.25f, VOICE_GROUP));
}

void AudioEngineComponent::HandleSound()
{
	for (auto entity : View<SoundComponent>(registry)) //Access an entity
	{
		SoundComponent* audio = registry.GetComponent<SoundComponent>(entity);
		for (int i = 0; i < CHANNEL_LIMIT; i++)
		{
			if (audio->playSound[i]) //Check if the entity wants to play a sound
			{
				//Play the sound
				if (this->channels[audio->channelIndex[i]] != nullptr)
				{
					this->channels[audio->channelIndex[i]]->stop(); //Stop the previous sound
					this->channels[audio->channelIndex[i]] = nullptr; //Set the channel to nullptr (this is to prevent sound cutting off)
				}
				this->system->playSound(this->sounds[audio->soundIndices[audio->soundIndex[i]]], this->groups[this->volumes[audio->soundIndices[audio->soundIndex[i]]].group], false, &this->channels[audio->channelIndex[i]]); //Play the new sound
				this->channels[audio->channelIndex[i]]->setVolume(this->volumes[audio->soundIndices[audio->soundIndex[i]]].volume);
				audio->playSound[i] = false;
			}
			else if (audio->stopSound[i])
			{
				//Stop the sound
				this->channels[audio->channelIndex[i]]->stop(); //Stop the previous sound
				this->channels[audio->channelIndex[i]] = nullptr; //Set the channel to nullptr (this is to prevent sound cutting off)
				audio->stopSound[i] = false;
			}
		}
	}
}

void AudioEngineComponent::AddChannel()
{
	//Create a new channel and push it back into the vector
	FMOD::Channel* toAdd = nullptr;
	this->channels.push_back(toAdd);
}

void AudioEngineComponent::HandleSpecificSound(bool& playSound, bool& stopSound, int& channelIndex, ML_Vector<int>& soundIndices, int& soundIndex)
{
	if (playSound) //Check if the entity wants to play a sound
	{
		//Play the sound
		if (this->channels[channelIndex] != nullptr)
		{
			this->channels[channelIndex]->stop(); //Stop the previous sound
			this->channels[channelIndex] = nullptr; //Set the channel to nullptr (this is to prevent sound cutting off)
		}
		this->system->playSound(this->sounds[soundIndices[soundIndex]], this->groups[this->volumes[soundIndices[soundIndex]].group], false, &this->channels[channelIndex]); //Play the new sound
		this->channels[channelIndex]->setVolume(this->volumes[soundIndices[soundIndex]].volume);
		playSound = false;
	}
	else if (stopSound)
	{
		//Stop the sound
		this->channels[channelIndex]->stop(); //Stop the previous sound
		this->channels[channelIndex] = nullptr; //Set the channel to nullptr (this is to prevent sound cutting off)
		stopSound = false;
	}
}

void AudioEngineComponent::StopAllSounds(bool stopMusic)
{
	int start = 0;
	if (!stopMusic)
	{
		start = 2;
	}
	for (int i = start; i < (int)this->channels.size(); i++) //Go through all channels that exist
	{
		if (this->channels[i] != nullptr) //Check if the channel is currently playing a sound
		{
			this->channels[i]->stop(); //Stop the sounds
			this->channels[i] = nullptr; //Set the channel to nullptr
		}
	}
}

void AudioEngineComponent::Destroy()
{
	//Stop all sounds and release everything
	for (int i = 0; i < (int)this->channels.size(); i++)
	{
		if (this->channels[i] != nullptr)
		{
			this->channels[i]->stop();
			this->channels[i] = nullptr;
		}
	}
	for (int i = 0; i < (int)this->sounds.size(); i++)
	{
		this->sounds[i]->release();
	}
	for (int i = 4; i >= 0; i--)
	{
		this->groups[i]->stop();
		this->groups[i]->release();
		this->groups[i] = nullptr;
	}
	this->volumes.clear();
	this->occasionalVoiceLinesPlayed.clear();
	this->groups.~ML_Vector();
	this->sounds.~ML_Vector();
	this->channels.~ML_Vector();
	this->freeChannels.~ML_Vector();
	this->volumes.~ML_Vector();
	this->occasionalVoiceLinesPlayed.~ML_Vector();
	this->system->close();
	this->system->release();
}

void SoundComponent::Load(const int EntityType)
{
	for (auto entity : View<AudioEngineComponent>(registry)) //Access an entity
	{
		AudioEngineComponent* audioEngine = registry.GetComponent<AudioEngineComponent>(entity);

		//Check if there is a free channel that already exists
		for (int i = 0; i < CHANNEL_LIMIT; i++)
		{
			if ((int)audioEngine->freeChannels.size() > 0)
			{

				this->channelIndex[i] = audioEngine->freeChannels[(audioEngine->freeChannels.size() - 1)];
				audioEngine->freeChannels.pop_back();
			}
			else //If not enough space exists, create a new one
			{
				audioEngine->AddChannel();
				this->channelIndex[i] = (audioEngine->channels.size() - 1);
			}
		}
	}

	switch (EntityType)
	{
	default:
		break;
	case MENU:
		//Push back all indices for the menu sounds into soundIndices
		this->soundIndices.push_back((int)MENU1);
		this->soundIndices.push_back((int)MENU2);
		this->soundIndices.push_back((int)MENU3);
		break;
	case MUSIC:
		//Push back all indices for the music sounds into soundIndices
		this->soundIndices.push_back((int)MUSIC1);
		this->soundIndices.push_back((int)MUSIC2);
		this->soundIndices.push_back((int)MUSIC3);
		this->soundIndices.push_back((int)MUSIC4);
		this->soundIndices.push_back((int)MUSIC5);
		this->soundIndices.push_back((int)AMBIENCE1);
		this->soundIndices.push_back((int)AMBIENCE2);
		this->soundIndices.push_back((int)AMBIENCE3);
		break;
	case PLAYER:
		//Push back all indices for the player sounds into soundIndices
		this->soundIndices.push_back((int)PLAYER1);
		this->soundIndices.push_back((int)PLAYER2);
		this->soundIndices.push_back((int)PLAYER3);
		this->soundIndices.push_back((int)PLAYER4);
		this->soundIndices.push_back((int)PLAYER5);
		this->soundIndices.push_back((int)PLAYER6);
		this->soundIndices.push_back((int)PLAYER7);
		this->soundIndices.push_back((int)PLAYER8);
		this->soundIndices.push_back((int)PLAYER9);
		this->soundIndices.push_back((int)PLAYER10);
		this->soundIndices.push_back((int)PLAYER11);
		this->soundIndices.push_back((int)PLAYER12);
		this->soundIndices.push_back((int)PLAYER13);
		this->soundIndices.push_back((int)PLAYER14);
		this->soundIndices.push_back((int)PLAYER15);
		this->soundIndices.push_back((int)PLAYER16);
		this->soundIndices.push_back((int)PLAYER17);
		this->soundIndices.push_back((int)PLAYER18);
		this->soundIndices.push_back((int)PLAYER19);
		this->soundIndices.push_back((int)PLAYER20);
		this->soundIndices.push_back((int)PLAYER21);
		this->soundIndices.push_back((int)PLAYER22);
		this->soundIndices.push_back((int)PLAYER23);
		this->soundIndices.push_back((int)PLAYER24);
		this->soundIndices.push_back((int)PLAYER25);
		this->soundIndices.push_back((int)PLAYER26);
		this->soundIndices.push_back((int)PLAYER27);
		this->soundIndices.push_back((int)PLAYER28);
		this->soundIndices.push_back((int)PLAYER29);
		this->soundIndices.push_back((int)PLAYER30);
		this->soundIndices.push_back((int)PLAYER31);
		this->soundIndices.push_back((int)PLAYER32);
		this->soundIndices.push_back((int)PLAYER33);
		this->soundIndices.push_back((int)PLAYER34);
		this->soundIndices.push_back((int)PLAYER35);
		this->soundIndices.push_back((int)PLAYER36);
		this->soundIndices.push_back((int)PLAYER37);
		this->soundIndices.push_back((int)PLAYER38);
		this->soundIndices.push_back((int)PLAYER39);
		this->soundIndices.push_back((int)PLAYER40);
		this->soundIndices.push_back((int)PLAYER41);
		this->soundIndices.push_back((int)PLAYER42);
		this->soundIndices.push_back((int)PLAYER43);
		this->soundIndices.push_back((int)PLAYER44);
		this->soundIndices.push_back((int)PLAYER45);
		this->soundIndices.push_back((int)PLAYER46);
		this->soundIndices.push_back((int)PLAYER47);
		this->soundIndices.push_back((int)PLAYER48);
		this->soundIndices.push_back((int)PLAYER49);
		this->soundIndices.push_back((int)PLAYER50);
		this->soundIndices.push_back((int)PLAYER51);
		this->soundIndices.push_back((int)PLAYER52);
		this->soundIndices.push_back((int)PLAYER53);
		this->soundIndices.push_back((int)PLAYER54);
		break;
	case EYE:
		//Push back all indices for the eye sounds into soundIndices
		this->soundIndices.push_back((int)EYE1);
		this->soundIndices.push_back((int)EYE2);
		this->soundIndices.push_back((int)EYE3);
		this->soundIndices.push_back((int)EYE4);
		break;
	case HELLHOUND:
		//Push back all indices for the hellhound sounds into soundIndices
		this->soundIndices.push_back((int)HELLHOUND1);
		this->soundIndices.push_back((int)HELLHOUND2);
		this->soundIndices.push_back((int)HELLHOUND3);
		this->soundIndices.push_back((int)HELLHOUND4);
		this->soundIndices.push_back((int)HELLHOUND5);
		break;
	case SKELETON:
		//Push back all indices for the skeleton sounds into soundIndices
		this->soundIndices.push_back((int)SKELETON1);
		this->soundIndices.push_back((int)SKELETON2);
		this->soundIndices.push_back((int)SKELETON3);
		break;
	case IMP:
		//Push back all indices for the imp sounds into soundIndices
		this->soundIndices.push_back((int)IMP1);
		this->soundIndices.push_back((int)IMP2);
		this->soundIndices.push_back((int)IMP3);
		this->soundIndices.push_back((int)IMP4);
		this->soundIndices.push_back((int)IMP5);
		break;
	case MINOTAUR:
		//Push back all indices for the minotaur sounds into soundIndices
		this->soundIndices.push_back((int)MINOTAUR1);
		this->soundIndices.push_back((int)MINOTAUR2);
		this->soundIndices.push_back((int)MINOTAUR3);
		this->soundIndices.push_back((int)MINOTAUR4);
		this->soundIndices.push_back((int)MINOTAUR5);
		this->soundIndices.push_back((int)MINOTAUR6);
		break;
	case MINIBOSS:
		//Push back all indices for the miniboss sounds into soundIndices
		this->soundIndices.push_back((int)MINIBOSS1);
		this->soundIndices.push_back((int)MINIBOSS2);
		this->soundIndices.push_back((int)MINIBOSS3);
		this->soundIndices.push_back((int)MINIBOSS4);
		this->soundIndices.push_back((int)MINIBOSS5);
		break;
	case BOSS:
		//Push back all indices for the boss sounds into soundIndices
		this->soundIndices.push_back((int)BOSS1);
		this->soundIndices.push_back((int)BOSS2);
		this->soundIndices.push_back((int)BOSS3);
		this->soundIndices.push_back((int)BOSS4);
		this->soundIndices.push_back((int)BOSS5);
		this->soundIndices.push_back((int)BOSS6);
		this->soundIndices.push_back((int)BOSS7);
		this->soundIndices.push_back((int)BOSS8);
		this->soundIndices.push_back((int)BOSS9);
		this->soundIndices.push_back((int)BOSS10);
		this->soundIndices.push_back((int)BOSS11);
		this->soundIndices.push_back((int)BOSS12);
		this->soundIndices.push_back((int)BOSS13);
		this->soundIndices.push_back((int)BOSS14);
		this->soundIndices.push_back((int)BOSS15);
		this->soundIndices.push_back((int)BOSS16);
		this->soundIndices.push_back((int)BOSS17);
		this->soundIndices.push_back((int)BOSS18);
		this->soundIndices.push_back((int)BOSS19);
		this->soundIndices.push_back((int)BOSS20);
		this->soundIndices.push_back((int)BOSS21);
		break;
	case SHOP:
		//Push back all indices for the shop sounds into soundIndices
		this->soundIndices.push_back((int)MENU1);
		this->soundIndices.push_back((int)MENU2);
		this->soundIndices.push_back((int)SHOP1);
		this->soundIndices.push_back((int)SHOP2);
		this->soundIndices.push_back((int)SHOP3);
		this->soundIndices.push_back((int)SHOP4);
		this->soundIndices.push_back((int)SHOP5);
		this->soundIndices.push_back((int)SHOP6);
		this->soundIndices.push_back((int)SHOP7);
		this->soundIndices.push_back((int)SHOP8);
		this->soundIndices.push_back((int)SHOP9);
		this->soundIndices.push_back((int)SHOP10);
		this->soundIndices.push_back((int)SHOP11);
		this->soundIndices.push_back((int)SHOP12);
		this->soundIndices.push_back((int)SHOP13);
		this->soundIndices.push_back((int)SHOP14);
		this->soundIndices.push_back((int)SHOP15);
		this->soundIndices.push_back((int)SHOP16);
		this->soundIndices.push_back((int)SHOP17);
		this->soundIndices.push_back((int)SHOP18);
		break;
	}
}

void SoundComponent::Play(const int SoundIndex, const int SelectedChannel)
{
	//Set playSound to true and the selected sound index, make sure the index is within the valid range as well
	this->playSound[SelectedChannel] = true;
	if ((int)this->soundIndices.size() > SoundIndex)
	{
		this->soundIndex[SelectedChannel] = SoundIndex;
	}
	else
	{
		this->soundIndex[SelectedChannel] = (this->soundIndices.size() - 1);
		if (this->soundIndex[SelectedChannel] == -1)
		{
			this->playSound[SelectedChannel] = false;
		}
	}
}

void SoundComponent::Stop(const int SelectedChannel)
{
	//Set stopSound to true
	this->stopSound[SelectedChannel] = true;
}

void SoundComponent::Unload()
{
	for (auto entity : View<AudioEngineComponent>(registry)) //Access an entity
	{
		AudioEngineComponent* audioEngine = registry.GetComponent<AudioEngineComponent>(entity);

		//Play the audio before removing it and set the index for the channel as free.
		for (int i = 0; i < CHANNEL_LIMIT; i++)
		{
			audioEngine->HandleSpecificSound(this->playSound[i], this->stopSound[i], this->channelIndex[i], this->soundIndices, this->soundIndex[i]);
			audioEngine->freeChannels.push_back(this->channelIndex[i]);
		}
	}
	this->soundIndices.~ML_Vector();
}
