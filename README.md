This is an attempt at compiling C++ to a Starcraft map (and its incredibly limited operations).

Come back later.

Originally derived from the Nios2 Target.

## Developer Setup
### Requirements
- LLVM source code
- CMake

### Instructions
#### Setting up
1. Clone [llvm](https://github.com/llvm-mirror/llvm) somewhere.
2. Clone *this* repository into `llvm/lib/Target/StarcraftEUD`.
3. Add `StarcraftEUD` to *subdirectories* in `llvm/lib/Target/LLVMBuild.txt`.
4. Add `StarcraftEUD` to *LLVM_ALL_TARGETS* in `llvm/CMakeLists.txt`.

#### Building
1. Go to the root llvm directory.
2. `mkdir build`
3. `cd build`
4. `cmake ../`
5. `cmake --build .`

It should now fully compile (including the StarcraftEUD Target).
