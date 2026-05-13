#!/bin/sh

set -eu

REPO_OWNER="Saku0512"
REPO_NAME="calc-c"
REPO_BRANCH="${REPO_BRANCH:-main}"
INSTALL_DIR="${INSTALL_DIR:-$HOME/.local/bin}"
TARGET_NAME="calcc"
SOURCE_URL="https://raw.githubusercontent.com/$REPO_OWNER/$REPO_NAME/$REPO_BRANCH/main.c"

find_downloader() {
    if command -v curl >/dev/null 2>&1; then
        echo "curl"
        return
    fi

    if command -v wget >/dev/null 2>&1; then
        echo "wget"
        return
    fi

    echo "Error: curl or wget is required." >&2
    exit 1
}

find_compiler() {
    for compiler in cc gcc clang; do
        if command -v "$compiler" >/dev/null 2>&1; then
            echo "$compiler"
            return
        fi
    done

    echo "Error: a C compiler (cc, gcc, or clang) is required." >&2
    exit 1
}

download_source() {
    downloader="$1"
    output_file="$2"

    if [ "$downloader" = "curl" ]; then
        curl -fsSL "$SOURCE_URL" -o "$output_file"
        return
    fi

    wget -qO "$output_file" "$SOURCE_URL"
}

main() {
    downloader="$(find_downloader)"
    compiler="$(find_compiler)"
    tmp_dir="$(mktemp -d)"
    source_file="$tmp_dir/main.c"
    target_file="$INSTALL_DIR/$TARGET_NAME"

    trap 'rm -rf "$tmp_dir"' EXIT INT TERM

    mkdir -p "$INSTALL_DIR"

    echo "Downloading $TARGET_NAME from $REPO_OWNER/$REPO_NAME..."
    download_source "$downloader" "$source_file"

    echo "Building $TARGET_NAME with $compiler..."
    "$compiler" -Wall -Wextra -pedantic -std=c11 "$source_file" -o "$target_file"

    echo "Installed to $target_file"
    echo "Run it with: $TARGET_NAME"
}

main "$@"
