#include "GameState.h"

using namespace JimmyGod::Input;
using namespace JimmyGod::Graphics;
using namespace JimmyGod::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::LightGray);

	mCamera.SetPosition({ 0.0f,0.0f,-5.0f });
	mCamera.SetDirection({ 0.0f,0.0f,1.0f });

//		   +---------------+
//		  /				  /|
//		 /				 / |
// 1.0	+---------------+  |
//		|		^		|  |
//		|		|		|  |
//		|<------|------>|  |1.0
//		|		|		| /
//		|		v		|/
//		+---------------+ -1.0
//	 -1.0			   1.0

	//Front
	mVertices.emplace_back(Vertex{ Vector3{ -0.5f,0.5f,-0.5f }, Color{Colors::Blue} });
	mVertices.emplace_back(Vertex{ Vector3{ 0.5f,0.5f,-0.5f }, Color{Colors::Blue} });
	mVertices.emplace_back(Vertex{ Vector3{ -0.5f,-0.5f,-0.5f }, Color{Colors::Blue} });
	mVertices.emplace_back(Vertex{ Vector3{ -0.5f,-0.5f,-0.5f }, Color{Colors::Blue} });
	mVertices.emplace_back(Vertex{ Vector3{ 0.5f,0.5f,-0.5f }, Color{Colors::Blue} });
	mVertices.emplace_back(Vertex{ Vector3{ 0.5f, -0.5f,-0.5f }, Color{Colors::Blue} });
	//left
	mVertices.emplace_back(Vertex{ Vector3{ -0.5f,0.5f, -0.5f }, Color{Colors::Green} });
	mVertices.emplace_back(Vertex{ Vector3{ -0.5f,-0.5f,0.5f }, Color{Colors::Green} });
	mVertices.emplace_back(Vertex{ Vector3{ -0.5f,0.5f, 0.5f }, Color{Colors::Green} });
	mVertices.emplace_back(Vertex{ Vector3{ -0.5f,-0.5f, 0.5f }, Color{Colors::Green} });
	mVertices.emplace_back(Vertex{ Vector3{ -0.5f,0.5f, -0.5f }, Color{Colors::Green} });
	mVertices.emplace_back(Vertex{ Vector3{ -0.5f,-0.5f, -0.5f }, Color{Colors::Green} });
	//Back
	mVertices.emplace_back(Vertex{ Vector3{ -0.5f,-0.5f,0.5f }, Color{Colors::Red} });
	mVertices.emplace_back(Vertex{ Vector3{ 0.5f,0.5f,0.5f }, Color{Colors::Red} });
	mVertices.emplace_back(Vertex{ Vector3{ -0.5f,0.5f,0.5f }, Color{Colors::Red} });
	mVertices.emplace_back(Vertex{ Vector3{ 0.5f, -0.5f,0.5f }, Color{Colors::Red} });
	mVertices.emplace_back(Vertex{ Vector3{ 0.5f,0.5f,0.5f }, Color{Colors::Red} });
	mVertices.emplace_back(Vertex{ Vector3{ -0.5f,-0.5f,0.5f }, Color{Colors::Red} });
	//Right
	mVertices.emplace_back(Vertex{ Vector3{ 0.5f,0.5f,-0.5f }, Color{Colors::Pink} });
	mVertices.emplace_back(Vertex{ Vector3{ 0.5f,-0.5f,0.5f }, Color{Colors::Pink} });
	mVertices.emplace_back(Vertex{ Vector3{ 0.5f,-0.5f,-0.5f }, Color{Colors::Pink} });
	mVertices.emplace_back(Vertex{ Vector3{ 0.5f,0.5f, -0.5f }, Color{Colors::Pink} });
	mVertices.emplace_back(Vertex{ Vector3{ 0.5f,0.5f, 0.5f }, Color{Colors::Pink} });
	mVertices.emplace_back(Vertex{ Vector3{ 0.5f,-0.5f, 0.5f }, Color{Colors::Pink} });
	//Top
	mVertices.emplace_back(Vertex{ Vector3{ 0.5f,0.5f,-0.5f }, Color{Colors::Black} });
	mVertices.emplace_back(Vertex{ Vector3{ -0.5f,0.5f,-0.5f }, Color{Colors::Black} });
	mVertices.emplace_back(Vertex{ Vector3{ 0.5f,0.5f, 0.5f }, Color{Colors::Black} });
	mVertices.emplace_back(Vertex{ Vector3{ -0.5f,0.5f,0.5f }, Color{Colors::Black} });
	mVertices.emplace_back(Vertex{ Vector3{ 0.5f,0.5f,0.5f }, Color{Colors::Black} });
	mVertices.emplace_back(Vertex{ Vector3{ -0.5f,0.5f, -0.5f }, Color{Colors::Black} });
	//Bottom
	mVertices.emplace_back(Vertex{ Vector3{ -0.5f,-0.5f,-0.5f }, Color{Colors::Silver} });
	mVertices.emplace_back(Vertex{ Vector3{ 0.5f,-0.5f,-0.5f }, Color{Colors::Silver} });
	mVertices.emplace_back(Vertex{ Vector3{ 0.5f,-0.5f, 0.5f }, Color{Colors::Silver} });
	mVertices.emplace_back(Vertex{ Vector3{ 0.5f,-0.5f,0.5f }, Color{Colors::Silver} });
	mVertices.emplace_back(Vertex{ Vector3{ -0.5f,-0.5f,0.5f }, Color{Colors::Silver} });
	mVertices.emplace_back(Vertex{ Vector3{ -0.5f,-0.5f, -0.5f }, Color{Colors::Silver} });


	
	auto device = GraphicsSystem::Get()->GetDevice();

	// Create Constant buffer first
	D3D11_BUFFER_DESC constantBufferDesc{};
	constantBufferDesc.ByteWidth = sizeof(Matrix4) * 3;
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	HRESULT hr = device->CreateBuffer(&constantBufferDesc, nullptr, &mConstantBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create constant buffer");

	// Create vertex buffer
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = mVertices.size()*sizeof(Vertex);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0; // Use CPU
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = mVertices.data();

	hr = device->CreateBuffer(&bufferDesc, &initData,&mVertexBuffer);

	// Compile and create vertex shader
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	hr = D3DCompileFromFile(
		L"../../Assets/Shaders/DoTransform.fx",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"VS", "vs_5_0",
		shaderFlags, 0, &shaderBlob, &errorBlob);
	if (errorBlob && errorBlob->GetBufferPointer())
		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	ASSERT(SUCCEEDED(hr), "Failed to complie vertex buffer");

	hr = device->CreateVertexShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mVertexShader);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex shader");

	vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout;
	vertexLayout.push_back({ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 });
	vertexLayout.push_back({ "COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 });

	hr = device->CreateInputLayout(
		vertexLayout.data(),
		(UINT)vertexLayout.size(),
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		&mInputLayout);
	ASSERT(SUCCEEDED(hr), "Failed to create input layout");

	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);

	hr = D3DCompileFromFile(
		L"../../Assets/Shaders/DoTransform.fx",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PS", "ps_5_0",
		shaderFlags, 0, &shaderBlob, &errorBlob);
	if (errorBlob && errorBlob->GetBufferPointer())
		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	ASSERT(SUCCEEDED(hr), "Failed to complie pixel buffer");

	hr = device->CreatePixelShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mPixelShader);
	ASSERT(SUCCEEDED(hr), "Failed to create pixel shader");

	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
}

