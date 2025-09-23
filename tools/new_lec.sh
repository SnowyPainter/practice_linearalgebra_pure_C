#!/usr/bin/env bash
set -e
name="$1"
if [ -z "$name" ]; then
  echo "Usage: tools/new_lec.sh lecXX_name"
  exit 1
fi
dst="lessons/$name"
if [ -d "$dst" ]; then
  echo "already exists: $dst"; exit 1
fi
mkdir -p "$dst"
cp lessons/template/main.c "$dst/main.c"
echo "Created $dst (edit main.c and build)"