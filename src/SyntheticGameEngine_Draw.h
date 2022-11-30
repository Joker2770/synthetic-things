/*************************************************************************
    > File Name: SyntheticGameEngine_Draw.h
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Tue Nov 29 10:39:42 2022
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

#ifndef _SYNTHETIC_GAME_ENGINE_DRAW_
#define _SYNTHETIC_GAME_ENGINE_DRAW_

#ifdef _SYNTHETIC_GAME_ENGINE_

void SYNTHETIC_GAME::SyntheticGameEngine::SGEDrawWindows()
{
	if (background != nullptr)
	{
		SetDrawTarget(2);
		DrawDecal({ 0.0f,0.0f }, background, backgroud_scale);
	}

	SetDrawTarget(1);
	Clear(olc::BLANK);

	if (line == nullptr)
	{
		DrawLine(0, SGE_Line, SGE_Width, SGE_Line, olc::VERY_DARK_MAGENTA);
	}
	else
	{
		DrawDecal({ 0,SGE_Line_f }, line, line_scale);
	}

	b2Body* body = m_world->GetBodyList();
	int32_t body_num = m_world->GetBodyCount();
	if (pictures.empty())
	{
		for (int32_t i = 0; i < body_num; i++, body = body->GetNext())
		{
			BodyInfo* info = (BodyInfo*)(body->GetUserData().pointer);

			if (info->level >= 0 && info->level < BallTypeNum)
			{
				b2Vec2 center{ body->GetPosition().x * SGE_Scale_B2,
					body->GetPosition().y * SGE_Scale_B2 };
				float Radius = body->GetFixtureList()->GetShape()->m_radius * SGE_Scale_B2;

				olc::Pixel ball_color = BallColor[info->level],
					line_color = BackGroundColor;

				for (auto& x : flash_list)
					if (x.body == body && x.clock > 0.0f)
						ball_color.a = uint8_t(x.alpha),
						line_color.a = uint8_t(x.alpha);

				FillCircle(center.x, center.y, Radius, ball_color);

				DrawLine(center.x, center.y,
					center.x + Radius * cosf(body->GetAngle()),
					center.y + Radius * sinf(body->GetAngle()),
					line_color);
			}
		}
	}
	else
	{
		for (int32_t i = 0; i < body_num; i++, body = body->GetNext())
		{
			BodyInfo* info = (BodyInfo*)(body->GetUserData().pointer);

			if (info->level >= 0 && info->level < BallTypeNum)
			{
				olc::Pixel ball_color = olc::WHITE;
				float Radius = body->GetFixtureList()->GetShape()->m_radius * SGE_Scale_B2;

				for (auto& x : flash_list)
					if (x.body == body && x.clock > 0.0f)
						ball_color.a = uint8_t(x.alpha);

				DrawRotatedDecal({ body->GetPosition().x * SGE_Scale_B2,
					body->GetPosition().y * SGE_Scale_B2 },
					pictures[info->level],
					body->GetAngle(),
					pictures_center[info->level],
					{ Radius * pictures_2inv[info->level].x,
					Radius * pictures_2inv[info->level].y },
					ball_color);
			}
		}
	}

	for (auto x : prop_body)
	{
		BodyInfo* info = (BodyInfo*)(x->GetUserData().pointer);
		info->prop->Draw(x);
	}

	for (auto& x : quick_move_list)
	{
		if (x.picture == nullptr)
		{
			b2Vec2 center{ x.now_pos.x * SGE_Scale_B2,
				x.now_pos.y * SGE_Scale_B2 };

			FillCircle(center.x, center.y, x.sge_radius, x.color);

			DrawLine(center.x, center.y,
				center.x + x.sge_radius * cosf(x.angle),
				center.y + x.sge_radius * sinf(x.angle),
				BackGroundColor);
		}
		else
		{
			DrawRotatedDecal({ x.now_pos.x * SGE_Scale_B2,
				x.now_pos.y * SGE_Scale_B2 },
				x.picture,
				x.angle,
				x.picture_center,
				x.picture_scale);
		}
	}

	SGEDrawSEffect();

	if (success_effect.clock > 0.0f)
	{
		olc::vf2d center{ SGE_Width_f * 0.5f, SGE_Height_f * 0.5f };
		float Radius = SGE_BallRadius[success_effect.level] * success_effect.scale;

		if (success != nullptr)
		{
			DrawRotatedDecal(center,
				success,
				success_effect.angle,
				{ success->sprite->width * 0.5f,success->sprite->width * 0.5f },
				success_scale * success_effect.scale);
		}

		if (pictures.empty())
		{
			olc::Pixel ball_color = BallColor[success_effect.level],
				line_color = BackGroundColor;

			FillCircle(center.x, center.y, Radius, ball_color);

			DrawLine(center.x, center.y,
				center.x + Radius * cosf(-success_effect.angle),
				center.y + Radius * sinf(-success_effect.angle),
				line_color);
		}
		else
		{
			DrawRotatedDecal(center,
				pictures[success_effect.level],
				-success_effect.angle,
				pictures_center[success_effect.level],
				{ Radius * pictures_2inv[success_effect.level].x,
				Radius * pictures_2inv[success_effect.level].y });
		}
	}

	SetDrawTarget(uint8_t(0));

	Clear(olc::BLANK);

	sprintf(string_buf, "Score:%d", score);
	DrawString(5, 5, string_buf, FontColor);

	if (combo > 1)
	{
		sprintf(string_buf, "Combo X%d !", combo);
		DrawString(5, 20, string_buf, FontColor);
	}

	if (dead)
	{
		auto pos = GetTextSize("Game Over!");
		DrawString((ScreenWidth() - pos.x) / 2, 30, "Game Over!", FontColor);
	}
	else if (!IsFocused())
	{
		auto pos = GetTextSize("Game Pause!");
		DrawString((ScreenWidth() - pos.x) / 2, 30, "Game Pause!", FontColor);
	}
}

#endif
#endif

