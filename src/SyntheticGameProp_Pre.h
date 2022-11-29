/*************************************************************************
    > File Name: SyntheticGameProp_Pre.h
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Tue Nov 29 10:07:48 2022
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

#ifndef _SYNTHETIC_GAME_PROP_PRE_
#define _SYNTHETIC_GAME_PROP_PRE_

#include "SyntheticGameSettings.h"
#include "box2d/box2d.h"

#define OLC_PGEX_SOUND
#include "olcPixelGameEngine/extensions/olcPGEX_Sound.h"

namespace SYNTHETIC_GAME
{
	class SyntheticGameProp_Pre
	{
	public:
		float radius = 1.0f;
		virtual void GameStart() = 0;
		virtual void Generate(b2Body* _self_) = 0;
		virtual void Down(b2Body* _self_) = 0;
		virtual void Crash(b2Body* _self_, b2Body* crash_ball) = 0;
		virtual void Clock(b2Body* _self_, float time) = 0;
		virtual void Free(b2Body* _self_) = 0;
		virtual void Draw(b2Body* _self_) = 0;
	};
}

#endif

