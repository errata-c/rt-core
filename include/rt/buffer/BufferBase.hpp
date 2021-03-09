#pragma once
#include <rt/Core.hpp>
#include <ez/BitFlags.hpp>
#include <vector>
#include <cassert>
#include <cinttypes>
#include <type_traits>

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

	/*
	Initialization flags for immutable storage buffers.
	The following operations are always valid on immutable buffers regardless of the flags field:
		Writing to the buffer with any rendering pipeline process.
		  These include Transform Feedback, Image Load Store, Atomic Counter, and Shader Storage Buffer Object.
		  Basically, anything that is part of the rendering pipeline that can write to a buffer will always work.

		Clearing the buffer.
		  Because this only transfers a few bytes of data, it is not considered "client-side" modification.

		Copying the buffer.
		  This copies from one buffer to another, so it all happens "server-side".

		Invalidating the buffer.
		  This only wipes out the contents of the buffer, so it is considered "server-side".

		Asynchronous pixel transfers into the buffer.
		  This sets the data in a buffer, but only through pure-OpenGL mechanisms.

		Using glGetBufferSubData to read a part of the buffer back to the CPU.
		  This is not a "server-side" operation, but it's always available regardless.
	*/
	enum class BufferInit {
		/*
		Allows the user to read the buffer via mapping the buffer. 
		Without this flag, attempting to map the buffer for reading will fail.
		*/
		Read = 0,

		/*
		Allows the user to map the buffer for writing. Without this flag, 
		attempting to map the buffer for writing will fail.
		*/
		Write = 1,

		// OpenGL will not syncronize, this can lead to undefined behavior if not used carefully.
		Unsyncronized = 5,

		/*
		Allows the buffer object to be mapped in such a way that it can be used while it is mapped. 
		Without this flag, attempting to perform any operation on the buffer while it is mapped will fail. 
		You must use one of the mapping bits when using this bit.
		*/
		Persistent = 6,
		
		/*
		Allows reads from and writes to a persistent buffer to be coherent with OpenGL, 
		without an explicit barrier. Without this flag, you must use an explicit barrier to achieve coherency. 
		You must use GL_PERSISTENT_BIT when using this bit.
		*/
		Coherent = 7,

		/*
		Allows the user to modify the contents of the storage with glBufferSubData. 
		Without this flag, attempting to call that function on this buffer will fail.
		*/
		Dynamic = 8,

		// When all other criteria for the buffer storage allocation are met, 
		// this bit may be used by an implementation to determine whether to use storage that is local to the server or
		// to the client to serve as the backing store for the buffer.
		// Note that this can cause GL_INVALID_VALUE error when mapping on some implementations.
		Client = 9,
		
		_Count,
		_EnableOperators
	};
	using BufferInits = ez::BitFlags<BufferInit>;

	enum class BufferFlag {
		// Indicates that the mapping is intended to be read from. Reading without this flag is an error.
		Read = 0,

		// Indicates that the mapping is intended to be written to. Writing without this flag is an error.
		Write = 1,

		// Indicates that the mapping should invalidate the range of the buffer being mapped.
		// This is only useful with the mapRange method.
		InvalidateRange = 2,

		// Indicates that the mapping should invalidate the entire buffer being mapped.
		// Useful for streaming data to the GPU. Read up on buffer invalidation for more info.
		InvalidateBuffer = 3,

		// Allows the use of the flush and flushRange methods, specifically on persistently mapped buffers.
		// It is an error to call those methods when the buffer is not currently mapped with this bit.
		FlushExplicit = 4,

		// Indicates that opengl should not synchronize operations on this buffer, because the programmer is going to 
		// manually handle the synchronization.
		Unsyncronized = 5,

		// Indicates that the buffer is going to be mapped even while other operations on it are being performed.
		// This means that you will have to manage the read and write ordering.
		Persistent = 6,

		// Indicates that writing to the buffer mapping should become visible to opengl immediately afterward without
		// any synchronization. Performance with this may vary, depending on the hardware. However, in general it will
		// likely be slower. Must be used with persistent.
		Coherent = 7,

		_Count,
		_EnableOperators
	};
	using BufferFlags = ez::BitFlags<BufferFlag>;

};


