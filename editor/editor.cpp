#include "editor.hpp"

#include <module/windowing/windowmodule.hpp>
#include <vfs/vfs.hpp>
#include <utils/shaderutils.hpp>
#include <inputsystem/input.hpp>
#include <module/editor/editorwindow.hpp>

#include "grapheditorwindow.hpp"
#include "meshbuilderwindow.hpp"

Vin::Vector2<int> mouseLastPos{};
float pitch = 0;
float yaw = 0;

std::shared_ptr<Vin::Program> program{};
std::shared_ptr<Vin::Material> material{};

void GinEditorModule::Start()
{
	Vin::VFS::AddFileSystem(std::make_shared<Vin::NativeFS>("./data"));

	program = Vin::LoadProgram("vs.glsl", "fs.glsl");
	material = std::make_shared<Vin::Material>(program);

	scene = std::make_shared<Vin::Scene>();

	Vin::Light mainLight{};
	mainLight.mainLight = true;

	mainLight.color = Vin::Vector3<float>{ 1.0f, 1.0f, 1.0f };
	mainLight.shadow.distance = 20;

	mainLight.direction = Vin::Vector3<float>{ 0.5, 1, 0.15 }.Normalize();
	mainLight.intensity = 3;

	(*scene)->CreateEntity(mainLight);

	Vin::Asset<Vin::WindowInfo> windowInfo = Vin::AssetDatabase::GetAsset<Vin::WindowInfo>(VIN_WINDOWINFO_ASSETPATH);

	camera = std::shared_ptr<Vin::Camera>{ new Vin::Camera{ {windowInfo->width, windowInfo->height} } };
	camera->SetFOV(40);
	camera->SetNearPlane(0.1);
	camera->SetFarPlane(4000);

	graph = std::make_shared<Gin::Graph::Graph>();

	std::unique_ptr<GraphEditorWindow> graphEditor{ std::make_unique<GraphEditorWindow>() };
	graphEditor->SetGraph(graph);

	std::unique_ptr<MeshBuilderWindow> meshBuilder{ std::make_unique<MeshBuilderWindow>() };
	meshBuilder->SetGraph(graph);
	meshBuilder->SetScene(scene);
	meshBuilder->SetMaterial(material);

	RegisterWindow(std::move(graphEditor), true);
	RegisterWindow(std::move(meshBuilder), true);

	SetupScene();
}

void GinEditorModule::Process()
{
	float deltaTime = GetApp()->GetDeltaTime().GetMillisecond();

	const float speed = 1;

	Vin::Vector2<int> mouseDelta = Vin::Input::GetMousePosition() - mouseLastPos;
	mouseLastPos = Vin::Input::GetMousePosition();

	pitch -= (float)mouseDelta.y * 0.005f;
	yaw -= (float)mouseDelta.x * 0.005f;

	pitch = Vin::Clamp<float>(pitch, -90 * Vin::deg2rad, 90 * Vin::deg2rad);

	Vin::Quaternion<float> qPitch = Vin::Quaternion<float>::Euler({ pitch, 0, 0 });
	Vin::Quaternion<float> qYaw = Vin::Quaternion<float>::Euler({ 0, yaw, 0 });

	camera->rotation = (qYaw * qPitch).Normalize();
	//camera->rotation = (camera->rotation * qYaw).Normalize();

	Vin::Vector3<float> translation{ 0.0f };

	if (Vin::Input::IsKeyPressed(Vin::Key::Key_W)) {
		translation += Vin::Vector3<float>{ 0, 0, -1 };
	}
	if (Vin::Input::IsKeyPressed(Vin::Key::Key_S)) {
		translation += Vin::Vector3<float>{ 0, 0, 1 };
	}
	if (Vin::Input::IsKeyPressed(Vin::Key::Key_A)) {
		translation += Vin::Vector3<float>{ -1, 0, 0};
	}
	if (Vin::Input::IsKeyPressed(Vin::Key::Key_D)) {
		translation += Vin::Vector3<float>{ 1, 0, 0 };
	}
	if (Vin::Input::IsKeyPressed(Vin::Key::Space)) {
		camera->position += Vin::Vector3<float>{ 0, speed* deltaTime * 0.01f, 0};
	}
	if (Vin::Input::IsKeyPressed(Vin::Key::LeftControl)) {
		camera->position += Vin::Vector3<float>{ 0, -speed * deltaTime * 0.01f, 0};
	}

	translation = translation.Normalize();

	translation *= deltaTime * 0.01f * speed;

	translation = (camera->rotation.GetRotationMatrix() * Vin::Vector4<float>{ translation.xyz, 1.0f }).xyz;

	if (Vin::Input::IsKeyPressed(Vin::Key::LeftShift))
		translation *= 3;

	camera->position += translation;

	//Vin::Logger::Log("Camera Position : {}", camera->position);
}

void GinEditorModule::Render()
{
	scene->Render(camera);
}

void GinEditorModule::LateRender()
{
	Vin::Renderer::Blit(camera->GetRenderTarget(), nullptr);
}

void GinEditorModule::OnEvent(Vin::EventHandler handler)
{
	if (Vin::WindowCloseEvent* event = handler.GetEvent<Vin::WindowCloseEvent>())
		GetApp()->Stop();
	if (Vin::WindowResizeEvent* event = handler.GetEvent<Vin::WindowResizeEvent>())
		camera->Resize({ event->width, event->height });
}

void GinEditorModule::SetupScene()
{
	Vin::Light mainLight{};
	mainLight.mainLight = true;

	mainLight.color = Vin::Vector3<float>{ 1.0f, 1.0f, 1.0f };
	mainLight.shadow.distance = 20;

	mainLight.direction = Vin::Vector3<float>{ 0.5, 1, 0.15 }.Normalize();
	mainLight.intensity = 3;

	(*scene)->CreateEntity(mainLight);
}

void GinEditorModule::RegisterWindow(std::unique_ptr<Vin::EditorWindow> window, bool show)
{
	Vin::EventHandler handler{};

	Vin::RegisterEditorWindowEvent registerEvent{};
	registerEvent.show = show;
	registerEvent.window = std::move(window);

	handler.Bind(std::move(registerEvent));
	DispatchEvent(std::move(handler));
}
