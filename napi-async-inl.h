#ifndef SRC_NAPI_ASYNC_POC_INL_H_
#define SRC_NAPI_ASYNC_POC_INL_H_

namespace Napi {

////////////////////////////////////////////////////////////////////////////////
// AsyncWork class
////////////////////////////////////////////////////////////////////////////////

inline AsyncWork::AsyncWork(Napi::Env env, 
                            AsyncWorkExecuteCallback execute,
                            AsyncWorkCompleteCallback complete,
                            void* data)
  : _env(env),
    _execute(execute),
    _complete(complete),
    _data(data) {

  napi_value resource_id;
  napi_status status = napi_create_string_latin1(
    env, "generic", NAPI_AUTO_LENGTH, &resource_id);
  
  NAPI_THROW_IF_FAILED(env, status);

  napi_async_work work;
  status = napi_create_async_work(
    env, nullptr, resource_id, OnExecute, OnComplete, this, &work);
  NAPI_THROW_IF_FAILED(env, status);
}

inline AsyncWork::~AsyncWork() {
  napi_delete_async_work(_env, _work);
}

inline AsyncWork::AsyncWork(AsyncWork&& other) {
  _env = other._env;
  other._env = nullptr;
  _work = other._work;
  other._work = nullptr;
}

inline AsyncWork& AsyncWork::operator =(AsyncWork&& other) {
  _env = other._env;
  other._env = nullptr;
  _work = other._work;
  other._work = nullptr;
  return *this;
}

inline AsyncWork::operator napi_async_work() const {
  return _work;
}

inline Napi::Env AsyncWork::Env() const {
  return _env;
}

inline void AsyncWork::Queue() {
  napi_status status = napi_queue_async_work(_env, _work);
  NAPI_THROW_IF_FAILED(_env, status);
}

inline void AsyncWork::Cancel() {
  napi_status status = napi_cancel_async_work(_env, _work);
  NAPI_THROW_IF_FAILED(_env, status);  
}

inline void AsyncWork::OnExecute(napi_env env, void* data) {
  AsyncWork* self = static_cast<AsyncWork*>(data);
  self->_execute(self->_data);
}

inline void AsyncWork::OnComplete(napi_env env, napi_status status, void* data) {  
  AsyncWork* self = static_cast<AsyncWork*>(data);  

  HandleScope scope(env);
  details::WrapCallback([&] {  
    self->_complete(env, self->_data);
    return nullptr;
  });  
}

} // namespace Napi

#endif // SRC_NAPI_ASYNC_POC_INL_H_
