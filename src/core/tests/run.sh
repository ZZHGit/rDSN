#!/bin/bash

for test_case in `ls config-test*.ini`; do
    echo "============ run dsn.core.tests $test_case ============"
    ./clear.sh
    ./dsn.core.tests ${test_case} <command.txt
    if [ $? -ne 0 ]; then
        echo "run dsn.core.tests $test_case failed"
        echo "---- ls ----"
        ls -l
        if find . -name log.1.txt; then
            echo "---- tail -n 100 log.1.txt ----"
            tail -n 100 `find . -name log.1.txt`
        fi
        if [ -f core ]; then
            echo "---- gdb ./dsn.core.tests core ----"
            gdb ./dsn.core.tests core -ex "thread apply all bt" -ex "set pagination 0" -batch
        fi
        exit -1
    fi
done

echo "============ done ============"

