pidcoll C++ small test project
===============================

# Project layout
```
├── CMakeLists.txt
├── README.md
├── include      # Header files
│   └── pidcoll.h
├── src          # Source files
│   └── pidcoll.cpp
├── tests        # Various tests
│   ├── integration
│   ├── performance
│   └── unit
├── conanfile.py # Conan package manager file
```

# Due to dependencies are managed by Conan, you need to install it first
```bash
pip install -r requirements.txt
```

# Build
```bash
conan install . --build=missing
cmake --preset conan-debug
cmake --build --preset conan-debug
```

# Run tests
```bash
cmake --build --preset conan-debug -t test
```


>📝
> Based on the template https://github.com/valden/cpptest