namespace rt {

	class Buffer {
	public:
		using Flag = BufferFlag;
		using Init = BufferInit;

		using Inits = BufferInits;
		using Flags = BufferFlags;

		Buffer()
			: id(0)
			, byteSize(0)
		{
			glCreateBuffers(1, &id);
			checkError();
			assert(isValid());
		}
		~Buffer() {
			if (isValid()) {
				glDeleteBuffers(1, &id);
				id = 0;
				checkError();
			}
		}
		Buffer(Buffer&& other) noexcept
			: id(other.id)
			, byteSize(other.byteSize)
		{
			other.id = 0;
		}

		Buffer& operator=(Buffer&& other) noexcept {
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

		Buffer(const Buffer&) = delete;
		Buffer& operator=(const Buffer&) = delete;

		// Note that this method will invalidate any bindings, ie if you've attached this buffer to a shader 
		// or a vertex array, you'll have to redo those again to avoid any errors.
		void reset() {
			if (isValid()) {
				glDeleteBuffers(1, &id);
				checkError();
				glCreateBuffers(1, &id);
				checkError();
				byteSize = 0;
			}
		}

		// Initializes this buffer as an immutable storage buffer, given a size in bytes.
		void initArray(size_t length, Inits flags) {
			assert(!isInitialized());
			assert(length > 0);
			assert(isValidInitializer(flags) && "Your initializer flags contain an invalid combination!");

			byteSize = length;
			glNamedBufferStorage(
				getId(), 
				sizeBytes(), 
				(const void*)0, 
				static_cast<GLbitfield>(flags.rawValue())
			);
			checkError();
		}
		// Initializes this buffer as an immutable storage buffer, given an array of objects to write to opengl.
		template<typename T>
		void initArray(const T* data, size_t length, Inits flags) {
			static_assert(std::is_trivially_copyable_v<T>, "Type passed into rt::Buffer::mapRange is not trivially copyable!");
			assert(!isInitialized());
			assert(length > 0);
			assert(isValidInitializer(flags) && "Your initializer flags contain an invalid combination!");

			byteSize = length * sizeof(T);
			glNamedBufferStorage(
				getId(),
				sizeBytes(), 
				(const void*)data, 
				static_cast<GLbitfield>(flags.rawValue())
			);
			checkError();
		}
		template<typename T>
		void initValue(const T& obj, Inits flags) {
			static_assert(std::is_trivially_copyable_v<T>, "Type passed into rt::Buffer::mapRange is not trivially copyable!");
			assert(!isInitialized());
			assert(isValidInitializer(flags) && "Your initializer flags contain an invalid combination!");

			byteSize = sizeof(T);
			glNamedBufferStorage(
				getId(), 
				sizeBytes(), 
				reinterpret_cast<const void*>(&obj), 
				static_cast<GLbitfield>(flags.rawValue())
			);
			checkError();
		}

		template<typename T>
		void subArray(const T* data, size_t length, intptr_t writeOffset = 0) {
			static_assert(std::is_trivially_copyable_v<T>, "Type passed into rt::Buffer::mapRange is not trivially copyable!");
			assert(isValid());
			assert(boundsCheckBytes(writeOffset, length * sizeof(T)) && "Attempted to write a value out of bounds!");
			glNamedBufferSubData(
				getId(),
				writeOffset, 
				length * sizeof(T), 
				reinterpret_cast<const void*>(data)
			);
			checkError();
		}

		template<typename T>
		void subValue(const T& obj, intptr_t writeOffset = 0) {
			static_assert(std::is_trivially_copyable_v<T>, "Type passed into rt::Buffer::mapRange is not trivially copyable!");
			assert(isValid());
			assert(boundsCheckBytes(writeOffset, sizeof(T)) && "Attempted to write a value out of bounds!");
			glNamedBufferSubData(
				getId(),
				writeOffset, 
				sizeof(T), 
				reinterpret_cast<const void*>(&obj)
			);
			checkError();
		}

		void invalidate() {
			assert(isValid());
			glInvalidateBufferData(id);
			checkError();
		}
		void invalidate(intptr_t startIndex, uintptr_t length) {
			assert(isValid());
			assert(boundsCheckBytes(startIndex, length) && "Attempted to invalidate out of bounds!");
			glInvalidateBufferSubData(getId(), startIndex, length);
			checkError();
		}

		void clearTo(GLfloat value) {
			assert(isValid());
			glClearNamedBufferData(getId(), GL_R32F, GL_RED, GL_FLOAT, &value);
		}
		void clearTo(GLint value) {
			assert(isValid());
			glClearNamedBufferData(getId(), GL_R32I, GL_RED, GL_INT, &value);
			checkError();
		}
		void clearTo(GLuint value) {
			assert(isValid());
			glClearNamedBufferData(getId(), GL_R32UI, GL_RED, GL_UNSIGNED_INT, &value);
			checkError();
		}
		void clearTo(GLshort value) {
			assert(isValid());
			glClearNamedBufferData(getId(), GL_R16I, GL_RED, GL_SHORT, &value);
			checkError();
		}
		void clearTo(GLushort value) {
			assert(isValid());
			glClearNamedBufferData(getId(), GL_R16UI, GL_RED, GL_UNSIGNED_SHORT, &value);
			checkError();
		}
		void clearTo(GLbyte value) {
			assert(isValid());
			glClearNamedBufferData(getId(), GL_R8I, GL_RED, GL_BYTE, &value);
			checkError();
		}
		void clearTo(GLubyte value) {
			assert(isValid());
			glClearNamedBufferData(getId(), GL_R8UI, GL_RED, GL_UNSIGNED_BYTE, &value);
			checkError();
		}

		void copyTo(const Buffer& buff) const {
			assert(isValid());
			assert(buff.sizeBytes() >= sizeBytes());

			glCopyNamedBufferSubData(getId(), buff.getId(), 0, 0, byteSize);
			checkError();
		}
		void copyTo(const Buffer& buff, size_t length, intptr_t readOffset = 0, intptr_t writeOffset = 0) const {
			assert(isValid());
			assert(boundsCheckBytes(readOffset, length));
			assert(buff.boundsCheckBytes(writeOffset, length));

			glCopyNamedBufferSubData(getId(), buff.getId(), readOffset, writeOffset, length);
			checkError();
		}

		// Binding ---
		void bindUBO(GLuint index) {
			assert(isValid());
			glBindBufferBase(GL_UNIFORM_BUFFER, index, getId());
			checkError();
		}
		void bindUBO(GLuint index, intptr_t startIndex, size_t length) {
			assert(isValid());
			glBindBufferRange(GL_UNIFORM_BUFFER, index, getId(), startIndex, length);
			checkError();
		}
		void bindSSBO(GLuint index) {
			assert(isValid());
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, getId());
			checkError();
		}
		void bindSSBO(GLuint index, intptr_t startIndex, size_t length) {
			assert(isValid());
			glBindBufferRange(GL_SHADER_STORAGE_BUFFER, index, getId(), startIndex, length);
			checkError();
		}

