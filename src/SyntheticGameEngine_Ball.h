/*************************************************************************
    > File Name: SyntheticGameEngine_Ball.h
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Tue Nov 29 10:44:09 2022
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

#ifndef _SYNTHETIC_GAME_ENGINE_BALL_
#define _SYNTHETIC_GAME_ENGINE_BALL_

#ifdef _SYNTHETIC_GAME_ENGINE_

//这个专门用于在扔球点生成球
inline void SYNTHETIC_GAME::SyntheticGameEngine::CreateNewBall()
{
	next_ball = CreateNewBall(0.0f, 0.0f, BallRand(), b2Vec2_zero, true);

	if (last_mouse != b2Vec2_zero)
	{
		MouseMove(last_mouse);
	}
	else
	{
		next_ball->SetTransform(b2Vec2(B2_Width * 0.5f + SGE_RandomError,
			B2_Generate - next_ball->GetFixtureList()->GetShape()->m_radius), 0.0f);
	}

	next_ball->SetEnabled(false);
	next_ball->SetType(b2_staticBody);
}

inline void SYNTHETIC_GAME::SyntheticGameEngine::DestroyBall(b2Body* body)
{
	BodyInfo* info = (BodyInfo*)(body->GetUserData().pointer);
	SGEDeleteStretch(body);

	if (info->prop != nullptr)
		SGEFreeProp(body);

	delete info;
	m_world->DestroyBody(body);
}

b2Body* SYNTHETIC_GAME::SyntheticGameEngine::CreateNewBall(float x, float y, int level, const b2Vec2& velocity, bool contain_prop)
{
	//设为圆形
	b2CircleShape shape;
	shape.m_p.SetZero();
	shape.m_radius = BallRadius[level];

	//设置位置
	b2BodyDef body_def;
	body_def.type = b2_dynamicBody;
	body_def.position.Set(x, y);

	//设置大小等级
	BodyInfo* new_info = new BodyInfo(level);

	body_def.userData.pointer = (uintptr_t)new_info;

	//生成并调整速度
	b2Body* body = m_world->CreateBody(&body_def);
	body->SetLinearVelocity(velocity);

	//设置物理属性（会根据难度进行设置）
	b2FixtureDef fixture_def;
	fixture_def.shape = &shape;
	fixture_def.density = 1.0f;
	fixture_def.friction = 0.2f / difficulty;
	fixture_def.restitution = 0.1f * difficulty;

	body->CreateFixture(&fixture_def);

	//随机生成道具
	if (contain_prop)
		SGESetProp(body);

	return body;
}

#endif
#endif

