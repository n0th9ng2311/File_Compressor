# File Compressor

## Table of Contents
- [Overview](#overview)
- [Supported File Formats](#supported-file-formats)
- [Compression performance](#compression-performance)
- [Compressed formats](#compressed-formats)
- [Project structure](#project-structure)
- [Installation and Usage](#installation-and-usage) 



## Overview
The File Compression is a high-performance, multi-format compression tool designed to efficiently reduce file sizes across various formats while maintaining data integrity. This professional-grade solution implements specialized compression algorithms tailored to each file type's unique characteristics.

---

## Supported File Formats

| File Type | Extension  | Description                        |
|-----------|------------|------------------------------------|
| TXT       | .txt       | Plain text files                   |
| BMP       | .bmp       | Bitmap image files                 |
| TIFF      | .tiff      | Tagged Image File Format           |
| PSD       | .psd       | Adobe Photoshop Document           |
| LOG       | .log       | Log files                          |
| CSV       | .csv       | Comma-separated values files       |
| WAV       | .wav       | Waveform Audio File Format         |
| XML       | .xml       | Extensible Markup Language files   |
| AIFF      | .aiff      | Audio Interchange File Format      |
| JSON      | .json      | JavaScript Object Notation files   |

---

## Compression Performance

| File Type | Compression Ratio     | External Library  |
|-----------|-----------------------|-------------------|
| TXT       | 60-70% reduction      | Zlib              |
| BMP       | 50-85% reduction      | stb               |
| TIFF      | 40-75% reduction      | LibTIFF           |
| PSD       | 30-60% reduction      | Zlib              |
| LOG       | 65-80% reduction      | Zlib              |
| CSV       | 70-85% reduction      | Zlib              |
| WAV       | 85-95% reduction      | LAME & AudioFile  |
| XML       | 60-75% reduction      | Zlib              |
| AIFF      | 80-90% reduction      | LAME & AudioFile  |
| JSON      | 65-80% reduction      | Zlib              |

---

## Compressed Formats

| Original Format | Compressed Format      | Extension |
|-----------------|------------------------|-----------|
| TXT             | Compressed Text        | .z       |
| BMP             | Compressed Bitmap      | .png      |
| TIFF            | Compressed TIFF        | .gz       |
| PSD             | Compressed Photoshop   | .psd      |
| LOG             | Compressed Log         | .gz       |
| CSV             | Compressed CSV         | .gz       |
| WAV             | Compressed Audio       | .mp4      |
| XML             | Compressed XML         | .gz       |
| AIFF            | Compressed AIFF        | .mp4      |
| JSON            | Compressed JSON        | .gz       |

---


## Project Structure
```
file_comp/
│
├── comp_algo/ # All compression algorithms implemented here
│ ├──aiff_/
│ │ │ ├── aiff_comp.cpp
│ │ │ ├── aiff_comp.h
│ ├──bmp_/
│ │ │ ├── bmp_comp.cpp
│ │ │ ├── bmp_comp.h
│ ├──csv_/
│ │ │ ├── csv_comp.cpp
│ │ │ ├── csv_comp.h
│ ├──.....other formats/
│ 
├── external/ # External libraries
│ ├── audioFile/
│ ├── lame/
│ ├── libtiff/
│ ├── stb/
│ └── zlib-1.3.1/
│
├── hdr/ # Header Files
│ ├── all_comp_algo.h
│ ├── common.h
│ ├── file_comp.h
|
├── src/ # Source Files
│ ├── file_comp.cpp
│ ├── main.cpp
|
├── .gitignore
└──  CMakeLists.txt
```
---



## Installation and Usage

### Building from Source
- Clone the repo\
  `git clone https://github.com/n0th9ng2311/File_Compressor.git`\
  `cd c-FileManager-cmd-based-`

- run CMake to build files\
  `<path to parent dir>\File_Compressor\file_comp\`\
  `mkdir build`\
  `cd build`\
  `cmake ..`\

  - then\
  `cmake --build .`\

  - once the build is done\
  `.\file_comp.exe`
  


