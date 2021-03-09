#pragma once
#include "BufferBase.hpp"

namespace rt {
	class MutableBuffer;

	class ImmutableBuffer : public Buffer {
	public:
		ImmutableBuffer(Inits f = Inits::None)
		{
			assert(isValidInitializer(f));
			setFlags(f);
		}

		ImmutableBuffer(size_t length, Inits f = Inits::None)
		{
			initArray(length, f);
		}

		~ImmutableBuffer() = default;

		ImmutableBuffer(ImmutableBuffer&& other) noexcept
			: Buffer(std::move(other))
			, validFlags(other.validFlags)
		{}
		ImmutableBuffer(Buffer&& other) noexcept
			: Buffer(std::move(other))
			, validFlags(Flags::None)
		{
			assert(other.isImmutable());
			glGetNamedBufferParameteriv(id, GL_BUFFER_STORAGE_FLAGS, reinterpret_cast<GLint*>(&validFlags));
			checkError();
		}

		ImmutableBuffer& operator=(ImmutableBuffer&& other) noexcept {
			Buffer::operator=(std::move(other));
			validFlags = other.validFlags;
			return *this;
		}
		ImmutableBuffer& operator=(Buffer&& other) noexcept {
			assert(other.isImmutable());
			Buffer::operator=(std::move(other));
			glGetNamedBufferParameteriv(id, GL_BUFFER_STORAGE_FLAGS, reinterpret_cast<GLint*>(&validFlags));
			checkError();
			return *this;
		}

		ImmutableBuffer& operator=(MutableBuffer&& other) noexcept = delete;

		ImmutableBuffer(const ImmutableBuffer&) = delete;
		ImmutableBuffer& operator=(const ImmutableBuffer&) = delete;

		template<typename T>
		ImmutableBuffer(const T* data, size_t length, Inits f = Inits::None)
		{
			initArray(data, length, f);
		}

		template<typename T>
		ImmutableBuffer(const T& obj, Inits f = Inits::None)
		{
			initValue<T>(obj, f);
		}

		// Returns a pointer representing the data contents of the buffer, cast to the template type.
		// Returns nullptr if the mapping fails.
		template<typename T = uint8_t>
		T* mapRange(intptr_t offset, GLsizeiptr length, Flags flags) {
			assert(checkFlags(flags)); // If you get this assertion, you have attempted a mapping that is not allowed on the current buffer.
			return Buffer::mapRange(offset, length, flags);
		}
		
		// Returns a pointer representing the data contents of the buffer, cast to the template type.
		// Returns nullptr if the mapping fails.
		template<typename T = uint8_t>
		T* map(Flags flags) {
			assert(checkFlags(flags)); // If you get this assertion, you have attempted a mapping that is not allowed on the current buffer.
			return Buffer::map<T>(flags);
		}

		// Returns a pointer representing the data contents of the buffer, cast to the template type.
		// Returns nullptr if the mapping fails.
		template<typename T = uint8_t>
		const T* mapRange(intptr_t offset, GLsizeiptr length, Flags flags) const {
			assert(checkFlags(flags)); // If you get this assertion, you have attempted a mapping that is not allowed on the current buffer.
			return Buffer::mapRange<T>(offset, length, flags);
		}

		// Returns a pointer representing the data contents of the buffer, cast to the template type.
		// Returns nullptr if the mapping fails.
		template<typename T = uint8_t>
		const T* map(Flags flags) const {
			assert(checkFlags(flags)); // If you get this assertion, you have attempted a mapping that is not allowed on the current buffer.
			return Buffer::map<T>(flags);
		}

		// Initialize the array to the input length in bytes, set the base flags.
		void initArray(size_t length, Inits f) {
			Buffer::initArray(length, f);
			setFlags(f);
		}

		// Initialize the array to the input c-data, set the base flags.
		template<typename T>
		void initArray(const T* data, size_t length, Inits f) {
			Buffer::initArray(data, length, f);
			setFlags(f);
		}

		// Initialize the buffer to the value of the object input, set the base flags.
		// Note that this is a direct copy of the object's underlying data, only use this if you know the object is standard layout.
		template<typename T>
		void initValue(const T& obj, Inits f) {
			Buffer::initValue(obj, f);
			setFlags(f);
		}
	private:
		Flags validFlags;

		static constexpr Flags Mask = 
			Flags{ Flag::Read } |
			Flags{ Flag::Write } |
			Flags{ Flag::Coherent } |
			Flags{ Flag::Persistent } |
			Flags{ Flag::Unsyncronized };

		void setFlags(Inits inits) noexcept {
			validFlags = Flags::fromRawValue(inits.rawValue()) & Mask;
		}
		bool checkFlags(Flags flags) const noexcept {
			// Coherent requires persistent
			if (flags.contains(Flag::Coherent) && !flags.contains(Flag::Persistent)) {
				return false;
			}
			Flags invalidFlags = ~validFlags;
			Flags checkable = flags & Mask;
			if (checkable.anyOf(invalidFlags)) {
				return false;
			}
			return true;
		}
	};
};