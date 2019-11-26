#pragma once

#include "Bindable.h"
#include <type_traits>
#include <memory>
#include <unordered_map>

namespace Bind
{
	class Codex
	{
	public:
		template<class T, typename...Params>
		static std::shared_ptr<T> resolve(Graphics& gfx, Params&&...p) noxnd
		{
			static_assert(std::is_base_of<Bindable, T>::value, "Can only resolve classes deriver from Bindable");
			return get().resolve_<T>(gfx, std::forward<Params>(p)...);
		}
	private:
		template<class T, typename...Params>
		std::shared_ptr<T> resolve_(Graphics& gfx, Params&&...p) noxnd
		{
			const auto key = T::generateUID(std::forward<Params>(p)...);
			const auto i = binds.find(key);
			if (i == binds.end())
			{
				auto bind = std::make_shared<T>(gfx, std::forward<Params>(p)...);
				binds[key] = bind;
				return bind;
			}
			else
				return std::static_pointer_cast<T>(i->second);
		}
		static Codex& get()
		{
			static Codex codex;
			return codex;
		}
	private:
		std::unordered_map<std::string, std::shared_ptr<Bindable>> binds;
	};
}