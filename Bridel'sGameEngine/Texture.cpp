#include "Texture.h"
#include "Surface.h"
#include "GraphicsThrowMacros.h"
#include "BindableCodex.h"

namespace Bind
{
	namespace wrl = Microsoft::WRL;

	Texture::Texture(Graphics& gfx, const std::string& path, UINT slot)
		:
		path(path),
		slot(slot)
	{
		INFOMAN(gfx);

		// load surface
		const auto s = Surface::fromFile(path);

		// create texture resource
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = s.getWidth();
		textureDesc.Height = s.getHeight();
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = s.getBufferPtr();
		sd.SysMemPitch = s.getWidth() * sizeof(Surface::Color);
		wrl::ComPtr<ID3D11Texture2D> pTexture;
		GFX_THROW_INFO(getDevice(gfx)->CreateTexture2D(&textureDesc, &sd, &pTexture));

		// create the resource view on the texture
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		GFX_THROW_INFO(getDevice(gfx)->CreateShaderResourceView(
			pTexture.Get(), &srvDesc, &pTextureView
		));
	}

	void Texture::bind(Graphics& gfx) noexcept
	{
		getContext(gfx)->PSSetShaderResources(slot, 1u, pTextureView.GetAddressOf());
	}

	std::shared_ptr<Texture> Texture::resolve(Graphics& gfx, const std::string& path, UINT slot)
	{
		return Codex::resolve<Texture>(gfx, path, slot);
	}

	std::string Texture::generateUID(const std::string& path, UINT slot)
	{
		using namespace std::string_literals;
		return typeid(Texture).name() + "#"s + path + "#" + std::to_string(slot);
	}

	std::string Texture::getUID() const noexcept
	{
		return generateUID(path, slot);
	}
}