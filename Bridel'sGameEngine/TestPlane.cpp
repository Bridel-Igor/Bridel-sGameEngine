#include "TestPlane.h"
#include "Plane.h"
#include "BindableCommon.h"
#include "ImGui/imgui.h"
#include "TransformCbufDual.h"

TestPlane::TestPlane(Graphics& gfx, float size, DirectX::XMFLOAT4 color)
	:
	pmc({ color })
{
	using namespace Bind;
	namespace dx = DirectX;

	auto model = Plane::make();
	model.transform(dx::XMMatrixScaling(size, size, 1.0f));
	const auto geometryTag = "$plane." + std::to_string(size);
	addBind(VertexBuffer::resolve(gfx, geometryTag, model.vertices));
	addBind(IndexBuffer::resolve(gfx, geometryTag, model.indices));

	auto pvs = VertexShader::resolve(gfx, "SolidVS.cso");
	auto pvsbc = pvs->getBytecode();
	addBind(std::move(pvs));

	addBind(PixelShader::resolve(gfx, "SolidPS.cso"));

	addBind(std::make_shared<PixelConstantBuffer<PSMaterialConstant>>(gfx, pmc, 1u));

	addBind(InputLayout::resolve(gfx, model.vertices.getLayout(), pvsbc));

	addBind(Topology::resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	addBind(std::make_shared<TransformCbuf>(gfx, *this, 0u));

	addBind(Blender::resolve(gfx, true, 0.5f));

	addBind(Rasterizer::resolve(gfx, true));
}

void TestPlane::setPos(DirectX::XMFLOAT3 pos) noexcept
{
	this->pos = pos;
}

void TestPlane::setRotation(float roll, float pitch, float yaw) noexcept
{
	this->roll = roll;
	this->pitch = pitch;
	this->yaw = yaw;
}

DirectX::XMMATRIX TestPlane::getTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw(roll, pitch, yaw) *
		DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
}

void TestPlane::spawnControlWindow(Graphics& gfx, const std::string& name) noexcept
{
	if (ImGui::Begin(name.c_str()))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &pos.x, -80.0f, 80.0f, "%.1f");
		ImGui::SliderFloat("Y", &pos.y, -80.0f, 80.0f, "%.1f");
		ImGui::SliderFloat("Z", &pos.z, -80.0f, 80.0f, "%.1f");
		ImGui::Text("Orientation");
		ImGui::SliderAngle("Roll", &roll, -180.0f, 180.0f);
		ImGui::SliderAngle("Pitch", &pitch, -180.0f, 180.0f);
		ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f);
		ImGui::Text("Shading");
		auto pBlender = queryBindable<Bind::Blender>();
		float factor = pBlender->getFactor();
		ImGui::SliderFloat("Translucency", &factor, 0.0f, 1.0f);
		pBlender->setFactor(factor);
	}
	ImGui::End();
}