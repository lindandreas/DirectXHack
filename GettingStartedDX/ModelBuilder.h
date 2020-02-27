#pragma once


// Code stolen from Emil Palm


#pragma once

#include <vector>
#include <memory>
#include "Mesh.h"

class ModelBuilder
{

	ModelBuilder();
	static std::unique_ptr<Mesh> createSquare();
	static std::unique_ptr<Mesh> createSphere(int resX, int resY);
};