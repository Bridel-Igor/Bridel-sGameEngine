#include "SolidSphere.h"
#include "BindableBase.h"
#include "GraphicsThrowMacros.h"
#include "Sphere.h"

SolidSphere::SolidSphere(Graphics& gfx, float radius)
{
	namespace dx = DirectX;

	if (!isStaticInitialized())
	{
		struct Vertex
		{
			dx::XMFLOAT3 pos;
		};
		auto model = Sphere::make<Vertex>();
		model.transform(dx::XMMatrixScaling(radius, radius, radius));
		addBind(std::make_unique<VertexBuffer>(gfx, model.vertices));
		addIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

		auto pvs = std::make_unique<VertexShader>(gfx, L"SolidVS.cso");
		auto pvsbc = pvs->getByteCode();
		addStaticBind(std::move(pvs));

		addStaticBind(std::make_unique<PixelShader>(gfx, L"SolidPS.cso"));

		struct PSColorConstant
		{
			dx::XMFLOAT3 color = { 1.0f, 1.0f, 1.0f };
			float padding;
		} colorConst;
		addStaticBind(std::make_unique<PixelConstantBuffer<PSColorConstant>>(gfx, colorConst));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		addStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		addStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
		setIndexFromStatic();
	addBind(std::make_unique<TransformCbuf>(gfx, *this));
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
