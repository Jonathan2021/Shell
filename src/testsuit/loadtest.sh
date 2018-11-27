#!/bin/bash
python3 -m venv env
source env/bin/activate
#pip install requirement.txt
pip install pytest
pip install pytest_yaml
pytest conftest.py testsuit.py
pytest conftest.py test_command.yml
deactivate
rm -rf env/
