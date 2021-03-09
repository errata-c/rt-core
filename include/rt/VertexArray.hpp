#pragma once
#include "Core.hpp"
#include "Buffer.hpp"

namespace rt {

	enum class Primitive {
		Points = GL_POINTS,
		Lines = GL_LINES,
		Triangles = GL_TRIANGLES
	};

	static GLenum convertGL(Primitive prim) noexcept {
		return static_cast<GLenum>(prim);
	}

	class VertexArray {
	public:
		static GLuint CurrentId() {
			GLint value = 0;
			glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &value);
			checkError();
			return value;
		}

		VertexArray() {
			glCreateVertexArrays(1, &id); 
			checkError();
		}
		VertexArray(VertexArray && other) noexcept
			: id(other.id)
		{
			other.id = 0;
		}
		~VertexArray() {
			if (isValid()) {
				glDeleteVertexArrays(1, &id);
				checkError();
				id = 0;
			}
		}

		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;

		VertexArray& operator=(VertexArray&& other) noexcept {
			if (isValid()) {
				glDeleteVertexArrays(1, &id); 
				checkError();
			}

			id = other.id;
			other.id = 0;

			return *this;
		}

		void bindVertex(const Buffer & buff, GLuint index, intptr_t offset, GLsizei stride) {
			glVertexArrayVertexBuffer(id, index, buff.getId(), offset, stride);
			checkError();
		}
		void unbindVertex(GLuint index) {
			glVertexArrayVertexBuffer(id, index, 0, 0, 0);
			checkError();
		}

		void bindIndex(const Buffer& buffer) {
			glVertexArrayElementBuffer(id, buffer.getId());
			checkError();
		}
		void unbindIndex() {
			glVertexArrayElementBuffer(id, 0);
			checkError();
		}

		void bind() {
			glBindVertexArray(id); 
			checkError();
		}
		void unbind() {
			glBindVertexArray(0); 
			checkError();
		}

		void attribEnable(GLuint index) {
			glEnableVertexArrayAttrib(id, index); checkError();
		}
		void attribDisable(GLuint index) {
			glDisableVertexArrayAttrib(id, index); checkError();
		}
		void attribDivisor(GLuint attrIndex, GLuint divisor) {
			glVertexArrayBindingDivisor(id, attrIndex, divisor); 
			checkError();
		}
		void attribBinding(GLuint attrIndex, GLuint bindingIndex) {
			glVertexArrayAttribBinding(id, attrIndex, bindingIndex); 
			checkError();
		}


		void attribFormatF32(GLuint attr, GLuint dimm, GLsizei offset, bool normalized = false, Type type = Type::Float) {
			glVertexArrayAttribFormat(id, attr, dimm, convertGL(type), normalized, offset); 
			checkError();
		}
		void attribFormatF64(GLuint attr, GLuint dimm, GLsizei offset, Type type = Type::Float) {
			glVertexArrayAttribLFormat(id, attr, dimm, convertGL(type), offset); 
			checkError();
		}
		void attribFormatI32(GLuint attr, GLuint dimm, GLsizei offset, Type type = Type::I32) {
			glVertexArrayAttribIFormat(id, attr, dimm, convertGL(type), offset); 
			checkError();
		}


