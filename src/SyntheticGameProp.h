/*************************************************************************
    > File Name: SyntheticGameProp.h
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Tue Nov 29 10:27:35 2022
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

#ifndef _SYNTHETIC_GAME_PROP_
#define _SYNTHETIC_GAME_PROP_

#ifdef _SYNTHETIC_GAME_ENGINE_

namespace SYNTHETIC_GAME
{
	class SyntheticGameProp :
		public SyntheticGameProp_Pre
	{
		friend class SyntheticGameEngine;
	public:
		using String = std::string;
		template<class _Ty>
		using Vector = std::vector<_Ty>;

		using BodyInfo = SyntheticGameEngine::BodyInfo;

		SyntheticGameEngine* engine = nullptr;

		String name;

		olc::Pixel color = olc::BLACK;

		String image_path;
		olc::Decal* image = nullptr;
		olc::vf2d image_center;
		olc::vf2d image_2inv;

		SyntheticGameProp() = default;

		inline void SetPropImagePath(const String& _image_path_)
		{
			image_path = _image_path_;
		}

		inline int32_t LoadAudio(const String& audio_path)
		{
			return olc::SOUND::LoadAudioSample(audio_path);
		}

		virtual void GameStart()
		{
			if (!image_path.empty())
			{
				if (image != nullptr)
					return;

				olc::Sprite* new_sprite = new olc::Sprite;
				if (new_sprite->LoadFromFile(image_path) == olc::rcode::OK)
				{
					image = new olc::Decal(new_sprite);

					image_center = { new_sprite->width * 0.5f, new_sprite->height * 0.5f };
					image_2inv = { 2.0f / new_sprite->width,2.0f / new_sprite->height };
				}
				else
				{
					delete new_sprite;
				}
			}
		}

		virtual void Generate(b2Body* _self_) {}

		virtual void Down(b2Body* _self_) {}

		virtual void Crash(b2Body* _self_, b2Body* crash_ball) {}

		virtual void Clock(b2Body* _self_, float time) {}

		virtual void Free(b2Body* _self_) {}

		virtual void Draw(b2Body* _self_)
		{
			if (image == nullptr)
			{
				b2Vec2 center{ _self_->GetPosition().x * engine->SGE_Scale_B2,
				_self_->GetPosition().y * engine->SGE_Scale_B2 };
				float Radius = _self_->GetFixtureList()->GetShape()->m_radius * engine->SGE_Scale_B2;

				olc::Pixel ball_color = color,
					line_color = engine->BackGroundColor;

				for (auto& x : engine->flash_list)
					if (x.body == _self_ && x.clock > 0.0f)
						ball_color.a = uint8_t(x.alpha),
						line_color.a = uint8_t(x.alpha);

				engine->FillCircle(center.x, center.y, Radius, ball_color);

				engine->DrawLine(center.x, center.y,
					center.x + Radius * cosf(_self_->GetAngle()),
					center.y + Radius * sinf(_self_->GetAngle()),
					line_color);

				auto pos = engine->GetTextSize(name);
				engine->DrawString(center.x - pos.x * 0.5f, center.y - pos.y * 0.5f, name, engine->FontColor);
			}
			else
			{
				olc::Pixel ball_color = olc::WHITE;
				float Radius = _self_->GetFixtureList()->GetShape()->m_radius * engine->SGE_Scale_B2;

				for (auto& x : engine->flash_list)
					if (x.body == _self_ && x.clock > 0.0f)
						ball_color.a = uint8_t(x.alpha);

				engine->DrawRotatedDecal({ _self_->GetPosition().x * engine->SGE_Scale_B2,
					_self_->GetPosition().y * engine->SGE_Scale_B2 },
					image,
					_self_->GetAngle(),
					image_center,
					{ Radius * image_2inv.x, Radius * image_2inv.y },
					ball_color);
			}
		}

	protected:
		inline bool IsProp(b2Body* _judge_body_)
		{
			BodyInfo* info = (BodyInfo*)(_judge_body_->GetUserData().pointer);
			return (info->prop != nullptr && info->level != -2);
		}

		inline bool IsNormal(b2Body* _judge_body_)
		{
			BodyInfo* info = (BodyInfo*)(_judge_body_->GetUserData().pointer);
			return (info->prop == nullptr && info->level != -2);
		}

		inline bool IsBall(b2Body* _judge_body_)
		{
			BodyInfo* info = (BodyInfo*)(_judge_body_->GetUserData().pointer);
			return (info->level != -2);
		}

		inline bool IsDown(b2Body* _judge_body_)
		{
			return (IsBall(_judge_body_) && _judge_body_ != engine->next_ball);
		}

		void SetLevel(b2Body* _body_, int32_t _level_)
		{
			if (!IsBall(_body_))
				return;

			BodyInfo* info = (BodyInfo*)(_body_->GetUserData().pointer);
			info->level = _level_;

			_body_->GetFixtureList()->GetShape()->m_radius = engine->BallRadius[info->level];
			_body_->SetAwake(true);
			engine->SGESetStretch(_body_);
			engine->SGEFreeProp(_body_);
		}

		inline b2Body * CreateBall(float pos_x,float pos_y)
		{
			return engine->CreateNewBall(pos_x, pos_y, 0, b2Vec2_zero, false);
		}

		inline void DestroyBall(b2Body* _body_)
		{
			if (IsBall(_body_))
			{
				for (auto x : engine->destroy_list)
				{
					if (x == _body_)
					{
						return;
					}
				}
				engine->destroy_list.push_back(_body_);
			}
		}

		inline int32_t GetGenerateLevel()
		{
			return engine->GenerateNum - 1;
		}

		inline int32_t GetMaxLevel()
		{
			return engine->BallTypeNum - 1;
		}

		inline int32_t GetScore()
		{
			return engine->score;
		}

		inline void AddScore(int32_t _change_score_)
		{
			engine->score += _change_score_;
		}

		inline void PlayAudio(AudioName Type, bool loop = false)
		{
			if (engine->AudioId[int(Type)] != -1)
			{
				olc::SOUND::PlaySample(engine->AudioId[int(Type)], loop);
			}
		}

		inline void PlayAudio(int32_t Id, bool loop = false)
		{
			if (Id != -1)
			{
				olc::SOUND::PlaySample(Id, loop);
			}
		}

		inline void StopAudio(AudioName Type)
		{
			if (engine->AudioId[int(Type)] != -1)
			{
				olc::SOUND::StopSample(engine->AudioId[int(Type)]);
			}
		}

		inline void StopAudio(int32_t Id)
		{
			if (Id != -1)
			{
				olc::SOUND::StopSample(Id);
			}
		}

		inline String GetImagePath()
		{
			return engine->picture_path;
		}

		inline float GetGravity()
		{
			return engine->m_world->GetGravity().y;
		}

		Vector<b2Body*> GetAllBalls()
		{
			Vector<b2Body*> result;

			b2Body* body = engine->m_world->GetBodyList();
			int32_t body_num = engine->m_world->GetBodyCount();
			for (int32_t i = 0; i < body_num; i++, body = body->GetNext())
			{
				if (IsDown(body))
				{
					result.push_back(body);
				}
			}

			return result;
		}

		Vector<b2Body*> GetAllNormalBalls()
		{
			Vector<b2Body*> result;

			b2Body* body = engine->m_world->GetBodyList();
			int32_t body_num = engine->m_world->GetBodyCount();
			for (int32_t i = 0; i < body_num; i++, body = body->GetNext())
			{
				if (IsDown(body) && !IsProp(body))
				{
					result.push_back(body);
				}
			}

			return result;
		}

		inline Vector<b2Body*> GetAllProps()
		{
			return engine->prop_body;
		}

		inline void SetPropBall(b2Body* body)
		{
			BodyInfo* info = (BodyInfo*)(body->GetUserData().pointer);
			info->prop = this;
			info->level = -1;
			engine->prop_body.push_back(body);
			info->prop->Generate(body);
		}

		inline void SetRandomPropBall(b2Body* body)
		{
			engine->SGESetProp(body);
		}

		inline void SetFlash(b2Body * body,float flash_time = 0.0f)
		{
			engine->flash_list.emplace_back(body);
			if (flash_time != 0.0f)
			{
				engine->flash_list.back().clock = flash_time;
			}
		}

		inline void SetQuickMove(b2Body* _self_, const b2Vec2& _target_pos_)
		{
			if (_self_->GetPosition() == _target_pos_)
				return;

			auto QMf = [=](const b2Vec2& _distance_)->float
			{
				return (abs(_distance_.x) + abs(_distance_.y));
			};
			
			engine->quick_move_list.emplace_back(radius * engine->SGE_Scale_B2,
				image,
				color,
				_self_->GetAngle(),
				_self_->GetPosition(),
				_target_pos_);
			engine->quick_move_list.back().speed *= 
				QMf(engine->quick_move_list.back().end_pos - engine->quick_move_list.back().now_pos);
		}

		inline void SetSyntheticEffect(const b2Vec2& effect_pos, int32_t effect_level)
		{
			engine->SGESetSEffect(effect_pos.x * engine->SGE_Scale_B2, effect_pos.y * engine->SGE_Scale_B2, effect_level);
		}

		inline Vector<Vector<b2Fixture*>>& GetTouchBallList()
		{
			return engine->touch_balls;
		}

		inline void Synthesis(b2Body* bodyA, b2Body* bodyB, int32_t level)
		{
			engine->PreSyntheticOperation(bodyA, bodyB, level);
		}

		inline bool IsDead()
		{
			return engine->dead;
		}

		inline void SetSuccessEffect(int32_t effect_level)
		{
			engine->SGESetSuccess(effect_level);
		}
	};
}

#endif
#endif

