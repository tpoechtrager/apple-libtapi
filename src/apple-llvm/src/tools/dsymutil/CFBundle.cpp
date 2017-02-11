
#include "CFBundle.h"

#ifdef __APPLE__
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"
#include <CoreFoundation/CoreFoundation.h>
#include <assert.h>
#include <glob.h>

//----------------------------------------------------------------------
// Templatized CF helper class that can own any CF pointer and will
// call CFRelease() on any valid pointer it owns unless that pointer is
// explicitly released using the release() member function. This class
// is designed to mimic the std::auto_ptr<T> class and has all of the
// same functions. The one thing to watch out for is the
// CFReleaser<T>::release() function won't actually CFRelease any owned
// pointer, it is designed to relinquish ownwership of the pointer just
// like std:auto_ptr<T>::release() does.
//----------------------------------------------------------------------
template <class T> class CFReleaser {
public:
  //----------------------------------------------------------
  // Constructor that takes a pointer to a CF object that is
  // to be released when this object goes out of scope
  //----------------------------------------------------------
  CFReleaser(T ptr = NULL) : _ptr(ptr) {}

  //----------------------------------------------------------
  // The copy constructor will retain the pointer contained in
  // the RHS if it contains a valid pointer value
  //----------------------------------------------------------
  CFReleaser(const CFReleaser &rhs) : _ptr(rhs.get()) {
    if (get())
      ::CFRetain(get());
  }

  //----------------------------------------------------------
  // The destructor will release the pointer that it contains
  // if it has a valid pointer.
  //----------------------------------------------------------
  virtual ~CFReleaser() { reset(); }

  //----------------------------------------------------------
  // Assignments
  //----------------------------------------------------------
  CFReleaser &operator=(const CFReleaser<T> &rhs) {
    if (this != &rhs) {
      // Replace our owned pointer with the new one
      reset(rhs.get());
      // Retain the current pointer that we own
      if (get())
        ::CFRetain(get());
    }
    return *this;
  }

  //----------------------------------------------------------
  // Get the address of the contained type in case it needs
  // to be passed to a function that will fill in a pointer
  // value. The function currently will assert if _ptr is not
  // NULL because the only time this method should be used is
  // if another function will modify the contents, and we
  // could leak a pointer if this is not NULL. If the
  // assertion fires, check the offending code, or call
  // reset() prior to using the "ptr_address()" member to make
  // sure any owned objects has CFRelease called on it.
  //----------------------------------------------------------
  T *ptr_address() {
    assert(_ptr == NULL);
    return &_ptr;
  }

  //----------------------------------------------------------
  // Access the pointer itself
  //----------------------------------------------------------
  T get() { return _ptr; }

  const T get() const { return _ptr; }

  //----------------------------------------------------------
  // Set a new value for the pointer and CFRelease our old
  // value if we had a valid one.
  //----------------------------------------------------------
  void reset(T ptr = NULL) {
    if ((_ptr != NULL) && (ptr != _ptr))
      ::CFRelease(_ptr);
    _ptr = ptr;
  }

  //----------------------------------------------------------
  // Release ownership without calling CFRelease. This class
  // is designed to mimic std::auto_ptr<T>, so the release
  // method releases ownership of the contained pointer
  // without calling CFRelease.
  //----------------------------------------------------------
  T release() {
    T tmp = _ptr;
    _ptr = NULL;
    return tmp;
  }

private:
  T _ptr;
};

class CFString : public CFReleaser<CFStringRef> {
public:
  //------------------------------------------------------------------
  // Constructors and Destructors
  //------------------------------------------------------------------
  CFString(CFStringRef cf_str = NULL);
  CFString(const char *s, CFStringEncoding encoding);
  CFString(const CFString &rhs);
  CFString &operator=(const CFString &rhs);
  virtual ~CFString();

  const char *GetFileSystemRepresentation(std::string &str);
  CFStringRef SetFileSystemRepresentation(const char *path);
  CFStringRef SetFileSystemRepresentationFromCFType(CFTypeRef cf_type);
  CFStringRef SetFileSystemRepresentationAndExpandTilde(const char *path);
  const char *UTF8(std::string &str);
  CFIndex GetLength() const;
  static const char *UTF8(CFStringRef cf_str, std::string &str);
  static const char *FileSystemRepresentation(CFStringRef cf_str,
                                              std::string &str);
};

class CFBundle : public CFReleaser<CFBundleRef> {
public:
  //------------------------------------------------------------------
  // Constructors and Destructors
  //------------------------------------------------------------------
  CFBundle(const char *path = NULL);
  CFBundle(const CFBundle &rhs);
  CFBundle(CFURLRef url);
  CFBundle &operator=(const CFBundle &rhs);
  virtual ~CFBundle();

  bool SetPath(const char *path);
  CFStringRef GetIdentifier() const;
  CFTypeRef GetValueForInfoDictionaryKey(CFStringRef key) const;
};

