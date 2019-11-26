#pragma once
#include "Bindable.h"

namespace Bind
{
	class PixelShader : public Bindable
	{
	public:
		PixelShader(Graphics& gfx, const std::string& path);
		void bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<PixelShader> resolve(Graphics& gfx, const std::string& path);
		static std::string generateUID(const std::string& path);
		std::string getUID() const noexcept override;
	protected:
		std::string path;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	};
}