# 1. Ssveptetris

脑空俄罗斯方块

- [1. Ssveptetris](#1-ssveptetris)
- [2. Prerequistes](#2-prerequistes)
  - [2.1. Install packages](#21-install-packages)
  - [2.2. Install brainflow](#22-install-brainflow)
- [3. Build](#3-build)
  - [3.1. Start Application](#31-start-application)

# 2. Prerequistes

## 2.1. Install packages

```bash
pacman -S qt6-base qt6-serialport
```

## 2.2. Install brainflow

# 3. Build

```bash
mkdir build
cd build
qmake6 ..
make -j4
```

## 3.1. Start Application

```bash
./SsvepTetris
```