		bool isAttribEnabled(GLuint attr) const {
			assert(isValid());
			GLint val;
			glGetVertexArrayIndexediv(id, attr, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &val); 
			checkError();
			return static_cast<bool>(val);
		}
		bool isAttribF64(GLuint attr) const {
			assert(isValid());
			GLint val;
			glGetVertexArrayIndexediv(id, attr, GL_VERTEX_ATTRIB_ARRAY_LONG, &val); 
			checkError();
			return static_cast<bool>(val);
		}
		bool isAttribNormalized(GLuint attr) const {
			assert(isValid());
			GLint val;
			glGetVertexArrayIndexediv(id, attr, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED, &val); 
			checkError();
			return static_cast<bool>(val);
		}
		bool isAttribInteger(GLuint attr) const {
			assert(isValid());
			GLint val;
			glGetVertexArrayIndexediv(id, attr, GL_VERTEX_ATTRIB_ARRAY_INTEGER, &val); 
			checkError();
			return static_cast<bool>(val);
		}
		int getAttribDim(GLuint attr) const {
			assert(isValid());
			GLint val;
			glGetVertexArrayIndexediv(id, attr, GL_VERTEX_ATTRIB_ARRAY_SIZE, &val); 
			checkError();
			return val;
		}
		int getAttribStride(GLuint attr) const {
			assert(isValid());
			GLint val;
			glGetVertexArrayIndexediv(id, attr, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &val); 
			checkError();
			return val;
		}
		int getAttribDivisor(GLuint attr) const {
			assert(isValid());
			GLint val;
			glGetVertexArrayIndexediv(id, attr, GL_VERTEX_ATTRIB_ARRAY_DIVISOR, &val); 
			checkError();
			return val;
		}
		GLenum getAttribType(GLuint attr) const {
			assert(isValid());
			GLint val;
			glGetVertexArrayIndexediv(id, attr, GL_VERTEX_ATTRIB_ARRAY_TYPE, &val); 
			checkError();
			return static_cast<GLenum>(val);
		}
		int getAttribRelativeOffset(GLuint attr) const {
			assert(isValid());
			GLint val;
			glGetVertexArrayIndexediv(id, attr, GL_VERTEX_ATTRIB_RELATIVE_OFFSET, &val); 
			checkError();
			return static_cast<int>(val);
		}
		int64_t getAttribOffset(GLuint attr) const {
			assert(isValid());
			GLint64 val;
			glGetVertexArrayIndexed64iv(id, attr, GL_VERTEX_BINDING_OFFSET, &val); 
			checkError();
			return val;
		}
		GLuint getAttribVertexBufferId(GLuint attr) const {
			assert(isValid());
			GLint val;
			glGetVertexArrayIndexediv(id, attr, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &val); 
			checkError();
			return static_cast<GLuint>(val);
		}

		GLuint getIndexBufferId() const {
			assert(isValid());
			int val;
			glGetVertexArrayiv(id, GL_ELEMENT_ARRAY_BUFFER_BINDING, &val); 
			checkError();
			return static_cast<GLuint>(val);
		}
	
		void drawArrays(Primitive prim, GLsizei count, uintptr_t offset = 0) {
			glDrawArrays(convertGL(prim), static_cast<GLint>(offset), count); 
			checkError();
		}
		void drawArraysInstanced(Primitive prim, GLsizei elemCount, GLsizei primCount, uintptr_t offset = 0) {
			glDrawArraysInstanced(convertGL(prim), static_cast<GLint>(offset), elemCount, primCount); 
			checkError();
		}

		void drawElements(Primitive prim, Index index, GLsizei count, uintptr_t offset = 0) {
			glDrawElements(
				convertGL(prim), 
				count, 
				convertGL(index), 
				reinterpret_cast<const void*>(offset)); 
			checkError();
		}
		void drawElementsInstanced(Primitive prim, Index index, GLsizei elemCount, GLsizei primCount, uintptr_t offset = 0) {
			glDrawElementsInstanced(
				convertGL(prim), 
				elemCount, 
				convertGL(index), 
				reinterpret_cast<const void*>(offset), 
				primCount); 
			checkError();
		}

		GLuint getId() const {
			return id;
		}

		bool isValid() const {
			return id != 0;
		}

		void reset() {
			if (isValid()) {
				glDeleteVertexArrays(1, &id); 
				checkError();
				glCreateVertexArrays(1, &id); 
				checkError();
			}
			else {
				glCreateVertexArrays(1, &id); 
				checkError();
			}
		}
	protected:
		GLuint id;
	};

}