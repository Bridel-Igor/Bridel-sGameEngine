#include "TestCube.h"
#include "Cube.h"
#include "BindableCommon.h"
#include "TransformCbufDual.h"
#include "ImGui/imgui.h"

TestCube::TestCube(Graphics& gfx, float size)
{
	using namespace Bind;
	namespace dx = DirectX;

	auto model = Cube::makeIndependentTextured();
	model.transform(dx::XMMatrixScaling(size, size, size));
	model.setNormalsIndependentFlat();
	const auto geometryTag = "$cube." + std::to_string(size);
	addBind(VertexBuffer::resolve(gfx, geometryTag, model.vertices));
	addBind(IndexBuffer::resolve(gfx, geometryTag, model.indices));

	addBind(Texture::resolve(gfx, "Images\\brickwall.jpg"));
	addBind(Texture::resolve(gfx, "Images\\brickwall_normal.jpg", 1u));

	auto pvs = VertexShader::resolve(gfx, "PhongVS.cso");
	auto pvsbc = pvs->getBytecode();
	addBind(std::move(pvs));

	addBind(PixelShader::resolve(gfx, "PhongPSNormalMap.cso"));

	addBind(PixelConstantBuffer<PSMaterialConstant>::resolve(gfx, pmc, 1u));

	addBind(InputLayout::resolve(gfx, model.vertices.getLayout(), pvsbc));

	addBind(Topology::resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	addBind(std::make_shared<TransformCbufDual>(gfx, *this, 0u, 2u));
}

void TestCube::setPos(DirectX::XMFLOAT3 pos) noexcept
{
	this->pos = pos;
}

void TestCube::setRotation(float roll, float pitch, float yaw) noexcept
{
	this->roll = roll;
	this->pitch = pitch;
	this->yaw = yaw;
}

DirectX::XMMATRIX TestCube::getTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw(roll, pitch, yaw) *
		DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
}

void TestCube::spawnControlWindow(Graphics& gfx) noexcept
{
	if (ImGui::Begin("Cube"))
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
		bool changed0 = ImGui::SliderFloat("Spec. Int.", &pmc.specularIntensity, 0.0f, 1.0f);
		bool changed1 = ImGui::SliderFloat("Spec. Power", &pmc.specularPower, 0.0f, 100.0f);
		bool checkState = pmc.normalMappingEnabled == TRUE;
		bool changed2 = ImGui::Checkbox("Enable Normal Map", &checkState);
		pmc.normalMappingEnabled = checkState ? TRUE : FALSE;
		if (changed0 || changed1 || changed2)
		{
			queryBindable<Bind::PixelConstantBuffer<PSMaterialConstant>>()->update(gfx, pmc);
		}
	}
	ImGui::End();
}
