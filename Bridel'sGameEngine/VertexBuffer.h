#pragma once
#include "Bindable.h"
#include "GraphicsThrowMacros.h"
#include "Vertex.h"

namespace Bind
{
	class VertexBuffer : public Bindable
	{
	public:
		VertexBuffer(Graphics& gfx, const Dvtx::VertexBuffer& vbuf);
		VertexBuffer(Graphics& gfx, const std::string& tag, const Dvtx::VertexBuffer& vbuf);
		void bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<VertexBuffer> resolve(Graphics& gfx, const std::string& tag,
			const Dvtx::VertexBuffer& vbuf);
		template<typename...Ignore>
		static std::string generateUID(const std::string& tag, Ignore&&...ignore)
		{
			return generateUID_(tag);
		}
		std::string getUID() const noexcept override;
	private:
		static std::string generateUID_(const std::string& tag);
	protected:
		std::string tag;
		UINT stride;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	};
}