#pragma once

#include <optional>
#include "Vertex.h"
#include "IndexedTriangleList.h"
#include <DirectXMath.h>
#include "MathInclude.h"
#include <array>

class Cube
{
public:
	static IndexedTriangleList makeIndependent(Dvtx::VertexLayout layout)
	{
		using namespace Dvtx;
		using Type = Dvtx::VertexLayout::ElementType;

		constexpr float side = 1.0f / 2.0f;

		VertexBuffer vertices(std::move(layout), 24u);
		vertices[0].attr<Type::Position3D>() = { -side,-side,-side };// 0 near side
		vertices[1].attr<Type::Position3D>() = { side,-side,-side };// 1
		vertices[2].attr<Type::Position3D>() = { -side,side,-side };// 2
		vertices[3].attr<Type::Position3D>() = { side,side,-side };// 3
		vertices[4].attr<Type::Position3D>() = { -side,-side,side };// 4 far side
		vertices[5].attr<Type::Position3D>() = { side,-side,side };// 5
		vertices[6].attr<Type::Position3D>() = { -side,side,side };// 6
		vertices[7].attr<Type::Position3D>() = { side,side,side };// 7
		vertices[8].attr<Type::Position3D>() = { -side,-side,-side };// 8 left side
		vertices[9].attr<Type::Position3D>() = { -side,side,-side };// 9
		vertices[10].attr<Type::Position3D>() = { -side,-side,side };// 10
		vertices[11].attr<Type::Position3D>() = { -side,side,side };// 11
		vertices[12].attr<Type::Position3D>() = { side,-side,-side };// 12 right side
		vertices[13].attr<Type::Position3D>() = { side,side,-side };// 13
		vertices[14].attr<Type::Position3D>() = { side,-side,side };// 14
		vertices[15].attr<Type::Position3D>() = { side,side,side };// 15
		vertices[16].attr<Type::Position3D>() = { -side,-side,-side };// 16 bottom side
		vertices[17].attr<Type::Position3D>() = { side,-side,-side };// 17
		vertices[18].attr<Type::Position3D>() = { -side,-side,side };// 18
		vertices[19].attr<Type::Position3D>() = { side,-side,side };// 19
		vertices[20].attr<Type::Position3D>() = { -side,side,-side };// 20 top side
		vertices[21].attr<Type::Position3D>() = { side,side,-side };// 21
		vertices[22].attr<Type::Position3D>() = { -side,side,side };// 22
		vertices[23].attr<Type::Position3D>() = { side,side,side };// 23

		return{
			std::move(vertices),{
				0,2, 1,    2,3,1,
				4,5, 7,    4,7,6,
				8,10, 9,  10,11,9,
				12,13,15, 12,15,14,
				16,17,18, 18,17,19,
				20,23,21, 20,22,23
			}
		};
	}
	static IndexedTriangleList makeIndependentTextured()
	{
		using namespace Dvtx;
		using Type = Dvtx::VertexLayout::ElementType;

		auto itl = makeIndependent(std::move(VertexLayout{}
			.append(Type::Position3D)
			.append(Type::Normal)
			.append(Type::Texture2D)
		));

		itl.vertices[0].attr<Type::Texture2D>() = { 0.0f,0.0f };
		itl.vertices[1].attr<Type::Texture2D>() = { 1.0f,0.0f };
		itl.vertices[2].attr<Type::Texture2D>() = { 0.0f,1.0f };
		itl.vertices[3].attr<Type::Texture2D>() = { 1.0f,1.0f };
		itl.vertices[4].attr<Type::Texture2D>() = { 0.0f,0.0f };
		itl.vertices[5].attr<Type::Texture2D>() = { 1.0f,0.0f };
		itl.vertices[6].attr<Type::Texture2D>() = { 0.0f,1.0f };
		itl.vertices[7].attr<Type::Texture2D>() = { 1.0f,1.0f };
		itl.vertices[8].attr<Type::Texture2D>() = { 0.0f,0.0f };
		itl.vertices[9].attr<Type::Texture2D>() = { 1.0f,0.0f };
		itl.vertices[10].attr<Type::Texture2D>() = { 0.0f,1.0f };
		itl.vertices[11].attr<Type::Texture2D>() = { 1.0f,1.0f };
		itl.vertices[12].attr<Type::Texture2D>() = { 0.0f,0.0f };
		itl.vertices[13].attr<Type::Texture2D>() = { 1.0f,0.0f };
		itl.vertices[14].attr<Type::Texture2D>() = { 0.0f,1.0f };
		itl.vertices[15].attr<Type::Texture2D>() = { 1.0f,1.0f };
		itl.vertices[16].attr<Type::Texture2D>() = { 0.0f,0.0f };
		itl.vertices[17].attr<Type::Texture2D>() = { 1.0f,0.0f };
		itl.vertices[18].attr<Type::Texture2D>() = { 0.0f,1.0f };
		itl.vertices[19].attr<Type::Texture2D>() = { 1.0f,1.0f };
		itl.vertices[20].attr<Type::Texture2D>() = { 0.0f,0.0f };
		itl.vertices[21].attr<Type::Texture2D>() = { 1.0f,0.0f };
		itl.vertices[22].attr<Type::Texture2D>() = { 0.0f,1.0f };
		itl.vertices[23].attr<Type::Texture2D>() = { 1.0f,1.0f };

		return itl;
	}
};