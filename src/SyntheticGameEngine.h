/*************************************************************************
    > File Name: SyntheticGameEngine.h
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Tue Nov 29 10:46:07 2022
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

#ifndef _SYNTHETIC_GAME_ENGINE_
#define _SYNTHETIC_GAME_ENGINE_

#include "SyntheticGameProp_Pre.h"

#define RAND_LIMIT 32767
#define SGE_RandomError FloatRandom(-0.01f, 0.01f)

namespace SYNTHETIC_GAME
{
	inline float FloatRandom(float lo, float hi)
	{
		float r = (float)(rand() & (RAND_LIMIT));
		r /= RAND_LIMIT;
		r = (hi - lo) * r + lo;
		return r;
	}

	inline int PropRand(int32_t prop_num,float _p_)
	{
		if (FloatRandom(0.0f, 1.0f) > _p_)
			return -1;
		return abs(rand() % prop_num);
	}

	enum class ColorArea { BackgroundArea, FontArea };

	enum class FileName { BackgroundName, LineName, BallName, BallBehindName, 
		EffectWaterName, EffectFlowerAName, EffectFlowerBName, SuccessName};

	enum class AudioName { BackgroundAudio, SynthesisAudio, DeadAudio, SuccessAudio };

	class SyntheticGameEngine :
		protected olc::PixelGameEngine, 
		protected b2ContactListener, 
		protected SyntheticGameSettings
	{
		template<class _Ty>
		using Vector = std::vector<_Ty>;
		using String = std::string;

		friend class SyntheticGameProp;

		inline int BallRand()
		{
			srand(time(nullptr));
			return abs(rand() % GenerateNum);
		}

	public:
		SyntheticGameEngine();

		~SyntheticGameEngine();

		bool GameStart(bool vsync = false);

		void SetPicturePath(const String& path);

		String GetPicturePath();

		void SetScale(float scale);

		void SetSGESize(int32_t width, int32_t height);

		void SetLineHeight(int32_t height);

		void SetColor(ColorArea area, const olc::Pixel& color);

		void SetFileName(FileName file_type, const String& file_name);

		void SetDifficulty(float _difficulty_);

		void SetSGEName(const String& app_name);

		void SetPropProbability(float _p_);

		void LoadAudio(AudioName AudioType, const String& AudioPath);

		template<class _Ty>
		void LoadProp(const String& picture_file_name = String());

		struct BodyInfo
		{
			int32_t level = 0;
			SyntheticGameProp_Pre* prop = nullptr;

			BodyInfo() = default;
			BodyInfo(int32_t _level_):level(_level_) {}
		};

	protected:
		String picture_path;

		Vector<olc::Decal*> pictures;
		olc::Decal* background = nullptr;
		olc::Decal* line = nullptr;
		olc::Decal* success = nullptr;

		Vector<olc::vf2d> pictures_center;
		Vector<olc::vf2d> pictures_2inv;

		olc::vf2d backgroud_scale;
		olc::vf2d line_scale;
		olc::vf2d success_scale;

		Vector<olc::Decal*> effect_water;
		Vector<olc::Decal*> effect_flower1;
		Vector<olc::Decal*> effect_flower2;

		Vector<olc::vf2d> effect_water_center;
		Vector<olc::vf2d> effect_flower1_center;
		Vector<olc::vf2d> effect_flower2_center;

		int AudioId[4] = { -1,-1,-1,-1 };

		b2World* m_world = nullptr;

		b2Vec2 last_mouse = b2Vec2_zero;

		Vector<Vector<b2Fixture *>> touch_balls;
		b2Body* next_ball = nullptr;
		int32_t score = 0;
		bool dead = false;

		Vector<SyntheticGameProp_Pre *> props;
		Vector<b2Body*> prop_body;

		Vector<b2Body*> destroy_list;

		char string_buf[128];

		float rebuild_time = 0.0f;
		float dead_clock = DeadTime;
		float dead_bomb_clock = DeadBombTime;

		int32_t combo = 0;

		struct Flash
		{
			b2Body* body = nullptr;
			float alpha = UINT8_MAX;
			float clock = 1.0f;
			bool minus = true;

			Flash() = default;
			Flash(b2Body* _body_) :body(_body_) {}
		};

		struct QuickMove
		{
			float sge_radius = 0.0f;

			olc::Pixel color = olc::WHITE;

			olc::Decal* picture = nullptr;
			olc::vf2d picture_center;
			olc::vf2d picture_scale;

			float angle = 0.0f;
			b2Vec2 now_pos;
			b2Vec2 end_pos;

			float speed = 25.0f;

			QuickMove() = default;
			QuickMove(float _sge_radius_, olc::Decal * _picture_,olc::Pixel _color_,
				float _angle_,const b2Vec2& _now_pos_, const b2Vec2& _end_pos_) :
				sge_radius(_sge_radius_), picture(_picture_), color(_color_),
				angle(_angle_),now_pos(_now_pos_), end_pos(_end_pos_) 
			{
				sge_radius *= 0.8f;
				if (picture != nullptr)
				{
					picture_center = { picture->sprite->width * 0.5f,
						picture->sprite->height * 0.5f };
					picture_scale = { sge_radius / picture_center.x ,
						sge_radius / picture_center.y };
				}
			}
		};

		struct Stretch
		{
			b2Body* body = nullptr;
			float step = 0.0f;
			std::list<float> target_radius;
		};

		struct SEffect
		{
			olc::vf2d pos;

			// 2.2
			olc::Decal* water = nullptr;
			olc::vf2d water_center;

			float clock = 0.5f;
			float alpha = 255.0f;
			float step = -510.0f;
			float wscale;

			// 0.5 -> 1.5
			olc::Decal* flower1 = nullptr;
			olc::Decal* flower2 = nullptr;
			olc::vf2d flower1_center;
			olc::vf2d flower2_center;

			struct Center
			{
				olc::vf2d center;
				float center_angle;

				olc::vf2d speed;
				bool is_one;
				
				float falpha;

				Center() = default;
				Center(const olc::vf2d& _center_, float _center_angle_, const olc::vf2d& _speed_, bool _is_one_, float _falpha_) :
					center(_center_), center_angle(_center_angle_), speed(_speed_), is_one(_is_one_), falpha(_falpha_){}
			};

			Vector<Center> centers;
			float wspeed = 25.0f;

			int32_t number = 10;
			
			float half_clock = 0.25f;
			float fscale = 1.0f;
			float fscale_step;

			float falpha_step = -100.0f;

			SEffect() = default;
			SEffect(olc::vf2d _pos_,
				olc::Decal* _water_, olc::vf2d _water_center_,
				olc::Decal* _flower1_, olc::vf2d _flower1_center,
				olc::Decal* _flower2_, olc::vf2d _flower2_center,
				float _clock_, int32_t _number_, float _ball_radius_)
				:pos(_pos_),
				water(_water_), water_center(_water_center_),
				flower1(_flower1_), flower1_center(_flower1_center),
				flower2(_flower2_), flower2_center(_flower2_center),
				clock(_clock_), number(_number_)
			{
				half_clock = clock * 0.5f;

				const float start_rate = 0.5f,
					end_rate = 1.5f;

				const float water_rate = 2.2f;

				if (water != nullptr)
				{
					step = - alpha / clock;
					float real_2radius = water_rate * _ball_radius_;
					real_2radius += real_2radius;
					wscale = real_2radius * real_2radius
						/ (water->sprite->width * water->sprite->height);
				}

				int32_t flower_size = int32_t(flower1 != nullptr) + int32_t(flower2 != nullptr);
				if (flower_size > 0)
				{
					srand(time(nullptr));
					auto OneOrTwo = [=]()->bool
					{
						if (flower_size == 1) 
							return (flower1 != nullptr);

						return (rand() % 2 == 0);
					};

					float radius = start_rate * _ball_radius_;
					float max_radius = end_rate * _ball_radius_;
					
					fscale_step = - fscale / half_clock;
					falpha_step = - 50.0f / clock;

					float angle = FloatRandom(0.0f, 6.283185f);
					float angle_distance = 6.283185f / number;

					float _cos_ = cosf(angle);
					float _sin_ = sinf(angle);

					float _cos_add_ = cosf(angle_distance);
					float _sin_add_ = sinf(angle_distance);

					float half_clock_inv = 1.0f / half_clock;

					for (int32_t i = 0; i < number; i++)
					{
						float radius_error = radius * FloatRandom(0.5f, 1.5f);
						float speed = (max_radius - radius) * half_clock_inv;

						centers.emplace_back(olc::vf2d(pos.x + radius_error * _cos_,
							pos.y + radius_error * _sin_), FloatRandom(0.0f, 6.283185f),
							olc::vf2d(speed * _cos_, speed * _sin_), OneOrTwo(), FloatRandom(200.0f,255.0f));

						if (i != number - 1)
						{
							float _new_cos_ = _cos_ * _cos_add_ - _sin_ * _sin_add_;
							float _new_sin_ = _sin_ * _cos_add_ + _cos_ * _sin_add_;

							_cos_ = _new_cos_;
							_sin_ = _new_sin_;
						}
					}
				}
			}
		};

		Vector<Flash> flash_list;

		Vector<QuickMove> quick_move_list;

		Vector<Stretch> stretch_list;

		Vector<SEffect> seffect_list;

		struct 
		{
			float clock = 0.0f;
			int32_t level = 0;
			float angle = 0.0f;
			float step = 1.0f;

			float scale = 0.0f;
			float scale_step = 5.0f;
			float scale_clock = 0.0f;
		} success_effect;

		void LoadPictures(const String& path);
		
		void ClearPictures();

		void SGESetProp(b2Body* body);

		void SGEFreeProp(b2Body* body);

		void SGEClearProp();

		void SGEFlash(float fElapsedTime);

		void SGEQuickMove(float fElapsedTime);

		void SGESetStretch(b2Body* body);
		
		void SGEDeleteStretch(b2Body* body);

		void SGEStretch(float fElapsedTime);

		void SGESetSEffect(float sge_x_, float sge_y_, int32_t level);

		void SGEDrawSEffect();

		void SGEEffect(float fElapsedTime);

		void SGESetSuccess(int32_t level);

		void SGESuccess(float fElapsedTime);

		void SGEDrawWindows();
		
		void CreateNewBall();
		b2Body* CreateNewBall(float x, float y, int level, 
			const b2Vec2& velocity = b2Vec2_zero,bool contain_prop = false);

		void DestroyBall(b2Body* body);
		
		bool DeadJudge();
		
		void DeadFlash();

		void DeadBomb();

		int FindFixture(b2Fixture* _find_);

		void PreSyntheticOperation(b2Body * body1,b2Body * body2, int32_t level);
		void SyntheticOperation();

		void PropEvent(float fElapsedTime);

		bool NotBeDestroy(b2Body* body);
	private:
		void MouseMove(const b2Vec2& p);
		
		void MouseDown(const b2Vec2& p);
		
		bool OnUserCreate() override;
		
		bool OnUserUpdate(float fElapsedTime) override;
		
		bool OnUserDestroy() override;
		
		void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
	};

	
}

#include "SyntheticGameEngine_Public.h"
#include "SyntheticGameEngine_Picture.h"
#include "SyntheticGameEngine_Effect.h"
#include "SyntheticGameEngine_Draw.h"
#include "SyntheticGameEngine_Ball.h"
#include "SyntheticGameEngine_Process.h"
#include "SyntheticGameEngine_Core.h"

#endif

