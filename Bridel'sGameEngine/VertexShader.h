#pragma once
#include "Bindable.h"

namespace Bind
{
	class VertexShader : public Bindable
	{
	public:
		VertexShader(Graphics& gfx, const std::string& path);
		void bind(Graphics& gfx) noexcept override;
		ID3DBlob* getBytecode() const noexcept;
		static std::shared_ptr<VertexShader> resolve(Graphics& gfx, const std::string& path);
		static std::string generateUID(const std::string& path);
		std::string getUID() const noexcept override;
	protected:
		std::string path;
		Microsoft::WRL::ComPtr<ID3DBlob> pBytecodeBlob;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	};
}