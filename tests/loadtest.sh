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
pip install pytest > /dev/null 2>/dev/null
pip install pytest_yaml > /dev/null 2> /dev/null
if [ -f conftest.py ] ; then
    ##pytest conftest.py testsuit.py
   # pytest conftest.py test_command.yml
   ./execpython.sh "pytest conftest.py" $1
else
#     pytest tests/conftest.py tests/testsuit.py
     pytest tests/conftest.py tests/test_yml/test_cmd.yml
fi
deactivate
if [ -f output.gv ] ; then
    rm output.gv
fi
if [ -f tests/output.gv ] ; then
    rm tests/output.gv
fi
rm -rf env/
rm -rf tests/__pycache__ __pycache__
rm -rf tests/.pytest_cache/ .pytest_cache/
rm -rf .42sh_history tests/.42sh_history
