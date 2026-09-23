# Compiling C

C  code has to be compiled into an executable before it can run. The general idea is:

```bash
gcc file.c -o program.exe
```

This tells GCC to compile `file.c` and create `program.exe`.

## Basic GCC

For a normal C file with no extra libraries:

```bash
gcc main.c -o main.exe
```

Then run:

```bash
./main.exe
```

On Windows PowerShell:

```powershell
.\main.exe
```

## Using an `executables` folder

You can keep the generated `.exe` files separate:

```bash
gcc main.c -o executables/main.exe
```

Just make sure the `executables` folder already exists.

## Compiling with libraries

Some projects use external or Windows libraries. These are added at the end of the GCC command using `-l`.

For example, the raylib app uses:

```bash
gcc app.c -o executables/app.exe -lraylib -lopengl32 -lgdi32 -lwinmm -lcomdlg32
```

The libraries here are:

* `raylib` — UI and graphics
* `opengl32` — OpenGL on Windows
* `gdi32` — Windows graphics functions
* `winmm` — Windows multimedia functions used by raylib
* `comdlg32` — Windows file picker

## raylib

`raylib.h` is not part of standard C.

If using MSYS2 MinGW64, raylib can be installed with:

```bash
pacman -S mingw-w64-x86_64-raylib
```

After that, the MSYS2 GCC installation should be able to find:

```c
#include "raylib.h"
```

Make sure VS Code is using the same GCC installation where raylib was installed.

You can check with:

```bash
where gcc
```

Ideally it should point to something similar to:

```text
C:\msys64\mingw64\bin\gcc.exe
```

## VS Code `tasks.json`

Instead of writing the GCC command manually every time, VS Code can run it automatically using `.vscode/tasks.json`.

Example used in this project:

```json
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "Build C",
      "type": "shell",
      "command": "gcc",
      "args": [
        "${file}",
        "-o",
        "executables/${fileBasenameNoExtension}.exe",
        "-lraylib",
        "-lopengl32",
        "-lgdi32",
        "-lwinmm",
        "-lcomdlg32"
      ],
      "group": {
        "kind": "build",
        "isDefault": true
      }
    }
  ]
}
```

`${file}` means the currently open C file.

`${fileBasenameNoExtension}` means its filename without `.c`.

For example:

```text
app.c
```

becomes:

```text
executables/app.exe
```

The default VS Code build shortcut is:

```text
Ctrl + Shift + B
```

## General idea

The basic pattern is always:

```text
C source code
    ↓
GCC
    ↓
Optional libraries
    ↓
.exe
```

Simple project:

```bash
gcc program.c -o program.exe
```

Project using raylib:

```bash
gcc program.c -o program.exe -lraylib -lopengl32 -lgdi32 -lwinmm
```

Project using raylib and the Windows file picker:

```bash
gcc program.c -o program.exe -lraylib -lopengl32 -lgdi32 -lwinmm -lcomdlg32
```
