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
	const auto geometryTag = "$sphere." + std::to_string(radius);
	addBind(std::make_shared<VertexBuffer>(gfx, geometryTag, model.vertices));
	addBind(std::make_shared<IndexBuffer>(gfx, geometryTag, model.indices));

	auto pvs = VertexShader::resolve(gfx, "SolidVS.cso");
	auto pvsbc = pvs->getBytecode();
	addBind(std::move(pvs));

	addBind(PixelShader::resolve(gfx, "SolidPS.cso"));

	struct PSColorConstant
	{
		dx::XMFLOAT3 color = { 1.0f, 1.0f, 1.0f };
		float padding;
	} colorConst;
	addBind(PixelConstantBuffer<PSColorConstant>::resolve(gfx, colorConst, 1u));

	addBind(InputLayout::resolve(gfx, model.vertices.getLayout(), pvsbc));

	addBind(Topology::resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	addBind(std::make_shared<TransformCbuf>(gfx, *this));

	addBind(Blender::resolve(gfx, false));

	addBind(Rasterizer::resolve(gfx, false));
}

void SolidSphere::setPos(DirectX::XMFLOAT3 pos) noexcept
{
	this->pos = pos;
}

DirectX::XMMATRIX SolidSphere::getTransformXM() const noexcept
{
	return DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
}
