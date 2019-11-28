#include "Sampler.h"
#include "GraphicsThrowMacros.h"
#include "BindableCodex.h"

namespace Bind
{
	Sampler::Sampler(Graphics& gfx)
	{
		INFOMAN(gfx);

		D3D11_SAMPLER_DESC samplerDesc = CD3D11_SAMPLER_DESC{ CD3D11_DEFAULT{} };
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MinLOD = 0.0f; // turn up this to lower texture resolution
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;//D3D11_FLOAT32_MAX;

		GFX_THROW_INFO(getDevice(gfx)->CreateSamplerState(&samplerDesc, &pSampler));
	}

	void Sampler::bind(Graphics& gfx) noexcept
	{
		getContext(gfx)->PSSetSamplers(0, 1, pSampler.GetAddressOf());
	}

	std::shared_ptr<Sampler> Sampler::resolve(Graphics& gfx)
	{
		return Codex::resolve<Sampler>(gfx);
	}

	std::string Sampler::generateUID()
	{
		return typeid(Sampler).name();
	}

	std::string Sampler::getUID() const noexcept
	{
		return generateUID();
	}
}