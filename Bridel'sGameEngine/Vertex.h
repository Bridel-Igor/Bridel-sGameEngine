#pragma once
#include <vector>
#include <DirectXMath.h>
#include <type_traits>
#include "Graphics.h"

namespace hw3dexp
{
	struct BRGAColor
	{
		unsigned char a;
		unsigned char r;
		unsigned char g;
		unsigned char b;
	};

	class VertexLayout
	{
	public:
		enum ElementType
		{
			Position2D,
			Position3D,
			Texture2D,
			Normal,
			Float3Color,
			Float4Color,
			BRGAColor,
			Count,
		};
		template<ElementType> struct Map;
		template<> struct Map<Position2D>
		{
			using SysType = DirectX::XMFLOAT2;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* semantic = "Position";
		};
		template<> struct Map<Position3D>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Position";
		};
		template<> struct Map<Texture2D>
		{
			using SysType = DirectX::XMFLOAT2;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* semantic = "Texcoord";
		};
		template<> struct Map<Normal>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Normal";
		};
		template<> struct Map<Float3Color>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Color";
		};
		template<> struct Map<Float4Color>
		{
			using SysType = DirectX::XMFLOAT4;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
			static constexpr const char* semantic = "Color";
		};
		template<> struct Map<BRGAColor>
		{
			using SysType = hw3dexp::BRGAColor;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
			static constexpr const char* semantic = "Color";
		};
		class Element
		{
		public:
			Element(ElementType type, size_t offset)
				:
				type(type),
				offset(offset)
			{}
			size_t getOffsetAfter() const noexcept(!IS_DEBUG)
			{
				return offset + size();
			}
			size_t getOffset() const
			{
				return offset;
			}
			size_t size() const noexcept(!IS_DEBUG)
			{
				return sizeOf(type);
			}
			static constexpr size_t sizeOf(ElementType type) noexcept(!IS_DEBUG)
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
			ElementType getType() const noexcept
			{
				return type;
			}
			D3D11_INPUT_ELEMENT_DESC getDesc() const noexcept(!IS_DEBUG)
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
			}
		private:
			template<ElementType type>
			static constexpr D3D11_INPUT_ELEMENT_DESC generateDesc(size_t offset) noexcept(!IS_DEBUG)
			{
				return { Map<type>::semantic, 0, Map<type>::dxgiFormat, 0, (UINT)offset, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			}
		private:
			ElementType type;
			size_t offset;
		};
	public:
		template<ElementType Type>
		const Element& resolve() const noexcept(!IS_DEBUG)
		{
			for (auto& e : elements)
				if (e.getType() == Type)
					return e;
			assert("Could not resolve element type" && false);
			return elements.front();
		}
		const Element& resolveByIndex(size_t i) const noexcept(!IS_DEBUG)
		{
			return elements[i];
		}
		VertexLayout& append(ElementType type) noexcept(!IS_DEBUG)
		{
			elements.emplace_back(type, size());
			return *this;
		}
		size_t size() const noexcept(!IS_DEBUG)
		{
			return elements.empty() ? 0u : elements.back().getOffsetAfter();
		}
		size_t getElementCount() const noexcept
		{
			return elements.size();
		}
		std::vector<D3D11_INPUT_ELEMENT_DESC> getD3DLayout() const noexcept(!IS_DEBUG)
		{
			std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
			desc.reserve(getElementCount());
			for (const auto& e : elements)
			{
				desc.push_back(e.getDesc());
			}
			return desc;
		}
	private:
		std::vector<Element> elements;
	};

	class Vertex
	{
		friend class VertexBuffer;
	public:
		template<VertexLayout::ElementType Type>
		auto& attr() noexcept(!IS_DEBUG)
		{
			auto pAttribute = pData + layout.resolve<Type>().getOffset();
			return *reinterpret_cast<typename VertexLayout::Map<Type>::SysType*>(pAttribute);
		}
		template<typename T>
		void setAttributeByIndex(size_t i, T&& val) noexcept(!IS_DEBUG)
		{
			const auto& element = layout.resolveByIndex(i);
			auto pAttribute = pData + element.getOffset();
			switch (element.getType())
			{
			case VertexLayout::Position2D:
				setAttribute<VertexLayout::Position2D>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Position3D:
				setAttribute<VertexLayout::Position3D>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Texture2D:
				setAttribute<VertexLayout::Texture2D>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Normal:
				setAttribute<VertexLayout::Normal>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Float3Color:
				setAttribute<VertexLayout::Float3Color>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Float4Color:
				setAttribute<VertexLayout::Float4Color>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::BRGAColor:
				setAttribute<VertexLayout::BRGAColor>(pAttribute, std::forward<T>(val));
				break;
			default:
				assert("Bad element type" && false);
			}
		}
	protected:
		Vertex(char* pData, const VertexLayout& layout) noexcept(!IS_DEBUG)
			:
			pData(pData),
			layout(layout)
		{
			assert(pData != nullptr);
		}
		template<typename First, typename ...Rest>
		// enables parameter pack setting of multiple parameters by element index
		void setAttributeByIndex(size_t i, First&& first, Rest&&... rest) noexcept(!IS_DEBUG)
		{
			setAttributeByIndex(i, std::forward<First>(first));
			setAttributeByIndex(i + 1, std::forward<Rest>(rest)...);
		}
		// helper to reduce code duplication in setAttributeByIndex
		template<VertexLayout::ElementType DestLayoutType, typename SrcType>
		void setAttribute(char* pAttribute, SrcType&& val) noexcept(!IS_DEBUG)
		{
			using Dest = typename VertexLayout::Map<DestLayoutType>::SysType;
			if constexpr (std::is_assignable<Dest, SrcType>::value)
				*reinterpret_cast<Dest*>(pAttribute) = val;
			else
				assert("Parameter attribute type mismatch" && false);
		}
	private:
		char* pData = nullptr;
		const VertexLayout& layout;
	};

	class ConstVertex
	{
	public:
		ConstVertex(const Vertex& v) noexcept(!IS_DEBUG)
			:
			vertex(v)
		{}
		template<VertexLayout::ElementType Type>
		const auto& attr() const noexcept(!IS_DEBUG)
		{
			return const_cast<Vertex&>(vertex).attr<Type>();
		}
	private:
		Vertex vertex;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer(VertexLayout layout) noexcept(!IS_DEBUG)
			:
			layout(std::move(layout))
		{}
		const char* getData() const noexcept(!IS_DEBUG)
		{
			return buffer.data();
		}
		const VertexLayout& getLayout() const noexcept
		{
			return layout;
		}
		size_t size() const noexcept(!IS_DEBUG)
		{
			return buffer.size() / layout.size();
		}
		size_t sizeBytes() const noexcept(!IS_DEBUG)
		{
			return buffer.size();
		}
		template<typename ...Params>
		void emplaceBack(Params&&... params) noexcept(!IS_DEBUG)
		{
			assert(sizeof...(params) == layout.getElementCount() && "Param count doesn't match number of vertex elements");
			buffer.resize(buffer.size() + layout.size());
			back().setAttributeByIndex(0u, std::forward<Params>(params)...);
		}
		Vertex back() noexcept(!IS_DEBUG)
		{
			assert(buffer.size() != 0u);
			return Vertex{ buffer.data() + buffer.size() - layout.size(), layout };
		}
		Vertex front() noexcept(!IS_DEBUG)
		{
			assert(buffer.size() != 0);
			return Vertex{ buffer.data() + buffer.size() - layout.size(), layout };
		}
		Vertex operator[](size_t i) noexcept(!IS_DEBUG)
		{
			assert(i < size());
			return Vertex{ buffer.data() + layout.size() * i, layout };
		}
		ConstVertex back() const noexcept(!IS_DEBUG)
		{
			return const_cast<VertexBuffer*>(this)->back();
		}
		ConstVertex front() const noexcept(!IS_DEBUG)
		{
			return const_cast<VertexBuffer*>(this)->front();
		}
		ConstVertex operator[](size_t i) const noexcept(!IS_DEBUG)
		{
			return const_cast<VertexBuffer&>(*this)[i];
		}
	private:
		std::vector<char> buffer;
		VertexLayout layout;
	};
}