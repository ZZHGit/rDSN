#!/bin/bash

./clear.sh
./dsn.tests config-test.ini
if [ $? -ne 0 ]; then
    echo "run dsn.tests failed"
    echo "---- ls ----"
    ls -l
    if find . -name log.1.txt; then
        echo "---- tail -n 100 log.1.txt ----"
        tail -n 100 `find . -name log.1.txt`
    fi
    if [ -f core ]; then
        echo "---- gdb ./dsn.tests core ----"
        gdb ./dsn.tests core -ex "thread apply all bt" -ex "set pagination 0" -batch
    fi
    exit -1
fi

