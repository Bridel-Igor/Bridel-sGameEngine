#include "SolidSphere.h"
#include "BindableCommon.h"
#include "GraphicsThrowMacros.h"
#include "Vertex.h"
#include "Sphere.h"

SolidSphere::SolidSphere(Graphics& gfx, float radius)
{
	using namespace Bind;
	namespace dx = DirectX;

	auto model = Sphere::make();
	model.transform(dx::XMMatrixScaling(radius, radius, radius));
	addBind(std::make_shared<VertexBuffer>(gfx, model.vertices));
	addBind(std::make_shared<IndexBuffer>(gfx, model.indices));

	auto pvs = std::make_shared<VertexShader>(gfx, L"SolidVS.cso");
	auto pvsbc = pvs->getBytecode();
	addBind(std::move(pvs));

	addBind(std::make_shared<PixelShader>(gfx, L"SolidPS.cso"));

	struct PSColorConstant
	{
		dx::XMFLOAT3 color = { 1.0f, 1.0f, 1.0f };
		float padding;
	} colorConst;
	addBind(std::make_shared<PixelConstantBuffer<PSColorConstant>>(gfx, colorConst));

	addBind(std::make_shared<InputLayout>(gfx, model.vertices.getLayout().getD3DLayout(), pvsbc));

	addBind(std::make_shared<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	addBind(std::make_shared<TransformCbuf>(gfx, *this));
}

void SolidSphere::setPos(DirectX::XMFLOAT3 pos) noexcept
{
	this->pos = pos;
}

DirectX::XMMATRIX SolidSphere::getTransformXM() const noexcept
{
	return DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
}