void GameState::Terminate()
{
	SafeRelease(mVertexBuffer);
	SafeRelease(mVertexShader);
	SafeRelease(mInputLayout);
	SafeRelease(mPixelShader);
	SafeRelease(mConstantBuffer);
}

void GameState::Update(float deltaTime)
{
	const float kMoveSpeed = 7.5f;
	const float kTurnSpeed = 0.5f;

	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::W))
	{
		mCamera.Walk(kMoveSpeed*deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::S))
	{
		mCamera.Walk(-kMoveSpeed * deltaTime);
	}
	mCamera.Yaw(inputSystem->GetMouseMoveX() * kTurnSpeed * deltaTime);
	mCamera.Pitch(inputSystem->GetMouseMoveY() * kTurnSpeed * deltaTime);

	if (inputSystem->IsKeyDown(KeyCode::A))
	{
		mRotation += deltaTime;
	}
	if (inputSystem->IsKeyDown(KeyCode::D))
	{
		mRotation -= deltaTime;
	}
}

void GameState::Render()
{
	auto context = GraphicsSystem::Get()->GetContext();
	
	auto matWorld = Matrix4::RotationY(mRotation);
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();
	auto matWVP = Transpose(matWorld * matView * matProj);

	context->UpdateSubresource(mConstantBuffer, 0, nullptr, &matWVP, 0, 0);
	context->VSSetConstantBuffers(0, 1, &mConstantBuffer);

	context->IASetInputLayout(mInputLayout);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	context->VSSetShader(mVertexShader, nullptr,0);
	context->PSSetShader(mPixelShader, nullptr, 0);

	context->Draw(mVertices.size(), 0);
}