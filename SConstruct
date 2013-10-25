
libraryPath = '/usr/lib'


sources = ['src/Sequence.cpp', 'src/Sequence.hpp']
test_sources = ['test/main-test.cpp','test/Sequence-test.cpp']
test_sources.extend(sources)

#prod = Environment()
test = Environment()

#prod.Append(CCFLAGS = ['-O3'])
#prod.Append(LIBPATH = [libraryPath])
test.Append(LIBPATH = [libraryPath])
test.Append(LINKFLAGS = ['-lboost_unit_test_framework'])

#mainSources = sources
#mainSources.append('src/FireflyAssembler-main.cpp')
#program = prod.Program('bin/FireflyAssembler', mainSources)
#Default(program)

test.Append(CCFLAGS = ['-g', '-O0'])
mainTest = test.Program(target='test/main-test', source=test_sources)
# Depend on the runner to ensure that it's built before running it.
mainTestAlias = Alias('test', [mainTest], mainTest[0].path)
# Simply required.  Without it, 'test' is never considered out of date.
AlwaysBuild(mainTestAlias)