static const char *CreatePathByExpandingTildePath(const char *path,
                                                  std::string &expanded_path) {
  glob_t globbuf;
  if (glob(path, GLOB_TILDE, NULL, &globbuf) == 0) {
    expanded_path = globbuf.gl_pathv[0];
    globfree(&globbuf);
  } else
    expanded_path.clear();

  return expanded_path.c_str();
}

//----------------------------------------------------------------------
// CFString constructor
//----------------------------------------------------------------------
CFString::CFString(CFStringRef s) : CFReleaser<CFStringRef>(s) {}

//----------------------------------------------------------------------
// CFString copy constructor
//----------------------------------------------------------------------
CFString::CFString(const CFString &rhs) : CFReleaser<CFStringRef>(rhs) {}

//----------------------------------------------------------------------
// CFString copy constructor
//----------------------------------------------------------------------
CFString &CFString::operator=(const CFString &rhs) {
  if (this != &rhs)
    *this = rhs;
  return *this;
}

CFString::CFString(const char *cstr, CFStringEncoding cstr_encoding)
    : CFReleaser<CFStringRef>() {
  if (cstr && cstr[0]) {
    reset(
        ::CFStringCreateWithCString(kCFAllocatorDefault, cstr, cstr_encoding));
  }
}

//----------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------
CFString::~CFString() {}

const char *CFString::GetFileSystemRepresentation(std::string &s) {
  return CFString::FileSystemRepresentation(get(), s);
}

CFStringRef CFString::SetFileSystemRepresentation(const char *path) {
  CFStringRef new_value = NULL;
  if (path && path[0])
    new_value =
        ::CFStringCreateWithFileSystemRepresentation(kCFAllocatorDefault, path);
  reset(new_value);
  return get();
}

CFStringRef CFString::SetFileSystemRepresentationFromCFType(CFTypeRef cf_type) {
  CFStringRef new_value = NULL;
  if (cf_type != NULL) {
    CFTypeID cf_type_id = ::CFGetTypeID(cf_type);

    if (cf_type_id == ::CFStringGetTypeID()) {
      // Retain since we are using the existing object
      new_value = (CFStringRef)::CFRetain(cf_type);
    } else if (cf_type_id == ::CFURLGetTypeID()) {
      new_value =
          ::CFURLCopyFileSystemPath((CFURLRef)cf_type, kCFURLPOSIXPathStyle);
    }
  }
  reset(new_value);
  return get();
}

CFStringRef
CFString::SetFileSystemRepresentationAndExpandTilde(const char *path) {
  std::string expanded_path;
  if (CreatePathByExpandingTildePath(path, expanded_path))
    SetFileSystemRepresentation(expanded_path.c_str());
  else
    reset();
  return get();
}

const char *CFString::UTF8(std::string &str) {
  return CFString::UTF8(get(), str);
}

// Static function that puts a copy of the UTF8 contents of CF_STR into STR
// and returns the C string pointer that is contained in STR when successful,
// else
// NULL is returned. This allows the std::string parameter to own the extracted
// string,
// and also allows that string to be returned as a C string pointer that can be
// used.

const char *CFString::UTF8(CFStringRef cf_str, std::string &str) {
  if (cf_str) {
    const CFStringEncoding encoding = kCFStringEncodingUTF8;
    CFIndex max_utf8_str_len = CFStringGetLength(cf_str);
    max_utf8_str_len =
        CFStringGetMaximumSizeForEncoding(max_utf8_str_len, encoding);
    if (max_utf8_str_len > 0) {
      str.resize(max_utf8_str_len);
      if (!str.empty()) {
        if (CFStringGetCString(cf_str, &str[0], str.size(), encoding)) {
          str.resize(strlen(str.c_str()));
          return str.c_str();
        }
      }
    }
  }
  return NULL;
}

// Static function that puts a copy of the file system representation of CF_STR
// into STR and returns the C string pointer that is contained in STR when
// successful, else NULL is returned. This allows the std::string parameter
// to own the extracted string, and also allows that string to be returned as
// a C string pointer that can be used.

const char *CFString::FileSystemRepresentation(CFStringRef cf_str,
                                               std::string &str) {
  if (cf_str) {
    CFIndex max_length =
        ::CFStringGetMaximumSizeOfFileSystemRepresentation(cf_str);
    if (max_length > 0) {
      str.resize(max_length);
      if (!str.empty()) {
        if (::CFStringGetFileSystemRepresentation(cf_str, &str[0],
                                                  str.size())) {
          str.erase(::strlen(str.c_str()));
          return str.c_str();
        }
      }
    }
  }
  str.erase();
  return NULL;
}

CFIndex CFString::GetLength() const {
  CFStringRef str = get();
  if (str)
    return CFStringGetLength(str);
  return 0;
}

//----------------------------------------------------------------------
// CFBundle constructor
//----------------------------------------------------------------------
CFBundle::CFBundle(const char *path) : CFReleaser<CFBundleRef>() {
  if (path && path[0])
    SetPath(path);
}

//----------------------------------------------------------------------
// CFBundle copy constructor
//----------------------------------------------------------------------
CFBundle::CFBundle(const CFBundle &rhs) : CFReleaser<CFBundleRef>(rhs) {}

