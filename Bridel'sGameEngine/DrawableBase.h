#pragma once
#include "Drawable.h"
#include "IndexBuffer.h"
#include "ConditionalNoexcept.h"

template<class T>
class DrawableBase : public Drawable
{
protected:
	static bool isStaticInitialized() noexcept
	{
		return !staticBinds.empty();
	}
	static void addStaticBind(std::unique_ptr <Bind::Bindable> bind) noxnd
	{
		assert("*Must* use addStaticIndexBuffer to bind index buffer" && typeid(*bind) != typeid(Bind::IndexBuffer));
		staticBinds.push_back(std::move(bind));
	}
	void addStaticIndexBuffer(std::unique_ptr<Bind::IndexBuffer> ibuf) noxnd
	{
		assert("Attemptimg to add index buffer a second time" && pIndexBuffer == nullptr);
		pIndexBuffer = ibuf.get();
		staticBinds.push_back(std::move(ibuf));
	}
	void setIndexFromStatic() noxnd
	{
		assert("Attemptimg to add index buffer a second time" && pIndexBuffer == nullptr);
		for (const auto& b : staticBinds)
			if (const auto p = dynamic_cast<Bind::IndexBuffer*>(b.get()))
			{
				pIndexBuffer = p;
				return;
			}
		assert("Failed to find index buffer in static binds" && pIndexBuffer != nullptr);
	}
private:
	const std::vector<std::unique_ptr<Bind::Bindable>>& getStaticBinds() const noexcept override
	{
		return staticBinds;
	}
private:
	static std::vector<std::unique_ptr<Bind::Bindable>> staticBinds;
};

template<class T>
std::vector<std::unique_ptr<Bind::Bindable>> DrawableBase<T>::staticBinds;