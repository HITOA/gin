#pragma once

#include <vin.hpp>
#include <module/editor/editorwindow.hpp>
#include <gin/graph/graph.hpp>

class GinEditorModule : public Vin::Module {
public:
	void Start();
	void Process();
	void Render();
	void LateRender();
	void OnEvent(Vin::EventHandler handler);

private:
	void SetupScene();
	void RegisterWindow(std::unique_ptr<Vin::EditorWindow> window, bool show);

private:
	std::shared_ptr<Vin::Scene> scene{};
	std::shared_ptr<Vin::Camera> camera{};
};