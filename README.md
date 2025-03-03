# 🛠️ MemDumper ![Linux](https://img.shields.io/badge/Platform-Linux-only-blue) ![Alpha](https://img.shields.io/badge/Status-Alpha-red)

**MemDumper** is an utility for analyzing Linux process memory using the native /proc/ interface. It provides an interactive view of memory maps and loaded libraries.

![Screenshot](screenshot.png)

## ✨ Features
- 🖥️ Interactive ncurses interface
- 📊 Structured memory map visualization
- 🎨 Color scheme
- 📁 Keyboard navigation support

## 📦 Prerequisites
- Linux system (kernel 4.4+)
- Ncurses library

## 🚀 Prerequisites
```bash
# Clone the repository
https://github.com/ItalianG0urmet/linux-process-dumper-gui.git
cd MemDumper

# Compile the project
mkdir build && cd build
cmake .. && make

# Run the application
./MemDumper -p <PID> [-f <filter>]
```
## 🕹️ Usage
| Option  | Description                | 	Example                 |
|---------|----------------------------|--------------------------|
| `-p`    | 	Specify the target PID    | `-p 1234`               |
| `-f`    |   Filter results (optional) | `-f "libc"`             |
