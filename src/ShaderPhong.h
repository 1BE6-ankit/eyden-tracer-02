#pragma once

#include "ShaderFlat.h"

class CScene;

class CShaderPhong : public CShaderFlat
{
public:
	/**
	 * @brief Constructor
	 * @param scene Reference to the scene
	 * @param color The color of the object
	 * @param ka The ambient coefficient
	 * @param kd The diffuse reflection coefficients
	 * @param ks The specular refelection coefficients
	 * @param ke The shininess exponent
	 */
	CShaderPhong(CScene& scene, Vec3f color, float ka, float kd, float ks, float ke)
		: CShaderFlat(color)
		, m_scene(scene)
		, m_ka(ka)
		, m_kd(kd)
		, m_ks(ks)
		, m_ke(ke)
	{}
	virtual ~CShaderPhong(void) = default;

	virtual Vec3f Shade(const Ray& ray) const override
	{
		// --- PUT YOUR CODE HERE ---

		// specular color for white highlights
		Vec3f m_cs = RGB(1, 1, 1);

		// Flat color of the object
		Vec3f m_cd = CShaderFlat::Shade(ray);

		// ambient color (set it to the color of the object)
		Vec3f m_ca = m_cd;

		// Shading normal vector
		Vec3f m_Normal = ray.hit.get()->GetNormal(ray);

		Vec3f L_a(0, 0, 0);

		Vec3f diffuseSum(0, 0, 0);
		Vec3f specularSum(0, 0, 0);
		Vec3f R_v, L_i;
		float dot_product;

		// direction from light source to surface point
		Ray I_i;
		I_i.org = ray.org + ray.t * ray.dir;

		for(auto l_source : m_scene.m_vpLights) {
			L_i = (l_source.get()->Illuminate(I_i)).value();

			// addup all the light sources to add ambience
			L_a += L_i;

			R_v = -I_i.dir + 2 * I_i.dir.dot(m_Normal) * m_Normal;

			dot_product = I_i.dir.dot(m_Normal);
			if(dot_product > 0)
				diffuseSum += L_i * dot_product;

			specularSum += L_i * pow(-ray.dir.dot(R_v), m_ke);
		}


		Vec3f L_r(0, 0, 0);

		for(int i=0; i<3; i++) {
			L_r[i] = m_ka * m_ca[i] * L_a[i] +
				m_kd * m_cd[i] * diffuseSum[i] + 
				m_ks * m_cs[i] * specularSum[i];
		}

		return L_r;
	}

	
private:
	CScene& m_scene;
	float 	m_ka;    ///< ambient coefficient
	float 	m_kd;    ///< diffuse reflection coefficients
	float 	m_ks;    ///< specular refelection coefficients
	float 	m_ke;    ///< shininess exponent
};
