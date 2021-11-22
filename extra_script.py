Import('env')
from os.path import join, realpath, dirname

# pass flags to a global build environment (for all libraries, etc)
global_env = DefaultEnvironment()
script_dir = dirname(realpath('__file__'))

# Print environment for debugging
#print(env.Dump())