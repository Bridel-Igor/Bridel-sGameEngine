#include "VertexBuffer.h"
#include "BindableCodex.h"

namespace Bind
{
	VertexBuffer::VertexBuffer(Graphics& gfx, const Dvtx::VertexBuffer& vbuf)
		:
		VertexBuffer(gfx, "?", vbuf)
	{}

	VertexBuffer::VertexBuffer(Graphics& gfx, const std::string& tag, const Dvtx::VertexBuffer& vbuf)
		:
		stride((UINT)vbuf.getLayout().size()),
		tag(tag)
	{
		INFOMAN(gfx);

		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = UINT(vbuf.sizeBytes());
		bd.StructureByteStride = stride;
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vbuf.getData();
		GFX_THROW_INFO(getDevice(gfx)->CreateBuffer(&bd, &sd, &pVertexBuffer));
	}

	void VertexBuffer::bind(Graphics& gfx) noexcept
	{
		const UINT offset = 0u;
		getContext(gfx)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
	}

	std::shared_ptr<VertexBuffer> VertexBuffer::resolve(Graphics& gfx, const std::string& tag, 
		const Dvtx::VertexBuffer& vbuf)
	{
		assert(tag != "?");
		return Codex::resolve<VertexBuffer>(gfx, tag, vbuf);
	}

	std::string VertexBuffer::getUID() const noexcept
	{
		return generateUID(tag);
	}

	std::string VertexBuffer::generateUID_(const std::string& tag)
	{
		using namespace std::string_literals;
		return typeid(VertexBuffer).name() + "#"s + tag;
	}
}