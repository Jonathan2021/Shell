#!/bin/bash
python3 -m venv env
source env/bin/activate
#pip install requirement.txt
pip install pytest
pip install pytest_yaml
if [ -f conftest.py ] ; then
    pytest conftest.py testsuit.py
    pytest conftest.py test_command.yml
else
     pytest tests/conftest.py tests/testsuit.py
     pytest tests/conftest.py tests/test_command.yml
fi
deactivate
rm -rf env/
rm -rf tests/__pycache__ __pycache__
