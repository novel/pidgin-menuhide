# vim:ft=python

import Utils
from Configure import conf

VERSION = '0.1.0'
APPNAME = '3.14dgin-menuhide'

srcdir = '.'
blddir = 'output'

def set_options(opt):
    pass

def configure(conf):
    conf.env["APPNAME"] = APPNAME
    conf.env["VERSION"] = VERSION

    conf.check_tool('gcc')
    if not conf.env['CC']:
        conf.fatal('gcc not found')

    conf.check_cfg(atleast_pkgconfig_version='0.0.0')
    conf.check_cfg(package='pidgin', uselib_store='PIDGIN', args='--cflags --libs')

def build(bld):
    obj = bld.new_task_gen('cc', 'sharedlib')
    obj.source = 'menuhide.c'
    obj.target = 'menuhide'
    obj.uselib = 'PIDGIN'
