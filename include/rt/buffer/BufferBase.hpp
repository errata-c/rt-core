#pragma once
#include <rt/Core.hpp>
#include <vector>
#include <cassert>
#include <cinttypes>

namespace rt {
	enum class MutableType : GLenum {
		StaticDraw = GL_STATIC_DRAW,
		DynamicDraw = GL_DYNAMIC_DRAW,
		StreamDraw = GL_STREAM_DRAW,

		StaticRead = GL_STATIC_READ,
		DynamicRead = GL_DYNAMIC_READ,
		StreamRead = GL_STREAM_READ,

		StaticCopy = GL_STATIC_COPY,
		DynamicCopy = GL_DYNAMIC_COPY,
		StreamCopy = GL_STREAM_COPY,
	};

	enum class BufferFlags : GLbitfield {
		None = 0,

		// This allows the buffer to have data substituted with glBufferSubData
		Dynamic = GL_DYNAMIC_STORAGE_BIT,

		// When all other criteria for the buffer storage allocation are met, 
		// this bit may be used by an implementation to determine whether to use storage that is local to the server or
		// to the client to serve as the backing store for the buffer.
		// Note that this can cause GL_INVALID_VALUE error when mapping on some implementations.
		Client = GL_CLIENT_STORAGE_BIT,

		// Indicates that the mapping is intended to be read from. Reading without this flag is undefined behavior.
		Read = GL_MAP_READ_BIT,

		// Indicates that the mapping is intended to be written to. Writing without this flag is undefined behavior.
		Write = GL_MAP_WRITE_BIT,

		// Allows the buffer to mapped, and have render functions called while its mapped. Must be initialized with this flag to map with it.
		Persistent = GL_MAP_PERSISTENT_BIT,

		// Tells opengl to make reads and writes from the buffer immediate, instead of explicitly synchronized.
		Coherent = GL_MAP_COHERENT_BIT,

		// OpenGL will not syncronize, this can lead to undefined behavior if not used carefully.
		Unsyncronized = GL_MAP_UNSYNCHRONIZED_BIT,

		// Invalidated the entire buffer upon mapping, cannot be used with Read.
		InvalidateBuffer = GL_MAP_INVALIDATE_BUFFER_BIT,

		// Invalidates the mapped range, Cannot be used with Read.
		InvalidateRange = GL_MAP_INVALIDATE_RANGE_BIT,

		// When this flag is set, modifications to the buffer must be explicitly flushed.
		FlushExplicit = GL_MAP_FLUSH_EXPLICIT_BIT,

		All = FlushExplicit | Dynamic | Client | Read | Write | Persistent | Coherent | Unsyncronized | InvalidateBuffer | InvalidateRange,

		ValidInitializers = Read | Write | Persistent | Coherent | Unsyncronized | Dynamic | Client,

		ValidMapping = Read | Write | Persistent | Coherent | InvalidateBuffer | InvalidateRange | FlushExplicit | Unsyncronized,
	};

	static bool allOf(BufferFlags lh, BufferFlags rh) {
		return (static_cast<GLbitfield>(lh) & static_cast<GLbitfield>(rh)) == static_cast<GLbitfield>(rh);
	}
	static bool anyOf(BufferFlags lh, BufferFlags rh) {
		return (static_cast<GLbitfield>(lh) & static_cast<GLbitfield>(rh)) != 0;
	}
};

inline constexpr rt::BufferFlags operator|(rt::BufferFlags lh, rt::BufferFlags rh) {
	GLbitfield tmp = static_cast<GLbitfield>(lh) | static_cast<GLbitfield>(rh);
	return static_cast<rt::BufferFlags>(tmp);
}
inline constexpr rt::BufferFlags operator&(rt::BufferFlags lh, rt::BufferFlags rh) {
	GLbitfield tmp = static_cast<GLbitfield>(lh) & static_cast<GLbitfield>(rh);
	return static_cast<rt::BufferFlags>(tmp);
}
inline constexpr rt::BufferFlags operator^(rt::BufferFlags lh, rt::BufferFlags rh) {
	GLbitfield tmp = static_cast<GLbitfield>(lh) ^ static_cast<GLbitfield>(rh);
	return static_cast<rt::BufferFlags>(tmp);
}
inline constexpr rt::BufferFlags operator~(rt::BufferFlags flags) {
	GLbitfield tmp = ~static_cast<GLbitfield>(flags);
	tmp &= static_cast<GLbitfield>(rt::BufferFlags::All);
	return static_cast<rt::BufferFlags>(tmp);
}
inline rt::BufferFlags& operator|=(rt::BufferFlags& lh, rt::BufferFlags rh) {
	GLbitfield tmp = static_cast<GLbitfield>(lh) | static_cast<GLbitfield>(rh);
	lh = static_cast<rt::BufferFlags>(tmp);
	return lh;
}
inline rt::BufferFlags& operator&=(rt::BufferFlags& lh, rt::BufferFlags rh) {
	GLbitfield tmp = static_cast<GLbitfield>(lh) & static_cast<GLbitfield>(rh);
	lh = static_cast<rt::BufferFlags>(tmp);
	return lh;
}
inline rt::BufferFlags& operator^=(rt::BufferFlags& lh, rt::BufferFlags rh) {
	GLbitfield tmp = static_cast<GLbitfield>(lh) ^ static_cast<GLbitfield>(rh);
	lh = static_cast<rt::BufferFlags>(tmp);
	return lh;
}

