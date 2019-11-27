#pragma once
#include "Drawable.h"
#include "BindableCommon.h"
#include "Vertex.h"
#include <optional>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "ConditionalNoexcept.h"

class ModelException : public ExceptionFinder
{
public:
	ModelException(int line, const char* file, std::string note) noexcept;
	const char* what() const noexcept override;
	const char* getType() const noexcept override;
	const std::string& getNote() const noexcept;
private:
	std::string note;
};

class Mesh : public Drawable
{
public:
	Mesh(Graphics& gfx, std::vector<std::shared_ptr<Bind::Bindable>> bindPtrs);
	void draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noxnd;
	DirectX::XMMATRIX getTransformXM() const noexcept override;
private:
	mutable DirectX::XMFLOAT4X4 transform;
};

class Node
{
	friend class Model;
public:
	Node(int id, const std::string& name, std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform) noxnd;
	void draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noxnd;
	void setAppliedTransform(DirectX::FXMMATRIX transform) noexcept;
	int getId() const noexcept;
	void showTree(Node*& pSelectedNode) const noexcept;
private:
	void addChild(std::unique_ptr<Node> pChild) noxnd;
private:
	std::string name;
	int id;
	std::vector<std::unique_ptr<Node>> childPtrs;
	std::vector<Mesh*> meshPtrs;
	DirectX::XMFLOAT4X4 transform;
	DirectX::XMFLOAT4X4 appliedTransform;
};

class Model
{
public:
	Model(Graphics& gfx, const std::string fileName);
	void draw(Graphics& gfx) const noxnd;
	void showWindow(const char* windowName = nullptr) noexcept;
	void setRootTransform(DirectX::FXMMATRIX tf) noexcept;
	~Model() noexcept;
private:
	static std::unique_ptr<Mesh> parseMesh(Graphics& gfx, const aiMesh& mesh, const aiMaterial* const* pMaterials);
	std::unique_ptr<Node> parseNode(int& nextId, const aiNode& node) noexcept;
private:
	std::unique_ptr<Node> pRoot;
	std::vector<std::unique_ptr<Mesh>> meshPtrs;
	std::unique_ptr<class ModelWindow> pWindow;
};