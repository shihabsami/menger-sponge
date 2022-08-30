#ifndef MENGER_SPONGE_SCENE_H
#define MENGER_SPONGE_SCENE_H

#include "../Component/Camera.h"
#include "../Component/Mesh.h"
#include "../Component/Light.h"
#include "../Main/Constants.h"

#include <memory>
#include <array>

class Scene {
public:
	virtual void init();

	virtual void toggle_lighting();

	virtual void toggle_backface_culling();

	virtual void toggle_depth_buffering();

	virtual void update() = 0;

	virtual void update_lighting(unsigned int point_light_count);

	virtual void update_subdivision(unsigned int subdivision);

	virtual void display() = 0;

	virtual void done();

	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<Mesh> m_mesh;
	std::unique_ptr<glm::mat4> m_projection_matrix;

	const char* m_name{};
	unsigned int m_subdivision{ 1 };
	unsigned int m_point_light_count{};
	bool m_lighting_enabled{};
	bool m_backface_culling_enabled{};
	bool m_depth_buffering_enabled{};
};

#endif // MENGER_SPONGE_SCENE_H
