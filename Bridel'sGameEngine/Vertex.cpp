#include "Vertex.h"

namespace Dvtx
{
	// VertexLayout
	const VertexLayout::Element& VertexLayout::resolveByIndex(size_t i) const noxnd
	{
		return elements[i];
	}

	VertexLayout& VertexLayout::append(ElementType type) noxnd
	{
		elements.emplace_back(type, size());
		return *this;
	}

	size_t VertexLayout::size() const noxnd
	{
		return elements.empty() ? 0u : elements.back().getOffsetAfter();
	}

	size_t VertexLayout::getElementCount() const noexcept
	{
		return elements.size();
	}

	std::vector<D3D11_INPUT_ELEMENT_DESC> VertexLayout::getD3DLayout() const noxnd
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
		desc.reserve(getElementCount());
		for (const auto& e : elements)
		{
			desc.push_back(e.getDesc());
		}
		return desc;
	}

	std::string VertexLayout::getCode() const noxnd
	{
		std::string code;
		for (const auto& e : elements)
			code += e.getCode();
		return code;
	}


	// VertexLayout::Element
	VertexLayout::Element::Element(ElementType type, size_t offset)
		:
		type(type),
		offset(offset)
	{}
	size_t VertexLayout::Element::getOffsetAfter() const noxnd
	{
		return offset + size();
	}
	size_t VertexLayout::Element::getOffset() const
	{
		return offset;
	}
	size_t VertexLayout::Element::size() const noxnd
	{
		return sizeOf(type);
	}
	constexpr size_t VertexLayout::Element::sizeOf(ElementType type) noxnd
	{
		switch (type)
		{
		case Position2D:
			return sizeof(Map<Position2D>::SysType);
		case Position3D:
			return sizeof(Map<Position3D>::SysType);
		case Texture2D:
			return sizeof(Map<Texture2D>::SysType);
		case Normal:
			return sizeof(Map<Normal>::SysType);
		case Float3Color:
			return sizeof(Map<Float3Color>::SysType);
		case Float4Color:
			return sizeof(Map<Float4Color>::SysType);
		case BRGAColor:
			return sizeof(Map<BRGAColor>::SysType);
		}
		assert("Invalid element type" && false);
		return 0u;
	}
	VertexLayout::ElementType VertexLayout::Element::getType() const noexcept
	{
		return type;
	}
	const char* Dvtx::VertexLayout::Element::getCode() const noexcept
	{
		switch (type)
		{
		case Position2D:
			return Map<Position2D>::code;
		case Position3D:
			return Map<Position3D>::code;
		case Texture2D:
			return Map<Texture2D>::code;
		case Normal:
			return Map<Normal>::code;
		case Float3Color:
			return Map<Float3Color>::code;
		case Float4Color:
			return Map<Float4Color>::code;
		case BRGAColor:
			return Map<BRGAColor>::code;
		}
		assert("Invalid element type" && false);
		return "Invalid";
	}
	D3D11_INPUT_ELEMENT_DESC VertexLayout::Element::getDesc() const noxnd
	{
		switch (type)
		{
		case Position2D:
			return generateDesc<Position2D>(getOffset());
		case Position3D:
			return generateDesc<Position3D>(getOffset());
		case Texture2D:
			return generateDesc<Texture2D>(getOffset());
		case Normal:
			return generateDesc<Normal>(getOffset());
		case Float3Color:
			return generateDesc<Float3Color>(getOffset());
		case Float4Color:
			return generateDesc<Float4Color>(getOffset());
		case BRGAColor:
			return generateDesc<BRGAColor>(getOffset());
		}
		assert("Invalid element type" && false);
		return { "INVALID",0,DXGI_FORMAT_UNKNOWN,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 };
	}


	// Vertex
	Vertex::Vertex(char* pData, const VertexLayout& layout) noxnd
		:
		pData(pData),
		layout(layout)
	{
		assert(pData != nullptr);
	}


	// ConstVertex
	ConstVertex::ConstVertex(const Vertex& v) noxnd
		:
	vertex(v)
	{}


	// VertexBuffer
	VertexBuffer::VertexBuffer(VertexLayout layout) noxnd
		:
		layout(std::move(layout))
	{}

	const char* VertexBuffer::getData() const noxnd
	{
		return buffer.data();
	}

	const VertexLayout& VertexBuffer::getLayout() const noexcept
	{
		return layout;
	}

	size_t VertexBuffer::size() const noxnd
	{
		return buffer.size() / layout.size();
	}

	size_t VertexBuffer::sizeBytes() const noxnd
	{
		return buffer.size();
	}

	Vertex VertexBuffer::back() noxnd
	{
		assert(buffer.size() != 0u);
		return Vertex{ buffer.data() + buffer.size() - layout.size(), layout };
	}

	Vertex VertexBuffer::front() noxnd
	{
		assert(buffer.size() != 0);
		return Vertex{ buffer.data() + buffer.size() - layout.size(), layout };
	}

	Vertex VertexBuffer::operator[](size_t i) noxnd
	{
		assert(i < size());
		return Vertex{ buffer.data() + layout.size() * i, layout };
	}

	ConstVertex VertexBuffer::back() const noxnd
	{
		return const_cast<VertexBuffer*>(this)->back();
	}

	ConstVertex VertexBuffer::front() const noxnd
	{
		return const_cast<VertexBuffer*>(this)->front();
	}

	ConstVertex VertexBuffer::operator[](size_t i) const noxnd
	{
		return const_cast<VertexBuffer&>(*this)[i];
	}
}


