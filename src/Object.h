#pragma once

#include "Mesh.h"
#include "Material.h"

class Object
{
	public:
		Mesh mesh;
		Material material;

		Object() = default;
		~Object() = default;

		Object(const Object& obj) {
			mesh = obj.mesh;
			material = obj.material;
		}

		Object(const Mesh& _mesh, const Material& _material) {
			mesh = _mesh;
			material = _material;
		}

		Object& operator= (const Object& obj) {
			if (&obj == this)
				return *this;

			mesh = obj.mesh;
			material = obj.material;

			return *this;
		}
};

