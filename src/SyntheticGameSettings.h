/*************************************************************************
    > File Name: SyntheticGameSettings.h
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Tue Nov 29 10:16:07 2022
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

#ifndef _SYNTHETIC_GAME_SETTINGS_
#define _SYNTHETIC_GAME_SETTINGS_

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

namespace SYNTHETIC_GAME
{
	class SyntheticGameSettings
	{
		using String = std::string;
		template<class _Ty>
		using Vector = std::vector<_Ty>;

	public:
		SyntheticGameSettings()
		{
			GameDifficulty = 5.0f;
			difficulty = GameDifficulty / 5.0f;

			BackgroundPictureName = "Background.png";
			BallPicturesName = "";
			LinePictureName = "Line.png";
			BehindName = "png";
			SuccessPictureName = "Success.png";

			WaterName = "Water";
			FlowerAName = "FlowerA";
			FlowerBName = "FlowerB";

			BallRadius =
			{ 1.0f,1.2f,1.8f,2.0f,2.4f,2.8f,3.1f,4.0f,4.4f,5.6f,6.0f };

			BallTypeNum = BallRadius.size();

			GenerateNum = BallTypeNum / 2;

			BallColor =
			{ olc::MAGENTA,olc::DARK_RED,olc::Pixel(255,165,0),olc::DARK_GREEN,
				olc::DARK_YELLOW,olc::CYAN,olc::VERY_DARK_MAGENTA ,olc::GREEN,
				olc::DARK_GREY,olc::BLUE,olc::RED };

			BackGroundColor = olc::WHITE;

			FontColor = olc::BLACK;

			SGE_Width = 400;
			SGE_Height = 800;
			SGE_Line = 150;
			SGE_Generate = SGE_Line * 4 / 5;

			FloatSet();

			SGE_Scale_B2 = 20.0f;

			ScaleMatch();

			RebuilTime = 0.5f;
			DeadTime = 0.4f * difficulty;
			StretchTime = 0.2f;
			DeadBombTime = 0.1f;

			FlashStep = 1530.0f;
			StretchRate = 0.2f;

			PProp = 0.1f;
		}

		void FloatSet()
		{
			SGE_Width_f = float(SGE_Width);
			SGE_Height_f = float(SGE_Height);
			SGE_Generate_f = float(SGE_Generate);
			SGE_Line_f = float(SGE_Line);
		}

		void ScaleMatch()
		{
			SGE_BallRadius.resize(BallRadius.size());
			for (size_t i = 0; i < BallRadius.size(); i++)
				SGE_BallRadius[i] = BallRadius[i] * SGE_Scale_B2;

			B2_Width = SGE_Width_f / SGE_Scale_B2;
			B2_Height = SGE_Height_f / SGE_Scale_B2;
			B2_Generate = SGE_Generate / SGE_Scale_B2;
			B2_Line = SGE_Line / SGE_Scale_B2;

			B2_Left = 0.1f;
			B2_Right = B2_Width - B2_Left;

			B2_Gravity = B2_Height * 1.25f;
		}

	protected:
		float GameDifficulty;
		float difficulty;

		int32_t BallTypeNum;

		int32_t GenerateNum;

		std::string BackgroundPictureName;
		std::string BallPicturesName;
		std::string LinePictureName;
		std::string BehindName;
		std::string SuccessPictureName;

		std::string WaterName;
		std::string FlowerAName;
		std::string FlowerBName;

		Vector<float> BallRadius;

		Vector<olc::Pixel> BallColor;

		olc::Pixel BackGroundColor;
		olc::Pixel FontColor;

		int32_t SGE_Width;
		int32_t SGE_Height;
		int32_t SGE_Generate;
		int32_t SGE_Line;

		float SGE_Width_f;
		float SGE_Height_f;
		float SGE_Generate_f;
		float SGE_Line_f;

		float SGE_Scale_B2;

		Vector<float> SGE_BallRadius;

		float B2_Width;
		float B2_Height;
		float B2_Generate;
		float B2_Line;

		float B2_Left;
		float B2_Right;

		float B2_Gravity;

		float RebuilTime;
		float DeadTime;
		float StretchTime;
		float DeadBombTime;

		float FlashStep;
		float StretchRate;

		float PProp;
	};
}

#endif
