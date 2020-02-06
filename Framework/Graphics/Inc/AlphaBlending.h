#pragma once

namespace JimmyGod::Graphics
{
	class AlphaBlending
	{
	public:
		void Initialize();
		void Terminate();

		void Bind() const;
		void UnBind() const;

	private:
		ID3D11BlendState* mBlendState = nullptr;
	};
}