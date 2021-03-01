#pragma once
#include "BufferBase.hpp"

namespace rt {
	class MutableBuffer;

	class ImmutableBuffer : public BufferBase {
	public:
		ImmutableBuffer(Flags f = Flags::None)
			: flags(f)
		{
			assert(isValidInitializer(flags));
		}

		ImmutableBuffer(size_t length, Flags f)
			: flags(f)
		{
			assert(isValidInitializer(flags));

			initArray(length, flags);
		}

		~ImmutableBuffer() = default;

		ImmutableBuffer(ImmutableBuffer&& other) noexcept
			: BufferBase(std::move(other))
			, flags(other.flags)
		{}
		ImmutableBuffer(BufferBase&& other) noexcept
			: BufferBase(std::move(other))
			, flags(Flags::None)
		{
			glGetNamedBufferParameteriv(id, GL_BUFFER_STORAGE_FLAGS, reinterpret_cast<GLint*>(&flags));
			checkError();
		}

		ImmutableBuffer& operator=(ImmutableBuffer&& other) noexcept {
			BufferBase::operator=(std::move(other));
			flags = other.flags;
			return *this;
		}
		ImmutableBuffer& operator=(BufferBase&& other) noexcept {
			BufferBase::operator=(std::move(other));
			glGetNamedBufferParameteriv(id, GL_BUFFER_STORAGE_FLAGS, reinterpret_cast<GLint*>(&flags));
			checkError();
			return *this;
		}

		ImmutableBuffer& operator=(MutableBuffer&& other) noexcept = delete;

		ImmutableBuffer(const ImmutableBuffer&) = delete;
		ImmutableBuffer& operator=(const ImmutableBuffer&) = delete;

		template<typename T>
		ImmutableBuffer(const T* data, size_t length, GLbitfield f)
		{
			initArray(data, length, f);
		}

		template<typename T>
		ImmutableBuffer(const T& obj, GLbitfield f)
		{
			initArray<T>(obj, f);
		}

		template<typename T>
		ImmutableBuffer(const std::vector<T>& data, GLbitfield f)
		{
			initArray<T>(data, f);
		}

		template<typename T>
		ImmutableBuffer(const std::vector<T>& data, intptr_t startIndex, size_t length, Flags f)
		{
			initArray<T>(data, startIndex, length, f);
		}

		// Returns a pointer representing the data contents of the buffer, cast to the template type.
		// Returns nullptr if the mapping fails.
		template<typename T = void>
		T* mapRange(intptr_t offset, GLsizeiptr length, Flags f) {
			assert(checkFlags(flags)); // If you get this assertion, you have attempted a mapping that is not allowed on the current buffer.
			T* tmp = (T*)glMapNamedBufferRange(id, offset * sizeof(T), length * sizeof(T), static_cast<GLbitfield>(flags));
			checkError();
			return tmp;
		}
		
		// Returns a pointer representing the data contents of the buffer, cast to the template type.
		// Returns nullptr if the mapping fails.
		template<typename T = void>
		T* map(Flags f) {
			assert(checkFlags(flags)); // If you get this assertion, you have attempted a mapping that is not allowed on the current buffer.
			T* tmp = (T*)glMapNamedBufferRange(id, 0, byteSize, static_cast<GLbitfield>(flags));
			checkError();
			return tmp;
		}

		// Returns a pointer representing the data contents of the buffer, cast to the template type.
		// Returns nullptr if the mapping fails.
		template<typename T = void>
		const T* mapRange(intptr_t offset, GLsizeiptr length, Flags flags) const {
			assert(checkFlags(flags)); // If you get this assertion, you have attempted a mapping that is not allowed on the current buffer.
			assert(boundsCheckBytes(offset * sizeof(T), length * sizeof(T)));
			T* tmp = (T*)glMapNamedBufferRange(id, offset * sizeof(T), length * sizeof(T), static_cast<GLbitfield>(flags));
			checkError();
			return tmp;
		}

		// Returns a pointer representing the data contents of the buffer, cast to the template type.
		// Returns nullptr if the mapping fails.
		template<typename T = void>
		const T* map(Flags flags) const {
			assert(checkFlags(flags)); // If you get this assertion, you have attempted a mapping that is not allowed on the current buffer.
			T* tmp = (T*)glMapNamedBufferRange(id, 0, byteSize, static_cast<GLbitfield>(flags));
			checkError();
			return tmp;
		}

		// Attempts to unmap the buffer, returns true if successful.
		bool unmap() {
			bool val = static_cast<bool>(glUnmapNamedBuffer(id));
			checkError();
			return val;
		}

		// Initialize the array to the input length in bytes, set the base flags.
		void initArray(size_t length, Flags f) {
			this->flags = f;
			BufferBase::initArray(length, f);
		}

		// Initialize the array to the input c-data, set the base flags.
		template<typename T>
		void initArray(const T* data, size_t length, Flags f) {
			this->flags = f;
			BufferBase::initArray(data, length, f);
		}

		// Initialize the buffer to the value of the object input, set the base flags.
		// Note that this is a direct copy of the object's underlying data, only use this if you know the object is standard layout.
		template<typename T>
		void initValue(const T& obj, Flags f) {
			this->flags = f;
			BufferBase::initValue(obj, f);
		}

		// Initialize the buffer to the contents of the input vector. Vector is supported because it has contiguous storage. 
		template<typename T>
		void initArray(const std::vector<T>& data, Flags f) {
			this->flags = f;
			BufferBase::initArray(data, f);
		}

		// Initialize the buffer to a slice of the input vector. Vector is supported because it has contiguous storage. 
		template<typename T>
		void initArray(const std::vector<T>& data, intptr_t startIndex, size_t length, Flags f) {
			this->flags = f;
			BufferBase::initArray(data, startIndex, length, f);
		}

		static bool isValidMapping(Flags flags) {
			// Read and invalidate is not allowed.
			if (allOf(flags, Flags::Read) && anyOf(flags, Flags::InvalidateBuffer | Flags::InvalidateRange)) {
				return false;
			}

			// Flush requires Write.
			if (allOf(flags, Flags::FlushExplicit) && !allOf(flags, Flags::Write)) {
				return false;
			}

			return true;
		}
	private:
		Flags flags;

		bool checkFlags(Flags mapFlags) const {
			if (!isValidMapping(mapFlags)) {
				return false;
			}

			// if a mapping flag is not allowed by the initializer flags:
			if (!allOf(mapFlags, flags)) {
				return false;
			}

			return true;
		}
	};
};