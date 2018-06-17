#pragma once
//used to get the correct fragment output from the shaders
enum class ERenderType
{
	G_PASS,
	T_PASS,
	LIGHTING_PASS,
	COMPOSITE_PASS,
	POST_PROCESSING_PASS,
};