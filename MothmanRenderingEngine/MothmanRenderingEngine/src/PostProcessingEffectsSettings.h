/*
Adding new postprocess:
- Put shader code in Shaders folder
- Add struct for new postprocess here
- Load new shader and register its uniforms/samplers in main program file
- Add new postprocess pass in main program file
- Enable postprocess in main()
- Add postprocess pass to main() loop
- Add new effect to ImGui window
*/

#pragma once

struct PostProcessingEffectSettings
{
public:
	bool IsEnabled() { return enabled; }
	void Enable() { enabled = true; }
	void Disable() { enabled = false; }

private:
	bool enabled;
};

struct Invert : PostProcessingEffectSettings 
{
public:

private:

};

struct Grayscale : PostProcessingEffectSettings
{
public:

private:

};

struct EdgeDetection : PostProcessingEffectSettings
{
public:
	float GetOffsetValue() { return offset; }
	void SetOffsetValue(float value) { offset = value; }

private:
	float offset = 1;
};

struct ColorCorrection : PostProcessingEffectSettings
{
public:

private:

};

struct Vignette : PostProcessingEffectSettings
{
public:
	float GetRadius() { return radius; }
	void SetRadius(float value) { radius  = value; }
	float GetSoftness() { return softness; }
	void SetSoftness(float value) { softness = value; }
	float GetIntensity() { return intensity; }
	void SetIntensity(float value) { intensity = value; }
private:
	float radius = 0.70f;
	float softness = 0.15f;
	float intensity = 0.5f;
};

struct LogoOverlay : PostProcessingEffectSettings
{
public:

private:

};


class PostProcessingEffectsSettings
{
public:
	PostProcessingEffectsSettings();
	~PostProcessingEffectsSettings();

	bool IsEnabled() { return enabled; }
	void Enable() { enabled = true; }
	void Disable() { enabled = false; }

	Invert invert;
	Grayscale grayscale;
	EdgeDetection edgeDetection;
	ColorCorrection colorCorrection;
	Vignette vignette;
	LogoOverlay logoOverlay;

private:
	bool enabled;
};