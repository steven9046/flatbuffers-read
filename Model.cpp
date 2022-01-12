#include "flatbuffers/flatbuffers.h"
namespace tflite {

/**
 * @brief 每个flatbuffers有一个root type,这里就是 Model, 根类型会有不同的函数，作为整个解析的入口
 * UnPackModel
 * -->  GetModel
 * -->  UnPack
 * -->  UnPackTo
 */
struct Model;
struct ModelBuilder;
struct ModelT;

inline std::unique_ptr<tflite::ModelT> UnPackModel(
    const void *buf,
    const flatbuffers::resolver_function_t *res = nullptr) {
  return std::unique_ptr<tflite::ModelT>(GetModel(buf)->UnPack(res));
}
/**
 * @brief 
 * @param [in] buf    序列化文件内存地址
 * 得到的是Model*,这个东西肯定不是空的，调用它的UnPack
 */
inline const tflite::Model *GetModel(const void *buf) {
  return flatbuffers::GetRoot<tflite::Model>(buf);
}

inline ModelT *Model::UnPack(const flatbuffers::resolver_function_t *_resolver) const {
  std::unique_ptr<tflite::ModelT> _o = std::unique_ptr<tflite::ModelT>(new ModelT());
  UnPackTo(_o.get(), _resolver);
  return _o.release();
}

inline void Model::UnPackTo(ModelT *_o, const flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  // Model的每一成员
  { auto _e = version(); 
    _o->version = _e; }
  
  // 下边相当于递归地解析所有东西了
  { auto _e = operator_codes(); 
    if (_e) { 
        _o->operator_codes.resize(_e->size()); 
        for (flatbuffers::uoffset_t _i = 0; _i < _e->size(); _i++) { 
        _o->operator_codes[_i] = std::unique_ptr<tflite::OperatorCodeT>(_e->Get(_i)->UnPack(_resolver));
        }   
    } 
  }
  // 解析subgraph
  { auto _e = subgraphs(); if (_e) { _o->subgraphs.resize(_e->size()); for (flatbuffers::uoffset_t _i = 0; _i < _e->size(); _i++) { _o->subgraphs[_i] = std::unique_ptr<tflite::SubGraphT>(_e->Get(_i)->UnPack(_resolver)); } } }
  { auto _e = description(); if (_e) _o->description = _e->str(); }
  { auto _e = buffers(); if (_e) { _o->buffers.resize(_e->size()); for (flatbuffers::uoffset_t _i = 0; _i < _e->size(); _i++) { _o->buffers[_i] = std::unique_ptr<tflite::BufferT>(_e->Get(_i)->UnPack(_resolver)); } } }
  { auto _e = metadata_buffer(); if (_e) { _o->metadata_buffer.resize(_e->size()); for (flatbuffers::uoffset_t _i = 0; _i < _e->size(); _i++) { _o->metadata_buffer[_i] = _e->Get(_i); } } }
  { auto _e = metadata(); if (_e) { _o->metadata.resize(_e->size()); for (flatbuffers::uoffset_t _i = 0; _i < _e->size(); _i++) { _o->metadata[_i] = std::unique_ptr<tflite::MetadataT>(_e->Get(_i)->UnPack(_resolver)); } } }
  { auto _e = signature_defs(); if (_e) { _o->signature_defs.resize(_e->size()); for (flatbuffers::uoffset_t _i = 0; _i < _e->size(); _i++) { _o->signature_defs[_i] = std::unique_ptr<tflite::SignatureDefT>(_e->Get(_i)->UnPack(_resolver)); } } }
}


} // namespace