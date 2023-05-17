#include "GraphicsContext.h"
#include "LogManager.h"
#include "SpriteBuilder.h"
#include "GameObjectDesigner.h"
#include "Material.h"
#include "MeshRenderer.h"

int main(int argc, char** argv) {

	try {
		std::unique_ptr<diamond_engine::GraphicsContext> graphicsContext = std::make_unique<diamond_engine::GraphicsContext>();
		graphicsContext->InitializeWindow({ 1280, 960 }, "Hello!");
		graphicsContext->InitializeGLEW();

		/*diamond_engine::GLAllocator glBufferAllocator(glGenBuffers, glDeleteBuffers);
		glBufferAllocator.Reserve(12);
		glBufferAllocator.Allocate(6);
		LOG_INFO(glBufferAllocator.ToString());
		glBufferAllocator.Allocate(6);
		LOG_INFO(glBufferAllocator.ToString());
		glBufferAllocator.Free(9);
		LOG_INFO(glBufferAllocator.ToString());

		GLuint buffer_1 = glBufferAllocator.Get();
		LOG_INFO("GOT BUFFER: " + std::to_string(buffer_1));
		LOG_INFO(glBufferAllocator.ToString());

		GLuint buffer_2 = glBufferAllocator.Get();
		LOG_INFO("GOT BUFFER: " + std::to_string(buffer_2));
		LOG_INFO(glBufferAllocator.ToString());

		GLuint buffer_3 = glBufferAllocator.Get();
		LOG_INFO("GOT BUFFER: " + std::to_string(buffer_3));
		LOG_INFO(glBufferAllocator.ToString());

		glBufferAllocator.Allocate(1);
		LOG_INFO(glBufferAllocator.ToString());

		GLuint buffer_4 = glBufferAllocator.Get();
		LOG_INFO("GOT BUFFER: " + std::to_string(buffer_4));
		LOG_INFO(glBufferAllocator.ToString());*/

		std::shared_ptr<diamond_engine::SharedShaderStore> sharedShaderStore = std::make_shared<diamond_engine::SharedShaderStore>();
		sharedShaderStore->Load(".\\shaders");

		std::unique_ptr<diamond_engine::Scene> scene = std::make_unique<diamond_engine::Scene>();

		std::unique_ptr<diamond_engine::GameObject> sprite				= std::make_unique<diamond_engine::GameObject>();
		std::unique_ptr<diamond_engine::SpriteBuilder> spriteBuilder	= std::make_unique<diamond_engine::SpriteBuilder>();
		spriteBuilder->SetCapacity(2);
		spriteBuilder->SetSharedShaderStore(sharedShaderStore);
		diamond_engine::GameObjectDesigner::Design(sprite.get(), spriteBuilder.get());
		scene->AddGameObject(std::move(sprite));

		std::unique_ptr<diamond_engine::GameObject> sprite_2 = std::make_unique<diamond_engine::GameObject>();
		diamond_engine::GameObjectDesigner::Design(sprite_2.get(), spriteBuilder.get());
		sprite_2->GetComponent<diamond_engine::MeshRenderer>("MeshRenderer")->GetMaterial()->SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
		sprite_2->GetComponent<diamond_engine::Transform>("Transform")->Translate(glm::vec3(2.0f, 1.0f, 0.0f));
		scene->AddGameObject(std::move(sprite_2));

		graphicsContext->SetScene(std::move(scene));

		/*std::shared_ptr<diamond_engine::Shader> vertexShader = std::make_shared<diamond_engine::Shader>(GL_VERTEX_SHADER);
		if (vertexShader->Load("./shaders/vertex_sprite.dsh")) {
			LOG_INFO("Compiled vertex shader - Source: " + vertexShader->GetSource());
		}

		std::shared_ptr<diamond_engine::Shader> fragmentShader = std::make_shared<diamond_engine::Shader>(GL_FRAGMENT_SHADER);
		if (fragmentShader->Load("./shaders/fragment_sprite.dsh")) {
			LOG_INFO("Compiled fragment shader - Source: " + fragmentShader->GetSource());
		}
		
		diamond_engine::ShaderProgram shaderProgram;
		shaderProgram.AttachShader(vertexShader);
		shaderProgram.AttachShader(fragmentShader);
		shaderProgram.Link();
		if (shaderProgram.IsLinked()) {
			LOG_INFO("Shader progam successfully linked!");
		}*/

		graphicsContext->Execute();
	}
	catch (const std::exception& e) {
		LOG_ERROR(e.what());

		std::this_thread::sleep_for(std::chrono::seconds(10));
	}

	return 0;
}