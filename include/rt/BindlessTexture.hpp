#pragma once
#include <cinttypes>

#include "Core.hpp"
#include "Texture.hpp"
#include "Sampler.hpp"

namespace rt {
	class TextureBase;

	enum class BindlessType {
		Tex1d = GL_TEXTURE_1D,
		Tex2d = GL_TEXTURE_2D,
		Tex3d = GL_TEXTURE_3D,
		Tex2dArray = GL_TEXTURE_2D_ARRAY,
		Tex1dArray = GL_TEXTURE_1D_ARRAY,
	};

	class BindlessTexture {
	public:
		static constexpr BindlessType
			Tex1d = BindlessType::Tex1d,
			Tex2d = BindlessType::Tex2d,
			Tex3d = BindlessType::Tex3d,
			Tex1dArray = BindlessType::Tex1dArray,
			Tex2dArray = BindlessType::Tex2dArray;

		BindlessTexture()
			: resident(false)
			, handle(0)
		{}

		BindlessTexture(const TextureBase & tex)
			: resident(false)
			, handle(0)
		{
			init(tex);
		}

		BindlessTexture(const TextureBase & tex, const Sampler & samp)
			: resident(false)
			, handle(0)
		{
			init(tex, samp);
		}

		BindlessTexture(BindlessTexture&& other) noexcept
			: resident(other.resident)
			, handle(other.handle)
		{
			other.resident = false;
			other.handle = 0;
		}

		~BindlessTexture() {
			makeNonResident();
		}

		BindlessTexture(const BindlessTexture&) = delete;
		BindlessTexture& operator=(const BindlessTexture&) = delete;

		bool init(const TextureBase & tex) {
			if (isValid()) {
				return false;
			}

			handle = glGetTextureHandleARB(tex.getId());
			checkError();

			return true;
		}

		bool init(const TextureBase & tex, const Sampler & samp) {
			if (isValid()) {
				return false;
			}

			handle = glGetTextureSamplerHandleARB(tex.getId(), samp.getId());
			checkError();

			return true;
		}

		bool makeResident() {
			assert(isValid());

			if (!resident) {
				glMakeTextureHandleResidentARB(handle); 
				checkError();

				return true;
			}
			return false;
		}

		bool makeNonResident() {
			assert(isValid());

			if (resident) {
				glMakeTextureHandleNonResidentARB(handle); 
				checkError();

				return true;
			}
			return false;
		}

		bool isResident() const {
			return resident;
		}

		bool isValid() const {
			return handle != 0;
		}

		void reset() {
			makeNonResident();
			// Handles are automatically destroyed when the resources they reference are destroyed.
			// So long as we make sure we're not resident we can just set the handle to zero.
			handle = 0;
			resident = false;
		}

		uint64_t getHandle() const {
			return handle;
		}
	private:
		bool resident;
		uint64_t handle;
	};
}