#pragma once
#include <vector>
#include <type_traits>
#include "Graphics.h"
#include "Color.h"
#include "ConditionalNoexcept.h"

namespace Dvtx
{
	class VertexLayout
	{
	public:
		enum ElementType
		{
			Position2D,
			Position3D,
			Texture2D,
			Normal,
			Tangent,
			Bitangent,
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
			static constexpr const char* code = "P2";
		};
		template<> struct Map<Position3D>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Position";
			static constexpr const char* code = "P3";
		};
		template<> struct Map<Texture2D>
		{
			using SysType = DirectX::XMFLOAT2;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* semantic = "Texcoord";
			static constexpr const char* code = "T2";
		};
		template<> struct Map<Normal>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Normal";
			static constexpr const char* code = "N";
		};
		template<> struct Map<Tangent>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Tangent";
			static constexpr const char* code = "Nt";
		};
		template<> struct Map<Bitangent>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Bitangent";
			static constexpr const char* code = "Nb";
		};
		template<> struct Map<Float3Color>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Color";
			static constexpr const char* code = "C3";
		};
		template<> struct Map<Float4Color>
		{
			using SysType = DirectX::XMFLOAT4;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
			static constexpr const char* semantic = "Color";
			static constexpr const char* code = "C4";
		};
		template<> struct Map<BRGAColor>
		{
			using SysType = ::BRGAColor;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
			static constexpr const char* semantic = "Color";
			static constexpr const char* code = "C8";
		};

		class Element
		{
		public:
			Element(ElementType type, size_t offset);
			size_t getOffsetAfter() const noxnd;
			size_t getOffset() const;
			size_t size() const noxnd;
			static constexpr size_t sizeOf(ElementType type) noxnd;
			ElementType getType() const noexcept;
			D3D11_INPUT_ELEMENT_DESC getDesc() const noxnd;
			const char* getCode() const noexcept;
		private:
			template<ElementType type>
			static constexpr D3D11_INPUT_ELEMENT_DESC generateDesc(size_t offset) noexcept
			{
				return { Map<type>::semantic, 0, Map<type>::dxgiFormat, 0, (UINT)offset, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			}
		private:
			ElementType type;
			size_t offset;
		};
	public:
		template<ElementType Type>
		const Element& resolve() const noxnd
		{
			for (auto& e : elements)
				if (e.getType() == Type)
					return e;
			assert("Could not resolve element type" && false);
			return elements.front();
		}
		const Element& resolveByIndex(size_t i) const noxnd;
		VertexLayout& append(ElementType type) noxnd;
		size_t size() const noxnd;
		size_t getElementCount() const noexcept;
		std::vector<D3D11_INPUT_ELEMENT_DESC> getD3DLayout() const noxnd;
		std::string getCode() const noxnd;
	private:
		std::vector<Element> elements;
	};

	class Vertex
	{
		friend class VertexBuffer;
	public:
		template<VertexLayout::ElementType Type>
		auto& attr() noxnd
		{
			auto pAttribute = pData + layout.resolve<Type>().getOffset();
			return *reinterpret_cast<typename VertexLayout::Map<Type>::SysType*>(pAttribute);
		}
		template<typename T>
		void setAttributeByIndex(size_t i, T&& val) noxnd
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
			case VertexLayout::Tangent:
				setAttribute<VertexLayout::Tangent>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Bitangent:
				setAttribute<VertexLayout::Bitangent>(pAttribute, std::forward<T>(val));
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
		Vertex(char* pData, const VertexLayout& layout) noxnd;
	private:
		// enables parameter pack setting of multiple parameters by element index
		template<typename First, typename ...Rest>
		void setAttributeByIndex(size_t i, First&& first, Rest&&... rest) noxnd
		{
			setAttributeByIndex(i, std::forward<First>(first));
			setAttributeByIndex(i + 1, std::forward<Rest>(rest)...);
		}
		// helper to reduce code duplication in setAttributeByIndex
		template<VertexLayout::ElementType DestLayoutType, typename SrcType>
		void setAttribute(char* pAttribute, SrcType&& val) noxnd
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
		ConstVertex(const Vertex& v) noxnd;
		template<VertexLayout::ElementType Type>
		const auto& attr() const noxnd
		{
			return const_cast<Vertex&>(vertex).attr<Type>();
		}
	private:
		Vertex vertex;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer(VertexLayout layout, size_t size = 0u) noxnd;
		const char* getData() const noxnd;
		const VertexLayout& getLayout() const noexcept;
		void resize(size_t newSize) noxnd;
		size_t size() const noxnd;
		size_t sizeBytes() const noxnd;
		template<typename ...Params>
		void emplaceBack(Params&&... params) noxnd
		{
			assert(sizeof...(params) == layout.getElementCount() && "Param count doesn't match number of vertex elements");
			buffer.resize(buffer.size() + layout.size());
			back().setAttributeByIndex(0u, std::forward<Params>(params)...);
		}
		Vertex back() noxnd;
		Vertex front() noxnd;
		Vertex operator[](size_t i) noxnd;
		ConstVertex back() const noxnd;
		ConstVertex front() const noxnd;
		ConstVertex operator[](size_t i) const noxnd;
	private:
		std::vector<char> buffer;
		VertexLayout layout;
	};
}