#pragma once
#include <memory>
#include "Shader.h"
#include "Transform.h"


// This is a simple container for things I want to access from the renderre
// Such as shaders, models, transforms, whatever.
class SceneResources
{
public:
	std::unique_ptr<Shader> shader;
	std::unique_ptr<Mesh> mesh;
	Transform testTransform;
};