import exporters
from Exporter import Exporter
from declarations import *
from enumerate import enumerate
from settings import *
from CodeUnit import CodeUnit
from EnumExporter import EnumExporter


#==============================================================================
# ClassExporter
#==============================================================================
class ClassExporter(Exporter):
    'Generates boost.python code to export a class declaration'
    

    def __init__(self, info, parser_tail=None):
        Exporter.__init__(self, info, parser_tail)
        # sections of code
        self.sections = {}
        # template: each item in the list is an item into the class_<...> 
        # section.
        self.sections['template'] = []  
        # constructor: each item in the list is a parameter to the class_ 
        # constructor, like class_<C>(...)
        self.sections['constructor'] = []
        # inside: everything within the class_<> statement        
        self.sections['inside'] = []        
        # scope: items outside the class statement but within its scope.
        # scope* s = new scope(class<>());
        # ...
        # delete s;
        self.sections['scope'] = []
        # declarations: outside the BOOST_PYTHON_MODULE macro
        self.sections['declaration'] = []
        self.sections['include'] = []
        # a list of Constructor instances
        self.constructors = []
        self.wrapper_generator = None
        # a list of code units, generated by nested declarations
        self.nested_codeunits = []


    def ScopeName(self):
        return _ID(self.class_.FullName()) + '_scope'


    def Name(self):
        return self.class_.FullName()


    def SetDeclarations(self, declarations):
        Exporter.SetDeclarations(self, declarations)
        decl = self.GetDeclaration(self.info.name)
        if isinstance(decl, Typedef):
            self.class_ = self.GetDeclaration(decl.type.name)
            if not self.info.rename:
                self.info.rename = decl.name
        else:
            self.class_ = decl
        self.public_members = \
            [x for x in self.class_.members if x.visibility == Scope.public]
        
        
    def Order(self):
        '''Return the TOTAL number of bases that this class has, including the
        bases' bases.  Do this because base classes must be instantialized
        before the derived classes in the module definition.  
        '''
        
        def BasesCount(classname):
            decl = self.GetDeclaration(classname)
            bases = [x.name for x in decl.bases]
            total = 0
            for base in bases:
                total += BasesCount(base)
            return len(bases) + total

        return BasesCount(self.class_.FullName())
                
    
    def Export(self, codeunit, exported_names):
        self.ExportBasics()
        self.ExportBases(exported_names)
        self.ExportConstructors()
        self.ExportVariables()
        self.ExportMethods()
        self.ExportVirtualMethods()
        self.ExportOperators()
        self.ExportNestedClasses(exported_names)
        self.ExportNestedEnums()
        self.Write(codeunit)


    def Write(self, codeunit):
        indent = self.INDENT
        boost_ns = namespaces.python
        pyste_ns = namespaces.pyste
        code = ''
        # begin a scope for this class if needed
        nested_codeunits = self.nested_codeunits
        needs_scope = self.sections['scope'] or nested_codeunits
        if needs_scope:
            scope_name = self.ScopeName()
            code += indent + boost_ns + 'scope* %s = new %sscope(\n' %\
                (scope_name, boost_ns)
        # export the template section
        template_params = ', '.join(self.sections['template'])
        code += indent + boost_ns + 'class_< %s >' % template_params
        # export the constructor section
        constructor_params = ', '.join(self.sections['constructor'])
        code += '(%s)\n' % constructor_params
        # export the inside section
        in_indent = indent*2
        for line in self.sections['inside']:
            code += in_indent + line + '\n' 
        # write the scope section and end it
        if not needs_scope:
            code += indent + ';\n'
        else:
            code += indent + ');\n'
            for line in self.sections['scope']:
                code += indent + line + '\n'
            # write the contents of the nested classes
            for nested_unit in nested_codeunits:
                code += '\n' + nested_unit.Section('module')
            # close the scope
            code += indent + 'delete %s;\n' % scope_name
            
        # write the code to the module section in the codeunit        
        codeunit.Write('module', code + '\n')
        
        # write the declarations to the codeunit        
        declarations = '\n'.join(self.sections['declaration'])
        for nested_unit in nested_codeunits:
            declarations += nested_unit.Section('declaration')
        if declarations:
            codeunit.Write('declaration', declarations + '\n')

        # write the includes to the codeunit
        includes = '\n'.join(self.sections['include'])
        for nested_unit in nested_codeunits:
            includes += nested_unit.Section('include')
        if includes:
            codeunit.Write('include', includes)


    def Add(self, section, item):
        'Add the item into the corresponding section'
        self.sections[section].append(item)

        
    def ExportBasics(self):
        'Export the name of the class and its class_ statement'
        self.Add('template', self.class_.FullName())
        name = self.info.rename or self.class_.name
        self.Add('constructor', '"%s"' % name)
        
        
    def ExportBases(self, exported_names):
        'Expose the bases of the class into the template section'        
        bases = self.class_.bases
        bases_list = []
        for base in bases:
            if base.visibility == Scope.public and base.name in exported_names:
                bases_list.append(base.name)
        if bases_list:
            code = namespaces.python + 'bases< %s > ' % \
                (', '.join(bases_list))
            self.Add('template', code)        


    def ExportConstructors(self):
        '''Exports all the public contructors of the class, plus indicates if the 
        class is noncopyable.
        '''
        py_ns = namespaces.python
        indent = self.INDENT
        
        def init_code(cons):
            'return the init<>() code for the given contructor'
            param_list = [p.FullName() for p in cons.parameters]
            min_params_list = param_list[:cons.minArgs]
            max_params_list = param_list[cons.minArgs:]
            min_params = ', '.join(min_params_list)
            max_params = ', '.join(max_params_list)
            init = py_ns + 'init< '
            init += min_params
            if max_params:
                if min_params:
                    init += ', '
                init += py_ns + ('optional< %s >' % max_params)
            init += ' >()'    
            return init
        
        constructors = [x for x in self.public_members if isinstance(x, Constructor)]
        self.constructors = constructors[:]
        # don't export the copy constructor if the class is abstract
        if self.class_.abstract:
            for cons in constructors:
                if cons.IsCopy():
                    constructors.remove(cons)
                    break
        if not constructors:
            # declare no_init
            self.Add('constructor', py_ns + 'no_init') 
        else:
            # write the constructor with less parameters to the constructor section
            smaller = None
            for cons in constructors:
                if smaller is None or len(cons.parameters) < len(smaller.parameters):
                    smaller = cons
            assert smaller is not None
            self.Add('constructor', init_code(smaller))
            constructors.remove(smaller)
            # write the rest to the inside section, using def()
            for cons in constructors:
                code = '.def(%s)' % init_code(cons) 
                self.Add('inside', code)
        # check if the class is copyable
        if not self.class_.HasCopyConstructor() or self.class_.abstract:
            self.Add('template', namespaces.boost + 'noncopyable')
            
        
    def ExportVariables(self):
        'Export the variables of the class, both static and simple variables'
        vars = [x for x in self.public_members if isinstance(x, Variable)]
        for var in vars:
            if self.info[var.name].exclude: 
                continue
            name = self.info[var.name].rename or var.name
            fullname = var.FullName() 
            if var.static:
                code = '%s->attr("%s") = %s;' % (self.ScopeName(), name, fullname)
                self.Add('scope', code)            
            else:
                if var.type.const:
                    def_ = '.def_readonly'
                else:
                    def_ = '.def_readwrite'
                code = '%s("%s", &%s)' % (def_, name, fullname)
                self.Add('inside', code)


    printed_policy_warnings = {}
    
    def CheckPolicy(self, m):
        'Warns the user if this method needs a policy'            
        def IsString(type):
            return type.const and type.name == 'char' and isinstance(type, PointerType)
        needs_policy = isinstance(m.result, (ReferenceType, PointerType))
        if IsString(m.result):
            needs_policy = False
        has_policy = self.info[m.name].policy is not None
        if needs_policy and not has_policy:
            warning = '---> Error: Method "%s" needs a policy.' % m.FullName()
            if warning not in self.printed_policy_warnings:
                print warning
                print 
                self.printed_policy_warnings[warning] = 1
            
    
    def ExportMethods(self):
        'Export all the non-virtual methods of this class'        
            
        def OverloadName(m):
            'Returns the name of the overloads struct for the given method'            
            return _ID(m.FullName()) + ('_overloads_%i_%i' % (m.minArgs, m.maxArgs))
        
        declared = {}
        def DeclareOverloads(m):
            'Declares the macro for the generation of the overloads'
            if not m.virtual:
                func = m.name
                code = 'BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(%s, %s, %i, %i)\n'
                code = code % (OverloadName(m), func, m.minArgs, m.maxArgs)
                if code not in declared:
                    declared[code] = True
                    self.Add('declaration', code)


        def Pointer(m):
            'returns the correct pointer declaration for the method m'
            # check if this method has a wrapper set for him
            wrapper = self.info[method.name].wrapper
            if wrapper:
                return '&' + wrapper.FullName()
            # return normal pointers to the methods of the class
            is_unique = self.class_.IsUnique(m.name)
            if is_unique:
                return '&' + method.FullName()
            else:
                return method.PointerDeclaration() 

        def IsExportable(m):
            'Returns true if the given method is exportable by this routine'
            ignore = (Constructor, ClassOperator, Destructor)
            return isinstance(m, Method) and not isinstance(m, ignore) and not m.virtual        
        
        methods = [x for x in self.public_members if IsExportable(x)]        
        
        for method in methods:
            if self.info[method.name].exclude:
                continue # skip this method

            name = self.info[method.name].rename or method.name
            
            # warn the user if this method needs a policy and doesn't have one
            self.CheckPolicy(method)            
            
            # check for policies
            policy = self.info[method.name].policy or ''
            if policy:
                policy = ', %s%s()' % (namespaces.python, policy.Code())
            # check for overloads
            overload = ''
            if method.minArgs != method.maxArgs:
                # add the overloads for this method
                overload_name = OverloadName(method)
                DeclareOverloads(method)
                overload = ', %s%s()' % (namespaces.pyste, overload_name)
        
            # build the .def string to export the method
            pointer = Pointer(method)
            code = '.def("%s", %s' % (name, pointer)
            code += policy
            code += overload
            code += ')'
            self.Add('inside', code)
            # static method
            if method.static:
                code = '.staticmethod("%s")' % name
                self.Add('inside', code)
            # add wrapper code if this method has one
            wrapper = self.info[method.name].wrapper
            if wrapper and wrapper.code:
                self.Add('declaration', wrapper.code)


    def ExportVirtualMethods(self):        
        # check if this class has any virtual methods
        has_virtual_methods = False
        for member in self.class_.members:
            if type(member) == Method and member.virtual:
                has_virtual_methods = True
                break

        if has_virtual_methods:
            generator = _VirtualWrapperGenerator(self.class_, self.info)
            self.Add('template', generator.FullName())
            for definition in generator.GenerateDefinitions():
                self.Add('inside', definition)
            self.Add('declaration', generator.GenerateVirtualWrapper(self.INDENT))
            

    # operators natively supported by boost
    BOOST_SUPPORTED_OPERATORS = '+ - * / % ^ & ! ~ | < > == != <= >= << >> && || += -='\
        '*= /= %= ^= &= |= <<= >>='.split()
    # create a map for faster lookup
    BOOST_SUPPORTED_OPERATORS = dict(zip(BOOST_SUPPORTED_OPERATORS, range(len(BOOST_SUPPORTED_OPERATORS))))

    # a dict of operators that are not directly supported by boost, but can be exposed
    # simply as a function with a special signature
    BOOST_RENAME_OPERATORS = {
        '()' : '__call__',
    }

    # converters which has a special name in python
    SPECIAL_CONVERTERS = {
        'double' : '__float__',
        'float' : '__float__',
        'int' : '__int__',
        'long' : '__long__',
    }
        
    
    def ExportOperators(self):
        'Export all member operators and free operators related to this class'
        
        def GetFreeOperators():
            'Get all the free (global) operators related to this class'
            operators = []
            for decl in self.declarations:
                if isinstance(decl, Operator):
                    # check if one of the params is this class
                    for param in decl.parameters:
                        if param.name == self.class_.FullName():
                            operators.append(decl)
                            break
            return operators

        def GetOperand(param):
            'Returns the operand of this parameter (either "self", or "other<type>")'
            if param.name == self.class_.FullName():
                return namespaces.python + 'self'
            else:
                return namespaces.python + ('other< %s >()' % param.name)


        def HandleSpecialOperator(operator):
            # gatter information about the operator and its parameters
            result_name = operator.result.name                        
            param1_name = ''
            if operator.parameters:
                param1_name = operator.parameters[0].name
                
            # check for str
            ostream = 'basic_ostream'
            is_str = result_name.find(ostream) != -1 and param1_name.find(ostream) != -1
            if is_str:
                namespace = namespaces.python + 'self_ns::'
                self_ = namespaces.python + 'self'
                return '.def(%sstr(%s))' % (namespace, self_)

            # is not a special operator
            return None
                

        
        frees = GetFreeOperators()
        members = [x for x in self.public_members if type(x) == ClassOperator]
        all_operators = frees + members
        operators = [x for x in all_operators if not self.info['operator'][x.name].exclude]
        
        for operator in operators:
            # gatter information about the operator, for use later
            wrapper = self.info['operator'][operator.name].wrapper
            if wrapper:
                pointer = '&' + wrapper.FullName()
                if wrapper.code:
                    self.Add('declaration', wrapper.code)
            elif isinstance(operator, ClassOperator) and self.class_.IsUnique(operator.name):
                pointer = '&' + operator.FullName()
            else:
                pointer = operator.PointerDeclaration()                 
            rename = self.info['operator'][operator.name].rename

            # check if this operator will be exported as a method
            export_as_method = wrapper or rename or operator.name in self.BOOST_RENAME_OPERATORS
            
            # check if this operator has a special representation in boost
            special_code = HandleSpecialOperator(operator)
            has_special_representation = special_code is not None
            
            if export_as_method:
                # export this operator as a normal method, renaming or using the given wrapper
                if not rename:
                    if wrapper:
                        rename = wrapper.name
                    else:
                        rename = self.BOOST_RENAME_OPERATORS[operator.name]
                policy = ''
                policy_obj = self.info['operator'][operator.name].policy
                if policy_obj:
                    policy = ', %s()' % policy_obj.Code() 
                self.Add('inside', '.def("%s", %s%s)' % (rename, pointer, policy))
            
            elif has_special_representation:
                self.Add('inside', special_code)
                
            elif operator.name in self.BOOST_SUPPORTED_OPERATORS:
                # export this operator using boost's facilities
                op = operator
                is_unary = isinstance(op, Operator) and len(op.parameters) == 1 or\
                           isinstance(op, ClassOperator) and len(op.parameters) == 0
                if is_unary:
                    self.Add('inside', '.def( %s%sself )' % \
                        (operator.name, namespaces.python))
                else:
                    # binary operator
                    if len(operator.parameters) == 2:
                        left_operand = GetOperand(operator.parameters[0])
                        right_operand = GetOperand(operator.parameters[1])
                    else:
                        left_operand = namespaces.python + 'self'
                        right_operand = GetOperand(operator.parameters[0])
                    self.Add('inside', '.def( %s %s %s )' % \
                        (left_operand, operator.name, right_operand))

        # export the converters.
        # export them as simple functions with a pre-determined name

        converters = [x for x in self.public_members if type(x) == ConverterOperator]
                
        def ConverterMethodName(converter):
            result_fullname = converter.result.name
            if result_fullname in self.SPECIAL_CONVERTERS:
                return self.SPECIAL_CONVERTERS[result_fullname]
            else:
                # extract the last name from the full name
                result_name = _ID(result_fullname.split('::')[-1])
                return 'to_' + result_name
            
        for converter in converters:
            info = self.info['operator'][converter.result.name]
            # check if this operator should be excluded
            if info.exclude:
                continue
            
            special_code = HandleSpecialOperator(converter)
            if info.rename or not special_code:
                # export as method
                name = info.rename or ConverterMethodName(converter)
                if self.class_.IsUnique(converter.name):
                    pointer = '&' + converter.FullName()
                else:
                    pointer = converter.PointerDeclaration()
                policy_code = ''
                if info.policy:
                    policy_code = ', %s()' % info.policy.Code()
                self.Add('inside', '.def("%s", %s%s)' % (name, pointer, policy_code))
                    
            elif special_code:
                self.Add('inside', special_code)



    def ExportNestedClasses(self, exported_names):
        nested_classes = [x for x in self.public_members if isinstance(x, NestedClass)]
        for nested_class in nested_classes:
            nested_info = self.info[nested_class.name]
            nested_info.include = self.info.include
            nested_info.name = nested_class.FullName()
            exporter = ClassExporter(nested_info)
            exporter.SetDeclarations(self.declarations + [nested_class])
            codeunit = CodeUnit(None)
            exporter.Export(codeunit, exported_names)
            self.nested_codeunits.append(codeunit)


    def ExportNestedEnums(self):
        nested_enums = [x for x in self.public_members if isinstance(x, ClassEnumeration)]
        for enum in nested_enums:
            enum_info = self.info[enum.name]
            enum_info.include = self.info.include
            enum_info.name = enum.FullName()
            exporter = EnumExporter(enum_info)
            exporter.SetDeclarations(self.declarations + [enum])
            codeunit = CodeUnit(None)
            exporter.Export(codeunit, None)
            self.nested_codeunits.append(codeunit)
            

            

