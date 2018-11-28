#!/bin/bash
if [ -d tests ] ; then
    cd tests > /dev/null
fi

if [ ! -f "../42sh" ] ; then
    echo "NO executable 42sh"
    exit 0
fi
python3 -m venv env > /dev/null
source env/bin/activate
#pip install requirement.txt
pip install pytest > /dev/null
pip install pytest_yaml > /dev/null
if [ -f conftest.py ] ; then
    ##pytest conftest.py testsuit.py
    pytest conftest.py test_command.yml
else
#     pytest tests/conftest.py tests/testsuit.py
     pytest tests/conftest.py tests/test_command.yml
fi
deactivate
if [ -f output.gv ] ; then
    rm output.gv
else
    rm tests/output.gv
fi
rm -rf env/
rm -rf tests/__pycache__ __pycache__
