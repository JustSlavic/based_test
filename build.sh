#!/bin/bash

set -e
os_name=$(uname -s)

PROJECT=based_test
COMPILER=g++
STANDARD=c++17
COMPILE_COMMANDS_FILE=compile_commands.json

case $os_name in
    Darwin)
        DEBUGGER=lldb
        ;;
    Linux)
        DEBUGGER=gdb
        ;;
    *)
        echo "Unrecognazied os name ($os_name)"
        ;;
esac


mkdir -p bin

if [ $# -eq 0 ]; then
    command="build"
else
    if [ "$1" = "debug" ]; then
        command="build"
        subcommand="debug"
    else
        command="$1"
        subcommand="$2"
    fi
fi

function build_() {
    $($1)
    if [ $? -eq 0 ]; then
        echo "[$1]... Success"
    fi
}

function build() {
    if [ "$command" = "pvs_analyze" ]; then
        COMPILE_DB_JSON="-MJ $COMPILE_COMMANDS_FILE"
    fi
    if [ "$subcommand" = "debug" ]; then
        # @todo: make way to remove this since I need only truncate function
        LIBS="-lm"
        DEBUG="-DDEBUG -g3"
        WARNINGS="-Wall"
    else
        LIBS="-lm"
        WARNINGS="-Wall -Werror"
    fi

    case $os_name in
        Darwin | Linux)
            build_command="$COMPILER main.cpp -o bin/test -std=$STANDARD -I based $WARNINGS $DEBUG"
            build_ "$build_command"

            ;;
        *)
            echo "Unrecognazied os name ($os_name)"
            ;;
    esac
}

function run() {
    if [ "$command" = "run" ] && [ "$subcommand" = "debug" ]; then
        ( cd www && $DEBUGGER ../bin/webspider )
    else
        ( cd www && ../bin/webspider )
    fi
}

function pvs_analyze() {
    build

    db_contents=$(cat $COMPILE_COMMANDS_FILE)
    db_contents_except_last_comma=${db_contents%?}
    echo "[$db_contents_except_last_comma]" > $COMPILE_COMMANDS_FILE

    pvs-studio-analyzer analyze -o pvs_output.log -j2
    plog-converter -a GA:1,2 -t json -o pvs_report.json pvs_output.log
    less pvs_report.json
}


function contains_in() {
    for it in $2; do
        if [ "$1" = "$it" ]; then
            return 0
        fi
    done
    return 1
}

if contains_in $command "build run pvs_analyze"; then
    "$command"
else
    echo "Could not recognize command '$command'"
fi

