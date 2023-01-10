#!/bin/bash -e

OPTIMIZATION_LEVEL=0
EXTRA_PASSES=

BOLD_PREFIX="\033[1m"
BOLD_SUFFIX="\033[0m"

function print_usage()
{
        echo -e \
"Usage: ./C_to_LLVM_IR.sh [${BOLD_PREFIX}-O${BOLD_SUFFIX}x]" \
"[${BOLD_PREFIX}--passes${BOLD_SUFFIX} \"xxx\" (comma separated)]" \
"${BOLD_PREFIX}-c${BOLD_SUFFIX} file"
}

while [[ $# -gt 0 ]]
do
        case $1 in
                -O*)
                        OPTIMIZATION_LEVEL=${1#*O}
                        shift
                        ;;
                -p|-passes)
                        EXTRA_PASSES=$2
                        shift
                        shift
                        ;;
                -p=*|-passes=*)
                        EXTRA_PASSES=${1#*=}
                        shift
                        ;;
                -c)
                        CSRC_FILE=$2
                        shift
                        shift
                        ;;
                *)
                        echo -e "Unrecognized argument: ${BOLD_PREFIX}$1${BOLD_SUFFIX}"
                        print_usage
                        exit -1
                        ;;
        esac
done

if [ -z ${CSRC_FILE} ]
then
        CSRC_FILE=$(realpath ${BASH_SOURCE[0]})
        CSRC_FILE=${CSRC_FILE/C_to_LLVM_IR.sh/main.c}
fi

EXTRA_OPT_PASSES=""

if [ ! -z ${EXTRA_PASSES} ]
then
        EXTRA_OPT_PASSES="-p=${EXTRA_PASSES}"
fi

CLANG_FLAGS="-O${OPTIMIZATION_LEVEL}"

if [ ${OPTIMIZATION_LEVEL} -eq "0" ]
then
        CLANG_FLAGS+=" -Xclang -disable-O0-optnone"
fi

LLVM_VERSION=${LLVM_VERSION:-16}

BC_FILE=${CSRC_FILE/.c/.bc}
LL_FILE=${CSRC_FILE/.c/.ll}

function print_and_exec()
{
        echo $@
        $@
}

print_and_exec clang-${LLVM_VERSION} ${CLANG_FLAGS} -emit-llvm -c ${CSRC_FILE} -o ${BC_FILE}
print_and_exec opt-${LLVM_VERSION} -S ${EXTRA_OPT_PASSES} ${BC_FILE} -o ${LL_FILE}
rm -f ${BC_FILE}
