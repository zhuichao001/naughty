from distutils.core import setup, Extension

module1 = Extension('shm_open',
        define_macros = [('MAJOR_VERSION', '1'),
            ('MINOR_VERSION', '0')],
        include_dirs = ['/usr/local/include'],
        libraries = ['rt'],
        library_dirs = ['/usr/local/lib'],
        sources = ['shm_open.c'])

setup (name = 'shm_open',
        version = '1.0',
        description = 'This is a demo package',
        author = 'Martin v. Loewis',
        author_email = 'martin@v.loewis.de',
        url = 'https://docs.python.org/extending/building',
        long_description = ''' This is really just a demo package.  ''', 
        ext_modules = [module1])
