#! /usr/bin/env python

from Synopsis.process import process
from Synopsis.Processor import Processor, Parameter, Composite
from Synopsis import AST
from Synopsis.Parsers import Cxx
from Synopsis.Processors import *
from Synopsis.Processors import Comments
from Synopsis.Processors.Comments.Filter import Filter
from Synopsis.Formatters import Dump
from Synopsis.Formatters import HTML
from Synopsis.Formatters.HTML.Views import *
from Synopsis.Formatters import SXR

parser = Cxx.Parser(primary_file_only = False,
                    syntax_prefix = 'links/',
                    xref_prefix = 'xref/')

xref = XRefCompiler(prefix='xref/')    # compile xref dictionary

linker = Linker(# filter out header guards
                MacroFilter(pattern = r'^boost_(.*)_hpp_$'),
                # filter out 'detail' namespace
                ModuleFilter(modules = [('boost', 'xml', 'dom', 'detail')]),
                # translate documentation
                Comments.Translator(markup = 'rst',
                                    filter = Comments.SSDFilter(),
                                    processor = Comments.Grouper()))

html = HTML.Formatter(title = 'Boost XML Reference Manual',
                      content = [Scope(),
                                 FileDetails(),
                                 InheritanceTree(),
                                 InheritanceGraph(),
                                 Source(prefix = 'links'),
                                 NameIndex(),
                                 XRef(xref_file = 'bxl.xref')])

sxr = SXR.Formatter(url = '/sxr.cgi',
                    src_dir = '../src',
                    xref_prefix='xref',
                    syntax_prefix='links')

process(parse = parser,
        xref = xref,
        link = linker,
        dump = Dump.Formatter(),
        html = html,
        sxr = sxr)
