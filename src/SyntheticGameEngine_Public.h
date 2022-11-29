/*************************************************************************
    > File Name: SyntheticGameEngine_Public.h
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Tue Nov 29 10:29:42 2022
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

#ifndef _SYNTHETIC_GAME_ENGINE_PUBLIC_
#define _SYNTHETIC_GAME_ENGINE_PUBLIC_

#ifdef _SYNTHETIC_GAME_ENGINE_

inline SYNTHETIC_GAME::SyntheticGameEngine::SyntheticGameEngine()
{
	sAppName = "Synthetic Things - By joker2770";
}

inline SYNTHETIC_GAME::SyntheticGameEngine::~SyntheticGameEngine()
{
	if (m_world != nullptr)
		delete m_world,
		m_world = nullptr;

	ClearPictures();
}

inline bool SYNTHETIC_GAME::SyntheticGameEngine::GameStart(bool vsync)
{
	GetLayers().clear();

	if (Construct(SGE_Width, SGE_Height, 1, 1, false, vsync) != olc::rcode::OK ||
		Start() != olc::rcode::OK)
		return false;

	return true;
}

inline void SYNTHETIC_GAME::SyntheticGameEngine::SetPicturePath(const String& path)
{
	picture_path = path;
}

inline std::string SYNTHETIC_GAME::SyntheticGameEngine::GetPicturePath()
{
	return picture_path;
}

inline void SYNTHETIC_GAME::SyntheticGameEngine::SetScale(float scale)
{
	SGE_Scale_B2 = scale;
	ScaleMatch();
}

inline void SYNTHETIC_GAME::SyntheticGameEngine::SetSGESize(int32_t width, int32_t height)
{
	SGE_Width = width;
	SGE_Height = height;
	FloatSet();
	ScaleMatch();
}

inline void SYNTHETIC_GAME::SyntheticGameEngine::SetLineHeight(int32_t height)
{
	SGE_Line = height;
	SGE_Generate = SGE_Line * 4 / 5;
	FloatSet();
	ScaleMatch();
}

inline void SYNTHETIC_GAME::SyntheticGameEngine::SetColor(ColorArea area, const olc::Pixel& color)
{
	switch (area)
	{
	case ColorArea::BackgroundArea:
		BackGroundColor = color;
		break;
	case ColorArea::FontArea:
		FontColor = color;
		break;
	}
}

inline void SYNTHETIC_GAME::SyntheticGameEngine::SetFileName(FileName file_type, const String& file_name)
{
	switch (file_type)
	{
	case FileName::BackgroundName:
		BackgroundPictureName = file_name;
		break;
	case FileName::LineName:
		LinePictureName = file_name;
		break;
	case FileName::BallName:
		BallPicturesName = file_name;
		break;
	case FileName::BallBehindName:
		BehindName = file_name;
		break;
	case FileName::EffectWaterName:
		WaterName = file_name;
		break;
	case FileName::EffectFlowerAName:
		FlowerAName = file_name;
		break;
	case FileName::EffectFlowerBName:
		FlowerBName = file_name;
		break;
	case FileName::SuccessName:
		SuccessPictureName = file_name;
		break;
	}
}

inline void SYNTHETIC_GAME::SyntheticGameEngine::SetDifficulty(float _difficulty_)
{
	if (_difficulty_ < 1.0f || _difficulty_ > 10.0f)
		return;
	GameDifficulty = _difficulty_;
	difficulty = GameDifficulty / 5.0f;
}

inline void SYNTHETIC_GAME::SyntheticGameEngine::SetSGEName(const String& app_name)
{
	sAppName = app_name;
}

inline void SYNTHETIC_GAME::SyntheticGameEngine::SetPropProbability(float _p_)
{
	if (_p_ < 0.0f || _p_ > 1.0f)
		return;
	PProp = _p_;
}

inline void SYNTHETIC_GAME::SyntheticGameEngine::LoadAudio(AudioName AudioType, const String& AudioPath)
{
	AudioId[int(AudioType)] = olc::SOUND::LoadAudioSample(AudioPath);
}

template<class _Ty>
inline void SYNTHETIC_GAME::SyntheticGameEngine::LoadProp(const String& picture_file_name)
{
	_Ty* new_prop = new _Ty;
	new_prop->engine = this;

	if (picture_file_name.empty())
		new_prop->SetPropImagePath(GetPicturePath() + "/" + new_prop->name + ".png");
	else
		new_prop->SetPropImagePath(GetPicturePath() + "/" + picture_file_name);

	props.push_back(new_prop);
}

#endif
#endif

