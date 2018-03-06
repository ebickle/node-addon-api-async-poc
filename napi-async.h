#ifndef SRC_NAPI_ASYNC_POC_H_
#define SRC_NAPI_ASYNC_POC_H_

#include "napi.h"

namespace Napi {
  class AsyncWork {
  public:
    typedef void (*AsyncWorkExecuteCallback)(void* data);
    typedef void (*AsyncWorkCompleteCallback)(Napi::Env env, void* data);

    explicit AsyncWork(Napi::Env env, 
                       AsyncWorkExecuteCallback execute,
                       AsyncWorkCompleteCallback complete,
                       void* data = nullptr);
    ~AsyncWork();

    // Async work can be moved but cannot be copied.
    AsyncWork(AsyncWork&& other);
    AsyncWork& operator =(AsyncWork&& other);
    AsyncWork(const AsyncWork&) = delete;
    AsyncWork& operator =(AsyncWork&) = delete;

    operator napi_async_work() const;

    Napi::Env Env() const;    

    void Queue();
    void Cancel();

  private:
    static void OnExecute(napi_env env, void* data);
    static void OnComplete(napi_env env, napi_status status, void* data);

    napi_env _env;
    napi_async_work _work;  
    AsyncWorkExecuteCallback _execute;
    AsyncWorkCompleteCallback _complete;
    void* _data;
  };
} // namespace Napi

// Inline implementations of all the above class methods are included here.
#include "napi-async-inl.h"

#endif // SRC_NAPI_ASYNC_POC_H_