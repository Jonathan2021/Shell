import subprocess

def test_echo():
    ref = subprocess.Popen("bash", stdout=subprocess.PIPE,
            stderr=subprocess.PIPE, stdin=subprocess.PIPE)

    mysh =  subprocess.Popen("42sh", stdout=subprocess.PIPE,
            stderr=subprocess.PIPE, stdin=subprocess.PIPE)

    command = str.encode("ls")
    refour , referr = ref.communicate(input=command)
    myour , myerr = mysh.communicate(input=command)

    assert(myour == refour)