namespace rt {

	class BufferBase {
	public:
		using Flags = BufferFlags;

		BufferBase()
			: id(0)
			, byteSize(0)
		{
			glCreateBuffers(1, &id);
			assert(isValid());
		}
		~BufferBase() {
			if (isValid()) {
				glDeleteBuffers(1, &id);
				checkError();
			}
		}
		BufferBase(BufferBase&& other) noexcept
			: id(other.id)
			, byteSize(other.byteSize)
		{
			other.id = 0;
		}

		BufferBase& operator=(BufferBase&& other) noexcept {
			if (isValid()) {
				glDeleteBuffers(1, &id);
				checkError();
			}

			id = other.id;
			byteSize = other.byteSize;

			other.id = 0;
			other.byteSize = 0;

			return *this;
		}

		BufferBase(const BufferBase&) = delete;
		BufferBase& operator=(const BufferBase&) = delete;

		// Value manipulation
		void reset() {
			if (isValid()) {
				glDeleteBuffers(1, &id);
				checkError();
				glCreateBuffers(1, &id);
				checkError();
				byteSize = 0;
			}
		}

		void initArray(size_t length, Flags flags) {
			assert(!isInitialized());
			assert(length > 0);
			assert(isValidInitializer(flags));

			this->byteSize = length;
			glNamedBufferStorage(this->id, this->byteSize, (const void*)0, static_cast<GLbitfield>(flags));
			checkError();
		}
		template<typename T>
		void initArray(const T* data, size_t length, Flags flags) {
			assert(!isInitialized());
			assert(length > 0);
			assert(isValidInitializer(flags));

			this->byteSize = length * sizeof(T);
			glNamedBufferStorage(this->id, this->byteSize, (const void*)data, static_cast<GLbitfield>(flags));
			checkError();
		}
		template<typename T>
		void initValue(const T& obj, Flags flags) {
			assert(!isInitialized());
			assert(isValidInitializer(flags));

			this->byteSize = sizeof(T);
			glNamedBufferStorage(this->id, this->byteSize, reinterpret_cast<const void*>(&obj), static_cast<GLbitfield>(flags));
			checkError();
		}

		template<typename T>
		void subArray(const T* data, size_t length, intptr_t writeOffset = 0) {
			assert(isValid());
			assert(boundsCheckBytes(writeOffset, length * sizeof(T)));
			glNamedBufferSubData(id, writeOffset, length * sizeof(T), reinterpret_cast<const void*>(data));
			checkError();
		}

		template<typename T>
		void subValue(const T& obj, intptr_t writeOffset = 0) {
			assert(isValid());
			assert(boundsCheckBytes(writeOffset, sizeof(T)));
			glNamedBufferSubData(id, writeOffset, sizeof(T), reinterpret_cast<const void*>(&obj));
			checkError();
		}

		void invalidate() {
			assert(isValid());
			glInvalidateBufferData(id);
			checkError();
		}
		void invalidate(intptr_t startIndex, uintptr_t length) {
			assert(isValid());
			assert(boundsCheckBytes(startIndex, length));
			glInvalidateBufferSubData(id, startIndex, length);
			checkError();
		}

		void clearTo(GLfloat value) {
			assert(isValid());
			glClearNamedBufferData(id, GL_R32F, GL_RED, GL_FLOAT, &value);
		}
		void clearTo(GLint value) {
			assert(isValid());
			glClearNamedBufferData(id, GL_R32I, GL_RED, GL_INT, &value);
			checkError();
		}
		void clearTo(GLuint value) {
			assert(isValid());
			glClearNamedBufferData(id, GL_R32UI, GL_RED, GL_UNSIGNED_INT, &value);
			checkError();
		}
		void clearTo(GLshort value) {
			assert(isValid());
			glClearNamedBufferData(id, GL_R16I, GL_RED, GL_SHORT, &value);
			checkError();
		}
		void clearTo(GLushort value) {
			assert(isValid());
			glClearNamedBufferData(id, GL_R16UI, GL_RED, GL_UNSIGNED_SHORT, &value);
			checkError();
		}
		void clearTo(GLbyte value) {
			assert(isValid());
			glClearNamedBufferData(id, GL_R8I, GL_RED, GL_BYTE, &value);
			checkError();
		}
		void clearTo(GLubyte value) {
			assert(isValid());
			glClearNamedBufferData(id, GL_R8UI, GL_RED, GL_UNSIGNED_BYTE, &value);
			checkError();
		}