		// Mapping ---
		template<typename T = uint8_t>
		T* mapRange(intptr_t offset, GLsizeiptr length, Flags flags) {
			static_assert(std::is_trivially_copyable_v<T>, "Type passed into rt::Buffer::mapRange is not trivially copyable!");
			assert(boundsCheckBytes(offset * sizeof(T), length * sizeof(T)));
			T* tmp = (T*)glMapNamedBufferRange(
							getId(), 
							offset * sizeof(T), 
							length * sizeof(T), 
							static_cast<GLbitfield>(flags)
					);
			checkError();
			return tmp;
		}
		template<typename T = uint8_t>
		T* map(Flags flags) {
			static_assert(std::is_trivially_copyable_v<T>, "Type passed into rt::Buffer::mapRange is not trivially copyable!");
			T* tmp = (T*)glMapNamedBufferRange(getId(), 0, byteSize, static_cast<GLbitfield>(flags.rawValue()));
			checkError();
			return tmp;
		}

		template<typename T = uint8_t>
		const T* mapRange(intptr_t offset, GLsizeiptr length, Flags flags) const {
			static_assert(std::is_trivially_copyable_v<T>, "Type passed into rt::Buffer::mapRange is not trivially copyable!");
			assert(boundsCheckBytes(offset * sizeof(T), length * sizeof(T)));
			T* tmp = (T*)glMapNamedBufferRange(getId(), offset * sizeof(T), length * sizeof(T), static_cast<GLbitfield>(flags));
			checkError();
			return tmp;
		}

