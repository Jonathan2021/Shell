# content of conftest.py
import pytest
import subprocess


def pytest_collect_file(parent, path):
    if path.ext == ".yml" and path.basename.startswith("test"):
        return YamlFile(path, parent)


class YamlFile(pytest.File):
    def collect(self):
        import yaml  # we need a yaml parser, e.g. PyYAML

        raw = yaml.safe_load(self.fspath.open())
        for name, spec in sorted(raw.items()):
            if spec['type'] == "output_diff":
                yield OutputDiffItem(name, self, spec)
            elif  spec['type'] == "external file":
                yield ExternalFileItem(name, self, spec)
            else:
                yield CommandDiffItem(name, self, spec)



class YamlItem(pytest.Item):
    def __init__(self, name, parent, spec):
        super(YamlItem, self).__init__(name, parent)
        self.spec = spec
        self.command = str.encode(self.spec['command'])
        if 'expected' in self.spec:
            self.expected = self.spec['expected']

    def runtest(self):
        process = subprocess.Popen(["bash"],
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                stdin=subprocess.PIPE)
        out, err = process.communicate(input=self.command)
        process.kill()

        if 'stdout' in self.expected:
            if self.expected['stdout'] != out:
                raise YamlException ('stdout', self.expected['stdout'], out)
        elif out:
            raise YamlException ('stdout', b'empty', out)
        
        if 'stderr' in self.expected:
            if self.expected['stderr'] != err:
                raise YamlException ('stderr', self.expected['stderr'], err)
        elif err:
            raise YamlException ('stderr', b'empty', err)
        


    def repr_failure(self, excinfo):
        """ called when self.runtest() raises an exception. """
        if isinstance(excinfo.value, YamlException):
            return "\n".join(
                [
                    "usecase execution failed",
                    "   spec failed: %r: %r" % excinfo.value.args[1:3],
                    "   no further details known at this point.",
                ]
            )

    def reportinfo(self):
        return self.fspath, 0, "usecase: %s" % self.name



class CommandDiffItem(YamlItem):
    def __init__(self, name, parent, spec):
        super().__init__(name, parent, spec)
        bash = subprocess.Popen(['bash'],
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                stdin=subprocess.PIPE)
        out, err = bash.communicate(input=self.command)
        self.expected = {}
        if out:
            self.expected['stdout'] = out
        if err:
            self.expected['stderr'] = err
        self.expected['rvalue'] = bash.returncode


class OutputDiffItem(YamlItem):
     def __init__(self, name, parent, spec):
        super().__init__(name, parent, spec)
        for item in self.expected:
            if item == "rvalue":
                self.expected["rvalue"] = int(self.expected["rvalue"])
                continue
            self.expected[item] = str.encode(self.expected[item])



class YamlException(Exception):
    """ custom exception for error reporting. """
    def __init__(self, expected_type, expected_value, output_value):
        self.expected_type = expected_type
        self.expected_value = expected_value
        self.output_value = output_value

