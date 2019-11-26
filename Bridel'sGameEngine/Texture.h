#pragma once
#include "Bindable.h"

class Surface;

namespace Bind
{
	class Texture : public Bindable
	{
	public:
		Texture(Graphics& gfx, const std::string& path, UINT slot = 0);
		void bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<Texture> resolve(Graphics& gfx, const std::string& path, UINT slot = 0);
		static std::string generateUID(const std::string& path, UINT slot = 0);
		std::string getUID() const noexcept override;
	private:
		unsigned int slot;
	protected:
		std::string path;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
	};
}