#pragma once

#include "ShaderFlat.h"

/**
 * @brief Eye-light shader class
 */
class CShaderEyelight : public CShaderFlat
{
public:
	/**
	 * @brief Constructor
	 * @details This is a texture-free and light-source-free shader
	 * @param scene Reference to the scene
	 * @param color The color of the object
	 */
	CShaderEyelight(Vec3f color = RGB(0.5f, 0.5f, 0.5f))
		: CShaderFlat(color)
	{}
	virtual ~CShaderEyelight(void) = default;

	virtual Vec3f Shade(const Ray& ray) const override
	{
		// --- PUT YOUR CODE HERE ---

		// dot product formula:
		// a.b = |a|.|b|.Cos(theta)

		Vec3f normal = ray.hit.get()->GetNormal(ray);
		float theta = (float) normal.dot(ray.dir) / (getMagnitude(normal) * getMagnitude(ray.dir)) ;
		theta = abs(theta);

		return CShaderFlat::Shade() * theta;
	}

	float getMagnitude(const Vec3f& vector) const {
		return sqrt(pow(vector[0], 2) + pow(vector[1], 2) + pow(vector[2], 2));
	}
};

