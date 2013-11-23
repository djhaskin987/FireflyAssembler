import sys

mode = ARGUMENTS.get('mode','release')
if not (mode in ['debug','release']):
    sys.stderr.write("Mode '%s' is invalid. It must be either 'debug' or 'release'.\n" % mode)
    Exit(1)

Export('mode')

SConscript('test/SConscript', variant_dir='%s-test' % mode, duplicate=0)
SConscript('src/SConscript', variant_dir='%s-main' % mode, duplicate=0)
