#pragma once
#include "Bindable.h"

namespace Bind
{
	class IndexBuffer : public Bindable
	{
	public:
		IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indices);
		IndexBuffer(Graphics& gfx, std::string tag, const std::vector<unsigned short>& indices);
		void bind(Graphics& gfx) noexcept override;
		UINT getCount() const noexcept;
		static std::shared_ptr<IndexBuffer> resolve(Graphics& gfx, const std::string& tag,
			const std::vector<unsigned short>& indices);
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
		UINT count;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	};
}