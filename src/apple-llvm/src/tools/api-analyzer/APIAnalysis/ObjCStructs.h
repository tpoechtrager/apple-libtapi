/// WriteModernMetadataDeclarations - Writes out metadata declarations for modern ABI.
/// struct _prop_t {
///   const char *name;
///   char *attributes;
/// }

template<typename _uintptr_t>
struct _prop_t {
  _uintptr_t name;
  _uintptr_t attributes;
};

/// struct _prop_list_t {
///   uint32_t entsize;      // sizeof(struct _prop_t)
///   uint32_t count_of_properties;
///   struct _prop_t prop_list[count_of_properties];
/// }

template<typename _uintptr_t>
struct _prop_list_t {
  uint32_t entsize;
  uint32_t count_of_properties;
  _uintptr_t prop_list;
};

/// struct _protocol_list_t {
///   long protocol_count;   // Note, this is 32/64 bit
///   struct _protocol_t * protocol_list[protocol_count];
/// }

template<typename _uintptr_t>
struct _protocol_list_t {
  _uintptr_t protocol_count;
  _uintptr_t protocol_list;
};

/// struct _objc_method {
///   SEL _cmd;
///   const char *method_type;
///   char *_imp;
/// }

template<typename _uintptr_t>
struct _objc_method {
  _uintptr_t _cmd;
  _uintptr_t method_type;
  _uintptr_t _imp;
};

/// struct _method_list_t {
///   uint32_t entsize;  // sizeof(struct _objc_method)
///   uint32_t method_count;
///   struct _objc_method method_list[method_count];
/// }

template<typename _uintptr_t>
struct _method_list_t {
  uint32_t entsize;
  uint32_t method_count;
  //_uintptr_t method_list;
};

/// struct _protocol_t {
///   id isa;  // NULL
///   const char *protocol_name;
///   const struct _protocol_list_t * protocol_list; // super protocols
///   const struct method_list_t *instance_methods;
///   const struct method_list_t *class_methods;
///   const struct method_list_t *optionalInstanceMethods;
///   const struct method_list_t *optionalClassMethods;
///   const struct _prop_list_t * properties;
///   const uint32_t size;  // sizeof(struct _protocol_t)
///   const uint32_t flags;  // = 0
///   const char ** extendedMethodTypes;
/// }

template<typename _uintptr_t>
struct _protocol_t {
  _uintptr_t isa;
  _uintptr_t protocol_name;
  _uintptr_t protocol_list;
  _uintptr_t instance_methods;
  _uintptr_t class_methods;
  _uintptr_t optionalInstanceMethods;
  _uintptr_t optionalClassMethods;
  _uintptr_t properties;
  uint32_t  size;
  uint32_t flags;
  _uintptr_t extendedMethodTypes;
};

/// struct _ivar_t {
///   unsigned long int *offset;  // pointer to ivar offset location
///   const char *name;
///   const char *type;
///   uint32_t alignment;
///   uint32_t size;
/// }

/// struct _ivar_list_t {
///   uint32 entsize;  // sizeof(struct _ivar_t)
///   uint32 count;
///   struct _ivar_t list[count];
/// }

/// struct _class_ro_t {
///   uint32_t flags;
///   uint32_t instanceStart;
///   uint32_t instanceSize;
///   uint32_t reserved;  // only when building for 64bit targets
///   const uint8_t *ivarLayout;
///   const char *name;
///   const struct _method_list_t *baseMethods;
///   const struct _protocol_list_t *baseProtocols;
///   const struct _ivar_list_t *ivars;
///   const uint8_t *weakIvarLayout;
///   const struct _prop_list_t *properties;
/// }

template<typename _uintptr_t>
struct _class_ro_t {
  uint32_t flags;
  uint32_t instanceStart;
  uint32_t instanceSize;
  uint32_t reserved;
  _uintptr_t ivarLayout;
  _uintptr_t name;
  _uintptr_t baseMethods;
  _uintptr_t baseProtocols;
  _uintptr_t ivars;
  _uintptr_t weakIvarLayout;
  _uintptr_t properties;
};

template<>
struct _class_ro_t<uint32_t> {
  uint32_t flags;
  uint32_t instanceStart;
  uint32_t instanceSize;
  uint32_t ivarLayout;
  uint32_t name;
  uint32_t baseMethods;
  uint32_t baseProtocols;
  uint32_t ivars;
  uint32_t weakIvarLayout;
  uint32_t properties;
};

/// struct _class_t {
///   struct _class_t *isa;
///   struct _class_t *superclass;
///   void *cache;
///   IMP *vtable;
///   struct _class_ro_t *ro;
/// }

template<typename _uintptr_t>
struct _class_t {
  _uintptr_t isa;
  _uintptr_t superclass;
  _uintptr_t cache;
  _uintptr_t vtable;
  _uintptr_t ro;
};

/// struct _category_t {
///   const char *name;
///   struct _class_t *cls;
///   const struct _method_list_t *instance_methods;
///   const struct _method_list_t *class_methods;
///   const struct _protocol_list_t *protocols;
///   const struct _prop_list_t *properties;
/// }

template<typename _uintptr_t>
struct _category_t {
  _uintptr_t name;
  _uintptr_t cls;
  _uintptr_t instance_methods;
  _uintptr_t class_methods;
  _uintptr_t protocols;
  _uintptr_t properties;
};

/// MessageRefTy - LLVM for:
/// struct _message_ref_t {
///   IMP messenger;
///   SEL name;
/// };

/// SuperMessageRefTy - LLVM for:
/// struct _super_message_ref_t {
///   SUPER_IMP messenger;
///   SEL name;
/// };