		template<typename T = uint8_t>
		const T* map(Flags flags) const {
			static_assert(std::is_trivially_copyable_v<T>, "Type passed into rt::Buffer::mapRange is not trivially copyable!");
			T* tmp = (T*)glMapNamedBufferRange(getId(), 0, byteSize, static_cast<GLbitfield>(flags));
			checkError();
			return tmp;
		}

		bool unmap() {
			bool res = static_cast<bool>(glUnmapNamedBuffer(getId()));
			checkError();
			return res;
		}

		void flush() {
			glFlushMappedNamedBufferRange(getId(), 0, sizeBytes());
			checkError();
		}

		void flushRange(intptr_t offset, GLsizeiptr length) {
			assert(boundsCheckBytes(offset, length));
			glFlushMappedNamedBufferRange(getId(), offset, length);
			checkError();
		}

		template<typename T>
		void flushRange(intptr_t offset, GLsizeiptr length) {
			assert(boundsCheckBytes(offset * sizeof(T), length * sizeof(T)));
			glFlushMappedNamedBufferRange(getId(), offset * sizeof(T), length * sizeof(T));
			checkError();
		}

		// Mutable methods ---
		template<typename T>
		void resizeValue(MutableType type, const T& obj) {
			static_assert(std::is_trivially_copyable_v<T>, "Type passed into rt::Buffer::mapRange is not trivially copyable!");
			assert(isValid());
			byteSize = sizeof(T);
			glNamedBufferSubData(getId(), sizeBytes(), reinterpret_cast<const void*>(&obj), static_cast<GLenum>(type));
			checkError();
		}

		void resizeArray(MutableType type, size_t ns) {
			assert(isValid());
#ifdef RENDER_TOOLS_ERROR_CHECKS
			if (isImmutable()) {
				throw std::exception("Called a Mutable buffer method on an immutable buffer type!");
			}
#endif
			byteSize = ns;
			glNamedBufferData(getId(), sizeBytes(), 0, static_cast<GLuint>(type));
			checkError();
		}
		template<typename T>
		void resizeArray(MutableType type, const T* data, size_t length) {
			static_assert(std::is_trivially_copyable_v<T>, "Type passed into rt::Buffer::mapRange is not trivially copyable!");
			assert(isValid());
#ifdef RENDER_TOOLS_ERROR_CHECKS
			if (isImmutable()) {
				throw std::exception("Called a Mutable buffer method on an immutable buffer type!");
			}
#endif
			byteSize = sizeof(T) * length;
			glNamedBufferData(getId(), sizeBytes(), reinterpret_cast<const void*>(data), static_cast<GLenum>(type));
			checkError();
		}

		// THIS FUNCTION WILL INVALIDATE ANY BINDINGS! If you've attached this buffer to anything, you'll
		// have to completely redo all of that.
		void changeMutableType(MutableType nType) {
#ifdef RENDER_TOOLS_ERROR_CHECKS
			if (isImmutable()) {
				throw std::exception("Called a Mutable buffer method on an immutable buffer type!");
			}
#endif
			if (isValid()) {
				GLuint tmpId;
				glCreateBuffers(1, &tmpId);
				checkError();
				assert(tmpId != 0);

				glNamedBufferData(tmpId, sizeBytes(), 0, static_cast<GLuint>(nType));
				checkError();

				glCopyNamedBufferSubData(getId(), tmpId, 0, 0, sizeBytes());
				checkError();

				glDeleteBuffers(1, &id);
				checkError();

				id = tmpId;
			}
		}


