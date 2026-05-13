# calc-c

Simple calculator CLI written in C.

## Install

```bash
curl -fsSL https://raw.githubusercontent.com/Saku0512/calc-c/main/install.sh | sh
```

By default this installs `calcc` to `~/.local/bin`.

## Build

```bash
make
```

## Usage

```bash
make run
```

Enter expressions in this format:

```text
10+5
8/2
1+2*3
(1+2)*3
```

Type `quit` to exit.

Clean the built binary with:

```bash
make clean
```
