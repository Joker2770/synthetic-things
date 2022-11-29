/*************************************************************************
	> File Name: main.cpp
	> Author: Jintao Yang
	> Mail: 18608842770@163.com
	> Created Time: Tue Nov 29 11:09:08 2022
 ************************************************************************/

/**
	A fantastic factory to make synthetic fruits.
	Copyright (C) 2022  joker2770

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "SyntheticGame.h"
//#define PHOTO_LOAD_TEST1
//#define SOUND_LOAD_TEST1

#ifdef PHOTO_LOAD_TEST
class PLTest : public olc::PixelGameEngine
{
	using String = std::string;

public:
	PLTest()
	{
		sAppName = "Picture Load Test";
	}

	olc::Sprite *spr_test = nullptr;
	olc::Decal *dec_test = nullptr;
	olc::vf2d scale;

public:
	bool OnUserCreate() override
	{
		String file_name;

		if (std::cout << "Please Input Picture Load Path:" << std::endl && std::cin >> file_name)
		{
			spr_test = new olc::Sprite;
			if (spr_test->LoadFromFile(file_name) == olc::rcode::OK)
			{
				dec_test = new olc::Decal(spr_test);
				std::cout << ScreenWidth() << std::endl;
				std::cout << ScreenHeight() << std::endl;
				scale = {ScreenWidth() / float(dec_test->sprite->width),
						 ScreenHeight() / float(dec_test->sprite->height)};
				std::cout << "Open pictures successful!" << std::endl;
				return true;
			}
		}

		std::cout << "Open pictures failed!" << std::endl;
		return false;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{

		DrawDecal({0, 0}, dec_test, scale);
		return true;
	}

	bool OnUserDestroy() override
	{
		if (spr_test != nullptr)
			delete spr_test,
				spr_test = nullptr;

		if (dec_test != nullptr)
			delete dec_test,
				dec_test = nullptr;

		return true;
	}
};
#elif SOUND_LOAD_TEST
class WAVTest : public olc::PixelGameEngine
{
	using String = std::string;

public:
	WAVTest()
	{
		sAppName = "Wave Load Test";
	}

	int WavId = -1;

public:
	bool OnUserCreate() override
	{
		String wave_path;
		olc::SOUND::InitialiseAudio(44100U, 2U);

		if (std::cin >> wave_path)
		{
			WavId = olc::SOUND::LoadAudioSample(wave_path);
			if (WavId != -1)
			{
				std::cout << "Load audio files successul!" << std::endl;
				olc::SOUND::PlaySample(WavId, true);
				return true;
			}
		}

		std::cout << "Load audio files failed!" << std::endl;
		return false;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		return true;
	}

	bool OnUserDestroy() override
	{
		olc::SOUND::DestroyAudio();
	}
};
#endif

int main()
{
#ifdef PHOTO_LOAD_TEST
	PLTest Test;
	if (Test.Construct(800, 600, 1, 1))
		Test.Start();
#else
#ifdef SOUND_LOAD_TEST
	WAVTest Test;
	if (Test.Construct(100, 100, 1, 1))
		Test.Start();
#else
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
	SYNTHETIC_GAME::GameStart();
#endif
#endif

	return 0;
}
