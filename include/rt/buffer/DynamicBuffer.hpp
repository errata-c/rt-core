#pragma once
#include "BufferBase.hpp"

namespace rt {

	class DynamicBuffer : public BufferBase {
	public:
		DynamicBuffer() = default;
		~DynamicBuffer() = default;

		DynamicBuffer(size_t length)
		{
			initArray(length);
		}

		DynamicBuffer(DynamicBuffer&& other) noexcept
			: BufferBase(std::move(other))
		{}

		DynamicBuffer& operator=(DynamicBuffer&& other) noexcept {
			BufferBase::operator=(std::move(other));
			return *this;
		}

		DynamicBuffer& operator=(ImmutableBuffer&& other) noexcept = delete;
		DynamicBuffer& operator=(MutableBuffer&& other) noexcept = delete;
		DynamicBuffer& operator=(CoherentBuffer&& other) noexcept = delete;
		DynamicBuffer(const DynamicBuffer&) = delete;
		DynamicBuffer& operator=(const DynamicBuffer&) = delete;

		template<typename T>
		DynamicBuffer(const T* data, size_t length)
		{
			initArray(data, length);
		}

		template<typename T>
		DynamicBuffer(const std::vector<T>& data)
		{
			initArray<T>(data);
		}

		template<typename T>
		DynamicBuffer(const std::vector<T>& data, intptr_t startIndex, size_t length)
		{
			initArray<T>(data, startIndex, length);
		}

		template<typename T>
		T* map() {
			return BufferBase::map<T>(Flags::Read | Flags::Write);
		}
		template<typename T>
		T* mapRange(intptr_t offset, uintptr_t length) {
			return BufferBase::mapRange<T>(offset, length, Flags::Read | Flags::Write);
		}

		template<typename T>
		const T* mapRead() const {
			return BufferBase::map<T>(Flags::Read);
		}
		template<typename T>
		const T* mapReadRange(intptr_t offset, uintptr_t length) const {
			return BufferBase::map<T>(offset, length, Flags::Read);
		}

		template<typename T>
		T* mapWrite() {
			return BufferBase::map<T>(Flags::Write);
		}
		template<typename T>
		T* mapWriteRange(intptr_t offset, uintptr_t length) {
			return BufferBase::map<T>(offset, length, Flags::Write);
		}

		void initArray(size_t length) {
			BufferBase::initArray(length, Flags::Read | Flags::Write | Flags::Dynamic);
		}
		void initArray(size_t length, Flags flags) = delete;

		template<typename T>
		void initArray(const T* data, size_t length) {
			BufferBase::initArray(data, length, Flags::Read | Flags::Write | Flags::Dynamic);
		}
		template<typename T>
		void initArray(const T* data, size_t length, Flags flags) = delete;

		template<typename T>
		void initValue(const T& obj) {
			initValue(obj, Flags::Read | Flags::Write | Flags::Dynamic);
		}
		template<typename T>
		void initValue(const T& obj, Flags flags) = delete;

		template<typename T>
		void initArray(const std::vector<T>& data) {
			BufferBase::initArray(data, Flags::Read | Flags::Write | Flags::Dynamic);
		}
		template<typename T>
		void initArray(const std::vector<T>& data, Flags flags) = delete;

		template<typename T>
		void initArray(const std::vector<T>& data, intptr_t startIndex, size_t length) {
			BufferBase::initArray(data, startIndex, length, Flags::Read | Flags::Write | Flags::Dynamic);
		}
		template<typename T>
		void initArray(const std::vector<T>& data, intptr_t startIndex, size_t length, Flags flags) = delete;
	};
};