#pragma once
#include "Core.hpp"

namespace rt {
	enum class FenceResult : GLenum {
		Signaled = GL_ALREADY_SIGNALED,
		Timeout = GL_TIMEOUT_EXPIRED,
		Satisfied = GL_CONDITION_SATISFIED,
		Failed = GL_WAIT_FAILED,
	};

	class Fence {
	public:
		static constexpr FenceResult
			Signaled = FenceResult::Signaled,
			Timeout = FenceResult::Timeout,
			Satisfied = FenceResult::Satisfied,
			Failed = FenceResult::Failed;
		using Result = FenceResult;

		Fence()
			: obj(nullptr)
		{}
		~Fence() {
			if (obj != nullptr) {
				glDeleteSync(obj); 
				checkError();
				obj = nullptr;
			}
		}

		Fence(Fence && other) noexcept
			: obj(other.obj)
		{
			other.obj = nullptr;
		}
		Fence& operator=(Fence&& other) noexcept {
			obj = other.obj;
			other.obj = nullptr;
			return *this;
		}

		Fence(const Fence &) = delete;
		Fence& operator=(const Fence&) = delete;

		FenceResult waitClient(uint64_t timeoutNanoseconds, bool flush = true) {
			assert(isValid());

			if (flush) {
				FenceResult res = static_cast<FenceResult>(glClientWaitSync(obj, GL_SYNC_FLUSH_COMMANDS_BIT, timeoutNanoseconds));
				checkError();
				return res;
			}
			else {
				FenceResult res = static_cast<FenceResult>(glClientWaitSync(obj, 0, timeoutNanoseconds));
				checkError();
				return res;
			}
		}

		void waitServer(bool flush = true) {
			assert(isValid());

			glWaitSync(obj, 0, GL_TIMEOUT_IGNORED); 
			checkError();

			if (flush) {
				glFlush(); checkError();
			}
		}

		bool isSignaled() const {
			if (!isValid()) {
				return false;
			}
			else {
				int value = 0;
				GLsizei count = 0;
				glGetSynciv(obj, GL_SYNC_STATUS, 1, &count, &value); 
				checkError();
				return value == GL_SIGNALED;
			}
		}

		bool init() {
			if (isValid()) {
				return false;
			}
			else {
				obj = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0); 
				checkError();
				return obj != nullptr;
			}
		}

		void reset() {
			if (isValid()) {
				glDeleteSync(obj); 
				checkError();
				obj = nullptr;
			}
		}

		bool isValid() const {
			return obj != nullptr;
		}

		static uint64_t getServerTimeout() {
			int64_t value = 0;
			glGetInteger64v(GL_MAX_SERVER_WAIT_TIMEOUT, &value); 
			checkError();
			return static_cast<uint64_t>(value);
		}
	private:
		GLsync obj;
	};
};