		void copyTo(const BufferBase& buff) const {
			assert(isValid());
			assert(buff.sizeBytes() >= sizeBytes());

			glCopyNamedBufferSubData(id, buff.getId(), 0, 0, byteSize);
			checkError();
		}
		void copyTo(const BufferBase& buff, size_t length, intptr_t readOffset = 0, intptr_t writeOffset = 0) const {
			assert(isValid());
			assert(boundsCheckBytes(readOffset, length));
			assert(buff.boundsCheckBytes(writeOffset, length));

			glCopyNamedBufferSubData(id, buff.getId(), readOffset, writeOffset, length);
			checkError();
		}

		// Binding ---
		void bindUBO(GLuint index) {
			assert(isValid());
			glBindBufferBase(GL_UNIFORM_BUFFER, index, id);
			checkError();
		}
		void bindUBO(GLuint index, intptr_t startIndex, size_t length) {
			assert(isValid());
			glBindBufferRange(GL_UNIFORM_BUFFER, index, id, startIndex, length);
			checkError();
		}
		void bindSSBO(GLuint index) {
			assert(isValid());
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, id);
			checkError();
		}
		void bindSSBO(GLuint index, intptr_t startIndex, size_t length) {
			assert(isValid());
			glBindBufferRange(GL_SHADER_STORAGE_BUFFER, index, id, startIndex, length);
			checkError();
		}

		// Mapping ---
		template<typename T = void>
		T* mapRange(intptr_t offset, GLsizeiptr length, Flags flags) {
			assert(isValidMapping(flags));
			assert(boundsCheckBytes(offset * sizeof(T), length * sizeof(T)));
			T* tmp = (T*)glMapNamedBufferRange(id, offset * sizeof(T), length * sizeof(T), static_cast<GLbitfield>(flags));
			checkError();
			return tmp;
		}
		template<typename T = void>
		T* map(Flags flags) {
			assert(isValidMapping(flags));
			T* tmp = (T*)glMapNamedBufferRange(id, 0, byteSize, static_cast<GLbitfield>(flags));
			checkError();
			return tmp;
		}

		template<typename T = void>
		const T* mapRange(intptr_t offset, GLsizeiptr length, Flags flags) const {
			assert(isValidMapping(flags));
			assert(boundsCheckBytes(offset * sizeof(T), length * sizeof(T)));
			T* tmp = (T*)glMapNamedBufferRange(id, offset * sizeof(T), length * sizeof(T), static_cast<GLbitfield>(flags));
			checkError();
			return tmp;
		}

		template<typename T = void>
		const T* map(Flags flags) const {
			assert(isValidMapping(flags));
			T* tmp = (T*)glMapNamedBufferRange(id, 0, byteSize, static_cast<GLbitfield>(flags));
			checkError();
			return tmp;
		}

		bool unmap() {
			bool res = static_cast<bool>(glUnmapNamedBuffer(id));
			checkError();
			return res;
		}

		void flush() {
			glFlushMappedNamedBufferRange(id, 0, byteSize);
			checkError();
		}

		void flushRange(intptr_t offset, GLsizeiptr length) {
			glFlushMappedNamedBufferRange(id, offset, length);
			checkError();
		}

		template<typename T>
		void flushRange(intptr_t offset, GLsizeiptr length) {
			assert(boundsCheckBytes(offset * sizeof(T), length * sizeof(T)));
			glFlushMappedNamedBufferRange(id, offset * sizeof(T), length * sizeof(T));
			checkError();
		}

		// Mutable methods ---
		template<typename T>
		void resizeValue(MutableType type, const T& obj) {
			assert(isValid());
			this->byteSize = sizeof(T);
			glNamedBufferSubData(this->id, sizeBytes(), reinterpret_cast<const void*>(&obj), static_cast<GLenum>(type));
			checkError();
		}

