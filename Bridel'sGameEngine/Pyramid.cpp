#include "Pyramid.h"
#include "BindableBase.h"
#include "GraphicsThrowMacros.h"
#include "Cone.h"

Pyramid::Pyramid(Graphics& gfx,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist)
	:
	r(rdist(rng)),
	droll(ddist(rng)),
	dpitch(ddist(rng)),
	dyaw(ddist(rng)),
	dphi(odist(rng)),
	dtheta(odist(rng)),
	dchi(odist(rng)),
	chi(adist(rng)),
	theta(adist(rng)),
	phi(adist(rng))
{
	namespace dx = DirectX;
	if (!isStaticInitialized())
	{
		struct Vertex
		{
			dx::XMFLOAT3 pos;
			struct
			{
				unsigned char r;
				unsigned char g;
				unsigned char b;
				unsigned char a;
			} color;
		};
		auto model = Cone::makeTesselated<Vertex>(4);
		// set vertex colors for mesh
		model.vertices[0].color = { 255, 255, 0 };
		model.vertices[1].color = { 255, 255, 0 };
		model.vertices[2].color = { 255, 255, 0 };
		model.vertices[3].color = { 255, 255, 0 };
		model.vertices[4].color = { 255, 255, 80 };
		model.vertices[5].color = { 255, 10, 0 };
		// deform mesh linearly
		model.transform(dx::XMMatrixScaling(1.0f, 1.0f, 1.2f));

		addStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

		auto pvs = std::make_unique<VertexShader>(gfx, L"ColorBlendVS.cso");
		auto pvsbc = pvs->getByteCode();
		addStaticBind(std::move(pvs));

		addStaticBind(std::make_unique<PixelShader>(gfx, L"ColorBlendPS.cso"));

		addStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		addStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		addStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
		setIndexFromStatic();
	addBind(std::make_unique<TransformCbuf>(gfx, *this));
}

void Pyramid::update(float dt) noexcept
{
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

DirectX::XMMATRIX Pyramid::getTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
}
