#include "SolidSphere.h"
#include "BindableCommon.h"
#include "GraphicsThrowMacros.h"
#include "Sphere.h"

SolidSphere::SolidSphere(Graphics& gfx, float radius)
{
	using namespace Bind;
	namespace dx = DirectX;

	if (!isStaticInitialized())
	{
		struct Vertex
		{
			dx::XMFLOAT3 pos;
		};
		auto model = Sphere::make<Vertex>();
		model.transform(dx::XMMatrixScaling(radius, radius, radius));
		addBind(std::make_unique<Bind::VertexBuffer>(gfx, model.vertices));
		addIndexBuffer(std::make_unique<Bind::IndexBuffer>(gfx, model.indices));

		auto pvs = std::make_unique<Bind::VertexShader>(gfx, L"SolidVS.cso");
		auto pvsbc = pvs->getBytecode();
		addStaticBind(std::move(pvs));

		addStaticBind(std::make_unique<Bind::PixelShader>(gfx, L"SolidPS.cso"));

		struct PSColorConstant
		{
			dx::XMFLOAT3 color = { 1.0f, 1.0f, 1.0f };
			float padding;
		} colorConst;
		addStaticBind(std::make_unique<Bind::PixelConstantBuffer<PSColorConstant>>(gfx, colorConst));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		addStaticBind(std::make_unique<Bind::InputLayout>(gfx, ied, pvsbc));

		addStaticBind(std::make_unique<Bind::Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
		setIndexFromStatic();
	addBind(std::make_unique<Bind::TransformCbuf>(gfx, *this));
}

void SolidSphere::update(float dt) noexcept
{}

void SolidSphere::setPos(DirectX::XMFLOAT3 pos) noexcept
{
	this->pos = pos;
}

DirectX::XMMATRIX SolidSphere::getTransformXM() const noexcept
{
	return DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
}
