#pragma once
//used to get the correct fragment output from the shaders
enum class ERenderType
{
	ALBEDO_PASS,
	POSITION_PASS,
	NORMAL_PASS,
	LIGHTING_PASS,
	POST_PROCESSING_PASS,
	G_PASS,
};