CFBundle::CFBundle(CFURLRef url)
    : CFReleaser<CFBundleRef>(url ? CFBundleCreate(NULL, url) : NULL) {}

//----------------------------------------------------------------------
// CFBundle copy constructor
//----------------------------------------------------------------------
CFBundle &CFBundle::operator=(const CFBundle &rhs) {
  if (this != &rhs)
    *this = rhs;
  return *this;
}

//----------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------
CFBundle::~CFBundle() {}

//----------------------------------------------------------------------
// Set the path for a bundle by supplying a
//----------------------------------------------------------------------
bool CFBundle::SetPath(const char *in_path) {
  // Release our old bundle and ULR
  reset(); // This class is a CFReleaser<CFBundleRef>

  if (in_path && in_path[0]) {
    char resolved_path[PATH_MAX];
    const char *path = ::realpath(in_path, resolved_path);
    if (path == NULL)
      path = in_path;

    CFAllocatorRef alloc = kCFAllocatorDefault;
    // Make our Bundle URL
    CFReleaser<CFURLRef> bundle_url(::CFURLCreateFromFileSystemRepresentation(
        alloc, (const UInt8 *)path, strlen(path), false));
    if (bundle_url.get()) {
      CFIndex last_length = LONG_MAX;

      while (bundle_url.get() != NULL) {
        // Check the Path range and make sure we didn't make it to just
        // "/", ".", or ".."
        CFRange rangeIncludingSeparators;
        CFRange range = ::CFURLGetByteRangeForComponent(
            bundle_url.get(), kCFURLComponentPath, &rangeIncludingSeparators);
        if (range.length > last_length)
          break;

        reset(::CFBundleCreate(alloc, bundle_url.get()));
        if (get() != NULL) {
          if (GetIdentifier() != NULL)
            break;
          reset();
        }
        bundle_url.reset(::CFURLCreateCopyDeletingLastPathComponent(
            alloc, bundle_url.get()));

        last_length = range.length;
      }
    }
  }
  return get() != NULL;
}

CFStringRef CFBundle::GetIdentifier() const {
  CFBundleRef bundle = get();
  if (bundle != NULL)
    return ::CFBundleGetIdentifier(bundle);
  return NULL;
}

CFTypeRef CFBundle::GetValueForInfoDictionaryKey(CFStringRef key) const {
  CFBundleRef bundle = get();
  if (bundle != NULL)
    return ::CFBundleGetValueForInfoDictionaryKey(bundle, key);
  return NULL;
}
#endif

void getBundleInfo(llvm::StringRef ExePath, std::string &bundleVersionStr,
                   std::string &bundleShortVersionStr, std::string &bundleIDStr,
                   bool &OmitShortVersion) {
#ifdef __APPLE__
  CFTypeRef cf = NULL;
  CFTypeID cf_type_id = 0;

  // Try and find the original executable's Info.plist information using
  // CoreFoundation calls by creating a URL for the executable and chopping off
  // the last path component. The CFBundle can then get the identifier and grab
  // any needed information from it directly.
  if (!ExePath.empty() && llvm::sys::fs::exists(ExePath)) {
    const char *exe_path = ExePath.data();
    CFBundle bundle(exe_path);
    CFStringRef bundleID = bundle.GetIdentifier();
    if (bundleID != NULL) {
      CFString::UTF8(bundleID, bundleIDStr);
      cf = bundle.GetValueForInfoDictionaryKey(CFSTR("CFBundleVersion"));
      if (cf != NULL) {
        cf_type_id = ::CFGetTypeID(cf);
        if (cf_type_id == ::CFStringGetTypeID())
          CFString::UTF8((CFStringRef)cf, bundleVersionStr);
        else {
          CFString cf_type_id_cfstr(::CFCopyTypeIDDescription(cf_type_id));
          std::string cf_type_id_str;
          llvm::errs() << "The Info.plist key \"CFBundleVersion\" is a "
                       << cf_type_id_cfstr.UTF8(cf_type_id_str)
                       << "but it should be a string in: " << exe_path << ".\n";
        }
      }
      cf = bundle.GetValueForInfoDictionaryKey(
          CFSTR("CFBundleShortVersionString"));
      if (cf != NULL) {
        cf_type_id = ::CFGetTypeID(cf);
        if (::CFGetTypeID(cf) == ::CFStringGetTypeID())
          CFString::UTF8((CFStringRef)cf, bundleShortVersionStr);
        else {
          CFString cf_type_id_cfstr(::CFCopyTypeIDDescription(cf_type_id));
          std::string cf_type_id_str;
          llvm::errs() << "The Info.plist key \"CFBundleShortVersionString\" is"
                       << "a " << cf_type_id_cfstr.UTF8(cf_type_id_str)
                       << ", but it should be a string in: " << exe_path
                       << ".\n";
        }
      }
      OmitShortVersion = bundleShortVersionStr.empty();
    }
  }

#endif
  if (bundleVersionStr.empty())
    bundleVersionStr = "1";

  if (bundleShortVersionStr.empty() && !OmitShortVersion)
    bundleShortVersionStr = "1.0";
}