		// Getters ---
		template<typename T>
		std::vector<T> getData(size_t length, intptr_t readOffset) const {
			static_assert(std::is_trivially_copyable_v<T>, "Type passed into rt::Buffer::mapRange is not trivially copyable!");
			assert(isValid());
			assert(boundsCheckBytes(readOffset, length * sizeof(T)) && "Attempted to read data out of bounds!");

			std::vector<T> ret;
			ret.resize(sizeBytes());

			glGetNamedBufferSubData(getId(), readOffset, length * sizeof(T), static_cast<void*>(ret.data()));
			checkError();

			return ret;
		}

		template<typename T = uint8_t>
		void getData(T * data, size_t length, intptr_t readOffset) const {
			static_assert(std::is_trivially_copyable_v<T>, "Type passed into rt::Buffer::mapRange is not trivially copyable!");
			assert(isValid());
			assert(boundsCheckBytes(readOffset, length * sizeof(T)) && "Attempted to read data out of bounds!");

			glGetNamedBufferSubData(getId(), readOffset, length * sizeof(T), static_cast<void*>(data));
			checkError();
		}

		bool isImmutable() const {
			assert(isValid());
			int val;
			glGetNamedBufferParameteriv(getId(), GL_BUFFER_IMMUTABLE_STORAGE, &val);
			checkError();
			return static_cast<bool>(val);
		}

		bool isMapped() const {
			assert(isValid());
			int val;
			glGetNamedBufferParameteriv(getId(), GL_BUFFER_MAPPED, &val);
			checkError();
			return static_cast<bool>(val);
		}

		template<typename T = uint8_t>
		T* getCurrentMap() {
			static_assert(std::is_trivially_copyable_v<T>, "Type passed into rt::Buffer::mapRange is not trivially copyable!");
			assert(isValid());
			void* ptr;
			glGetNamedBufferPointerv(getId(), GL_BUFFER_MAP_POINTER, &ptr);
			checkError();
			return (T*)ptr;
		}

		template<typename T = uint8_t>
		const T* getCurrentMap() const {
			static_assert(std::is_trivially_copyable_v<T>, "Type passed into rt::Buffer::mapRange is not trivially copyable!");
			assert(isValid());
			void* ptr;
			glGetNamedBufferPointerv(getId(), GL_BUFFER_MAP_POINTER, &ptr);
			checkError();
			return (const T*)ptr;
		}

		int64_t getMapLength() const {
			assert(isValid());
			int64_t val;
			glGetNamedBufferParameteri64v(getId(), GL_BUFFER_MAP_LENGTH, &val);
			checkError();
			return val;
		}

		int64_t getMapOffset() const {
			assert(isValid());
			int64_t val = 0;
			glGetNamedBufferParameteri64v(getId(), GL_BUFFER_MAP_OFFSET, &val);
			checkError();
			return val;
		}

		GLenum getStorageFlags() const {
			assert(isValid());
			int val = 0;
			glGetNamedBufferParameteriv(getId(), GL_BUFFER_STORAGE_FLAGS, &val);
			checkError();
			return static_cast<GLenum>(val);
		}

		MutableType getMutableType() const {
			assert(isValid());
			int val;
			glGetNamedBufferParameteriv(getId(), GL_BUFFER_USAGE, &val);
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
			bool val = glIsBuffer(getId()) == GL_TRUE;
			checkError();
			return val;
		}

		bool isValid() const {
			return getId() != 0;
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

		static bool isValidInitializer(Inits flags) {
			// if persistent and not read or write
			if (flags.allOf(Init::Persistent) && !flags.anyOf(Init::Write | Init::Read)) {
				return false;
			}

			// if coherent and NOT persistent
			if (flags.allOf(Init::Coherent) && !flags.allOf(Init::Persistent)) {
				return false;
			}
			return true;
		}
	protected:
		size_t byteSize;
		GLuint id;
	};

	
};