

# get the needed system libraries
m           = cc.find_library('m')
pthread     = cc.find_library('pthread')
rt          = cc.find_library('rt')
subunit     = cc.find_library('subunit')

# get the third party libraries
check       = dependency('check')

deps = [check, m, pthread, rt, subunit]

e = executable(
    'check_page',
    'check_page.c',
    include_directories: incdir,
    dependencies : deps,
    link_with : ezdblib)

test('check page', e, suite: 'page')
