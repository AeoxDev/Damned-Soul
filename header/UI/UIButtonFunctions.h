#pragma once

#include "Relics/RelicFunctions.h"
#include "MemLib/ML_Vector.hpp"

namespace UIFunctions
{

	namespace MainMenu
	{
		void Start(void* args, int a);

		void SetSettings(void* args, int a);

		void SetCredits(void* args, int a);

		void Quit(void* args, int a);

	}

	namespace Game
	{
		void LoadNextLevel(void* args, int a);

		void SetMainMenu(void* args, int a);

	}

	namespace Settings
	{
		void Back(void* args, int a);

		void SetLowRes(void* args, int a);

		void SetMediumRes(void* args, int a);

		void SetHighRes(void* args, int a);

		void SetFullscreen(void* args, int a);

		void SwitchTimer(void* args, int a);

		namespace Volume
		{
			void Master_Press(void* args, int a);

			void Master_Release(void* args, int a);

			void Voice_Press(void* args, int a);

			void Voice_Release(void* args, int a);

			void SFX_Press(void* args, int a);

			void SFX_Release(void* args, int a);

			void Music_Press(void* args, int a);

			void Music_Release(void* args, int a);

			void Ambient_Press(void* args, int a);

			void Ambient_Release(void* args, int a);
		}
	}


	void Credits_Back(void* args, int a);
	
	namespace Pause
	{
		void Resume(void* args, int a);

		void SetSettings(void* args, int a);

		void Back(void* args, int a);

		void SetMainMenu(void* args, int a);
	}

	namespace OnClick
	{
		void None(void* args, int index);

		void SelectRelic(void* args, int index);

		void HealPlayer(void* args, int index);

		void RerollRelic(void* args, int index);

		void LockRelic(void* args, int index);

		void BuyRelic(void* args, int index);
	}

	namespace OnHover
	{
		void None(void* args, int index, bool hover);

		void Image(void* args, int index, bool hover);

		void ShopButton(void* args, int index, bool hover);

		void ShopRelic(void* args, int index, bool hover);

		void PlayerRelic(void* args, int index, bool hover);
	}

}
