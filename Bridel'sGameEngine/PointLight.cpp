#include "PointLight.h"
#include "ImGui/imgui.h"

PointLight::PointLight(Graphics& gfx, float radius)
	:
	mesh(gfx, radius),
	cbuf(gfx)
{}

void PointLight::spawnControlWindow() noexcept
{
	if (ImGui::Begin("Light"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &pos.x, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Y", &pos.y, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Z", &pos.z, -60.0f, 60.0f, "%.1f");
		if (ImGui::Button("Reset"))
			reset();
	}
	ImGui::End();
}

void PointLight::reset() noexcept
{
	pos = { 0.0f, 0.0f, 0.0f };
}

void PointLight::draw(Graphics& gfx) const noexcept(!IS_DEBUG)
{
	mesh.setPos(pos);
	mesh.draw(gfx);
}

void PointLight::bind(Graphics& gfx) const noexcept
{
	cbuf.update(gfx, PointLightCBuf{ pos });
	cbuf.bind(gfx);
}