		void resizeArray(MutableType type, size_t ns) {
			assert(isValid());
			this->byteSize = ns;
			glNamedBufferData(this->id, sizeBytes(), 0, static_cast<GLuint>(type));
			checkError();
		}
		template<typename T>
		void resizeArray(MutableType type, const T* data, size_t length) {
			assert(isValid());
			this->byteSize = sizeof(T) * length;
			glNamedBufferData(this->id, sizeBytes(), reinterpret_cast<const void*>(data), static_cast<GLenum>(type));
			checkError();
		}

		void changeMutableType(MutableType nType) {
			if (isValid()) {
				GLuint tmpId;
				glCreateBuffers(1, &tmpId);
				assert(tmpId != 0);

				glNamedBufferData(tmpId, sizeBytes(), 0, static_cast<GLuint>(nType));
				checkError();

				glCopyNamedBufferSubData(this->id, tmpId, 0, 0, sizeBytes());
				checkError();

				glDeleteBuffers(1, &this->id);
				checkError();

				this->id = tmpId;
			}
		}


		// Getters ---
		template<typename T>
		std::vector<T> getData(size_t length, intptr_t readOffset) const {
			assert(isValid());
			assert(boundsCheckBytes(readOffset, length * sizeof(T)));

			std::vector<T> ret;
			ret.resize(sizeBytes());

			glGetNamedBufferSubData(id, readOffset, length * sizeof(T), static_cast<void*>(ret.data()));
			checkError();

			return ret;
		}

		template<typename T = void>
		void getData(T * data, size_t length, intptr_t readOffset) const {
			assert(isValid());
			assert(boundsCheckBytes(readOffset, length * sizeof(T)));

			glGetNamedBufferSubData(id, readOffset, length * sizeof(T), static_cast<void*>(data));
			checkError();
		}

		bool isImmutable() const {
			int val;
			glGetNamedBufferParameteriv(id, GL_BUFFER_IMMUTABLE_STORAGE, &val);
			checkError();
			return static_cast<bool>(val);
		}

		bool isMapped() const {
			int val;
			glGetNamedBufferParameteriv(id, GL_BUFFER_MAPPED, &val);
			checkError();
			return static_cast<bool>(val);
		}

		template<typename T = void>
		T* getCurrentMap() {
			void* ptr;
			glGetNamedBufferPointerv(id, GL_BUFFER_MAP_POINTER, &ptr);
			checkError();
			return (T*)ptr;
		}

		template<typename T = void>
		const T* getCurrentMap() const {
			void* ptr;
			glGetNamedBufferPointerv(id, GL_BUFFER_MAP_POINTER, &ptr);
			checkError();
			return (const T*)ptr;
		}

		int64_t getMapLength() const {
			int64_t val;
			glGetNamedBufferParameteri64v(id, GL_BUFFER_MAP_LENGTH, &val);
			checkError();
			return val;
		}

		int64_t getMapOffset() const {
			int64_t val = 0;
			glGetNamedBufferParameteri64v(id, GL_BUFFER_MAP_OFFSET, &val);
			checkError();
			return val;
		}

		GLenum getStorageFlags() const {
			int val = 0;
			glGetNamedBufferParameteriv(id, GL_BUFFER_STORAGE_FLAGS, &val);
			checkError();
			return static_cast<GLenum>(val);
		}

		MutableType getMutableType() const {
			int val;
			glGetNamedBufferParameteriv(id, GL_BUFFER_USAGE, &val);
			checkError();
			return static_cast<MutableType>(val);
		}

		size_t sizeBytes() const {
			return byteSize;
		}

		GLuint getId() const {
			return id;
		}

		bool verifyId() const {
			bool val = glIsBuffer(id) == GL_TRUE;
			checkError();
			return val;
		}

		bool isValid() const {
			return id != 0;
		}

		bool isInitialized() const {
			return sizeBytes() > 0;
		}

		bool boundsCheckBytes(size_t byteCount) const {
			return byteCount < sizeBytes();
		}
		bool boundsCheckBytes(intptr_t offset, size_t byteCount) const {
			return (offset + byteCount) <= sizeBytes();
		}

		static bool isValidInitializer(Flags flags) {
			// if persistent and not read or write
			if (allOf(flags, Flags::Persistent) && !anyOf(flags, Flags::Write | Flags::Read)) {
				return false;
			}

			// if any flags NOT in ValidInitializers
			if (anyOf(flags, ~Flags::ValidInitializers)) {
				return false;
			}

			// if coherent and NOT persistent
			if (allOf(flags, Flags::Coherent) && !allOf(flags, Flags::Persistent)) {
				return false;
			}
			return true;
		}

		static bool isValidMapping(Flags flags) {
			// if any flags NOT in ValidMapping
			return !anyOf(flags, ~Flags::ValidMapping);
		}
	protected:
		size_t byteSize;
		GLuint id;
	};

	
};