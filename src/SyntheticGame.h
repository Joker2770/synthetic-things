/*************************************************************************
    > File Name: SyntheticGame.h
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Tue Nov 29 10:51:59 2022
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

#ifndef _SYNTHETIC_GAME_
#define _SYNTHETIC_GAME_

#include "SyntheticGameEngine.h"
#include "SyntheticGameProp.h"
#include "SyntheticGameProp_Extensions.h"

namespace SYNTHETIC_GAME
{
	//调用此函数快速开始游戏
	inline void GameStart()
	{
		SyntheticGameEngine* MyGame = new SyntheticGameEngine;

		//设置游戏难度
		//MyGame->SetDifficulty(5.0f);

		//设置游戏窗口名称
		MyGame->SetSGEName("Synthetic Things - By joker2770");

		//设置窗口大小和对应到“物理世界”的尺寸
		//MyGame->SetSGESize(400, 800);
		//MyGame->SetScale(20.0f);
		//MyGame->SetLineHeight(150);

		//设置颜色
		//MyGame->SetColor(ColorArea::BackgroundArea,olc::WHITE);
		//MyGame->SetColor(ColorArea::FontArea, olc::BLACK);

		//设置图片路径
		MyGame->SetPicturePath("./Picture");
		MyGame->SetFileName(FileName::BackgroundName, "Background.png");
		MyGame->SetFileName(FileName::BallName, "");
		MyGame->SetFileName(FileName::LineName, "Line.png");
		MyGame->SetFileName(FileName::BallBehindName, "png");
		MyGame->SetFileName(FileName::SuccessName, "Success.png");
		MyGame->SetFileName(FileName::EffectWaterName, "Water");
		MyGame->SetFileName(FileName::EffectFlowerAName, "FlowerA");
		MyGame->SetFileName(FileName::EffectFlowerBName, "FlowerB");

		//加载音频
		MyGame->LoadAudio(AudioName::SynthesisAudio, "./Sound/synthesis.wav");
		MyGame->LoadAudio(AudioName::DeadAudio, "./Sound/dead.wav");
		MyGame->LoadAudio(AudioName::BackgroundAudio, "./Sound/background.wav");
		MyGame->LoadAudio(AudioName::SuccessAudio, "./Sound/success.wav");

		//加载道具
		//注：道具加载的图片为 
		//图片路径（在上面已设置） + 道具名称 + 后缀名（在上面已设置）
		MyGame->LoadProp<PropChange>();
		MyGame->LoadProp<PropDestroy>();
		MyGame->LoadProp<PropRandom>();
		MyGame->LoadProp<PropGravity>();
		MyGame->LoadProp<PropFuse>();

		//设置道具生成的概率
		//MyGame->SetPropProbability(0.1f);

		MyGame->GameStart();

		delete MyGame;
	}
}

#endif

