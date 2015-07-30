from pyNastran.bdf.bdf import BDF
import sys
import numpy
write = sys.stdout.buffer.write
writeInt = lambda val: write(val.to_bytes(4, sys.byteorder))
#fname = 'C:\\Users\\BOPOHOB\\Documents\\Qt\\work\\NPO\\METEORIT.bdf'
mesh = BDF(debug=False,log=None)
mesh.read_bdf(input("read") if 'fname' not in locals() else fname, xref=True)
m=numpy.zeros(max(mesh.nodes.keys()) * 3 + 3, numpy.float32)
for i, node in mesh.nodes.items():
    m[i * 3 : i * 3 + 3] = node.get_position()
input("readed")

#store the vertexes
writeInt(m.itemsize)
writeInt(len(m))
write(m.tobytes())

#store the elements
writeInt(max(mesh.elements.keys()))
writeInt(len(mesh.elements))
for i, element in mesh.elements.items():
    writeInt(i)
    cropLimit = len(element.nodeIDs())
    if 'CQUAD' in element.type.upper():
        cropLimit = 4
        writeInt(1)
    elif 'CTETRA' in element.type.upper():
        cropLimit = 4
        writeInt(2)
    elif 'CTRIA' in element.type.upper():
        cropLimit = 3
        writeInt(4)
    elif 'CBAR' in element.type.upper():
        cropLimit = 0
        writeInt(6)
    elif 'CBUSH' in element.type.upper():
        cropLimit = 0
        writeInt(5)
    elif 'CHEXA' in element.type.upper():
        cropLimit = 8
        writeInt(3)
    elif 'RBE' in element.type.upper():
        cropLimit = 0
        writeInt(-1)
    else:
        writeInt(-1)
        cropLimit = 0
        #assert 'convertor fail'
    writeInt(element.Pid())#it's a shell
    writeInt(cropLimit)
    if cropLimit:
        write(numpy.array(element.nodeIDs(), dtype=numpy.int32)[:cropLimit].tobytes())
