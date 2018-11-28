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
     pytest src/testsuit/conftest.py src/testsuit/testsuit.py
     pytest src/testsuit/conftest.py src/testsuit/test_command.yml
fi
deactivate
rm -rf env/
