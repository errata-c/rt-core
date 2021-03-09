#pragma once
#include "BufferBase.hpp"

namespace rt {

	class MutableBuffer : public Buffer {
	public:
		MutableBuffer(MutableType nType)
			: type(nType)
		{}
		~MutableBuffer() = default;

		MutableBuffer(const MutableBuffer&) = delete;
		MutableBuffer& operator=(const MutableBuffer&) = delete;

		void initArray(size_t length, Flags flags) = delete;

		template<typename T>
		void initArray(const T* data, size_t length, Flags flags) = delete;

		template<typename T>
		void initValue(const T& obj, Flags flags) = delete;

		template<typename T>
		void initArray(const std::vector<T>& data, Flags flags) = delete;

		template<typename T>
		void initArray(const std::vector<T>& data, intptr_t startIndex, size_t length, Flags flags) = delete;

		template<typename T>
		void resizeValue(const T& obj) {
			assert(isValid());
			this->byteSize = sizeof(T);
			glNamedBufferSubData(this->id, sizeBytes(), reinterpret_cast<const void*>(&obj), static_cast<GLenum>(type));
			checkError();
		}
		template<typename T>
		void resizeValue(MutableType t, const T& obj) {
			type = t;
			resizeValue(obj);
		}

		void resizeArray(size_t ns) {
			assert(isValid());
			this->byteSize = ns;
			glNamedBufferData(this->id, sizeBytes(), 0, static_cast<GLuint>(type));
			checkError();
		}
		template<typename T>
		void resizeArray(MutableType t, size_t ns) {
			type = t;
			resizeArray(ns);
		}

		template<typename T>
		void resizeArray(const std::vector<T>& data) {
			assert(isValid());
			this->byteSize = sizeof(T) * data.size();
			glNamedBufferData(this->id, sizeBytes(), reinterpret_cast<const void*>(&data.data()), static_cast<GLenum>(type));
			checkError();
		}
		template<typename T>
		void resizeArray(MutableType t, const std::vector<T>& data) {
			type = t;
			resizeArray(data);
		}

		template<typename T>
		void resizeArray(const std::vector<T>& data, size_t length, intptr_t readIndex = 0) {
			assert(isValid());
			assert(readIndex + length <= data.size());

			this->byteSize = sizeof(T) * length;
			glNamedBufferData(this->id, sizeBytes(), reinterpret_cast<const void*>(&data[readIndex]), static_cast<GLenum>(type));
			checkError();
		}
		template<typename T>
		void resizeArray(MutableType t, const std::vector<T>& data, size_t length, intptr_t readIndex) {
			type = t;
			resizeArray(data, length, readIndex);
		}

		template<typename T>
		void resizeArray(const T* data, size_t length, intptr_t readIndex = 0) {
			assert(isValid());
			this->byteSize = sizeof(T) * length;
			glNamedBufferData(this->id, sizeBytes(), reinterpret_cast<const void*>(&data[readIndex]), static_cast<GLenum>(type));
			checkError();
		}
		template<typename T>
		void resizeArray(MutableType t, const T* data, size_t length, intptr_t readIndex) {
			type = t;
			resizeArray(data, length, readIndex);
		}

		void changeMutableType(MutableType nType) {
			if (isValid()) {
				type = nType;
				GLuint tmpId;
				glCreateBuffers(1, &tmpId); checkError();

				glNamedBufferData(tmpId, sizeBytes(), 0, static_cast<GLuint>(type)); checkError();

				glCopyNamedBufferSubData(this->id, tmpId, 0, 0, sizeBytes()); checkError();
				glDeleteBuffers(1, &this->id); checkError();

				this->id = tmpId;
			}
			else {
				type = nType;
			}
		}

		// Overload
		MutableType getMutableType() const {
			return type;
		}

		bool isImmutable() const {
			return false;
		}
	private:
		MutableType type;
	};
};