#include <Actor.h>
#include <ResourceManager.h>
#include <Sprite.h>
#include <World.h>
#include <components/SpriteRenderer.h>
#include <components/Transform.h>
#include <rendering/Camera.h>
#include <rendering/RenderContext.h>
#include <rendering/Shader.h>

SpriteRenderer::SpriteRenderer(const std::shared_ptr<Actor>& owner, const std::shared_ptr<Sprite>& sprite) : Component{ owner }, m_sprite{ sprite }
{
	const auto& cam = owner->get_world().get_camera();
	const auto& sprite_shader = ResourceManager::get_from_cache<Shader>({ .m_resource_type = ResourceType::shader, .m_name = "SpriteShader" });
	render_context() = {.view = cam->get_view_matrix(), .projection = cam->get_projection_matrix(), .shader = sprite_shader.value()};
}

void SpriteRenderer::update(float delta_time)
{
}

void SpriteRenderer::render(const RenderContext& ctx)
{
	if (!m_sprite) { return; }

	if (const auto& ow = owner().lock(); ow)
	{
		const auto& transform = ow->transform();
		m_sprite->render(ctx, *transform);
	}
}
