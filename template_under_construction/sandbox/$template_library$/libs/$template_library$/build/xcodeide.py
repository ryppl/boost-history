# template script

if template.options.get_boolean('xcodeide', False):

    log.message('Processing Xcode IDE template.')
    
else:

    template.ignore_subdirectory('xcodeide')
