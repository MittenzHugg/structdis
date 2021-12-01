import sys
import re

sys.path.extend(['.','..'])

from pycparser import parse_file, c_ast
from ctypes import *
import uuid

RE_CHILD_ARRAY = re.compile(r'(.*)\[(.*)\]')
RE_INTERNAL_ATTR = re.compile('__.*__')

baseTypes = {
    'unsignedchar':     c_uint8,
    'unsignedshort':    c_uint16,
    'unsignedshortint': c_uint16,
    'unsignedint':      c_uint32,
    'unsignedlong':     c_uint32,
    'unsignedlongint':  c_uint32,
    'unsignedlonglong': c_uint64,
    'unsignedlonglongint': c_uint64,
    'char':             c_int8,
    'signedchar':       c_int8,
    'short':            c_int16,
    'shortint':         c_int16,
    'signedshort':      c_int16,
    'signedshortint':   c_int16,
    'int':              c_int32,
    'signedint':        c_int32,
    'long':             c_int32,
    'longint':          c_int32,
    'signedlong':       c_int32,
    'longlong':         c_int64,
    'longlongint':      c_int64,
    'signedlonglong':   c_int64,
    'signedlonglongint': c_int64,
    'float':            c_float,
    'double':           c_double
}

n64types = baseTypes
struct_decls = {}
none_decls = {}

def __parse_dim(val):
    global n64types
    if(type(val) == c_ast.Constant):
        return int(val.value, 0)
    elif(type(val) == c_ast.BinaryOp):
        left = __parse_dim(val.left)
        right = __parse_dim(val.right)
        if(val.op == '*'):
            return left * right
        elif(val.op == '+'):
            return left + right
        elif(val.op == '-'):
            return left - right
        #TODO: division, mod?
        else:
            return 0
    elif (type(val) == c_ast.UnaryOp):
        if val.op == 'sizeof':
            return sizeof( n64types[''.join(val.expr.type.type.names)])
        else:
            return 0
    else:
        return 0

def __parse_array(node):
    global n64types
    dim = __parse_dim(node.dim)
    if type(node.type) == c_ast.ArrayDecl:
        return __parse_array(node.type) * dim
    elif type(node.type) == c_ast.PtrDecl:
        return c_uint32* dim
    elif type(node.type) == c_ast.TypeDecl:
        return __parse_typedecl(node.type)* dim
    else:
        return n64types[''.join(node.type.type.type.names)] * dim


def __parse_fields(node):
    global none_decls
    fields = []
    annon = []

    if node.type.decls == None :
        n_match = filter(lambda none_dec: node.type.name == none_dec.type.name,  none_decls)
        node = next(n_match)

    for member in node.type.decls:
        if(type(member.type) == c_ast.TypeDecl): #predefined type
            if(member.bitsize == None):
                fields.append((member.name, __parse_typedecl(member.type)))
            else:
                fields.append((member.name, __parse_typedecl(member.type), int(member.bitsize.value,0)))
        elif type(member.type) == c_ast.PtrDecl: #ptr cast as u32
            fields.append((member.name, c_uint32))
        elif type(member.type) == c_ast.ArrayDecl: #array
            arr = __parse_array(member.type)
            fields.append((member.name, arr))
        elif type(member.type) == c_ast.Union:
            if member.type.name == None:
                _anonymous_ = 'anon_' + str(uuid.uuid4())[0:8]
                flds, anons = __parse_fields(member)
                uni = type(_anonymous_, (BigEndianUnion, ), dict(_anonymous_ = anons, _fields_ = flds))
                fields.append((_anonymous_, uni))
                annon.append(_anonymous_)
            else:
                uni = type(member.declname, (BigEndianUnion, ), dict(_fields_ = __parse_fields(member.type.decls)))
                fields.append((member.name, uni))
        elif type(member.type) == c_ast.Struct:
            if member.type.name == None:
                _anonymous_ = 'anon_' + str(uuid.uuid4())[0:8]
                flds, anons = __parse_fields(member)
                struct = type(_anonymous_, (BigEndianStructure, ), dict(_anonymous_ = anons, _fields_ = flds))
                fields.append((_anonymous_, struct))
                annon.append(_anonymous_)
            else:
                struct = type(member.declname, (BigEndianStructure, ), dict(_fields_ = __parse_fields(member.type.decls)))
                fields.append((member.name, struct))
        else:
            print(member)
        """ TODO: implement bitfield and union members"""
    return fields, annon

def __parse_typedecl(node):
    global n64types
    #print(node)
    if type(node.type) == c_ast.IdentifierType and ''.join(node.type.names) in n64types:
        return n64types[''.join(node.type.names)]
    elif type(node.type) == c_ast.Struct:
        flds, anon = __parse_fields(node)
        return type(node.declname, (BigEndianStructure, ), dict(_anonymous_ = anon, _fields_ = flds))
    elif type(node.type) == c_ast.Union:
        flds, anon = __parse_fields(node)
        return type(node.declname, (BigEndianUnion, ), dict(_anonymous_ = anon, _fields_ = flds))
    else:
        print("!!!!!!!BAD NODE" + node.declname)

def __parse_ast(ast):
    global n64types
    global none_decls

    # ptrs can be processed first since we don't care about their type in this instance
    ptrdefs = filter(lambda node: type(node) == c_ast.Typedef and type(node.type) == c_ast.PtrDecl, ast.ext)
    ptrdefs = filter(lambda node: node.name not in baseTypes, ptrdefs)
    for node in ptrdefs:
        n64types[node.name] = c_int32

    none_decls = list(filter(lambda node: type(node) == c_ast.Decl and (node.name == None), ast.ext))

    #process types and arraytypes together
    typedefs = filter(lambda node: type(node) == c_ast.Typedef and (type(node.type) == c_ast.TypeDecl or type(node.type) == c_ast.ArrayDecl), ast.ext)
    typedefs = filter(lambda node: node.name not in baseTypes, typedefs)
    #typedecls = list(map(lambda node: node.type, typedefs))
    typedefs = list(typedefs)

    for node in typedefs:
        if type(node.type) == c_ast.ArrayDecl:
            n64types[node.name] = __parse_array(node.type)
        else:
            n64types[node.type.declname] = __parse_typedecl(node.type)
        # __parse_typedecl(node)

def from_c_header(filename):
    ast = parse_file(filename, use_cpp=True)
    __parse_ast(ast)
    

def get_type(typename):
    if typename in n64types:
        return n64types[typename]
#-PRINTING------------------------------------------------------

def print_ctype(val):
    string = ''
    if type(val) is float:
        string += str(val) + "f, "
    elif type(val) is int:
        if(val < 0):
            string += '-0x%x' % -val + ", "
        else:
            string += '0x%x' % val + ", "
    elif type(type(val)) is type(c_int*4):
        string += "{ "
        for r in val:
            string += print_ctype(r)
        string = string[:-2]
        string += "}, "
    else:
        string += "{ "
        for field_name, field_type in val._fields_:
            string += print_ctype(getattr(val, field_name))
        string = string[:-2]
        string += "}, "
    return string
        



#---------------------------------------------------------------
if __name__ == "__main__":
    if len(sys.argv) > 5:

        offset = 0;
        """ Load C file into dict representation of ast  """
        from_c_header(sys.argv[1])

        """ generate using struct module """
        f = open(sys.argv[2], "rb").read()
        offset = int(sys.argv[3])
        s = n64types[sys.argv[4]].from_buffer_copy(f, offset)
        print(sys.argv[4] + " " + sys.argv[5] + " = " + print_ctype(s)[:-2] + ";")

        
    else:
        print("Please provide a filename as argument")
