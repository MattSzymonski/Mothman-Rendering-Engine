#include "PostProcessingEffectsSettings.h"

PostProcessingEffectsSettings::PostProcessingEffectsSettings()
{
	enabled = true;

	invert = Invert();
	grayscale = Grayscale();
	edgeDetection = EdgeDetection();
	colorCorrection = ColorCorrection();
	vignette = Vignette();
	logoOverlay = LogoOverlay();
}

PostProcessingEffectsSettings::~PostProcessingEffectsSettings()
{

}

