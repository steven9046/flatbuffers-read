#include "flatbuffers/flatbuffers.h"
namespace tflite {

/**
 * @brief 序列化数据 CustomQuantization 与程序中数据结构 CustomQuantizationT 的相互转换
 * Part1: 主要数据结构声明
 * Part2: CustomQuantizationT   程序里的数据结构
 * Part3: CustomQuantization    序列化的数据，类成员函数可以实现 CustomQuantization -> CustomQuantizationT 解析， 和 CustomQuantizationT -> CustomQuantization 序列化
 * Part4: CustomQuantizationT -> CustomQuantization 序列化函数
 */

/*****************************  Part 1  **************************************/ 
struct CustomQuantization; // Table
struct CustomQuantizationBuilder; // 用来生成序列化的工具
struct CustomQuantizationT; // NativeTable
// 这声明位置我调整了一下，没啥用
inline const flatbuffers::TypeTable *CustomQuantizationTypeTable();
flatbuffers::Offset<CustomQuantization> CreateCustomQuantization(flatbuffers::FlatBufferBuilder &_fbb, const CustomQuantizationT *_o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);

/*****************************  Part 2  **************************************/ 
// 每个struct对应的 *T ,继承自NativeTable
struct CustomQuantizationT : public flatbuffers::NativeTable {
  typedef CustomQuantization TableType;
  std::vector<uint8_t> custom; // 这应该是存真正数据的地方
  // 默认构造函数
  CustomQuantizationT() {}
};

/*****************************  Part 3  **************************************/ 
// 序列化数据
struct CustomQuantization FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef CustomQuantizationT NativeTableType;
  typedef CustomQuantizationBuilder Builder;
  static const flatbuffers::TypeTable *MiniReflectTypeTable() {
    return CustomQuantizationTypeTable();
  }
  // 这个应该是说这个数据会占多少内存，一个内存的偏移量
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_CUSTOM = 4
  };
  const flatbuffers::Vector<uint8_t> *custom() const {
    return GetPointer<const flatbuffers::Vector<uint8_t> *>(VT_CUSTOM);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_CUSTOM) &&
           verifier.VerifyVector(custom()) &&
           verifier.EndTable();
  }
  // 打包，解包函数， 后边有实现
  CustomQuantizationT *UnPack(const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(CustomQuantizationT *_o, const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static flatbuffers::Offset<CustomQuantization> Pack(flatbuffers::FlatBufferBuilder &_fbb, const CustomQuantizationT* _o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

/**
 * @brief 把序列化文件解包成 C++ 类   CustomQuantization --> CustomQuantizationT
 * @param [in] _resolver    flatbuffers解析器
 */
inline CustomQuantizationT *CustomQuantization::UnPack(const flatbuffers::resolver_function_t *_resolver) const {
  // 先new一个 CustomQuantizationT
  std::unique_ptr<tflite::CustomQuantizationT> _o = std::unique_ptr<tflite::CustomQuantizationT>(new CustomQuantizationT());
  // 调用这个函数
  UnPackTo(_o.get(), _resolver);
  // 释放
  return _o.release();
}

/**
 * @brief 
 * @param [in] _o           指针
 * @param [in] _resolver    flatbuffers解析器
 * 1. 类型转换？？？
 * 2. 转移数据到 *T
 */
inline void CustomQuantization::UnPackTo(CustomQuantizationT *_o, const flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  
  { 
    auto _e = custom(); // 得到内存？
    if (_e) { 
        _o->custom.resize(_e->size()); // 我们知道o_是个new实例，这里把他的数据区初始化了
        for (flatbuffers::uoffset_t _i = 0; _i < _e->size(); _i++) { 
            _o->custom[_i] = _e->Get(_i); // 给new实例数据区设置数据
        } 
    } 
  }
}

/**
 * @brief 这个是 CustomQuantization 的成员函数，但是是通过调用类外函数实现的
 * @param [in]          _fbb
 * @param [in]          _o
 * @param [in]          _rehasher
 */
inline flatbuffers::Offset<CustomQuantization> CustomQuantization::Pack(flatbuffers::FlatBufferBuilder &_fbb, const CustomQuantizationT* _o, const flatbuffers::rehasher_function_t *_rehasher) {
  return CreateCustomQuantization(_fbb, _o, _rehasher);
}


/*****************************  Part 4  **************************************/ 

/**
 * @brief 这部分是实现给数据序列化的，一共有两条路径
 * 1. 通过调用 CustomQuantization 的 Pack() 函数,这个函数会调用类外函数
 *    Pack(_fbb, _o, _rehasher) 
 *      --> CreateCustomQuantization(_fbb, _o, _rehasher) 
 *      --> CreateCustomQuantization(_fbb,_custom) 
 *      --> CustomQuantizationBuilder.add_custom(custom)
 * 2. 直接写入 
 *    CreateCustomQuantizationDirect(_fbb, custom)
 *      --> CreateCustomQuantization(_fbb,_custom) 
 *      --> CustomQuantizationBuilder.add_custom(custom)
 * PS: 1. 方法2只不过是不是通过调用 CustomQuantization 的成员函数而已，但是也需要通过CustomQuantizationBuilder，后边路径是一样的
 *     2. rehasher不知道有啥用
 */

/**
 * @brief 主要是 add_custom 的实现,调用 FlatBufferBuilder 的 AddOffset 函数
 * 就是用来序列化的一个工具
 */
struct CustomQuantizationBuilder {

  typedef CustomQuantization Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;

  void add_custom(flatbuffers::Offset<flatbuffers::Vector<uint8_t>> custom) {
    // 这应该是根据内存偏移量把custom里的数据写到序列化文件里
    fbb_.AddOffset(CustomQuantization::VT_CUSTOM, custom);
  }

  explicit CustomQuantizationBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }

  CustomQuantizationBuilder &operator=(const CustomQuantizationBuilder &);

  flatbuffers::Offset<CustomQuantization> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<CustomQuantization>(end);
    return o;
  }

};


inline flatbuffers::Offset<CustomQuantization> CreateCustomQuantization(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<uint8_t>> custom = 0) {
  CustomQuantizationBuilder builder_(_fbb);
  builder_.add_custom(custom);
  return builder_.Finish();
}

/**
 * @brief 
 * @param [in]      _fbb        flatbuffers 的 builder
 * @param [in]      custom      数据
 */
inline flatbuffers::Offset<CustomQuantization> CreateCustomQuantizationDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<uint8_t> *custom = nullptr) {
  if (custom) { _fbb.ForceVectorAlignment(custom->size(), sizeof(uint8_t), 16); }
  auto custom__ = custom ? _fbb.CreateVector<uint8_t>(*custom) : 0;
  return tflite::CreateCustomQuantization(
      _fbb,
      custom__);
}

/**
 * @brief 
 * @param [in] _fbb         flatbuffers的builder,显然是用来生成序列化文件的
 * @param [in] _o           一个 CustomQuantizationT 指针
 * @param [in] _rehasher    应该是编码用的算法，怎么变成01
 * 1. 通过flatbuffers的builder得到要写入的custom数据
 */
inline flatbuffers::Offset<CustomQuantization> CreateCustomQuantization(flatbuffers::FlatBufferBuilder &_fbb, const CustomQuantizationT *_o, const flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { flatbuffers::FlatBufferBuilder *__fbb; const CustomQuantizationT* __o; const flatbuffers::rehasher_function_t *__rehasher; }_va = { &_fbb, _o, _rehasher}; 
  (void)_va; // 这玩意儿用到了么？
  _fbb.ForceVectorAlignment(_o->custom.size(), sizeof(uint8_t), 16);
  auto _custom = _o->custom.size() ? _fbb.CreateVector(_o->custom) : 0;
  
  return tflite::CreateCustomQuantization(
      _fbb,
      _custom);
}

} // namespace