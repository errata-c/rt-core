#pragma once
#include "BufferBase.hpp"

namespace rt {
	class DynamicBuffer;
	
	class CoherentBuffer : public BufferBase {
	public:
		static constexpr Flags mappingFlags = Flags::Coherent | Flags::Persistent;

		CoherentBuffer() = default;
		~CoherentBuffer() = default;

		CoherentBuffer(size_t length)
		{
			initArray(length);
		}

		CoherentBuffer(CoherentBuffer&& other) noexcept
			: BufferBase(std::move(other))
		{}

		CoherentBuffer& operator=(CoherentBuffer&& other) noexcept {
			BufferBase::operator=(std::move(other));
			return *this;
		}

		CoherentBuffer& operator=(ImmutableBuffer&& other) noexcept = delete;
		CoherentBuffer& operator=(MutableBuffer&& other) noexcept = delete;
		CoherentBuffer& operator=(DynamicBuffer&& other) noexcept = delete;
		CoherentBuffer(const CoherentBuffer&) = delete;
		CoherentBuffer& operator=(const CoherentBuffer&) = delete;

		template<typename T>
		CoherentBuffer(const T* data, size_t length)
		{
			initArray<T>(data, length);
		}

		template<typename T>
		CoherentBuffer(const std::vector<T>& data)
		{
			initArray<T>(data);
		}

		template<typename T>
		CoherentBuffer(const std::vector<T>& data, intptr_t startIndex, size_t length)
		{
			initArray<T>(data, startIndex, length);
		}

		// Dynamic substitution functions not available for this class.
		template<typename T>
		void subArray(const T* data, size_t length, intptr_t writeOffset, intptr_t readIndex) = delete;

		template<typename T>
		void subArray(const std::vector<T>& data, size_t length, intptr_t writeOffset, intptr_t readIndex) = delete;

		template<typename T>
		void subValue(const T& obj, intptr_t writeOffset) = delete;

		// Overloads of the mapping functions.
		template<typename T = void>
		T* mapRange(intptr_t offset, GLsizeiptr length, Flags flags) {
			assert(boundsCheckBytes(offset * sizeof(T), length * sizeof(T)));

			assert(checkFlags(flags));

			T* tmp = (T*)glMapNamedBufferRange(id, offset * sizeof(T), length * sizeof(T), (GLbitfield)flags);
			checkError();
			return tmp;
		}
		template<typename T = void>
		T* map(Flags flags) {
			assert(checkFlags(flags));
			T* tmp = (T*)glMapNamedBufferRange(id, 0, byteSize, (GLbitfield)flags);
			checkError();
			return tmp;
		}

		template<typename T = void>
		const T* mapRange(intptr_t offset, GLsizeiptr length, Flags flags) const {
			assert(boundsCheckBytes(offset * sizeof(T), length * sizeof(T)));

			assert(checkFlags(flags));

			T* tmp = (T*)glMapNamedBufferRange(id, offset * sizeof(T), length * sizeof(T), (GLbitfield)flags);
			checkError();
			return tmp;
		}
		template<typename T = void>
		const T* map(Flags flags) const {
			assert(checkFlags(flags));

			T* tmp = (T*)glMapNamedBufferRange(id, 0, byteSize, (GLbitfield)flags);
			checkError();
			return tmp;
		}

		template<typename T>
		T* map() {
			return BufferBase::map<T>(Flags::Write | Flags::Read | mappingFlags);
		}
		template<typename T>
		T* mapRange(intptr_t offset, uintptr_t length) {
			return BufferBase::mapRange<T>(offset, length, Flags::Write | Flags::Read | mappingFlags);
		}

		template<typename T>
		const T* mapRead() const {
			return BufferBase::map<T>(Flags::Read | mappingFlags);
		}
		template<typename T>
		const T* mapReadRange(intptr_t offset, uintptr_t length) const {
			return BufferBase::map<T>(offset, length, Flags::Read | mappingFlags);
		}

		template<typename T>
		T* mapWrite() {
			return BufferBase::map<T>(Flags::Write | mappingFlags);
		}
		template<typename T>
		T* mapWriteRange(intptr_t offset, uintptr_t length) {
			return BufferBase::map<T>(offset, length, Flags::Write | mappingFlags);
		}

		void initArray(size_t length) {
			BufferBase::initArray(length, Flags::Write | Flags::Read | mappingFlags);
		}
		void initArray(size_t length, Flags flags) = delete;

		template<typename T>
		void initArray(const T* data, size_t length) {
			BufferBase::initArray(data, length, Flags::Write | Flags::Read | mappingFlags);
		}
		template<typename T>
		void initArray(const T* data, size_t length, Flags flags) = delete;

		template<typename T>
		void initValue(const T& obj) {
			initValue(obj, Flags::Write | Flags::Read | mappingFlags);
		}
		template<typename T>
		void initValue(const T& obj, Flags flags) = delete;

		template<typename T>
		void initArray(const std::vector<T>& data) {
			BufferBase::initArray(data, Flags::Write | Flags::Read | mappingFlags);
		}
		template<typename T>
		void initArray(const std::vector<T>& data, Flags flags) = delete;

		template<typename T>
		void initArray(const std::vector<T>& data, intptr_t startIndex, size_t length) {
			BufferBase::initArray(data, startIndex, length, Flags::Write | Flags::Read | mappingFlags);
		}
		template<typename T>
		void initArray(const std::vector<T>& data, intptr_t startIndex, size_t length, Flags flags) = delete;

		// Check that the flags are consistent with this type of buffer
		static constexpr bool checkFlags(Flags flags) {
			return (flags & (Flags::Write | Flags::Read | mappingFlags)) != Flags::None;
		}
	};

};