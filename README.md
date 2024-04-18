pidcoll C++ small test project
===============================

# Task

There is a binary protocol which is used to communicate between two devices. The protocol is based on messages. Each message has a header and a body. 
Header is 8 bytes long. Body is 184 bytes long. 
The header contains the following fields:

| Field | Size (bits) | Description                                               |
|-------|-------------|-----------------------------------------------------------|
| h1    | 8           |                                                           |
| h2    | 1           |                                                           |
| h3    | 1           |                                                           |
| h4    | 3           |                                                           |
| PID   | 13          | Sequential message id                                     |
| h5    | 2           |                                                           |
| h6    | 2           |                                                           |
| CC    | 4           | Collision counter                                         |
|---------------------------------------------------------------------------------|

The body content is not important for this task.
PID is a sequential message id. It is a 13-bit field. The value of PID is incremented by 1 for each new message. In case of next PID(n) != PID(n-1) + 1, it is considered as a collision. In this case, the collision counter (CC) should be incremented by 1.
Write a `size_t checkPID(const char * buf, size_t size)` function which should count the number of collisions in the given buffer. The buffer contains a sequence of messages. The size of the buffer is not limited. The function should return the number of collisions.


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
