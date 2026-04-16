#!/usr/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"
EXERCISE_DIR="${ROOT_DIR}/exercises/20_mybash"

cd "${EXERCISE_DIR}" || exit 1
./bin/mybash "${SCRIPT_DIR}/mybash_cmd.sh"
