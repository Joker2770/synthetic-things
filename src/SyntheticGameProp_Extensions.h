/*************************************************************************
    > File Name: SyntheticGameProp_Extensions.h
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Tue Nov 29 10:22:12 2022
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

#ifndef _SYNTHETIC_GAME_PROP_EXTENSIONS_
#define _SYNTHETIC_GAME_PROP_EXTENSIONS_

#ifdef _SYNTHETIC_GAME_PROP_

namespace SYNTHETIC_GAME
{
	class PropChange :
		public SyntheticGameProp
	{
	public:
		PropChange()
		{
			name = "Change";
			color = olc::Pixel(238, 180, 180);
			radius = 1.5f;
		}

		void Crash(b2Body* _self_, b2Body* crash_ball) override
		{
			if (IsNormal(crash_ball))
			{
				BodyInfo* info = (BodyInfo*)(crash_ball->GetUserData().pointer);
				if (info->level <= GetGenerateLevel())
					SetLevel(_self_, info->level);
			}
		}
	};

	class PropDestroy :
		public SyntheticGameProp
	{
	public:
		int32_t audio_id = -1;

		PropDestroy()
		{
			name = "Destroy";
			color = olc::Pixel(245, 245, 220);
			radius = 1.7f;

			audio_id = LoadAudio("./Sound/destroy.wav");
		}

		void Crash(b2Body* _self_, b2Body* crash_ball) override
		{
			if (IsBall(crash_ball))
			{
				DestroyBall(crash_ball);
				DestroyBall(_self_);
				SetQuickMove(_self_, crash_ball->GetPosition());
				SetSyntheticEffect(crash_ball->GetPosition(), ((BodyInfo*)crash_ball->GetUserData().pointer)->level);

				if (audio_id != -1)
					PlayAudio(audio_id);
				else
					PlayAudio(AudioName::SynthesisAudio);
			}
		}
	};

	class PropRandom :
		public SyntheticGameProp
	{
	public:
		int32_t audio_id = -1;
		int32_t bell_audio_id = -1;

		PropRandom()
		{
			name = "Random";
			color = olc::Pixel(240, 128, 128);
			radius = 1.4f;

			audio_id = LoadAudio("./Sound/random.wav");
			bell_audio_id = LoadAudio("./Sound/random_bell.wav");
		}

		struct RandomClock
		{
			b2Body* body = nullptr;
			float clock = 3.0f;

			float bell_clock = 1.0f;

			RandomClock() = default;
			RandomClock(b2Body* _body_) :body(_body_) {}
		};

		Vector<RandomClock> clock_list;

		void Down(b2Body* _self_) override
		{
			clock_list.emplace_back(_self_);
			SetFlash(_self_, 3.0f);
		}

		void Clock(b2Body* _self_, float time) override
		{
			if (!IsDown(_self_))
				return;

			for (auto& x : clock_list)
			{
				if (x.body == _self_)
				{
					x.clock -= time;
					x.bell_clock -= time;

					if (x.clock <= 0.0f)
					{
						auto _list_ = GetAllNormalBalls();
						if (_list_.size() > 0)
						{
							SetLevel(_self_, ((BodyInfo*)(_list_[abs(int32_t(rand() % _list_.size()))]->GetUserData().pointer))->level);
						}
						else
						{
							SetLevel(_self_, 0);
						}

						SetSyntheticEffect(_self_->GetPosition(), ((BodyInfo*)_self_->GetUserData().pointer)->level);

						if (audio_id != -1)
						{
							PlayAudio(audio_id);
						}
					}
					else if (x.bell_clock <= 0.0f)
					{
						x.bell_clock = 1.0f;

						if (bell_audio_id != -1)
						{
							PlayAudio(bell_audio_id);
						}
					}
					return;
				}
			}
		}

		void Free(b2Body* _self_) override
		{
			int32_t _index_ = -1;
			for (auto& x : clock_list)
			{
				_index_++;
				if (x.body == _self_)
				{
					clock_list.erase(clock_list.begin() + _index_);
				}
			}
		}
	};

	class PropGravity :
		public SyntheticGameProp
	{
	public:
		PropGravity()
		{
			name = "Anti-G";
			color = olc::Pixel(221, 160, 221);
			radius = 1.2f;
		}

		void Crash(b2Body* _self_, b2Body* crash_ball) override
		{
			if (IsNormal(crash_ball))
			{
				crash_ball->SetGravityScale(0.0f);
				SetQuickMove(_self_, crash_ball->GetPosition());
				SetSyntheticEffect(crash_ball->GetPosition(), ((BodyInfo*)crash_ball->GetUserData().pointer)->level);
				DestroyBall(_self_);
				PlayAudio(AudioName::SynthesisAudio);
			}
		}
	};

	class PropFuse :
		public SyntheticGameProp
	{
	public:
		PropFuse()
		{
			name = "Fuse";
			color = olc::Pixel(64, 224, 208);
			radius = 2.2f;
		}

		void Crash(b2Body* _self_, b2Body* crash_ball) override
		{
			if (IsNormal(crash_ball))
			{
				BodyInfo* info = (BodyInfo*)(crash_ball->GetUserData().pointer);

				if (info->level == GetMaxLevel())
					return;

				for (auto x : GetAllNormalBalls())
				{
					if (x == crash_ball)
						continue;

					BodyInfo* ball_info = (BodyInfo*)(x->GetUserData().pointer);

					if (info->level == ball_info->level)
					{
						Synthesis(crash_ball, x, info->level);
						SetQuickMove(_self_, crash_ball->GetPosition());
						DestroyBall(_self_);
						return;
					}
				}
			}
		}
	};
}

#endif 
#endif

