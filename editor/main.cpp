#include <vin.hpp>

#include <module/windowing/windowmodule.hpp>
#include <module/editor/editormodule.hpp>
#include <module/forwardrenderer/forwardrenderer.hpp>

#include <module.hpp>
#include <nfd.h>

#include "editor.hpp"

class GinEditor : public Vin::App {
public:
	void Build() {
		Vin::WindowInfo winfo{};
		winfo.title = "Editor";

		Vin::AssetDatabase::AddAsset<Vin::WindowInfo>(std::move(winfo), VIN_WINDOWINFO_ASSETPATH);

		AddModule<Vin::WindowModule>();
		AddModule<Vin::ForwardRendererModule>();
		AddModule<Vin::EditorModule>();
		AddModule<GinEditorModule>();
	}
};

Vin::App* Vin::CreateApp() {
	Initialize();
	NFD_Init();
	return new GinEditor{};
}

void Vin::DestroyApp(Vin::App* app) {
	NFD_Quit();
	delete app;
}