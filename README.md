# Canonical assignment to fetch releases

## Prerequisites

- cmake
- vcpkg (added as submodule under `external/vcpkg`)

## Build instructions

### for all platforms:

update submodules to fetch `vcpkg`.

```
git submodule update --init --recursive
```

### Windows

I used both Visual Studio 2022 Preview and CLion (using MSVC) as IDE. The most comfortable option in
Windows would be using VS or an IDE that supports cmake.

Nevertheless, I implemented a very basic build script. To build the application using `build.ps1`,
either you need to add `cmake` to `$PATH`, or _(assuming you installed all the c++ dev requirements
in Visual Studio Installer)_, just open Developer Powershell for VS.

- cd into repo.
- update submodules (to be sure that correct version of `vcpkg` fetched)

```powershell
git submodule update --init --recursive
```

- then run `build.ps1`

```powershell
PS C:\code\canonical-simple-meta-parser> ./build.ps1
```

default build folder is `build`. You can change it directly in the scirpt. I'm directly executing the cli here:

```powershell
PS C:\code\canonical-simple-meta-parser> .\build\src\cli\Debug\meta_fetcher.exe -h
A tool to fetch Ubuntu cloud release info
Usage:
  meta_fetcher [OPTION...]

  -l, --list               Return a list of all currently supported Ubuntu
                           releases
  -L, --lts                Return the current Ubuntu LTS version
  -r, --release_title arg  Return the sha256 of disk1.img for a given
                           Ubuntu release (default: "")
  -h, --help               Print usage
PS C:\code\canonical-simple-meta-parser> .\build\src\cli\Debug\meta_fetcher.exe -l
Release: 20.04 LTS, Version: 20.04, LTS: true, sha256: 266663b10f788f784a425873086051910084bc08c059bf8e4a9490ce306f8d7e
Release: 22.04 LTS, Version: 22.04, LTS: true, sha256: de5e632e17b8965f2baf4ea6d2b824788e154d9a65df4fd419ec4019898e15cd
Release: 24.04 LTS, Version: 24.04, LTS: true, sha256: 32a9d30d18803da72f5936cf2b7b9efcb4d0bb63c67933f17e3bdfd1751de3f3
PS C:\code\canonical-simple-meta-parser> .\build\src\cli\Debug\meta_fetcher.exe -L
Current LTS Release: 24.04 LTS, Version: 24.04
PS C:\code\canonical-simple-meta-parser> .\build\src\cli\Debug\meta_fetcher.exe -r "20.04 LTS"
image sha256 for 20.04 LTS: 266663b10f788f784a425873086051910084bc08c059bf8e4a9490ce306f8d7e
PS C:\code\canonical-simple-meta-parser> .\build\src\cli\Debug\meta_fetcher.exe -r "24.04 LTS"
image sha256 for 24.04 LTS: 32a9d30d18803da72f5936cf2b7b9efcb4d0bb63c67933f17e3bdfd1751de3f3
PS C:\code\canonical-simple-meta-parser> .\build\src\cli\Debug\meta_fetcher.exe -r "10.04"
No image sha256 found for release: 10.04
PS C:\code\canonical-simple-meta-parser> .\build\src\cli\Debug\meta_fetcher.exe -r "10.04 LTS"
image sha256 for 10.04 LTS: 983b7be9dd1ed8c0b3118e309cdd2b2869b74ddcbb3e4c6c4c77e28e0a7b5e59
```

### Linux/Mac