def _ID(name):
    'Returns the name as a valid identifier'
    for invalidchar in ('::', '<', '>', ' ', ','):
        name = name.replace(invalidchar, '_') 
    # avoid duplications of '_' chars
    names = [x for x in name.split('_') if x]
    return '_'.join(names)


#==============================================================================
# Virtual Wrapper utils
#==============================================================================

def _ParamsInfo(m, count=None):
    if count is None:
        count = len(m.parameters)
    param_names = ['p%i' % i for i in range(count)]
    param_types = [x.FullName() for x in m.parameters[:count]]
    params = ['%s %s' % (t, n) for t, n in zip(param_types, param_names)]
    #for i, p in enumerate(m.parameters[:count]):
    #    if p.default is not None:
    #        #params[i] += '=%s' % p.default
    #        params[i] += '=%s' % (p.name + '()')
    params = ', '.join(params) 
    return params, param_names, param_types

            
class _VirtualWrapperGenerator(object):
    'Generates code to export the virtual methods of the given class'

    def __init__(self, class_, info):
        self.class_ = class_
        self.info = info
        self.wrapper_name = _ID(class_.FullName()) + '_Wrapper'


    def DefaultImplementationNames(self, method):
        '''Returns a list of default implementations for this method, one for each
        number of default arguments. Always returns at least one name, and return from 
        the one with most arguments to the one with the least.
        '''
        base_name = 'default_' + method.name 
        minArgs = method.minArgs
        maxArgs = method.maxArgs
        if minArgs == maxArgs:
            return [base_name]
        else:
            return [base_name + ('_%i' % i) for i in range(minArgs, maxArgs+1)]                


    def Declaration(self, method, indent):
        '''Returns a string with the declarations of the virtual wrapper and
        its default implementations. This string must be put inside the Wrapper
        body.        
        '''
        pyste = namespaces.pyste
        python = namespaces.python
        rename = self.info[method.name].rename or method.name
        result = method.result.FullName()
        return_str = 'return '
        if result == 'void':
            return_str = ''
        params, param_names, param_types = _ParamsInfo(method)
        constantness = ''
        if method.const:
            constantness = ' const'
        
        # call_method callback
        decl  = indent + '%s %s(%s)%s {\n' % (result, method.name, params, constantness)
        param_names_str = ', '.join(param_names)
        if param_names_str:
            param_names_str = ', ' + param_names_str
        decl += indent*2 + '%s%scall_method< %s >(self, "%s"%s);\n' %\
            (return_str, python, result, rename, param_names_str)
        decl += indent + '}\n'

        # default implementations (with overloading)
        # only for classes that are not abstract, and public methods 
        if not method.abstract and method.visibility == Scope.public:
            minArgs = method.minArgs
            maxArgs = method.maxArgs
            impl_names = self.DefaultImplementationNames(method)
            for impl_name, argNum in zip(impl_names, range(minArgs, maxArgs+1)): 
                params, param_names, param_types = _ParamsInfo(method, argNum)            
                decl += '\n'
                decl += indent + '%s %s(%s)%s {\n' % (result, impl_name, params, constantness)
                decl += indent*2 + '%s%s::%s(%s);\n' % \
                    (return_str, self.class_.FullName(), method.name, ', '.join(param_names))
                decl += indent + '}\n'                
        return decl
            

    def MethodDefinition(self, method):
        '''Returns a list of lines, which should be put inside the class_
        statement to export this method.'''
        # dont define abstract methods
        pyste = namespaces.pyste
        rename = self.info[method.name].rename or method.name
        default_names = self.DefaultImplementationNames(method)
        class_name = self.class_.FullName()
        wrapper_name = pyste + self.wrapper_name
        result = method.result.FullName()
        is_method_unique = self.class_.IsUnique(method.name)
        constantness = ''
        if method.const:
            constantness = ' const'
        
        # create a list of default-impl pointers
        minArgs = method.minArgs
        maxArgs = method.maxArgs 
        if is_method_unique:
            default_pointers = ['&%s::%s' % (wrapper_name, x) for x in default_names]
        else:
            default_pointers = []
            for impl_name, argNum in zip(default_names, range(minArgs, maxArgs+1)):
                param_list = [x.FullName() for x in method.parameters[:argNum]]
                params = ', '.join(param_list)             
                signature = '%s (%s::*)(%s)%s' % (result, wrapper_name, params, constantness)
                default_pointer = '(%s)%s::%s' % (signature, wrapper_name, impl_name)
                default_pointers.append(default_pointer)
            
        # get the pointer of the method
        if is_method_unique:
            pointer = '&' + method.FullName()
        else:
            pointer = method.PointerDeclaration()

        # generate the defs
        definitions = []
        # basic def
        definitions.append('.def("%s", %s, %s)' % (rename, pointer, default_pointers[-1]))
        for default_pointer in default_pointers[:-1]:
            definitions.append('.def("%s", %s)' % (rename, default_pointer))
        return definitions

    
    def FullName(self):
        return namespaces.pyste + self.wrapper_name


    def VirtualMethods(self):
        def IsVirtual(m):
            return type(m) == Method and m.virtual        
        return [m for m in self.class_.members if IsVirtual(m)]
        
            
    
    def Constructors(self):
        def IsValid(m):
            return isinstance(m, Constructor) and m.visibility == Scope.public
        return [m for m in self.class_.members if IsValid(m)]
    
        
    def GenerateDefinitions(self):
        defs = []
        for method in self.VirtualMethods():
            exclude = self.info[method.name].exclude
            # generate definitions only for public methods and non-abstract methods
            if method.visibility == Scope.public and not method.abstract and not exclude:
                defs.extend(self.MethodDefinition(method))
        return defs

        
    def GenerateVirtualWrapper(self, indent):
        'Return the wrapper for this class'
        
        # generate the class code
        class_name = self.class_.FullName()
        code = 'struct %s: %s\n' % (self.wrapper_name, class_name)
        code += '{\n'
        # generate constructors (with the overloads for each one)
        for cons in self.Constructors(): # only public constructors
            minArgs = cons.minArgs
            maxArgs = cons.maxArgs
            # from the min number of arguments to the max number, generate
            # all version of the given constructor
            cons_code = ''
            for argNum in range(minArgs, maxArgs+1):
                params, param_names, param_types = _ParamsInfo(cons, argNum)
                if params:
                    params = ', ' + params
                cons_code += indent + '%s(PyObject* self_%s):\n' % \
                    (self.wrapper_name, params)
                cons_code += indent*2 + '%s(%s), self(self_) {}\n\n' % \
                    (class_name, ', '.join(param_names))
            code += cons_code
        # generate the body
        body = []
        for method in self.VirtualMethods():
            if not self.info[method.name].exclude:
                body.append(self.Declaration(method, indent))            
        body = '\n'.join(body) 
        code += body + '\n'
        # add the self member
        code += indent + 'PyObject* self;\n'
        code += '};\n'
        return code 
