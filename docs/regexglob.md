# regexglob

Module for performing glob-like file matching using regular expressions.

## files needed

- [regexglob.hpp](../include/regexglob.hpp)
- [regexglob.cpp](../impl/regexglob.cpp)

## example

An imaginary filesystem:

```
root
│  file1.txt
│  file2.txt
|
└───dir1
│   │  file3.txt
│   │
│   └───dir2
│       │  file4.txt
│       │  _file.txt
│
└───dir3
    │  file5.txt
    │  _file.txt
```

```cpp
regexglob::set_preferred_separator('/');

std::vector<std::filesystem::path> matches =
  regexglob::fmatch("root", "file[2-5]\\.txt");

// `matches`:
//    root/file2.text
//    root/dir1/file3.txt
//    root/dir1/dir2/file4.txt
//    root/dir3/file5.txt
```